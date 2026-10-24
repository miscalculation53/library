const { test } = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs/promises');
const os = require('node:os');
const path = require('node:path');
const vm = require('node:vm');
const { spawnSync } = require('node:child_process');
const { createRequire } = require('node:module');
const extensionPath = path.join(__dirname, '..', 'vscode-submit-status', 'extension.js');
const extensionRequire = createRequire(extensionPath);
const { hash, paths, selectFile, snapshot, present } = extensionRequire('./status');

function data(bytes = 42000) {
    return { source: '/contest/main.cpp', sourceName: 'main.cpp', sourceHash: 'source', actualBytes: bytes, recordMatches: true,
        stamp: { source: 'source', bodyMatches: true }, changedInputs: [] };
}

function job(phase = 'cleaning') {
    const value = data();
    return { ...value, state: { token: 'new', phase, bytes: value.actualBytes },
        request: { token: 'new', source: value.source, source_digest: value.sourceHash },
        artifact: { output_digest: 'output' }, outputHash: 'output' };
}

async function fixture(t, name = 'main.cpp', outputName) {
    const temporary = await fs.mkdtemp(path.join(os.tmpdir(), 'submit-status-'));
    t.after(() => fs.rm(temporary, { recursive: true, force: true }));
    const root = await fs.realpath(temporary);
    const source = path.join(root, name);
    const dependency = path.join(root, 'dependency.hpp');
    const sourceText = '#include "dependency.hpp"\n// 解法メモ\nint main() { return value; }\n';
    const dependencyText = '#pragma once\n/*! Header license. */\nint value = 0;\n';
    await fs.writeFile(source, sourceText);
    await fs.writeFile(dependency, dependencyText);
    const target = paths(source, outputName ? path.join(root, outputName) : undefined);
    function generate() {
        const result = spawnSync('python3', [path.join(__dirname, '..', 'submit_code.py'),
            'expand', source, '-o', target.output, '--no-fallback'], { encoding: 'utf8', timeout: 10000 });
        assert.equal(result.status, 0, result.stderr);
    }
    generate();
    return { root, source, dependency, sourceText, dependencyText, target, generate };
}

test('automatic display reports agreement independently of generation stages or byte limit', () => {
    assert.match(present(data(65535)).text, /main.cpp と一致 65,535 B/);
    assert.match(present(data(65536)).text, /main.cpp と一致 65,536 B/);
    assert.match(present(data(65536)).detail, /サイズ制限/);
    assert.match(present({}).text, /未生成/);
    assert.match(present({ actualBytes: 10 }).text, /照合情報なし/);
    assert.match(present({ ...data(), sourceHash: null }).text, /main.cpp と不一致/);
});

test('bundle and clean progress remain visible alongside source agreement', () => {
    for (const [phase, label] of [['queued', 'bundle 開始待ち'], ['expanding', 'bundle 中'], ['cleaning', 'clean 中']]) {
        const value = job(phase);
        assert.match(present(value).text, /sync~spin/);
        assert.ok(present(value).text.includes(label));
        assert.match(present(value).text, /main.cpp と一致/);
        assert.match(present({ ...value, sourceHash: 'unsaved edit' }).text, /main.cpp と不一致/);
        assert.ok(present({ ...value, stamp: null, actualBytes: null }).text.includes(label));
    }
});

test('completion belongs only to the current matching output and generation', () => {
    const value = job('done');
    assert.match(present(value).text, /clean 完了.*main.cpp と一致/);
    value.state.reused = true;
    assert.match(present(value).detail, /再利用/);
    for (const changed of [
        { outputHash: 'later manual expansion' }, { artifact: null }, { outputHash: null },
        { actualBytes: 1 }, { sourceHash: 'edited source' }, { changedInputs: ['edited.hpp'] },
    ]) assert.doesNotMatch(present({ ...value, ...changed }).text, /clean 完了/);
});

test('stale generation phases cannot describe a newer request or another source', () => {
    const value = job();
    value.state.token = 'old';
    assert.match(present(value).text, /切替中.*main.cpp と一致/);
    assert.doesNotMatch(present(value).text, /clean 中/);
    value.request.source = '/another/main.cpp';
    assert.doesNotMatch(present(value).text, /切替中|clean 中/);
});

