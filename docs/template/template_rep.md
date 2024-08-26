## テンプレート（rep）

指定する引数は Python の range と同じ。つまり、

- `rep(l, r, d)`: $l$ から始めて $d$ ずつ変えるのを $r$ まで（$r$ は含まない）
- `rep(l, r)`: `rep(l, r, 1)` と同じ
- `rep(r)`: `rep(0, r, 1)` と同じ

引数の数が違う `rep1`, `rep2`, `rep3` をすべて `rep` で呼べるようにしてある（参考： https://trap.jp/post/1224/ ）。

また、`rep` では `ll` 型を、`repi` では `int` 型を用いる。