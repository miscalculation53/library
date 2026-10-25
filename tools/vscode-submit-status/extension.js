const vscode = require('vscode');
const fs = require('node:fs/promises');
const path = require('node:path');
const { selectFile, snapshot, present } = require('./status');

function activate(context) {
    const bar = vscode.window.createStatusBarItem('kyoproSubmitStatus', vscode.StatusBarAlignment.Left, 50);
    bar.name = 'oj-b / cln 準備状況';
    bar.command = 'kyoproSubmitStatus.showLog';
    const output = vscode.window.createOutputChannel('oj-b / cln');
    let target = null, selection = 0, busy = false, disposed = false;
    let logOpen = false, previousLog = null;

    async function showLog() {
        if (!target) return;
        logOpen = true;
        await refreshLog(target);
        output.show(true); // Preserve editor focus; Code Runner can still use its terminal.
    }

    async function refreshLog(current) {
        if (!logOpen) return;
        let text;
        try { text = await fs.readFile(path.join(current.job, 'log.txt'), 'utf8'); }
        catch (error) { text = error.code === 'ENOENT' ? '生成ログはまだありません。' : error.message; }
        if (disposed || current !== target) return;
        text = `${current.source}\n\n${text}`;
        if (text !== previousLog) {
            output.clear();
            output.append(text);
            previousLog = text;
        }
    }

    async function refresh() {
        if (!target || busy || disposed) return;
        busy = true;
        const current = target;
        try {
            const data = await snapshot(current);
            if (disposed || target !== current) return;
            const view = present(data);
            bar.text = view.text;
            bar.tooltip = `${path.basename(current.source)} → ${path.basename(current.output)}\n${view.detail}\n\n${current.source}\nクリックするとログを表示します。表示中も Ctrl+Option+N で実行できます。`;
            bar.backgroundColor = view.error ? new vscode.ThemeColor('statusBarItem.errorBackground')
                : view.warning ? new vscode.ThemeColor('statusBarItem.warningBackground') : undefined;
            bar.show();
            await refreshLog(current);
        } catch (error) {
            if (!disposed && target === current) {
                bar.text = '$(warning) oj-b / cln: 状態を確認中';
                bar.tooltip = error.message;
                bar.backgroundColor = new vscode.ThemeColor('statusBarItem.warningBackground');
                bar.show();
            }
        } finally { busy = false; }
    }

    async function select(editor) {
        if (!editor || editor.document.uri.scheme !== 'file') return;
        const version = ++selection;
        try {
            const candidate = await selectFile(editor.document.uri.fsPath);
            if (disposed || version !== selection || !candidate
                || (candidate.source === target?.source && candidate.output === target?.output)) return;
            target = candidate;
            bar.text = '$(sync~spin) oj-b / cln: 確認中';
            bar.backgroundColor = undefined;
            bar.show();
            await refresh();
        } catch { /* Unsaved files have no background job yet. */ }
    }

    context.subscriptions.push(bar, output,
        vscode.commands.registerCommand('kyoproSubmitStatus.showLog', showLog),
        vscode.commands.registerCommand('kyoproSubmitStatus.openBundle', async () => {
            if (target) {
                try { await vscode.window.showTextDocument(vscode.Uri.file(target.output), { preview: true }); }
                catch { vscode.window.showInformationMessage('提出用コードはまだ出力されていません。'); }
            }
        }),
        vscode.window.onDidChangeActiveTextEditor(select),
        vscode.workspace.onDidSaveTextDocument(() => { void refresh(); }));
    const timer = setInterval(refresh, 500);
    context.subscriptions.push({ dispose() { disposed = true; clearInterval(timer); } });
    void select(vscode.window.activeTextEditor);
}

module.exports = { activate };
