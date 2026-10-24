このドキュメントには AI による作成・変更が含まれている可能性があります。

## 概要

各数列への独立した一次関数作用 $a_{i,d}\leftarrow b_d a_{i,d}+c_d$ と、混合累乗和の区間取得を処理する作用つきモノイド。
`ActedMonoidAffineMultivariatePowerSum<T, Ds...>` の `Ds...` に数列ごとの次数上限を指定する。
作用 `F` の `b[d]`, `c[d]` が数列 $d$ の係数で、`F()` はすべての数列への恒等作用。

| 操作 | `b[d]` | `c[d]` |
| --- | --- | --- |
| $x$ を加算 | $1$ | $x$ |
| $x$ を乗算 | $x$ | $0$ |
| $x$ を代入 | $0$ | $x$ |
| $bx+c$ を作用 | $b$ | $c$ |

`seg.prod(l, r).at({p, q, ...})` で $\sum_{i=l}^{r-1}a_{i,0}^{p}a_{i,1}^{q}\cdots$ を取得する。
数列数を $K$、成分数を $L=\prod_d(D_d+1)$ とする。
`LazySegmentTree` と組み合わせた構築は $O((L+K)N)$、区間更新は $O(L\sum_d(D_d+1)\log N)$、区間取得は $O(L\log N)$。

一つの変数の指数だけを動かした成分列に、[ActedMonoidAffinePowerSum](affine_power_sum.md) の二項展開を適用する。これを変数ごとに繰り返す。
展開には加算・乗算を使うので、合成数を法とする `modint` にも対応する。
整数型の中間積和は unsigned で計算し、結果を `T` に戻す。

加算専用の [ActedMonoidAddMultivariatePowerSum](add_multivariate_power_sum.md)、代入専用の [ActedMonoidUpdMultivariatePowerSum](upd_multivariate_power_sum.md) もある。

## 使用例

```cpp
#include "algebra/acted_monoid/affine_multivariate_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "math/modint/modint.hpp"

using mint = modint998244353;
using AM = ActedMonoidAffineMultivariatePowerSum<mint, 2, 3>;
LazySegmentTree<AM> seg(vc<array<mint, 2>>{{1, 2}, {3, 4}, {5, 6}});

auto f = AM::id();
f.c[0] = 2; // 数列 a に 2 を加算する
seg.apply(0, 2, f); // (a, b) = {(3, 2), (5, 4), (5, 6)}
assert(seg.all_prod().at({1, 1}) == mint(56));

// b = {2, 0}, c = {1, 3}：a を 2a+1 に更新し、b に 3 を代入する
seg.apply(1, 3, AM::F({2, 0}, {1, 3}));
assert(seg.all_prod().at({1, 1}) == mint(72));
assert(seg.all_prod().at({2, 1}) == mint(744));
```

## 詳細なドキュメント

### ActedMonoidAffineMultivariatePowerSum

```cpp
ActedMonoidAffineMultivariatePowerSum<T, Ds...>
```

`M = MonoidMultivariatePowerSum<T, Ds...>`、`S = M::S`。
`S` の構築・メンバは [MonoidMultivariatePowerSum](../multivariate_power_sum.md) を参照。
零列の構築には `vc<array<T, K>>(n)` を渡す。

##### 制約

- $K\geq 1$、すべての $D_d\geq 0$
- `T` は $0$, $1$, 加算、乗算、等値比較を持つ可換半環で、`T{}` は $0$
- 符号付き整数型では、保持する各混合累乗和と合成した作用の係数が `T` に収まる
- 整数型以外では、展開係数と作用の積和を `T` で計算できる

#### F のコンストラクタ

```cpp
(1) F()
(2) F(const array<T, K>& b, const array<T, K>& c)
```

- (1)：すべての `b[d]` が $1$、`c[d]` が $0$ の恒等作用を構築する。
- (2)：各数列に $x\mapsto b_dx+c_d$ を作用させる更新を構築する。

##### 計算量

- $O(K)$

#### op

```cpp
S op(const S& a, const S& b)
```

混合累乗和を成分ごとに加算する。

##### 計算量

- $O(L)$

#### e

```cpp
S e()
```

空の要素を返す。

##### 計算量

- $O(L)$

#### mapping

```cpp
S mapping(const F& f, S x)
```

各数列に指定した一次関数を作用させた混合累乗和を返す。指数がすべて $0$ の成分は維持する。

$$
\mathrm{res}[\boldsymbol{k}]
=\sum_{0\leq j_d\leq k_d}
\left(\prod_d\binom{k_d}{j_d}b_d^{j_d}c_d^{k_d-j_d}\right)
x[\boldsymbol{j}]
$$

##### 計算量

- $O(L\sum_d(D_d+1))$
- すべての変数について `b[d] == 0` または `c[d] == 0` の場合は $O(KL)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を作用させる合成を返す。
各係数は `b[d] = f.b[d] * g.b[d]`、`c[d] = f.b[d] * g.c[d] + f.c[d]`。

##### 計算量

- $O(K)$

#### id

```cpp
F id()
```

すべての数列への恒等作用 `F()` を返す。

##### 計算量

- $O(K)$
