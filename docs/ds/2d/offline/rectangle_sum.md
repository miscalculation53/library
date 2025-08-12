## 矩形和（オフライン）

平面上の点が事前に複数与えられたとき、矩形領域の点の重みの和を求めるクエリをオフラインでまとめて $O((N + Q) \log (N + Q))$ 時間で処理する。可換群が載る。左下に無限に広がる矩形領域 $(-\infty, \mathrm{rx}) \times (-\infty, \mathrm{ry})$ の場合は可換モノイドでよく、これも実装している。

以下では一般の矩形領域の場合のインターフェースを説明する。左下に無限に広がる矩形領域の場合に使える `LowerLeftSum` もインターフェースはほとんど同じ。

### コンストラクタ

```cpp
RectangleSum<G, I>
```

`G` は可換群、`I` は座標の型

### メンバ関数

#### point_add

```cpp
void point_add(I x, I y, G::S w)
```

「点 $(x, y)$ に重み $w$ を加える」というクエリを追加する。すべての `rectangle_sum` よりも前に呼ぶ。

##### 計算量

- $O(1)$

#### rectangle_sum

```cpp
void rectangle_sum(I lx, I rx, I ly, I ry)
```

「矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの和を求める」というクエリを追加する。すべての `point_add` よりも後に呼ぶ。

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

- $O((n+q) \log (n+q))$
