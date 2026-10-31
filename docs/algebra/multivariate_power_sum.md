このドキュメントには AI による作成・変更が含まれています。

## 概要

$K$ 個の数列について、混合累乗和

$$
\sum_i\prod_{d=0}^{K-1}a_{i,d}^{k_d}
\qquad (0\leq k_d\leq D_d)
$$

を持つモノイド。`MonoidMultivariatePowerSum<T, Ds...>` の `Ds...` に数列ごとの次数上限を指定する。
`s.at({k_0, k_1, ...})` で各成分を取得し、指数がすべて $0$ の成分は要素数を表す。$0^0=1$ とする。
各変数の次数が上限内の多項式 $P$ に対する $\sum_i P(a_{i,0},\ldots,a_{i,K-1})$ を、混合累乗和の線形結合で求められる。

保持する成分数を $L=\prod_d(D_d+1)$ とする。`power_sum` は `array<T, L>` で、末尾の変数の指数を最も速く動かす順に格納する。
例えば次数上限が $(2,3)$ のとき、指数 $(p,q)$ の位置は `4 * p + q`。

各数列への独立した更新には、[一次関数作用](acted_monoid/affine_multivariate_power_sum.md)、[加算](acted_monoid/add_multivariate_power_sum.md)、[代入](acted_monoid/upd_multivariate_power_sum.md) の作用つきモノイドを使える。
$K=1$ は [MonoidPowerSum](power_sum.md) と同じ累乗和を保持する。

## 使用例

```cpp
#include "algebra/multivariate_power_sum.hpp"

using M = MonoidMultivariatePowerSum<ll, 2, 3>;
auto s = M::op(M::S({2, 3}), M::S({4, 5}));
assert(s.at({0, 0}) == 2);
assert(s.at({1, 0}) == 6);   // sum a
assert(s.at({0, 1}) == 8);   // sum b
assert(s.at({1, 1}) == 26);  // sum a * b
assert(s.at({2, 3}) == 2108); // sum a^2 * b^3
assert(s.power_sum[M::index({1, 1})] == 26);
```

## 詳細なドキュメント

### MonoidMultivariatePowerSum

```cpp
MonoidMultivariatePowerSum<T, Ds...>
```

`dim` は数列数 $K$、`degree[d]` は次数上限 $D_d$、`size` は成分数 $L$、`stride[d]` は変数 $d$ の指数を $1$ 増やしたときの格納位置の増分。

##### 制約

- $K\geq 1$、すべての $D_d\geq 0$
- `T` は $0$, $1$, 加算、乗算を持つ可換半環で、`T{}` は $0$
- 符号付き整数型では、保持する各混合累乗和が `T` に収まる
- 整数型以外では、混合累乗の計算を `T` で行える

#### S のコンストラクタ

```cpp
(1) S()
(2) S(const array<T, dim>& x)
```

- (1)：各成分が $0$ の空の要素を構築する。
- (2)：同じ添字にある $K$ 個の値 `x` を一点として構築する。`at(exponent)` は $\prod_d x_d^{k_d}$。

##### 計算量

- (1)：$O(L)$
- (2)：$O(L+K)$

#### S::at

```cpp
T& at(const array<int, dim>& exponent)
const T& at(const array<int, dim>& exponent) const
```

指定した指数の混合累乗和への参照を返す。

##### 制約

- すべての $d$ について $0\leq\mathrm{exponent}[d]\leq D_d$

##### 計算量

- $O(K)$

#### index

```cpp
int index(const array<int, dim>& exponent)
```

`power_sum` 内の格納位置 $\sum_d\mathrm{exponent}[d]\cdot\mathrm{stride}[d]$ を返す。

##### 制約

- すべての $d$ について $0\leq\mathrm{exponent}[d]\leq D_d$

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

#### pow

```cpp
template <class I>
static constexpr S pow(const S& x, I k);
```

すべての混合累乗和を `k` 倍する。指数 $k$ は非負整数とし、`k = 0` なら `e()` を返す。型と整数倍の条件は [pow の共通仕様](monoid_power.md) を参照。

##### 計算量

- $O(L)$。整数倍を使う場合は、その演算を $O(1)$ とする。
