[miscalc](https://x.com/miscalc53) の競プロ用 C++ ライブラリです。

- 現在移植作業中なのでいろいろなものが足りていないです。
- 利用は自由です。
- verify したりテストを書いたりするようにはしているつもりですが、バグっていたらごめんなさい。
- 「『自分が』『競技で』使いやすい」ことを第一に作っているため、
  - 意識的または無意識にお行儀の悪いコードがあると思います。
  - 他の人が使いにくい場合があるかもしれません。
    - テンプレも含めてコピペしないと動かないことが多いです。
    - ACL と併用する場合は `using namespace atcoder;` をせずに毎回 `atcoder::` を書いてください。（名前空間周りは後でどうにかしたいかも）

---

verification helper についての備忘録：
- https://github.com/online-judge-tools/verification-helper/issues/411
- https://qiita.com/KakurenboUni/items/b47d9e6e3582e2149d63
- https://maspypy.github.io/library/
- https://zenn.dev/harui/articles/ec9ad6a3d3e7ce
  - 手元で yukicoder の問題を verify したいときは `export YUKICODER_TOKEN='(ここにAPIキーを入れる)'`
- https://kaage.hatenablog.com/entry/2020/12/18/162553
  - これをすると 10 分でタイムアウトするやつがなくなって便利
  - と思ったが `timeout=6000` でやったらそれより前に 40 分ほどで fail してしまった（謎）
  - `timeout` をデフォルトの 10 分にしておいて、`for i in $(seq 10); do git pull && git commit --allow-empty -m "." && git push; sleep 900; done` で 15 分ごとに自動コミットしてプッシュする
- コミットする前は exec, pch などの大きいファイルをコミットしないようにする必要がある。これは .gitignore で対応している（.verify-helper 下の .gitignore ではできないので注意）
  - （以下は .gitignore ができないと勘違いしていたときのメモ）
    - `find . -type f -name "exec"` で探してから `find . -type f -name "exec" -delete` で消す
    - もしこれを忘れてしまって怒られたら `git log` で履歴を見る → `git reset --soft (戻したいコミットのid)`
- ドキュメントを書くとき
  - `|` を使うと表と解釈されてやばいことになる。数式では `\lvert`, `\rvert` を使う
  - `{ }` も `\lbrace`, `\rbrace` を使うのが無難
    - プログラム中の `{ {` が間を開けないと表示がバグったことがある
  - 複数行にわたる数式を書きたいとき：`$\begin{aligned} \end{aligned}$` で挟み、改行は `\\\\  `（バックスラッシュ $4$ つ + 半角スペース $2$ つ）を使う
  - `\:` は使わない、`\` を使う
  - $\boldsymbol{1}$ は `\boldsymbol{1}` と書く
  - 参考リンク：
    - https://marshmallow444.github.io/tech_blog/2021/07/29/tex-with-github-pages.html
    - https://qiita.com/BurnEtz/items/e79999264125eb128ae7