test('failure and stop are visible even when a matching checkpoint exists', () => {
    const failed = job('failed');
    failed.state.error = 'compiler failed';
    assert.match(present(failed).text, /生成エラー.*main.cpp と一致/);
    assert.equal(present(failed).error, true);
    assert.match(present(failed).detail, /compiler failed/);
    assert.match(present(job('stopped')).text, /停止.*main.cpp と一致/);
});

test('editing interruptions are distinct from generation failures, including existing jobs', () => {
    for (const state of [
        { phase: 'interrupted', reason: 'source_changed', message: '元コードの編集により中断しました。' },
        { phase: 'interrupted', reason: 'output_changed', message: '提出用ファイルの編集により中断しました。' },
        { phase: 'failed', error: '元コードが変更されたため中止しました。次の実行で再生成します。' },
        { phase: 'failed', error: '提出用ファイルが別の操作で変更されたため中止しました。' },
    ]) {
        const value = job(state.phase);
        Object.assign(value.state, state);
        const view = present({ ...value, sourceHash: 'edited source' });
        assert.match(view.text, /編集により中断.*main.cpp と不一致/);
        assert.doesNotMatch(view.text, /生成エラー/);
        assert.ok(!view.error);
        assert.match(view.detail, /Ctrl\+Option\+N/);
        assert.ok(view.detail.includes(state.message || state.error));
    }
});

test('manual expansion is automatically checked with no background job or command', async t => {
    const { target, sourceText } = await fixture(t);
    const content = await fs.readFile(target.output, 'utf8');
    const lastLine = content.trimEnd().split('\n').at(-1);
    const identifier = hash(Buffer.from(sourceText)).slice(0, 12);
    assert.match(lastLine, /^\/\/ submit-code: hash=[a-f0-9]{12} source=[a-f0-9]{64} input=[a-f0-9]{64} body=[a-f0-9]{64}$/);
    assert.ok(lastLine.includes('hash=' + identifier));
    assert.ok(!content.split('\n')[0].includes('submit-code:'));
    assert.equal((await snapshot(target)).stamp.source, hash(Buffer.from(sourceText)));
    assert.match(present(await snapshot(target)).text, /main.cpp と一致/);
    assert.ok(present(await snapshot(target)).text.includes('#' + identifier));
});

test('saved source edits and reverting them update agreement automatically', async t => {
    const { target, source, sourceText } = await fixture(t);
    await fs.writeFile(source, sourceText.replace('return value;', 'return value + 1;'));
    assert.match(present(await snapshot(target)).text, /main.cpp と不一致/);
    await fs.writeFile(source, sourceText);
    assert.match(present(await snapshot(target)).text, /main.cpp と一致/);
});

test('external header edits, removal and reverting are detected without a new run', async t => {
    const { target, dependency, dependencyText } = await fixture(t);
    await fs.writeFile(dependency, dependencyText.replace('value = 0', 'value = 1'));
    assert.match(present(await snapshot(target)).text, /ヘッダに変更あり/);
    await fs.rm(dependency);
    assert.match(present(await snapshot(target)).text, /ヘッダに変更あり/);
    await fs.writeFile(dependency, dependencyText);
    assert.match(present(await snapshot(target)).text, /main.cpp と一致/);
});

test('unsaved editor content is compared for the source, generated code and referenced headers', async t => {
    const { root, target, source, sourceText, dependency, dependencyText } = await fixture(t);
    const originalOutput = await fs.readFile(target.output);
    for (const [file, text, expected] of [
        [source, sourceText + '// unsaved', /main.cpp と不一致/],
        [target.output, originalOutput.toString() + '// unsaved', /生成本文に変更あり/],
        [dependency, dependencyText + '// unsaved', /ヘッダに変更あり/],
    ]) {
        const edits = new Map([[file, Buffer.from(text)]]);
        assert.match(present(await snapshot(target, edits)).text, expected);
    }
    const edits = new Map([[source, Buffer.from(sourceText)], [path.join(root, 'unrelated.hpp'), Buffer.from('edit')]]);
    assert.match(present(await snapshot(target, edits)).text, /main.cpp と一致/);
});

test('manually changed or deleted generated code is detected automatically', async t => {
    const { target } = await fixture(t);
    await fs.appendFile(target.output, '// edit\n');
    assert.match(present(await snapshot(target)).text, /生成本文に変更あり/);
    await fs.rm(target.output);
    assert.match(present(await snapshot(target)).text, /未生成/);
});

