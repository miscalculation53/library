このドキュメントには AI による作成・変更が含まれています。

## 概要

区間への一次関数作用 $a_i\leftarrow ba_i+c$ と、$0$ 乗から $D$ 乗までの区間和取得を処理する作用つきモノイド。

| 操作 | `F{b, c}` |
| --- | --- |
| $x$ を加算 | `{1, x}` |
| $x$ を乗算 | `{x, 0}` |
| $x$ を代入 | `{0, x}` |
| $bx+c$ を作用 | `{b, c}` |

`seg.prod(l, r).power_sum[k]` で $\sum_{i=l}^{r-1}a_i^k$ を取得する。
$P(x)=\sum_{k=0}^{D}p_kx^k$ に対する $\sum_i P(a_i)$ も、累乗和の線形結合で求められる。

`LazySegmentTree` と組み合わせた構築は $O((D+1)N)$、区間更新は $O((D+1)^2\log N)$、区間取得は $O((D+1)\log N)$。
乗算・代入による更新は $O((D+1)\log N)$。

加算専用の [ActedMonoidAddPowerSum](add_power_sum.md) と、代入専用の [ActedMonoidUpdPowerSum](upd_power_sum.md) もある。これらでは更新値を一つ渡す。

各次数の更新には二項展開

$$
\sum_i(ba_i+c)^k
=\sum_{j=0}^{k}\binom{k}{j}b^jc^{k-j}\sum_i a_i^j
$$

を使う。展開係数は加算・乗算で計算するので、合成数を法とする `modint` にも使える。
整数型の中間積和は unsigned で計算し、結果を `T` に戻す。

## 使用例

```cpp
#include "algebra/acted_monoid/affine_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "math/modint/modint.hpp"

using mint = modint998244353;
using AM = ActedMonoidAffinePowerSum<mint, 4>;
LazySegmentTree<AM> seg(vc<mint>{1, 2, 3, 4});

seg.apply(1, 3, AM::F{1, 5}); // {1, 7, 8, 4}
assert(seg.prod(0, 3).power_sum[3] == mint(856));
assert(seg.prod(1, 4).power_sum[4] == mint(6753));

seg.apply(0, 2, AM::F{2, 0}); // {2, 14, 8, 4}
seg.apply(2, 4, AM::F{0, 3}); // {2, 14, 3, 3}
assert(seg.all_prod().power_sum[2] == mint(218));
```

[AALC001 G - Sum of k-th Powers](https://atcoder.jp/contests/aalc001/tasks/aalc001_g) は、`D = 4` とし、区間加算に `{1, x}` を使う。

## 詳細なドキュメント

### ActedMonoidAffinePowerSum

```cpp
ActedMonoidAffinePowerSum<T, D>
```

`M = MonoidPowerSum<T, D>`、`MF = MonoidAffineFunction<T>`、`S = M::S`、`F = MF::S`。
`S` の構築・メンバは [MonoidPowerSum](../power_sum.md) を参照。
構築時は値の列を渡す。`LazySegmentTree<AM>(n)` は空要素を $n$ 個並べるので、零列の構築には `vc<T>(n, T{})` を渡す。

##### 制約

- $D \geq 0$
- `T` は $0$, $1$, 加算、乗算、等値比較を持つ可換半環で、`T{}` は $0$
- 符号付き整数型では、保持する値・各次数の累乗和・合成した作用の係数が `T` に収まる
- 整数型以外では、展開係数と作用の積和を `T` で計算できる

#### F のコンストラクタ

```cpp
F(T b, T c)
```

作用 $x\mapsto bx+c$ を構築する。

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

各値に $x\mapsto f.b\cdot x+f.c$ を作用させた累乗和を返す。
`power_sum[0]` は維持する。

##### 計算量

- $O(D^2)$
- `f.b == 0` または `f.c == 0` の場合は $O(D)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を作用させる合成 $f\circ g$ を返す。
係数は `{f.b * g.b, f.b * g.c + f.c}`。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

恒等関数 `{1, 0}` を返す。

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
