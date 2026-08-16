## 概要

テンプレート（rep）

## 詳細なドキュメント

### rep

指定する引数は Python の range と同じ。つまり、

- `rep(i, l, r, d)`： $l$ から始めて $d$ ずつ変えるのを $r$ まで（$r$ は含まない）
- `rep(i, l, r)`： `rep(i, l, r, 1)` と同じ
- `rep(i, r)`： `rep(i, 0, r, 1)` と同じ

引数の数が違う `rep1`, `rep2`, `rep3` をすべて `rep` で呼べるようにしてある（参考： https://trap.jp/post/1224/ ）。

また、`rep` では `ll` 型を、`repi` では `int` 型を用いる。

引数は**最初の $1$ 回だけ**評価される。

（滅多にないと思うが、）`lllll`, `rrrrr`, `ddddd`, `nnnnn` という名前の変数を `rep` や `repi` の内部で使わないこと。

### 範囲 for 

- `fec(e : v)`
  - `for (const auto &e : v)` と同じ
  - for each const
  - **だいたいはこれを使っておくべき**
- `fe(e : v)`
  - `for (auto e : v)` と同じ
  - for each
  - コピーを作って中で変更したいときのみ使う（`fec` で済むときにこれを使うと遅い）
- `fem(e : v)`
  - `for (auto &e : v)` と同じ
  - for each mutable
  - 本当に内部を変更したいときにしか使わない

`e` には `[a, b]` とかも入れられる。区切りが通常の範囲 for と同様の `:` であることに注意（マクロ化する際の都合）。
