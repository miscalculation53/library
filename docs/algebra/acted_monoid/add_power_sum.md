このドキュメントには AI による作成・変更が含まれています。

## 概要

区間加算 $a_i\leftarrow a_i+f$ と、$0$ 乗から $D$ 乗までの区間和取得を処理する作用つきモノイド。
作用型 `F` は `T` で、`seg.apply(l, r, f)` に加算値を渡す。
`seg.prod(l, r).power_sum[k]` は $\sum_{i=l}^{r-1}a_i^k$。

[ActedMonoidAffinePowerSum](affine_power_sum.md) の $b=1$ の特殊版で、二項展開による累乗和の更新を共有する。
整数型の中間積和は unsigned で計算し、結果を `T` に戻す。

`LazySegmentTree` と組み合わせた構築は $O((D+1)N)$、区間更新は $O((D+1)^2\log N)$、区間取得は $O((D+1)\log N)$。

## 使用例

```cpp
#include "algebra/acted_monoid/add_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "math/modint/modint.hpp"

using mint = modint998244353;
using AM = ActedMonoidAddPowerSum<mint, 4>;
LazySegmentTree<AM> seg(vc<mint>{1, 2, 3, 4});

seg.apply(1, 3, mint(5)); // {1, 7, 8, 4}
assert(seg.prod(0, 3).power_sum[3] == mint(856));
assert(seg.prod(1, 4).power_sum[4] == mint(6753));

seg.apply(0, 4, mint(-1)); // {0, 6, 7, 3}
assert(seg.all_prod().power_sum[0] == mint(4));
assert(seg.all_prod().power_sum[2] == mint(94));
```

## 詳細なドキュメント

### ActedMonoidAddPowerSum

```cpp
ActedMonoidAddPowerSum<T, D>
```

`M = MonoidPowerSum<T, D>`、`MF = MonoidAdd<T>`、`S = M::S`、`F = T`。
`S` の構築・メンバは [MonoidPowerSum](../power_sum.md) を参照。
零列の構築には `vc<T>(n, T{})` を渡す。

##### 制約

- $D \geq 0$
- `T` は $0$, $1$, 加算、乗算、等値比較を持つ可換半環で、`T{}` は $0$
- 符号付き整数型では、保持する値・各次数の累乗和・合成した加算値が `T` に収まる
- 整数型以外では、展開係数と作用の積和を `T` で計算できる

#### op

```cpp
S op(const S& a, const S& b)
```

次数ごとに累乗和を加算する。

##### 計算量

- $O(D)$

#### e

```cpp
S e()
```

空の要素を返す。

##### 計算量

- $O(D)$

#### mapping

```cpp
S mapping(const F& f, const S& x)
```

各値に `f` を加算した累乗和を返す。`power_sum[0]` は維持する。

$$
\mathrm{res.power\_sum}[k]
=\sum_{j=0}^{k}\binom{k}{j}f^{k-j}\,x.\mathrm{power\_sum}[j]
$$

##### 計算量

- $O(D^2)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を加える合成 `f + g` を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

加算値 $0$ を返す。

##### 計算量

- $O(1)$