test('editing the displayed identifier is detected even when the generated body is unchanged', async t => {
    const { target } = await fixture(t);
    const content = await fs.readFile(target.output, 'utf8');
    const modified = content.replace(/hash=[0-9a-f]{12}/, 'hash=000000000000');
    assert.notEqual(modified, content);
    await fs.writeFile(target.output, modified);
    assert.match(present(await snapshot(target)).text, /生成本文に変更あり/);
});

test('replaying old output against newer input records never reports agreement', async t => {
    const { target, dependency, dependencyText, generate } = await fixture(t);
    const oldOutput = await fs.readFile(target.output);
    await fs.writeFile(dependency, dependencyText.replace('value = 0', 'value = 1'));
    generate();
    assert.match(present(await snapshot(target)).text, /main.cpp と一致/);
    await fs.writeFile(target.output, oldOutput);
    assert.match(present(await snapshot(target)).text, /照合情報を確認中/);
});

test('file mappings support generated code, custom names, spaces and files without background jobs', async t => {
    const { source, target, root } = await fixture(t, 'answer with spaces.cpp');
    assert.deepEqual(await selectFile(source), target);
    assert.deepEqual(await selectFile(target.output), target);
    assert.equal(await selectFile(path.join(root, 'tempCodeRunnerFile.cpp')), null);
    assert.equal(await selectFile(path.join(root, 'header.hpp')), null);
    const custom = await fixture(t, 'solution.cc', 'submission.cpp');
    assert.deepEqual(await selectFile(custom.target.output), custom.target);
    const main = await fixture(t);
    await fs.rm(path.join(main.target.job, 'comments.json'));
    assert.deepEqual(await selectFile(main.target.output), main.target);
    assert.match(present(await snapshot(main.target)).text, /main.cpp と一致/);
});

test('background job control files do not override current source and body hashes', async t => {
    const { source, target, sourceText } = await fixture(t);
    await fs.writeFile(path.join(target.job, 'request.json'), JSON.stringify({ source, source_digest: hash(Buffer.from(sourceText)) }));
    await fs.writeFile(path.join(target.job, 'status.json'), JSON.stringify({ phase: 'failed', error: 'old failure' }));
    await fs.writeFile(path.join(target.job, 'artifact.json'), JSON.stringify({ output_digest: 'old' }));
    assert.match(present(await snapshot(target)).text, /main.cpp と一致/);
    await fs.writeFile(source, sourceText + '// changed\n');
    assert.match(present(await snapshot(target)).text, /main.cpp と不一致/);
});

test('background progress is read from current job files without changing generated content', async t => {
    const { source, target, sourceText } = await fixture(t);
    const original = await fs.readFile(target.output);
    const request = { token: 'current', source, source_digest: hash(Buffer.from(sourceText)) };
    await fs.writeFile(path.join(target.job, 'request.json'), JSON.stringify(request));
    for (const [phase, expected] of [['queued', /bundle 開始待ち/], ['expanding', /bundle 中/], ['cleaning', /clean 中/]]) {
        await fs.writeFile(path.join(target.job, 'status.json'), JSON.stringify({ token: request.token, phase }));
        const view = present(await snapshot(target));
        assert.match(view.text, expected);
        assert.match(view.text, /main.cpp と一致/);
    }
    await fs.writeFile(path.join(target.job, 'status.json'), JSON.stringify({ token: request.token, phase: 'done', bytes: original.length }));
    await fs.writeFile(path.join(target.job, 'artifact.json'), JSON.stringify({ output_digest: hash(original) }));
    assert.match(present(await snapshot(target)).text, /clean 完了/);
    assert.deepEqual(await fs.readFile(target.output), original);
    assert.ok(!original.toString().trimEnd().split('\n').at(-1).includes('state='));
});

test('source and generated identifiers are both visible when they differ', async t => {
    const { target, source, sourceText } = await fixture(t);
    const edited = sourceText + '// edited\n';
    await fs.writeFile(source, edited);
    const view = present(await snapshot(target));
    assert.match(view.text, /main.cpp と不一致/);
    assert.ok(view.text.includes('main.cpp #' + hash(Buffer.from(edited)).slice(0, 12)));
    assert.ok(view.text.includes('bundle #' + hash(Buffer.from(sourceText)).slice(0, 12)));
});

