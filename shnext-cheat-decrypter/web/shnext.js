// ShnExt cheat files, same format as ../main.c
//
// raw deflate( JSON ), where
//   "ProcessName"  = base64( IV[16] + AES-256-CBC(.NET BinaryFormatter string) )
//                    key = SHA256("#my*S3cr3t")
//   "patchEntries" = [ base64( AES-256-CBC(raw deflate(JSON)) ), ... ]
//                    key/iv = one byte of SHNEXT_FIELD, picked by entry index

import { subtle, enc, dec, b64decode, b64encode, concat, inflateRaw, deflateRaw, cbcDecrypt, cbcEncrypt } from '../../common/web/crypto.js';

const SHNEXT_FIELD = new Uint8Array([
    0x04, 0x14, 0x02, 0x80, 0x01, 0x20, 0xF3, 0xEB,
    0x03, 0x01, 0x02, 0x80, 0x00, 0x20, 0xF3, 0xEB,
    0x05, 0x18, 0x02, 0x80, 0x00, 0x20, 0xF3, 0xEB,
    0x00, 0x1C, 0x02, 0x80, 0x71, 0x0F, 0x8C, 0xBF,
    0xF2, 0x0E, 0x06, 0x08, 0x03, 0x06, 0x10, 0x10,
    0x00, 0x07, 0x24, 0xC8, 0x08, 0x05, 0x0E, 0x3E,
    0x00, 0x04, 0x20, 0xC8, 0x01, 0x07, 0x25, 0xC8,
    0x00, 0x05, 0x28, 0xC8, 0x00, 0x06, 0x2C, 0xC8,
]);

// .NET BinaryFormatter SerializationHeaderRecord (RootId=1, HeaderId=-1, v1.0)
const NET_BINFMT_HEADER = new Uint8Array([
    0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
]);

const processKey = () => subtle.digest('SHA-256', enc.encode('#my*S3cr3t')).then((h) => new Uint8Array(h));

function entryKeyIv(index) {
    return {
        key: new Uint8Array(32).fill(SHNEXT_FIELD[(index * 5) % 64]),
        iv: new Uint8Array(16).fill(SHNEXT_FIELD[(index * 5) % 32]),
    };
}

async function decryptProcessName(b64) {
    const raw = b64decode(b64);
    const plain = await cbcDecrypt(await processKey(), raw.subarray(0, 16), raw.subarray(16));

    // BinaryObjectString record: 0x06, ObjectId (int32), 7-bit encoded length, UTF-8 string
    let pos = NET_BINFMT_HEADER.length;
    if (plain[pos] !== 0x06) throw new Error('Unexpected ProcessName record');
    pos += 5;

    let len = 0;
    for (let shift = 0; pos < plain.length && shift < 35; shift += 7) {
        const b = plain[pos++];
        len |= (b & 0x7F) << shift;
        if (!(b & 0x80)) break;
    }
    if (pos + len > plain.length) throw new Error('Bad ProcessName length');

    return dec.decode(plain.subarray(pos, pos + len));
}

async function encryptProcessName(name) {
    const str = enc.encode(name);
    const varint = [];
    for (let v = str.length; ; v >>>= 7) {
        varint.push((v & 0x7F) | (v > 0x7F ? 0x80 : 0));
        if (v <= 0x7F) break;
    }

    const plain = concat(
        NET_BINFMT_HEADER,
        new Uint8Array([0x06, 0x01, 0x00, 0x00, 0x00]),  // BinaryObjectString, ObjectId = 1
        new Uint8Array(varint),
        str,
        new Uint8Array([0x0B]),                           // MessageEnd
    );
    const iv = globalThis.crypto.getRandomValues(new Uint8Array(16));

    return b64encode(concat(iv, await cbcEncrypt(await processKey(), iv, plain)));
}

async function decryptEntry(b64, index) {
    const { key, iv } = entryKeyIv(index);
    const json = await inflateRaw(await cbcDecrypt(key, iv, b64decode(b64)));
    return JSON.parse(dec.decode(json));
}

async function encryptEntry(entry, index) {
    const { key, iv } = entryKeyIv(index);
    const comp = await deflateRaw(enc.encode(JSON.stringify(entry)));
    return b64encode(await cbcEncrypt(key, iv, comp));
}

// Returns { json: Uint8Array (pretty JSON), info: { game, process, entries[] } }
export async function shnextDecrypt(data) {
    const root = JSON.parse(dec.decode(await inflateRaw(data)));

    if (typeof root.ProcessName === 'string')
        root.ProcessName = await decryptProcessName(root.ProcessName);

    const entries = Array.isArray(root.patchEntries) ? root.patchEntries : [];
    for (let i = 0; i < entries.length; i++)
        if (typeof entries[i] === 'string')
            entries[i] = await decryptEntry(entries[i], i);

    return {
        json: enc.encode(JSON.stringify(root, null, '\t')),
        info: shnextInfo(root),
    };
}

export async function shnextEncrypt(data) {
    const root = JSON.parse(dec.decode(data));
    const info = shnextInfo(root);

    if (typeof root.ProcessName === 'string')
        root.ProcessName = await encryptProcessName(root.ProcessName);

    const entries = Array.isArray(root.patchEntries) ? root.patchEntries : [];
    for (let i = 0; i < entries.length; i++)
        if (entries[i] && typeof entries[i] === 'object')
            entries[i] = await encryptEntry(entries[i], i);

    return { data: await deflateRaw(enc.encode(JSON.stringify(root))), info };
}

function shnextInfo(root) {
    return {
        game: root.Game ?? '',
        process: typeof root.ProcessName === 'string' ? root.ProcessName : '',
        entries: (root.patchEntries ?? []).map((e) => (e && typeof e === 'object' ? e.name ?? '' : '')),
    };
}
