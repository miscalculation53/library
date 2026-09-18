## 概要

添字の多項式 $P(i)=\sum_{k=0}^{D}c_k i^k$ の区間加算と、区間和取得を処理する作用つきモノイド。
係数は定数項から順に `{c0, c1, ..., cD}` と並べる。

各要素に添字の累乗和を持たせ、総和に $\sum_k c_k\sum_i i^k$ を加える。
構築時は `S(i, a[i])` を並べた列を渡す。`set` でも `S(i, val)` として添字を保持する。

整数の積和は unsigned で計算し、最終結果を `T` に戻す。整数型以外は `T` の演算を使う。

## 使用例

```cpp
#include "algebra/acted_monoid/polyadd_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"

using AM = ActedMonoidPolyAddSum<ll, 2>;
vc<AM::S> init;
for (int i = 0; i < 5; i++) init.emplace_back(i, 0);
LazySegmentTree<AM> seg(init);

seg.apply(1, 4, AM::F{1, 2, 3}); // 1 + 2i + 3i^2 を加算
assert(seg.prod(1, 4).val == 57);

int l = 2, r = 5;
ll a = 10, d = -2;
seg.apply(l, r, AM::F{a - d * l, d, 0}); // 初項 a、公差 d
assert(seg.get(2).val == 27);
```

区間左端基準の $Q(i-l)$ は、絶対添字 $i$ の多項式へ展開して渡す。

## 詳細なドキュメント

### ActedMonoidPolyAddSum

```cpp
ActedMonoidPolyAddSum<T, D>
```

`D` は扱う多項式の最大次数。
`M = MonoidIndexPowerSum<T, D>`、`S = M::S`、`F = array<T, D + 1>`。
`S` の構築・メンバは [MonoidIndexPowerSum](../index_power_sum.md) を参照。

##### 制約

- $D \geq 0$
- `T` は $0$, $1$, 整数からの変換、加算、乗算を持つ可換半環
- 整数型では、保持する係数・値・区間和・添字の累乗和が `T` に収まる
- 整数型以外では、作用の積和を `T` で計算できる

#### op

```cpp
S op(const S& a, const S& b)
```

値の和と添字の累乗和をそれぞれ加算する。

##### 計算量

- $O(D+1)$

#### e

```cpp
S e()
```

空の要素を返す。

##### 計算量

- $O(D+1)$

#### mapping

```cpp
S mapping(const F& f, S x)
```

`x.val` に $\sum_{k=0}^{D} f[k]\cdot x.power\_sum[k]$ を加える。
`power_sum` は維持する。

##### 計算量

- $O(D+1)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を加える合成として、係数ごとの和を返す。

##### 計算量

- $O(D+1)$

#### id

```cpp
F id()
```

すべての係数が $0$ の多項式を返す。

##### 計算量

- $O(D+1)$
