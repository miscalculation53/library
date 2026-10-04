#!/usr/bin/env bash
set -euo pipefail

wip_root="$(git -C "$(dirname -- "${BASH_SOURCE[0]}")" rev-parse --show-toplevel)"
wip_branch="$(git -C "$wip_root" branch --show-current)"
if [[ "$wip_branch" != wip ]]; then
  printf 'library の wip ブランチで実行してください（現在: %s）。\n' "${wip_branch:-detached HEAD}" >&2
  exit 1
fi

for wip_direction in fetch push; do
  if [[ "$wip_direction" == push ]]; then
    wip_remote="$(git -C "$wip_root" remote get-url --push --all origin)"
  else
    wip_remote="$(git -C "$wip_root" remote get-url --all origin)"
  fi
  case "$wip_remote" in
    git@github.com:miscalculation53/library.git|https://github.com/miscalculation53/library.git) ;;
    *)
      printf 'origin の %s 先を確認してください: %s\n' "$wip_direction" "$wip_remote" >&2
      exit 1
      ;;
  esac
done

if command -v codex >/dev/null 2>&1; then
  wip_codex="$(command -v codex)"
elif [[ -x /Applications/ChatGPT.app/Contents/Resources/codex ]]; then
  wip_codex=/Applications/ChatGPT.app/Contents/Resources/codex
elif [[ -x /Applications/Codex.app/Contents/Resources/codex ]]; then
  wip_codex=/Applications/Codex.app/Contents/Resources/codex
else
  printf 'Codex CLI をインストールし、codex login を実行してください。\n' >&2
  exit 1
fi

"$wip_codex" exec --approve-for-me --cd "$wip_root" - <<'PROMPT'
library の現在の WIP を commit・push してください。
このコマンドの実行は、現在の変更一式を git@github.com:miscalculation53/library.git
（同じリポジトリの HTTPS URL も可）の wip ブランチへ保存する依頼です。

1. AGENTS.md、git status、origin の fetch/push URL、現在のブランチを確認する。
   対象はこの library の wip と origin/wip のみ。ブランチや送信先が異なれば停止する。
   既存の merge/rebase/cherry-pick/revert の途中なら、状態と必要な対応を報告して停止する。
2. 差分を確認し、通常のソース・文書・テスト・計測結果の変更、新規ファイル、削除を
   git add -A でまとめ、変更があればメッセージ wip で commit する。
   認証情報などの機密情報を検出した場合は、その値を表示せず対象ファイルを報告して停止する。
   WIP の保存なので、既存の実装を改善したり全テストを実行したりする必要はない。
3. origin を fetch し、origin/wip の更新があれば merge で取り込む。
   分岐している場合は、両側の commit を維持する merge を使う。
   競合時は共通祖先・手元・リモートを比較し、双方の意図を保つ最小限の編集で統合する。
   競合箇所に関係する検証を実行してから、merge commit を作る。
   仕様上の判断が必要な競合は、今回開始した merge を abort して、保存済み commit と
   判断が必要な箇所を報告して停止する。今回以外の操作は abort しない。
4. git push origin HEAD:refs/heads/wip で push する。
   リモートが先に更新されたことによる拒否なら、再度 fetch・merge して一度だけ再試行する。
   認証・通信・承認審査の失敗は理由を報告する。審査の拒否は迂回しない。
5. リモートの wip と HEAD の一致、作業ツリーの状態を確認し、結果を日本語で短く伝える。

force push、reset --hard、git clean、機械的な ours/theirs の一括採用は禁止する。
リポジトリ設定・Git の認証設定・権限設定は変更しない。
実行中に別の作業で生じた変更を上書きせず、取り込みに支障があれば保存済みの状態で停止する。
PROMPT

# Codex の説明だけでなく、実際に保存された状態から終了コードを決める。
wip_remote_head="$(git -C "$wip_root" ls-remote --exit-code origin refs/heads/wip)"
wip_local_head="$(git -C "$wip_root" rev-parse HEAD)"
if [[ "${wip_remote_head%%$'\t'*}" != "$wip_local_head" ]] ||
   [[ -n "$(git -C "$wip_root" status --porcelain)" ]]; then
  printf '保存が未完了です。上の Codex の説明を確認してください。\n' >&2
  exit 1
fi
