[miscalc](https://x.com/miscalc53) のライブラリです。現在移植作業中

他の人が使いやすいものにはあまりならないかもしれません

---

verification helper についての備忘録：
- https://github.com/online-judge-tools/verification-helper/issues/411
- https://qiita.com/KakurenboUni/items/b47d9e6e3582e2149d63
- https://maspypy.github.io/library/
- https://zenn.dev/harui/articles/ec9ad6a3d3e7ce
  - 手元で yukicoder の問題を verify したいときは `export YUKICODER_TOKEN='(ここにAPIキーを入れる)'`
- コミットする前は exec ファイルを消す（消さないとファイルがでかすぎると怒られる）。もしこれを忘れてしまって怒られたら `git log` で履歴を見る → `git reset --soft (戻したいコミットのid)`
- ドキュメントを書くとき
  - `|` を使うと表と解釈されてやばいことになる。数式では `\lvert`, `\rvert` を使う
  - `{ }` も `\lbrace`, `\rbrace` を使うのが無難
    - プログラム中の `{ {` が間を開けないと表示がバグったことがある
  - 複数行にわたる数式を書きたいとき：`$\begin{aligned} \end{aligned}$` で挟み、改行は `\\\\  `（バックスラッシュ $4$ つ + 半角スペース $2$ つ）を使う
  - 参考リンク：
    - https://marshmallow444.github.io/tech_blog/2021/07/29/tex-with-github-pages.html
    - https://qiita.com/BurnEtz/items/e79999264125eb128ae7