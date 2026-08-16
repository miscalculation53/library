## 概要

可換群の演算を各単位領域に作用させ、矩形内の値を同じ演算で集約するクエリをオフラインで処理する。通常の矩形加算・矩形和は `G = GroupAddSub<S>` とすればよい。

群演算を乗法的に書き、$w$ を $k$ 回演算したものを $w^{[k]}$ とする。「$(a,b)$ の右上に $w$ を作用させる」更新が「$(x,y)$ の左下を集約する」クエリへ与える寄与は

$$
w^{[(x-a)(y-b)]}
=w^{[xy]} \circ (w^{[b]})^{[-x]} \circ (w^{[a]})^{[-y]} \circ w^{[ab]}
$$

となる。可換性により、条件を満たす更新について $(w,w^{[b]},w^{[a]},w^{[ab]})$ の $4$ 成分をそれぞれ集約すればよい。これらを Fenwick Tree の一つの要素にまとめて持つため、座標圧縮・ソート・Fenwick Tree 上の走査はいずれも一度で済む。

実装では $xy$ や $ab$ を座標型のまま計算せず、`pow_group(pow_group(w, x), y)` としている。`G::pow` があれば使用し、なければ二分累乗へ自動的にフォールバックする。

一般化が有用な例として、次がある。

- `GroupAddSub<S>`：矩形加算・矩形和。累乗は整数倍なので $O(1)$。
- `GroupXor<T>`：矩形 XOR・矩形 XOR。累乗は回数の偶奇だけで決まり $O(1)$。
- `GroupMulDiv<mint>`：非零値の矩形乗算・矩形積。累乗には通常 $O(\log C)$ かかる。
- 複数の値をまとめた自作の直積群：複数種類の矩形集約を同時に処理できる。

`chmin` 更新・`min` 取得は可換モノイドではあるが、矩形の差分と包含排除に必要な逆元がないため、この実装には載せられない。初期値が $+\infty$ の場合、この問題は「クエリ矩形と交わる更新矩形の重みの最小値」という別の矩形交差問題になる。

右上への作用・左下の集約だけでよい場合は、それ専用のメソッドを使える。矩形の包含排除が不要なため、登録する点とクエリ数がそれぞれ $1/4$ になる。

## 使用例

```cpp
StaticRectangleAddRectangleSum<GroupAddSub<mint>, ll> sum;
sum.rectangle_add(lx, rx, ly, ry, w);
sum.rectangle_sum(qlx, qrx, qly, qry);
vc<mint> ans = sum.run();

StaticRectangleAddRectangleSum<GroupXor<ull>, ll> xr;
xr.rectangle_add(lx, rx, ly, ry, mask);
xr.rectangle_sum(qlx, qrx, qly, qry);
vc<ull> xor_ans = xr.run();
```

## 詳細なドキュメント

#### コンストラクタ

```cpp
StaticRectangleAddRectangleSum<G, I>
```

`G` は可換群、`I` は整数の座標型。

#### upper_right_add

```cpp
void upper_right_add(I lx, I ly, G::S w)
```

領域 $[\mathrm{lx}, \infty) \times [\mathrm{ly}, \infty)$ の各単位領域に `w` を作用させる。すべての集約クエリより前に呼ぶ。

##### 計算量

絶対値が最大の座標を $C$、`pow_monoid<G>` の計算量を $P(C)$ として

- $O(P(C))$

#### rectangle_add

```cpp
void rectangle_add(I lx, I rx, I ly, I ry, G::S w)
```

矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の各単位領域に `w` を作用させるクエリを追加する。すべての集約クエリより前に呼ぶ。

##### 制約

- $\mathrm{lx} \leq \mathrm{rx}$
- $\mathrm{ly} \leq \mathrm{ry}$

##### 計算量

- $O(P(C))$

#### lower_left_sum

```cpp
void lower_left_sum(I rx, I ry)
```

領域 $(-\infty, \mathrm{rx}) \times (-\infty, \mathrm{ry})$ の値を群演算で集約するクエリを追加する。すべての更新クエリより後に呼ぶ。

##### 計算量

- $O(1)$

#### rectangle_sum

```cpp
void rectangle_sum(I lx, I rx, I ly, I ry)
```

矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の値を群演算で集約するクエリを追加する。すべての更新クエリより後に呼ぶ。

##### 制約

- $\mathrm{lx} \leq \mathrm{rx}$
- $\mathrm{ly} \leq \mathrm{ry}$

##### 計算量

- $O(1)$

#### run

```cpp
vc<G::S> run()
```

すべてのクエリにまとめて答える。答えは `lower_left_sum` と `rectangle_sum` を呼んだ順に格納される。

##### 計算量

更新クエリを $n$ 回、集約クエリを $q$ 回追加したとして

- $O((n+q)\log(n+q)+qP(C))$

`G = GroupAddSub<S>` または `G = GroupXor<T>` では $P(C)=O(1)$。