test('existing first-line stamps retain automatic agreement and the same identifier', async t => {
    const { target } = await fixture(t);
    const bytes = await fs.readFile(target.output);
    const record = (await snapshot(target)).stamp;
    const footerIndex = bytes.toString().lastIndexOf('\n\n// submit-code:');
    const old = `// submit-code: source=${record.source} input=${record.input} body=${record.body}\n`
        + bytes.subarray(0, Buffer.byteLength(bytes.toString().slice(0, footerIndex))).toString();
    await fs.writeFile(target.output, old);
    const view = present(await snapshot(target));
    assert.match(view.text, /main.cpp と一致/);
    assert.ok(view.text.includes('#' + record.source.slice(0, 12)));
});

test('typing preserves saved hashes while saving and generation update the display', async t => {
    const { source, target, sourceText, dependency, dependencyText, generate } = await fixture(t);
    await fs.writeFile(path.join(target.job, 'log.txt'), 'cleanup finished');
    let editorText = sourceText;
    const doc = { uri: { scheme: 'file', fsPath: source }, isDirty: false, getText: () => editorText };
    const outputDoc = { uri: { scheme: 'file', fsPath: target.output }, isDirty: true,
        getText: () => '// unsaved output' };
    const headerDoc = { uri: { scheme: 'file', fsPath: dependency }, isDirty: true,
        getText: () => dependencyText + '// unsaved header' };
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
    vm.runInNewContext(await fs.readFile(extensionPath, 'utf8'), {
        module, Buffer, require: name => name === 'vscode' ? api : extensionRequire(name),
        setInterval: callback => { tick = callback; return 1; }, clearInterval() {},
    });
    module.exports.activate(context);
    t.after(() => context.subscriptions.forEach(item => item.dispose()));
    async function waitFor(pattern) {
        for (let attempt = 0; attempt < 100 && !pattern.test(bar.text); attempt++)
            await new Promise(resolve => setTimeout(resolve, 10));
        assert.match(bar.text, pattern);
    }
    await waitFor(/main.cpp と一致/);
    await commands['kyoproSubmitStatus.showLog']();
    assert.equal(output.preserveFocus, true);
    assert.match(output.text, /cleanup finished/);
    assert.deepEqual(Object.keys(commands).sort(), ['kyoproSubmitStatus.openBundle', 'kyoproSubmitStatus.showLog']);
    const initialText = bar.text;
    const initialTooltip = bar.tooltip;
    api.workspace.textDocuments.push(outputDoc, headerDoc);
    doc.isDirty = true;
    editorText = sourceText + '// unsaved';
    handlers.change?.({ document: doc });
    await tick();
    assert.equal(bar.text, initialText);
    assert.equal(bar.tooltip, initialTooltip);
    await handlers.select({ document: doc });
    await tick();
    assert.equal(bar.text, initialText);

    doc.isDirty = false;
    await fs.writeFile(source, editorText);
    handlers.save(doc);
    await waitFor(/main.cpp と不一致/);
    const savedHash = hash(Buffer.from(editorText));
    assert.ok(bar.text.includes('main.cpp #' + savedHash.slice(0, 12)));
    assert.ok(bar.text.includes('bundle #' + hash(Buffer.from(sourceText)).slice(0, 12)));
    const savedText = bar.text;
    doc.isDirty = true;
    editorText += '// more typing';
    handlers.change?.({ document: doc });
    await tick();
    assert.equal(bar.text, savedText);

    const request = { token: 'current', source, source_digest: savedHash };
    await fs.writeFile(path.join(target.job, 'request.json'), JSON.stringify(request));
    await fs.writeFile(path.join(target.job, 'status.json'), JSON.stringify({ token: request.token, phase: 'cleaning' }));
    await tick();
    assert.match(bar.text, /clean 中.*main.cpp と不一致/);
    assert.ok(bar.text.includes('main.cpp #' + savedHash.slice(0, 12)));
    generate();
    const generated = await fs.readFile(target.output);
    await fs.writeFile(path.join(target.job, 'artifact.json'), JSON.stringify({ output_digest: hash(generated) }));
    await fs.writeFile(path.join(target.job, 'status.json'), JSON.stringify({ token: request.token, phase: 'done', bytes: generated.length }));
    await tick();
    assert.match(bar.text, /clean 完了.*main.cpp と一致/);
    assert.ok(bar.text.includes('#' + savedHash.slice(0, 12)));

    await fs.writeFile(source, sourceText);
    await tick();
    assert.match(bar.text, /main.cpp と不一致/);
    assert.ok(bar.text.includes('main.cpp #' + hash(Buffer.from(sourceText)).slice(0, 12)));
    assert.ok(bar.text.includes('bundle #' + savedHash.slice(0, 12)));
});
