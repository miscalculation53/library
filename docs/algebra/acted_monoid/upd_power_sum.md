このドキュメントには AI による作成・変更が含まれています。

## 概要

区間代入 $a_i\leftarrow f$ と、$0$ 乗から $D$ 乗までの区間和取得を処理する作用つきモノイド。
`seg.apply(l, r, AM::F(f))` に代入値を渡す。
`seg.prod(l, r).power_sum[k]` は $\sum_{i=l}^{r-1}a_i^k$。

[ActedMonoidAffinePowerSum](affine_power_sum.md) の $b=0$ の特殊版で、長さと代入値の累乗から各次数の和を計算する。
作用型は `is_id` と `val` を持ち、$0$ の代入と恒等作用を区別する。
整数型の中間積和は unsigned で計算し、結果を `T` に戻す。

`LazySegmentTree` と組み合わせた構築は $O((D+1)N)$、区間更新・区間取得は $O((D+1)\log N)$。

## 使用例

```cpp
#include "algebra/acted_monoid/upd_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "math/modint/modint.hpp"

using mint = modint998244353;
using AM = ActedMonoidUpdPowerSum<mint, 4>;
LazySegmentTree<AM> seg(vc<mint>{1, 2, 3, 4});

seg.apply(1, 4, AM::F(mint(2))); // {1, 2, 2, 2}
assert(seg.all_prod().power_sum[4] == mint(49));

seg.apply(0, 2, AM::F(mint(0))); // {0, 0, 2, 2}
seg.apply(0, 4, AM::id());
assert(seg.all_prod().power_sum[0] == mint(4));
assert(seg.all_prod().power_sum[2] == mint(8));
```

## 詳細なドキュメント

### ActedMonoidUpdPowerSum

```cpp
ActedMonoidUpdPowerSum<T, D>
```

`M = MonoidPowerSum<T, D>`、`MF = MonoidUpdFlag<T>`、`S = M::S`、`F = MF::S`。
`S` の構築・メンバは [MonoidPowerSum](../power_sum.md) を参照。
零列の構築には `vc<T>(n, T{})` を渡す。

##### 制約

- $D \geq 0$
- `T` は $0$, $1$, 加算、乗算、等値比較を持つ可換半環で、`T{}` は $0$
- 符号付き整数型では、保持する値と各次数の累乗和が `T` に収まる
- 整数型以外では、作用の積和を `T` で計算できる

#### F のコンストラクタ

```cpp
(1) F()
(2) F(T val)
(3) F(bool is_id, T val)
```

- (1)：恒等作用を構築する。
- (2)：各値を `val` に代入する作用を構築する。
- (3)：`is_id` が `true` なら恒等作用、`false` なら `val` への代入を構築する。

##### 計算量

- $O(1)$

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

`f.is_id` が `true` なら `x` を返す。
それ以外では、`power_sum[0]` を維持し、$k\geq 1$ の成分を `x.power_sum[0] * f.val^k` に更新する。

##### 計算量

- $O(D)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を作用させる合成を返す。
`f` が恒等作用なら `g`、それ以外では `f` を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

恒等作用 `{true, T{}}` を返す。

##### 計算量

- $O(1)$

### pow（集約値の反復）

```cpp
template <class I>
static constexpr S pow(const S& x, I k);
```

このヘッダにある各型で、集約値 `S` を `op` により `k` 回結合する。値モノイド `M` の高速な `pow` を使う。指数 $k$ は非負整数とし、`k = 0` なら `e()` を返す。型と整数倍の条件は [pow の共通仕様](../monoid_power.md) を参照。

##### 計算量

- $O(D)$。整数倍を使う場合は、その演算を $O(1)$ とする。
