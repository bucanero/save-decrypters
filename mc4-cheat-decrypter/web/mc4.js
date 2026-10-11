// MC4 cheat files, same format as ../main.c
//
// base64( AES-256-CBC(XML) ), fixed key and IV. The XML is padded with '\n'
// up to the block size instead of PKCS#7.

import { enc, dec, b64decode, b64encode, cbcDecryptRaw, cbcEncryptRaw } from '../../common/web/crypto.js';

const MC4_KEY = enc.encode('304c6528f659c766110239a51cl5dd9c');
const MC4_IV = enc.encode('u@}kzW2u[u(8DWar');

export async function mc4Decrypt(data) {
    const raw = b64decode(dec.decode(data));
    return cbcDecryptRaw(MC4_KEY, MC4_IV, raw);
}

export async function mc4Encrypt(data) {
    const padded = new Uint8Array(Math.ceil(data.length / 16) * 16).fill(0x0A);
    padded.set(data);
    return enc.encode(b64encode(await cbcEncryptRaw(MC4_KEY, MC4_IV, padded)));
}
