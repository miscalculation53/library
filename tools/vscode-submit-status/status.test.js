const { test } = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs/promises');
const os = require('node:os');
const path = require('node:path');
const vm = require('node:vm');
const { hash, paths, selectFile, snapshot, present } = require('./status');

function data(phase = 'done', bytes = 42000) {
    return { state: { token: 'new', phase, bytes }, request: { token: 'new', source_digest: 'source' },
        artifact: { output_digest: 'output' }, sourceHash: 'source', outputHash: 'output', actualBytes: bytes };
}

test('expansion and cleanup show separate stages', () => {
    for (const [phase, expected] of [['queued', '開始待ち'], ['expanding', '展開中'], ['cleaning', '短縮中']])
        assert.match(present(data(phase, null)).text, new RegExp(expected));
});

test('strict byte limit and usable intermediate result', () => {
    assert.match(present(data('cleaning', 65535)).text, /提出準備OK.*65,535 B.*spin/);
    assert.match(present(data('done', 65535)).text, /提出準備OK/);
    assert.match(present(data('cleaning', 65536)).text, /短縮中/);
    assert.match(present(data('done', 65536)).text, /サイズ制限未達/);
});

test('unsaved source, output or header edits invalidate readiness', () => {
    assert.match(present(data(), true).text, /変更あり/);
    assert.match(present(data(), false, true).text, /変更あり/);
});

test('saved source edits invalidate completed output', () => {
    assert.match(present({ ...data(), sourceHash: 'edited' }).text, /変更あり/);
});

test('manually changed or deleted output is never marked ready', () => {
    for (const outputHash of ['edited', null])
        assert.match(present({ ...data(), outputHash }).text, /出力を確認/);
    assert.match(present({ ...data(), actualBytes: 1 }).text, /出力を確認/);
});

test('old generation status cannot mark a new request ready', () => {
    const value = data();
    value.state.token = 'old';
    assert.match(present(value).text, /切替中/);
});

test('failed or stopped cleanup remains visible even with a usable checkpoint', () => {
    assert.match(present(data('failed')).text, /エラー/);
    assert.match(present(data('stopped')).text, /停止/);
});

test('never-run file and cached completion', () => {
    assert.match(present({}).text, /未準備/);
    const value = data();
    value.state.reused = true;
    assert.match(present(value).detail, /再利用/);
});

test('file mappings and snapshots match background job files', async t => {
    const root = await fs.mkdtemp(path.join(os.tmpdir(), 'submit-status-'));
    t.after(() => fs.rm(root, { recursive: true, force: true }));
    const source = path.join(await fs.realpath(root), 'answer with spaces.cpp');
    await fs.writeFile(source, 'SOURCE');
    const target = paths(source);
    await fs.mkdir(target.job, { recursive: true });
    await fs.writeFile(target.output, 'OUTPUT');
    await fs.writeFile(path.join(target.job, 'request.json'), JSON.stringify({ token: 't', source, source_digest: hash(Buffer.from('SOURCE')) }));
    await fs.writeFile(path.join(target.job, 'status.json'), JSON.stringify({ token: 't', phase: 'done', bytes: 6 }));
    await fs.writeFile(path.join(target.job, 'artifact.json'), JSON.stringify({ output_digest: hash(Buffer.from('OUTPUT')) }));
    assert.deepEqual(await selectFile(source), target);
    assert.deepEqual(await selectFile(target.output), target);
    assert.equal(await selectFile(path.join(root, 'tempCodeRunnerFile.cpp')), null);
    assert.equal(await selectFile(path.join(root, 'header.hpp')), null);
    assert.match(present(await snapshot(target)).text, /提出準備OK 6 B/);
    await fs.writeFile(source, 'CHANGED');
    assert.match(present(await snapshot(target)).text, /変更あり/);
});

test('status display and log viewing preserve editor focus and run commands', async t => {
    const root = await fs.mkdtemp(path.join(os.tmpdir(), 'submit-status-ui-'));
    t.after(() => fs.rm(root, { recursive: true, force: true }));
    const source = path.join(await fs.realpath(root), 'main.cpp');
    await fs.writeFile(source, 'SOURCE');
    const target = paths(source);
    await fs.mkdir(target.job, { recursive: true });
    await fs.writeFile(target.output, 'OUTPUT');
    await fs.writeFile(path.join(target.job, 'request.json'), JSON.stringify({ token: 't', source, source_digest: hash(Buffer.from('SOURCE')) }));
    await fs.writeFile(path.join(target.job, 'status.json'), JSON.stringify({ token: 't', phase: 'done', bytes: 6 }));
    await fs.writeFile(path.join(target.job, 'artifact.json'), JSON.stringify({ output_digest: hash(Buffer.from('OUTPUT')) }));
    await fs.writeFile(path.join(target.job, 'log.txt'), 'cleanup finished');
    const doc = { uri: { scheme: 'file', fsPath: source }, isDirty: false };
    const bar = { show() {}, dispose() {} };
    const output = { text: '', clear() { this.text = ''; }, append(text) { this.text += text; },
        show(preserveFocus) { this.preserveFocus = preserveFocus; }, dispose() {} };
    const handlers = {}, commands = {}, context = { subscriptions: [] };
    const event = name => callback => { handlers[name] = callback; return { dispose() {} }; };
    const api = {
        StatusBarAlignment: { Left: 1 }, ThemeColor: class { constructor(name) { this.name = name; } },
        window: { createStatusBarItem: () => bar, createOutputChannel: () => output,
            activeTextEditor: { document: doc }, onDidChangeActiveTextEditor: event('select') },
        workspace: { textDocuments: [doc], onDidChangeTextDocument: event('change'), onDidSaveTextDocument: event('save') },
        commands: { registerCommand: (name, callback) => { commands[name] = callback; return { dispose() {} }; } },
    };
    let tick;
    const module = { exports: {} };
    vm.runInNewContext(await fs.readFile(path.join(__dirname, 'extension.js'), 'utf8'), {
        module, require: name => name === 'vscode' ? api : require(name),
        setInterval: callback => { tick = callback; return 1; }, clearInterval() {},
    });
    module.exports.activate(context);
    t.after(() => context.subscriptions.forEach(item => item.dispose()));
    for (let attempt = 0; attempt < 100 && !bar.text?.includes('提出準備OK'); attempt++)
        await new Promise(resolve => setTimeout(resolve, 10));
    assert.match(bar.text, /提出準備OK/);
    await commands['kyoproSubmitStatus.showLog']();
    assert.equal(output.preserveFocus, true);
    assert.match(output.text, /cleanup finished/);
    assert.deepEqual(Object.keys(commands).sort(), ['kyoproSubmitStatus.openBundle', 'kyoproSubmitStatus.showLog']);
    doc.isDirty = true;
    await tick();
    assert.match(bar.text, /変更あり/);
    doc.isDirty = false;
    await fs.writeFile(source, 'SAVED EDIT');
    await tick();
    assert.match(bar.text, /変更あり/);
});
