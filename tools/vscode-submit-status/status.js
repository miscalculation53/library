const fs = require('node:fs/promises');
const path = require('node:path');
const crypto = require('node:crypto');

const hash = data => data === null ? null : crypto.createHash('sha256').update(data).digest('hex');
const fields = 'source=(?<source>[0-9a-f]{64}) input=(?<input>[0-9a-f]{64}) body=(?<body>[0-9a-f]{64})';
const header = new RegExp('^// submit-code: ' + fields + '\\n');
const footer = new RegExp('\\n\\n// submit-code: hash=(?<hash>[0-9a-f]{12}) ' + fields + '\\n', 'g');

function readStamp(output) {
    if (output === null) return null;
    const text = output.toString('utf8');
    let match = [...text.matchAll(footer)].at(-1), body;
    const footerAtEnd = !match || match.index + match[0].length === text.length;
    if (match) body = output.subarray(0, Buffer.byteLength(text.slice(0, match.index)));
    else {
        match = header.exec(text);
        if (match) body = output.subarray(Buffer.byteLength(match[0]));
    }
    if (!match) return null;
    const stamp = match.groups;
    return { source: stamp.source, input: stamp.input, body: stamp.body,
        bodyMatches: footerAtEnd && hash(body) === stamp.body && (!stamp.hash || stamp.hash === stamp.source.slice(0, 12)) };
}

async function read(file) {
    try { return await fs.readFile(file); }
    catch (error) { if (error.code === 'ENOENT') return null; throw error; }
}

async function json(file) {
    const data = await read(file);
    return data === null ? null : JSON.parse(data.toString());
}

function paths(source, output) {
    const name = path.basename(source);
    const outputName = name === 'main.cpp' ? 'bundle.cpp' : path.parse(name).name + '-bundle.cpp';
    output ??= path.join(path.dirname(source), outputName);
    return { source, output, job: path.join(path.dirname(output), '.submit-code', path.basename(output)) };
}

async function selectFile(file) {
    if (!/\.(cpp|cc|cxx|C)$/.test(file) || path.basename(file).startsWith('tempCodeRunnerFile.')
        || file.split(path.sep).includes('.submit-code')) return null;
    file = await fs.realpath(file);
    const job = path.join(path.dirname(file), '.submit-code', path.basename(file));
    const record = await json(path.join(job, 'comments.json'));
    if (record?.source && record.source !== file) return paths(await fs.realpath(record.source), file);
    if (path.basename(file) === 'bundle.cpp' || path.basename(file).endsWith('-bundle.cpp')) {
        const request = await json(path.join(job, 'request.json'));
        if (request?.source) return paths(await fs.realpath(request.source), file);
        const stem = path.basename(file) === 'bundle.cpp' ? 'main' : path.basename(file).slice(0, -11);
        for (const extension of ['.cpp', '.cc', '.cxx', '.C']) {
            const source = path.join(path.dirname(file), stem + extension);
            if (await read(source) !== null) return paths(await fs.realpath(source), file);
        }
        return null;
    }
    return paths(file);
}

async function snapshot(target, edits = new Map()) {
    const current = file => edits.has(file) ? edits.get(file) : read(file);
    const [record, source, output, state, request, artifact] = await Promise.all([
        json(path.join(target.job, 'comments.json')), current(target.source), current(target.output),
        json(path.join(target.job, 'status.json')), json(path.join(target.job, 'request.json')),
        json(path.join(target.job, 'artifact.json')),
    ]);
    const stamp = readStamp(output);
    const inputs = record?.inputs;
    const hasInputs = inputs && typeof inputs === 'object' && !Array.isArray(inputs);
    const recordMatches = !record || (hasInputs && record.input_digest === stamp?.input
        && record.source === target.source && inputs[target.source] === stamp?.source);
    const changedInputs = [];
    if (stamp && recordMatches && hasInputs) {
        await Promise.all(Object.entries(inputs).map(async ([file, expected]) => {
            const actual = file === target.source ? source : await current(file);
            if (hash(actual) !== expected) changedInputs.push(file);
        }));
    }
    return { source: target.source, sourceName: path.basename(target.source), sourceHash: hash(source), stamp,
        state, request, artifact, outputHash: hash(output),
        actualBytes: output?.length ?? null, recordMatches, changedInputs };
}

