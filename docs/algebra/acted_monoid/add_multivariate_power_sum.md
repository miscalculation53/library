このドキュメントには AI による作成・変更が含まれている可能性があります。

## 概要

各数列への独立した区間加算 $a_{i,d}\leftarrow a_{i,d}+f_d$ と、混合累乗和の区間取得を処理する作用つきモノイド。
`ActedMonoidAddMultivariatePowerSum<T, Ds...>` の `Ds...` に数列ごとの次数上限を指定する。
作用型 `F` は `array<T, K>` で、数列ごとの加算値を渡す。

[ActedMonoidAffineMultivariatePowerSum](affine_multivariate_power_sum.md) のすべての `b[d]` を $1$ とした特殊版。
数列数を $K$、成分数を $L=\prod_d(D_d+1)$ とすると、`LazySegmentTree` の構築は $O((L+K)N)$、区間更新は $O(L\sum_d(D_d+1)\log N)$、区間取得は $O(L\log N)$。

## 使用例

```cpp
#include "algebra/acted_monoid/add_multivariate_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"

using AM = ActedMonoidAddMultivariatePowerSum<ll, 2, 3>;
LazySegmentTree<AM> seg(vc<array<ll, 2>>{{1, 2}, {3, 4}, {5, 6}});
seg.apply(0, 2, AM::F{2, 0}); // a だけに 2 を加算する
assert(seg.all_prod().at({1, 1}) == 56);
seg.apply(1, 3, AM::F{-1, 3});
assert(seg.all_prod().at({1, 1}) == 70);
```

## 詳細なドキュメント

### ActedMonoidAddMultivariatePowerSum

```cpp
ActedMonoidAddMultivariatePowerSum<T, Ds...>
```

`M = MonoidMultivariatePowerSum<T, Ds...>`、`S = M::S`、`F = array<T, K>`。
`S` の構築・メンバは [MonoidMultivariatePowerSum](../multivariate_power_sum.md) を参照。

##### 制約

- $K\geq 1$、すべての $D_d\geq 0$
- `T` は $0$, $1$, 加算、乗算、等値比較を持つ可換半環で、`T{}` は $0$
- 符号付き整数型では、保持する各混合累乗和と合成した加算値が `T` に収まる
- 整数型以外では、展開係数と作用の積和を `T` で計算できる

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

数列 $d$ に `f[d]` を加算した混合累乗和を返す。指数がすべて $0$ の成分は維持する。

##### 計算量

- $O(L\sum_d(D_d+1))$

#### composition

```cpp
F composition(const F& f, const F& g)
```

各数列の加算値を合成した `f[d] + g[d]` を返す。

##### 計算量

- $O(K)$

#### id

```cpp
F id()
```

すべての成分が $0$ の加算値を返す。

##### 計算量

- $O(K)$
