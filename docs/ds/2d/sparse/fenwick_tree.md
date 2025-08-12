## $2$ 次元 Fenwick Tree（疎、クエリ点先読み）

一点加算・矩形和ができる（ただし、一点加算が起こる点は（本ライブラリでは）先読みする必要がある）。log ふたつ。

中身：Fenwick Tree の中に（座圧した）Fenwick Tree を持つイメージ。

どうしても点が先読みできない場合（そんな場合あるのか？）、map を使うことになりそう（遅そう）。

### コンストラクタ

```cpp
FenwickTree2DSparse<G, I>(vc<pair<I, I>> xys)
```

- `G` は可換で、群（一部の操作はモノイドでよい）
- `I` は座標の型
- `xys` は `add` クエリが飛んでくる点たち

**x 方向の座圧は自動でやらないので注意！**

##### 計算量

$n = \max(x)$ として

- $O(n \log n)$

### メンバ関数

#### add

```cpp
void add(I x, I y, G::S w)
```

点 $(x, y)$ に重み $w$ を加える。

##### 制約

- $x, y$ は `xys` のいずれかに含まれる
- `G` はモノイドでよい

##### 計算量

$n = \max(x)$ として

- $O(\log^2 n)$

#### sum

```cpp
(1) G::S sum(I rx, I ry)
(2) G::S sum(I rx, I ly, I ry)
(3) G::S sum(I lx, I rx, I ly, I ry)
```

- (1) 矩形領域 $(-\infty, \mathrm{rx}) \times (-\infty, \mathrm{ry})$ の点の重みの和を求める。
- (2) 矩形領域 $(-\infty, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの和を求める。
- (3) 矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの和を求める。


##### 制約

- (1)：`G` はモノイドでよい
- (2)、(3)：`G` は群である必要がある
- (2) では $\mathrm{ly} \leq \mathrm{ry}$、(3) ではそれに加え $\mathrm{lx} \leq \mathrm{rx}$

##### 計算量

$n = \max(x)$ として

- $O(\log^2 n)$

#### content

```cpp
vc<tuple<I, I, G::S>> content()
```

$(x\ 座標, y\ 座標, 重み)$ の組のうち、重みが `S::e()` でないものを格納した vector を返す。デバッグ用を想定。

##### 計算量

$n = \max(x)$ として

- $O(n \log^2 n)$

