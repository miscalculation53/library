## 矩形加算 → 点取得（オフライン）

普通に平面走査をして区間加算・一点更新にしてもよいが、左下に無限に広がる矩形領域の和に帰着することもできる（本ライブラリでは一旦こちらの実装を採用）。時間計算量は $O((N + Q) \log (N + Q))$。

$\begin{aligned}
\displaystyle \mathrm{ans}(x, y) &= \sum_{(\mathrm{lx}, \mathrm{rx}, \mathrm{ly}, \mathrm{ry}, w) \in Q} \boldsymbol{1}[\mathrm{lx} \leq x < \mathrm{rx}]\boldsymbol{1}[\mathrm{ly} \leq y < \mathrm{ry}] w \\\\  
&= \sum_{(\mathrm{lx}, \mathrm{rx}, \mathrm{ly}, \mathrm{ry}, w) \in Q} (\boldsymbol{1}[\mathrm{lx} \leq x] - \boldsymbol{1}[\mathrm{rx} \leq x])(\boldsymbol{1}[\mathrm{ly} \leq y] - \boldsymbol{1}[\mathrm{ry} \leq y]) w \\\\  
&= \sum_{(\mathrm{lx}, \mathrm{ly}, w) \in Q_1} \boldsymbol{1}[\mathrm{lx} \leq x] \boldsymbol{1}[\mathrm{ly} \leq y] w + (略)
\end{aligned}$

という感じになっているため。たとえば第 $1$ 項は点と長方形を入れ替えて考えて、点 $(\mathrm{lx}, \mathrm{rx})$ が長方形領域 $(-\infty, x] \times (-\infty, y]$ に含まれるという条件だと思えばよい。

### コンストラクタ

```cpp
StaticRectangleAddPointGet<G, I>
```

`G` は可換群、`I` は座標の型

### メンバ関数

#### rectangle_add

```cpp
void rectangle_add(I lx, I rx, I ly, I ry, G::S w)
```

「矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ に重み $w$ を加える」というクエリを追加する。すべての `point_get` よりも前に呼ぶ。

##### 制約

- $\mathrm{lx} \leq \mathrm{rx}$
- $\mathrm{ly} \leq \mathrm{ry}$

##### 計算量

- $O(1)$

#### point_get

```cpp
void point_get(I x, I y)
```

「点 $(x, y)$ の重みの和を求める」というクエリを追加する。すべての `rectangle_add` よりも後に呼ぶ。

##### 計算量

- $O(1)$

#### run

```cpp
vc<G::S> run()
```

すべてのクエリにまとめて答える。答えは `point_get` を呼んだ順に格納される。

##### 計算量

`rectangle_add` を $n$ 回、`point_get` を $q$ 回呼んだとして

- $O((n+q) \log (n+q))$
