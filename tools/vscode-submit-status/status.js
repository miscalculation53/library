const fs = require('node:fs/promises');
const path = require('node:path');
const crypto = require('node:crypto');

const hash = data => data === null ? null : crypto.createHash('sha256').update(data).digest('hex');

async function read(file) {
    try { return await fs.readFile(file); }
    catch (error) { if (error.code === 'ENOENT') return null; throw error; }
}

async function json(file) {
    const data = await read(file);
    return data === null ? null : JSON.parse(data.toString());
}

function paths(source) {
    const name = path.basename(source);
    const outputName = name === 'main.cpp' ? 'bundle.cpp' : path.parse(name).name + '-bundle.cpp';
    return { source, output: path.join(path.dirname(source), outputName),
        job: path.join(path.dirname(source), '.submit-code', outputName) };
}

async function selectFile(file) {
    if (!/\.(cpp|cc|cxx|C)$/.test(file) || path.basename(file).startsWith('tempCodeRunnerFile.')
        || file.split(path.sep).includes('.submit-code')) return null;
    file = await fs.realpath(file);
    if (path.basename(file) === 'bundle.cpp' || path.basename(file).endsWith('-bundle.cpp')) {
        const request = await json(path.join(path.dirname(file), '.submit-code', path.basename(file), 'request.json'));
        return request?.source ? paths(request.source) : null;
    }
    return paths(file);
}

async function snapshot(target) {
    const [state, request, artifact, source, output] = await Promise.all([
        json(path.join(target.job, 'status.json')), json(path.join(target.job, 'request.json')),
        json(path.join(target.job, 'artifact.json')), read(target.source), read(target.output),
    ]);
    return { state, request, artifact, sourceHash: hash(source), outputHash: hash(output),
        actualBytes: output?.length ?? null };
}

function present(data, dirty = false, headerChanged = false) {
    const { state, request, artifact, sourceHash, outputHash, actualBytes } = data;
    if (!state || !request) return { text: '$(circle-outline) oj-b / cln: 未準備',
        detail: 'Ctrl+Option+N で実行すると、提出用コードの生成も始まります。' };
    if (dirty || headerChanged || sourceHash !== request.source_digest) return {
        text: '$(edit) oj-b / cln: 変更あり', warning: true,
        detail: 'コードに変更があります。Ctrl+Option+N で保存・実行し、提出用コードを再生成してください。',
    };
    if (state.token !== request.token) return { text: '$(sync~spin) oj-b / cln: 切替中', detail: '最新の実行要求に切り替えています。' };
    const validSize = Number.isInteger(state.bytes) && state.bytes >= 0;
    const checked = validSize && actualBytes === state.bytes && outputHash !== null
        && outputHash === artifact?.output_digest;
    const size = validSize ? state.bytes.toLocaleString('en-US') + ' B' : '';
    if (validSize && !checked) return { text: '$(warning) oj-b / cln: 出力を確認', warning: true,
        detail: '出力が削除・変更されたか、更新中です。状態が変わらない場合は再実行してください。' };
    const ready = checked && state.bytes < 65536;
    switch (state.phase) {
    case 'queued': return { text: '$(sync~spin) oj-b: 開始待ち', detail: '生成処理の開始を待っています。' };
    case 'expanding': return { text: '$(sync~spin) oj-b: 展開中', detail: 'ローカルのインクルードを展開しています。' };
    case 'cleaning': return ready
        ? { text: `$(check) 提出準備OK ${size} $(sync~spin)`, detail: '65,536 B 未満の中間結果を出力済みです。cln はさらに短縮を続けています。' }
        : { text: `$(sync~spin) cln: 短縮中${size ? ' ' + size : ''}`, detail: '未使用コードを短縮しています。目標は 65,536 B 未満です。' };
    case 'done': return ready
        ? { text: `$(check) 提出準備OK ${size}`, detail: `短縮が完了しました。${state.reused ? '前回と同じ展開結果のため、生成済みのコードを再利用しました。' : ''}` }
        : { text: `$(warning) cln: サイズ制限未達 ${size}`, warning: true, detail: '短縮は完了しましたが、65,536 B 未満になっていません。' };
    case 'failed': return { text: '$(error) oj-b / cln: エラー', error: true, detail: state.error || '生成に失敗しました。クリックしてログを確認できます。' };
    case 'stopped': return { text: '$(debug-stop) oj-b / cln: 停止', detail: '生成を停止しました。最後に出力したファイルは残っています。' };
    default: return { text: '$(warning) oj-b / cln: 状態不明', warning: true, detail: 'クリックしてログを確認できます。' };
    }
}

module.exports = { hash, paths, selectFile, snapshot, present };
