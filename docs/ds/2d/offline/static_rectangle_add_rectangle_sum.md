## 矩形加算 → 矩形和（オフライン）

まず、「$(a, b)$ の右上（端含む）に $c$ を加算」クエリと「$(x, y)$ の左下（端含まない）の和」クエリに分解しておく。すると

$\begin{aligned}
\displaystyle \mathrm{ans}(x, y) &= \sum_{(a, b, c) \in Q} \bm{1}[a < x] \bm{1}[b < y] c(x - a)(y - b) \\\\  
&= \sum_{(a, b, c) \in Q} \bm{1}[a < x] \bm{1}[b < y] (c \cdot xy - ca \cdot x - cb \cdot y + cab)
\end{aligned}$

となるので、「$c$ を $(a, b)$ に加算」「$-ca$ を $(a, b)$ に加算」「$-cb$ を $(a, b)$ に加算」「$cab$ を $(a, b)$ に加算」の $4$ つの矩形和クエリに帰着される。いわゆる「変数分離」。

定数倍がすごいことになる。これをどうにかする方法はあるのか……？

なお、右上加算・左下和だけでいい場合、それ専用のメソッドを実装している。これを使うと定数倍が $1/4$ くらいになる。

書いてから気づいたが、pow が高速に計算できる可換群に一般化できる（$c^{(x-a)(y-b)}$ などと考える）。そんなものは $+$ くらいしかないという説もある。


### コンストラクタ

```cpp
StaticRectangleAddPointGet<S, I>
```

`S` は整数や modint など、加減算および整数倍が定義される型

`I` は座標の型

### メンバ関数

#### rectangle_add

```cpp
void rectangle_add(I lx, I rx, I ly, I ry, G::S w)
```

「矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ に重み $w$ を加える」というクエリを追加する。すべての `rectangle_sum` よりも前に呼ぶ。

##### 制約

- $\mathrm{lx} \leq \mathrm{rx}$
- $\mathrm{ly} \leq \mathrm{ry}$

##### 計算量

- $O(1)$

#### rectangle_sum

```cpp
void rectangle_sum(I lx, I rx, I ly, I ry)
```

「矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の重みの和を求める」というクエリを追加する。すべての `rectangle_add` よりも後に呼ぶ。

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

`rectangle_add` を $n$ 回、`rectangle_sum` を $q$ 回呼んだとして

- $O((n+q) \log (n+q))$
