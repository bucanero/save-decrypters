// Gate for the mc4/shnext web pages (<tool>/web/): their JS must agree with the C tools on
// the samples in this repo. Run from the repo root, after building
// mc4-cheat-decrypter and shnext-cheat-decrypter:
//
//   node .github/scripts/test-cheat-pages.mjs

import { readFileSync, writeFileSync, copyFileSync, mkdtempSync } from 'node:fs';
import { execFileSync } from 'node:child_process';
import { tmpdir } from 'node:os';
import { basename, join, resolve } from 'node:path';
import { pathToFileURL } from 'node:url';

const c = {
    ...await import(pathToFileURL(resolve('mc4-cheat-decrypter/web/mc4.js'))),
    ...await import(pathToFileURL(resolve('shnext-cheat-decrypter/web/shnext.js'))),
};
const tmp = mkdtempSync(join(tmpdir(), 'cheats-'));
const same = (a, b) => Buffer.compare(Buffer.from(a), Buffer.from(b)) === 0;
let failed = 0;

function check(name, ok) {
    console.log(`${ok ? 'ok  ' : 'FAIL'} ${name}`);
    if (!ok) failed++;
}

// Run a C tool in place on a copy of `src`, return the result.
function cTool(tool, opt, src, data) {
    const file = join(tmp, basename(src));
    data ? writeFileSync(file, data) : copyFileSync(src, file);
    execFileSync(`./${tool}/${tool}`, [opt, file], { stdio: 'ignore' });
    return readFileSync(file);
}

// MC4: decrypt must match the C tool; encrypting that must give back the original file.
{
    const src = 'mc4-cheat-decrypter/samples/CUSA32091_01.02.mc4';
    const plain = await c.mc4Decrypt(readFileSync(src));
    check('mc4: decrypt matches C tool', same(plain, cTool('mc4-cheat-decrypter', '-d', src)));
    check('mc4: decrypt matches samples .DEC', same(plain, readFileSync(src + '.DEC')));
    check('mc4: encrypt(decrypt(x)) == x', same(await c.mc4Encrypt(plain), readFileSync(src)));
}

// ShnExt: same JSON as the C tool, and each side can read what the other wrote.
{
    const src = 'shnext-cheat-decrypter/samples/7-Days-To-Die_CUSA03446_01.18_illusional.ShnExt';
    const norm = (bytes) => JSON.stringify(JSON.parse(Buffer.from(bytes).toString('utf8')));
    const { json } = await c.shnextDecrypt(readFileSync(src));
    const cJson = cTool('shnext-cheat-decrypter', '-d', src);
    check('shnext: decrypt matches C tool', norm(json) === norm(cJson));

    const { data } = await c.shnextEncrypt(json);
    check('shnext: C tool decrypts web-encrypted file',
        norm(cTool('shnext-cheat-decrypter', '-d', 'web.ShnExt', data)) === norm(cJson));

    const cEnc = cTool('shnext-cheat-decrypter', '-e', 'c.json', cJson);
    check('shnext: web decrypts C-encrypted file', norm((await c.shnextDecrypt(cEnc)).json) === norm(cJson));
}

process.exit(failed ? 1 : 0);
