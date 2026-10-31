このドキュメントには AI による作成・変更が含まれています。

## 概要

各数列への独立した区間代入と、混合累乗和の区間取得を処理する作用つきモノイド。
`ActedMonoidUpdMultivariatePowerSum<T, Ds...>` の `Ds...` に数列ごとの次数上限を指定する。
作用型 `F` は `array<MonoidUpdFlag<T>::S, K>`。`f[d]` に代入値を設定し、更新を省略する数列には恒等作用を使う。

[ActedMonoidAffineMultivariatePowerSum](affine_multivariate_power_sum.md) の各数列への作用を、恒等作用または代入に限定した特殊版。
数列数を $K$、成分数を $L=\prod_d(D_d+1)$ とすると、`LazySegmentTree` の構築は $O((L+K)N)$、区間更新は $O(KL\log N)$、区間取得は $O(L\log N)$。

## 使用例

```cpp
#include "algebra/acted_monoid/upd_multivariate_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"

using AM = ActedMonoidUpdMultivariatePowerSum<ll, 2, 3>;
LazySegmentTree<AM> seg(vc<array<ll, 2>>{{1, 2}, {3, 4}, {5, 6}});

auto f = AM::id();
f[1] = 3; // b に 3 を代入する
seg.apply(1, 3, f); // (a, b) = {(1, 2), (3, 3), (5, 3)}
assert(seg.all_prod().at({1, 1}) == 26);

f = AM::id();
f[0] = 0; // a に 0 を代入する
seg.apply(0, 2, f);
assert(seg.all_prod().at({0, 0}) == 3);
assert(seg.all_prod().at({1, 1}) == 15);
```

## 詳細なドキュメント

### ActedMonoidUpdMultivariatePowerSum

```cpp
ActedMonoidUpdMultivariatePowerSum<T, Ds...>
```

`M = MonoidMultivariatePowerSum<T, Ds...>`、`S = M::S`。
`S` の構築・メンバは [MonoidMultivariatePowerSum](../multivariate_power_sum.md) を参照。
`F` の各成分は `is_id`, `val` を持ち、初期値は恒等作用。`T` からの構築は、その値への代入を表す。

##### 制約

- $K\geq 1$、すべての $D_d\geq 0$
- `T` は $0$, $1$, 加算、乗算、等値比較を持つ可換半環で、`T{}` は $0$
- 符号付き整数型では、保持する各混合累乗和が `T` に収まる
- 整数型以外では、作用の積和を `T` で計算できる

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
S mapping(const F& f, const S& x)
```

`f[d].is_id` が `false` の数列 $d$ に `f[d].val` を代入した混合累乗和を返す。
指数がすべて $0$ の成分は維持する。

##### 計算量

- $O(KL)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を作用させる合成を返す。
数列 $d$ ごとに、`f[d]` が恒等作用なら `g[d]`、代入なら `f[d]` を返す。

##### 計算量

- $O(K)$

#### id

```cpp
F id()
```

すべての数列への恒等作用を返す。

##### 計算量

- $O(K)$

### pow（集約値の反復）

```cpp
template <class I>
static constexpr S pow(const S& x, I k);
```

このヘッダにある各型で、集約値 `S` を `op` により `k` 回結合する。値モノイド `M` の高速な `pow` を使う。指数 $k$ は非負整数とし、`k = 0` なら `e()` を返す。型と整数倍の条件は [pow の共通仕様](../monoid_power.md) を参照。

##### 計算量

- $O(\prod_j(D_j+1))$。整数倍を使う場合は、その演算を $O(1)$ とする。
