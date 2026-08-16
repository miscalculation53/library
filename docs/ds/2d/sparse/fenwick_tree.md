## 概要

一点加算・矩形和ができる（ただし、一点加算が起こる点は（本ライブラリでは）先読みする必要がある）。log ふたつ。

中身：Fenwick Tree の各ノードに、座標圧縮した $y$ 軸上のデータ構造を持つイメージ。内側はデフォルトでは Fenwick Tree だが、Segment Tree や Cumulative Sum などへ変更できる。

構築時に与えた点の添字を `point_id` として更新に使う。$x$ 座標は内部で自動的に座標圧縮する。更新経路上の局所 $y$ 座標の添字も構築時に接続しておくため、`add` では座標の二分探索を行わない。

各ノードの $y$ 座標列は CSR で管理する。親ノードでの $y$ 添字への対応表は CSR の平坦な添字を共有し、値はノードごとの `D<M>` が管理する。

どうしても点が先読みできない場合（そんな場合あるのか？）、map を使うことになりそう（遅そう）。

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) FenwickTree2DSparse<M, I, D>(vc<pair<I, I>> xys)
(2) FenwickTree2DSparse<M, I, D>(vc<tuple<I, I, M::S>> xyws)
```

- `M` は可換モノイド。逆元を使う操作では可換群である必要がある
- `I` は座標の型
- `D<M>` は各外側ノードに載せるデータ構造。`D` のデフォルトは `FenwickTree`
- (1) の `xys` は `add` クエリが飛んでくる点たちで、初期値はすべて `M::e()`
- (2) の `xyws` は点と初期値
- `xys[i]` または `xyws[i]` の点は `point_id = i` で指定する
- 同じ座標が複数回含まれる場合、それらの `point_id` は同じ点を指す
- `D<M>` は `vc<M::S>` から構築できる

##### 計算量

$p$ を入力した点の個数として、`D<M>` の構築が要素数に対して線形なら

- $O(p \log^2 p)$

#### add

```cpp
void add(int point_id, M::S w)
```

構築時の入力で `point_id` 番目に与えた点へ重み $w$ を加える。

##### 制約

- $0 \leq \mathrm{point\_id} < p$
- `D<M>` は `add`、または `get` と `set` を持つ

##### 計算量

`D<M>` の一点加算の計算量を $f(p)$ として

- $O(\log p \cdot f(p))$

デフォルトの `FenwickTree` では $O(\log^2 p)$。

#### sum

```cpp
(1) M::S sum(I rx, I ry)
(2) M::S sum(I rx, I ly, I ry)
(3) M::S sum(I lx, I rx, I ly, I ry)
```

- (1) 矩形領域 $(-\infty, \mathrm{rx}) \times (-\infty, \mathrm{ry})$ の点の重みの和を求める。
- (2) 矩形領域 $(-\infty, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの和を求める。
- (3) 矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの和を求める。

##### 制約

- `M` は可換モノイド
- (1)：`D<M>` は `sum(r)`、`sum(l, r)`、`prod(l, r)` のいずれかを持つ
- (2)、(3)：`D<M>` は `sum(l, r)` または `prod(l, r)` を持つ
- (3)：`M` は可換群である必要がある
- デフォルトの `FenwickTree` で (2) を使う場合も、内側の区間和を求めるため `M` は可換群である必要がある
- (2) では $\mathrm{ly} \leq \mathrm{ry}$、(3) ではそれに加え $\mathrm{lx} \leq \mathrm{rx}$

##### 計算量

`D<M>` の区間クエリの計算量を $f(p)$ として

- $O(\log p \cdot (\log p + f(p)))$

デフォルトの `FenwickTree` では $O(\log^2 p)$。

#### content

```cpp
vc<tuple<I, I, M::S>> content()
```

$(x\ 座標, y\ 座標, 重み)$ の組のうち、重みが `M::e()` でないものを格納した vector を返す。デバッグ用を想定。

##### 制約

- `M` は可換群
- `D<M>` は `sum(l, r)` または `prod(l, r)` を持つ

##### 計算量

$u$ を異なる登録点の個数、$p$ を入力した点の個数、`D<M>` の区間クエリの計算量を $f(p)$ として

- $O(u \log p \cdot (\log p + f(p)))$