function agreement(data) {
    const { sourceName = 'main.cpp', sourceHash, stamp, actualBytes, recordMatches, changedInputs = [] } = data;
    if (actualBytes === null || actualBytes === undefined) return {
        text: `$(circle-outline) ${sourceName}: 未生成`, detail: 'Ctrl+Option+N で実行すると提出用コードを生成します。',
    };
    if (!stamp) return { text: '$(warning) 照合情報なし', warning: true,
        detail: 'Ctrl+Option+N または oj-b で再生成すると、元コードとの対応を自動で確認できます。' };
    if (sourceHash !== stamp.source) return { text: `$(warning) ${sourceName} と不一致`, warning: true,
        detail: `現在の ${sourceName} と生成時の内容が異なります。Ctrl+Option+N で再生成してください。` };
    if (!stamp.bodyMatches) return { text: '$(warning) 生成本文に変更あり', warning: true,
        detail: '提出用コードが生成後に編集されています。Ctrl+Option+N で再生成してください。' };
    if (!recordMatches) return { text: '$(warning) 照合情報を確認中', warning: true,
        detail: '生成本文とヘッダの記録が切り替わっています。表示が続く場合は再生成してください。' };
    if (changedInputs.length) return { text: '$(warning) ヘッダに変更あり', warning: true,
        detail: `参照ヘッダが生成時から変更されています。Ctrl+Option+N で再生成してください。\n${changedInputs.join('\n')}` };
    const size = actualBytes.toLocaleString('en-US') + ' B';
    return { text: `$(check) ${sourceName} と一致 ${size}`, detail:
        `現在の ${sourceName} と生成元のハッシュが一致し、生成本文のハッシュも一致しています。`
        + (actualBytes >= 65536 ? '\nサイズ制限 65,536 B 以上です。' : '') };
}

function present(data) {
    const view = agreement(data);
    const { state, request, artifact, source, sourceHash, outputHash, actualBytes } = data;
    const currentHash = sourceHash?.slice(0, 12), bundledHash = data.stamp?.source.slice(0, 12);
    const identifier = currentHash && bundledHash && currentHash !== bundledHash
        ? `${data.sourceName || 'main.cpp'} #${currentHash} / bundle #${bundledHash}`
        : `#${bundledHash || currentHash || ''}`;
    if (currentHash || bundledHash) view.text += ` | ${identifier}`;
    if (sourceHash) view.detail += `\n元コード SHA-256: ${sourceHash}`;
    if (data.stamp) view.detail += `\n生成元 SHA-256: ${data.stamp.source}`;
    if (!state || !request?.token || request.source !== source) return view;
    if (state.token !== request.token) return {
        ...view, text: `$(sync~spin) 切替中 | ${view.text}`,
        detail: `最新の生成要求に切り替えています。\n${view.detail}`,
    };
    const stages = {
        queued: ['bundle 開始待ち', '生成処理の開始を待っています。'],
        expanding: ['bundle 中', 'インクルードを展開しています。'],
        cleaning: ['clean 中', '未使用コードの短縮を進めています。'],
    };
    if (stages[state.phase]) {
        const [label, detail] = stages[state.phase];
        return { ...view, text: `$(sync~spin) ${label} | ${view.text}`, detail: `${detail}\n${view.detail}` };
    }
    // Existing jobs may have recorded a normal edit interruption as a failure.
    const legacyInterruption = state.phase === 'failed'
        && /^(元コードが変更されたため中止|提出用ファイルが別の操作で変更されたため中止)/.test(state.error || '');
    if (state.phase === 'interrupted' || legacyInterruption) return {
        ...view, text: `$(debug-pause) 編集により中断 | ${view.text}`,
        detail: `${state.message || state.error || '編集中の内容を保護するため生成を中断しました。'}\nCtrl+Option+N で最新コードから再生成します。\n${view.detail}`,
    };
    if (state.phase === 'failed') return {
        ...view, error: true, text: `$(error) 生成エラー | ${view.text}`,
        detail: `${state.error || '生成に失敗しました。ログを確認してください。'}\n${view.detail}`,
    };
    if (state.phase === 'stopped') return {
        ...view, text: `$(debug-stop) 停止 | ${view.text}`, detail: `生成を停止しました。\n${view.detail}`,
    };
    // Completion belongs to the current output only. A later manual expansion
    // can have the same source hash while producing a different, uncleaned body.
    if (state.phase === 'done' && !view.warning && sourceHash === request.source_digest
        && typeof outputHash === 'string' && outputHash === artifact?.output_digest && actualBytes === state.bytes) return {
        ...view, text: `clean 完了 | ${view.text}`,
        detail: `短縮が完了しました。${state.reused ? '前回の生成結果を再利用しました。' : ''}\n${view.detail}`,
    };
    return view;
}

module.exports = { hash, paths, selectFile, snapshot, present };
