## 概要

点加算クエリと矩形和クエリが混ざって与えられるとき、矩形和（オフライン）を用いてオフラインで処理する。

分割統治をする。番号 $[l, r)$ のクエリをすべて処理する関数を $f(l, r)$ とする。
$f(l, r)$ は、$f(l, m)$ をして、$[l, m)$ から $[m, r)$ への寄与を処理して、$f(m, r)$ をする。
寄与の処理は、$[l, m)$ の点加算 → $[m, r)$ の矩形和クエリを処理すればよい。
$O((N + Q) \log^2 (N + Q))$ 時間。

Library Checker にある定式化のほかに、後のほうの点加算クエリの重みが前のほうの矩形和クエリに依存する場合も同様にして解ける（これは DP するときなどに割とよくある）。本ライブラリでは前のクエリ $1$ 個のみに依存する場合の処理を実装している。

`RectangleSum` に対する `LowerLeftSum` と同様、`PointAddRectangleSum` に対する `PointAddLowerLeftSum` もある。

## 詳細なドキュメント

#### コンストラクタ

```cpp
PointAddRectangleSum<G, I>
```

`G` は可換群、`I` は座標の型

#### point_add

```cpp
void point_add(I x, I y, G::S w)
```
「点 $(x, y)$ に重み $w$ を加える」というクエリを追加する。

##### 計算量

- $O(1)$

#### point_add_fn

```cpp
void point_add_fn(I x, I y, int qi, function<G::S(G::S)> f)
```

重みが前の矩形和クエリ $1$ 個に依存する点加算クエリを追加する。
具体的には、「点 $(x, y)$ に重み $f(\mathrm{qi}\text{ 番目のクエリの答え})$ を追加する」というクエリ。

##### 計算量

- $O(1)$

#### rectangle_sum

```cpp
void rectangle_sum(I lx, I rx, I ly, I ry)
```
「矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの和を求める」というクエリを追加する。

##### 制約

- $\mathrm{lx} \leq \mathrm{rx}$
- $\mathrm{ly} \leq \mathrm{ry}$

##### 計算量

- $O(1)$

#### run

```cpp
vc<G::S> run()
```
すべてのクエリにまとめて答える。答えは `rectangle_sum` を呼んだ順に格納される。

##### 計算量

`point_add` を $n$ 回、`rectangle_sum` を $q$ 回呼んだとして

- $O((n+q) \log^2 (n+q))$
