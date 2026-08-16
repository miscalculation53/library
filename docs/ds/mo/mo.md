## 概要

静的・オフラインな区間クエリについて、区間の端を追加・削除する処理を $O(N\sqrt{Q})$ 回、現在の区間の答えを計算する処理を $O(Q)$ 回行うことで処理するアルゴリズム。

定数倍については https://noshi91.hatenablog.com/entry/2023/04/13/224811

もしかして：

- 削除が苦しいとき → Rollback Mo を検討
- 平方分割を使って追加・削除を $O(1)$ 時間、答えの取得を $O(\sqrt{N})$ 時間にできれば、全体 $O(N\sqrt{Q} + Q\sqrt{N})$ 時間にできる

## 詳細なドキュメント

#### mo

```cpp
(1) mo(int n, vc<pair<I, I>> lrs, auto add_l, auto add_r, auto del_l, auto del_r, auto rem)
(2) mo(int n, vc<pair<I, I>> lrs, auto add, auto del, auto rem)
(3) mo(int n, vc<pair<I, I>> lrs, Slider &slider, auto rem)
```

$[0, n)$ 内の区間 $[l, r)$ たちに関するクエリを処理する。

`rem(q)` は、今の区間が $q$ 番目のクエリだとして、その結果を確定させる関数として渡す。

- (1) だけで渡す関数：
  - `add_l(l, r)`：今の区間が $[l+1, r)$ であるとき、$l$ を追加して $[l, r)$ にする関数
  - `add_r(l, r)`：今の区間が $[l, r)$ であるとき、$r$ を追加して $[l, r+1)$ にする関数
  - `del_l(l, r)`：今の区間が $[l, r)$ であるとき、$l$ を削除して $[l+1, r)$ にする関数
  - `del_r(l, r)`：今の区間が $[l, r+1)$ であるとき、$r$ を削除して $[l, r)$ にする関数
- (2) だけで渡す関数：
  - `add(i, isleft)`：今の区間に $i$ を追加する関数。`isleft` が `true` のときは左端に、`false` のときは右端に追加する
  - `del(i, isleft)`：今の区間から $i$ を削除する関数。`isleft` が `true` のときは左端から、`false` のときは右端から削除する
- (3) では `slider` を渡す。`slider` の例としては ds/fenwick_tree/inversion.hpp に実装した `InversionSlider` がある。`slider` 内部に実装するものは：
  - `l`, `r`：現在の区間 $[l, r)$
  - `lpp()`, `lmm()`, `rpp()`, `rmm()`：現在の区間を更新する関数
  - `set(nl, nr)`：`lpp()`, `lmm()`, `rpp()`, `rmm()` を用いて区間を $[\mathrm{nl}, \mathrm{nr})$ に更新する関数

##### 制約

- $0 \leq l, r \leq n$
  - 実は区間に限らず平面上の点に対しても適用できる（$l \leq r$ でなくてもよい）

##### 計算量

$q = \lvert \mathrm{lrs} \rvert$ として

- $O(n\sqrt{q})$ 回の `add`, `del` 呼び出し
- $O(q)$ 回の `rem` 呼び出し
