## 矩形加算・点取得（オフライン）

矩形加算 → 点取得（オフライン）と同様に点加算 → 矩形和（オフライン）に帰着させる。時間計算量は $O((N + Q) \log^2 (N + Q))$。

### コンストラクタ

```cpp
RectangleAddPointGet<G, I>
```

`G` は可換群、`I` は座標の型

### メンバ関数

#### rectangle_add

```cpp
void rectangle_add(I lx, I rx, I ly, I ry, G::S w)
```

「矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ に重み $w$ を加える」というクエリを追加する。

##### 制約

- $\mathrm{lx} \leq \mathrm{rx}$
- $\mathrm{ly} \leq \mathrm{ry}$

##### 計算量

- $O(1)$

#### point_get

```cpp
void point_get(I x, I y)
```

「点 $(x, y)$ の重みの和を求める」というクエリを追加する。

##### 計算量

- $O(1)$

#### run

```cpp
vc<G::S> run()
```

すべてのクエリにまとめて答える。答えは `point_get` を呼んだ順に格納される。

##### 計算量

`rectangle_add` を $n$ 回、`point_get` を $q$ 回呼んだとして

- $O((n+q) \log^2 (n+q))$
