// Shared UI for the browser tool pages: pick a file, decrypt or encrypt it,
// download the result. Everything stays in the browser.
//
// setupPage({
//   detect(bytes)  -> 'decrypt' | 'encrypt'   the likely action for this file
//   decrypt(bytes) -> { data, summary? }
//   encrypt(bytes) -> { data, summary? }
//   decryptedName(name), encryptedName(name)   output file names
// })
// summary is a list of [label, value] pairs; value may be an array (rendered as a list).

const $ = (id) => document.getElementById(id);

function formatSize(n) {
    return n < 1024 ? `${n} bytes` : `${(n / 1024).toFixed(1)} KB`;
}

export function setupPage(tool) {
    const drop = $('drop'), input = $('file');
    const filePanel = $('file-panel'), resultPanel = $('result-panel');
    const btnDecrypt = $('decrypt'), btnEncrypt = $('encrypt');
    let file = null, bytes = null, url = null;

    async function load(f) {
        file = f;
        bytes = new Uint8Array(await f.arrayBuffer());
        $('file-name').textContent = f.name;
        $('file-meta').textContent = formatSize(bytes.length);

        const guess = tool.detect(bytes);
        $('file-guess').textContent = guess === 'decrypt'
            ? 'Looks encrypted — ready to decrypt.'
            : 'Looks decrypted — ready to encrypt.';
        btnDecrypt.classList.toggle('primary', guess === 'decrypt');
        btnEncrypt.classList.toggle('primary', guess === 'encrypt');

        filePanel.hidden = false;
        resultPanel.hidden = true;
    }

    async function run(action) {
        const status = $('status');
        btnDecrypt.disabled = btnEncrypt.disabled = true;
        resultPanel.hidden = false;
        $('download').hidden = true;
        $('preview').hidden = true;
        $('summary').replaceChildren();
        status.className = 'status';
        status.textContent = action === 'decrypt' ? 'Decrypting…' : 'Encrypting…';

        try {
            const out = await tool[action](bytes);
            const name = action === 'decrypt' ? tool.decryptedName(file.name) : tool.encryptedName(file.name);

            if (url) URL.revokeObjectURL(url);
            url = URL.createObjectURL(new Blob([out.data], { type: 'application/octet-stream' }));
            const link = $('download');
            link.href = url;
            link.download = name;
            link.textContent = `Download ${name}`;
            link.hidden = false;

            status.className = 'status ok';
            status.textContent = `${action === 'decrypt' ? 'Decrypted' : 'Encrypted'} — ${formatSize(out.data.length)}`;
            renderSummary(out.summary ?? []);

            if (action === 'decrypt') {
                const text = new TextDecoder().decode(out.data);
                $('preview').textContent = text.length > 20000 ? text.slice(0, 20000) + '\n…' : text;
                $('preview').hidden = false;
            }
        } catch (err) {
            console.error(err);
            status.className = 'status err';
            status.textContent = `Could not ${action} this file: ${err.message || err}`;
        } finally {
            btnDecrypt.disabled = btnEncrypt.disabled = false;
        }
    }

    function renderSummary(items) {
        const ul = $('summary');
        for (const [label, value] of items) {
            const li = document.createElement('li');
            const k = document.createElement('span');
            k.className = 'k';
            k.textContent = `${label}: `;
            li.append(k);
            if (Array.isArray(value)) {
                li.append(`${value.length}`);
                const ol = document.createElement('ol');
                for (const v of value) {
                    const item = document.createElement('li');
                    item.textContent = v || '(unnamed)';
                    ol.append(item);
                }
                li.append(ol);
            } else {
                li.append(String(value));
            }
            ul.append(li);
        }
    }

    input.addEventListener('change', () => input.files[0] && load(input.files[0]));
    drop.addEventListener('dragover', (e) => { e.preventDefault(); drop.classList.add('over'); });
    drop.addEventListener('dragleave', () => drop.classList.remove('over'));
    drop.addEventListener('drop', (e) => {
        e.preventDefault();
        drop.classList.remove('over');
        if (e.dataTransfer.files[0]) load(e.dataTransfer.files[0]);
    });
    btnDecrypt.addEventListener('click', () => run('decrypt'));
    btnEncrypt.addEventListener('click', () => run('encrypt'));
}

// "game.mc4" -> "game.mc4.xml"; "game.mc4.xml" -> "game.mc4"
export function swapExt(name, plainExt, encExt, decrypting) {
    const lower = name.toLowerCase();
    if (decrypting)
        return name + plainExt;
    if (lower.endsWith(plainExt.toLowerCase()))
        return name.slice(0, -plainExt.length);
    return lower.endsWith(encExt.toLowerCase()) ? name : name + encExt;
}
