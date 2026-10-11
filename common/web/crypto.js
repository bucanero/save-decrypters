// Shared helpers for the browser versions of the tools in this repo:
// base64, raw deflate and AES-CBC on top of WebCrypto. No DOM in here, so it
// also runs under Node 20+ (WebCrypto, CompressionStream and atob/btoa are
// globals there).

export const subtle = globalThis.crypto.subtle;
export const enc = new TextEncoder();
export const dec = new TextDecoder();

export function b64decode(str) {
    const bin = atob(str.replace(/\s+/g, ''));
    const out = new Uint8Array(bin.length);
    for (let i = 0; i < bin.length; i++) out[i] = bin.charCodeAt(i);
    return out;
}

export function b64encode(bytes) {
    let bin = '';
    for (let i = 0; i < bytes.length; i += 0x8000)
        bin += String.fromCharCode.apply(null, bytes.subarray(i, i + 0x8000));
    return btoa(bin);
}

export function concat(...parts) {
    const out = new Uint8Array(parts.reduce((n, p) => n + p.length, 0));
    let pos = 0;
    for (const p of parts) { out.set(p, pos); pos += p.length; }
    return out;
}

async function pipe(bytes, stream) {
    const res = new Response(new Blob([bytes]).stream().pipeThrough(stream));
    return new Uint8Array(await res.arrayBuffer());
}

export const inflateRaw = (bytes) => pipe(bytes, new DecompressionStream('deflate-raw'));
export const deflateRaw = (bytes) => pipe(bytes, new CompressionStream('deflate-raw'));

const aesKey = (raw) => subtle.importKey('raw', raw, 'AES-CBC', false, ['encrypt', 'decrypt']);

// AES-CBC with PKCS#7 padding (what WebCrypto does natively).
export async function cbcDecrypt(key, iv, data) {
    return new Uint8Array(await subtle.decrypt({ name: 'AES-CBC', iv }, await aesKey(key), data));
}

export async function cbcEncrypt(key, iv, data) {
    return new Uint8Array(await subtle.encrypt({ name: 'AES-CBC', iv }, await aesKey(key), data));
}

// AES-CBC with no padding. WebCrypto always checks PKCS#7 on decrypt, so add
// one extra block that decrypts to a full block of 0x10 padding: encrypting
// sixteen 0x10 bytes with the last ciphertext block as IV produces it.
export async function cbcDecryptRaw(key, iv, data) {
    if (data.length === 0 || data.length % 16) throw new Error('Data is not a multiple of the AES block size');
    const k = await aesKey(key);
    const last = data.subarray(data.length - 16);
    const pad = new Uint8Array(await subtle.encrypt({ name: 'AES-CBC', iv: last }, k, new Uint8Array(16).fill(16)));
    const out = await subtle.decrypt({ name: 'AES-CBC', iv }, k, concat(data, pad.subarray(0, 16)));
    return new Uint8Array(out);
}

// Input must already be block aligned; drop the PKCS#7 block WebCrypto appends.
export async function cbcEncryptRaw(key, iv, data) {
    if (data.length % 16) throw new Error('Data is not a multiple of the AES block size');
    return (await cbcEncrypt(key, iv, data)).subarray(0, data.length);
}
