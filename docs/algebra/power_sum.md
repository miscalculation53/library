このドキュメントには AI による作成・変更が含まれています。

## 概要

値の $0$ 乗から $D$ 乗までの区間和を持つモノイド。
`power_sum[k]` は $\sum_i a_i^k$、`power_sum[0]` は要素数を `T` で表した値。
$0^0=1$ として、値が $0$ の要素も数える。

[ActedMonoidAffinePowerSum](acted_monoid/affine_power_sum.md) と組み合わせると、区間加算・乗算・代入・一次関数作用に対応する。
加算専用には [ActedMonoidAddPowerSum](acted_monoid/add_power_sum.md)、代入専用には [ActedMonoidUpdPowerSum](acted_monoid/upd_power_sum.md) を使える。
[MonoidIndexPowerSum](index_power_sum.md) は固定した添字の累乗和と、値の和を持つ用途で使う。
[MonoidMultivariatePowerSum](multivariate_power_sum.md) は複数の数列について $\sum_i a_i^p b_i^q\cdots$ を保持し、各数列への独立した更新に対応する。

## 使用例

```cpp
#include "algebra/power_sum.hpp"

using M = MonoidPowerSum<ll, 4>;
auto s = M::op(M::S(2), M::S(3));
assert((s.power_sum == array<ll, 5>{2, 5, 13, 35, 97}));
```

## 詳細なドキュメント

### MonoidPowerSum

```cpp
MonoidPowerSum<T, D>
```

要素型 `S` は `array<T, D + 1>` 型の `power_sum` を持つ。

##### 制約

- $D \geq 0$
- `T` は $0$, $1$, 加算、乗算を持つ可換半環で、`T{}` は $0$
- 符号付き整数型では、保持する値と各次数の累乗和が `T` に収まる

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T x)
```

- (1)：累乗和がすべて $0$ の空の要素を構築する。
- (2)：値 `x` の一点を構築する。`power_sum[k]` は $x^k$、`power_sum[0]` は $1$。

##### 計算量

- $O(D)$

#### op

```cpp
S op(const S& a, const S& b)
```

`power_sum` の各成分を加算する。

##### 計算量

- $O(D)$

#### e

```cpp
S e()
```

空の要素 `S()` を返す。

##### 計算量

- $O(D)$

#### pow

```cpp
template <class I>
static constexpr S pow(const S& x, I k);
```

各次数の和を `k` 倍する。指数 $k$ は非負整数とし、`k = 0` なら `e()` を返す。型と整数倍の条件は [pow の共通仕様](monoid_power.md) を参照。

##### 計算量

- $O(D)$。整数倍を使う場合は、その演算を $O(1)$ とする。
