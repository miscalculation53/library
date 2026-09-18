## 概要

添字の多項式 $P(i)=\sum_{k=0}^{D}c_k i^k$ の区間代入と、区間和取得を処理する作用つきモノイド。
係数は定数項から順に `{c0, c1, ..., cD}` と並べる。

各要素に添字の累乗和を持たせ、総和を $\sum_k c_k\sum_i i^k$ に置き換える。
構築時は `S(i, a[i])` を並べた列を渡す。`set` でも `S(i, val)` として添字を保持する。

整数の積和は unsigned で計算し、最終結果を `T` に戻す。整数型以外は `T` の演算を使う。

## 使用例

```cpp
#include "algebra/acted_monoid/polyupd_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"

using AM = ActedMonoidPolyUpdSum<ll, 2>;
vc<AM::S> init;
for (int i = 0; i < 5; i++) init.emplace_back(i, 10);
LazySegmentTree<AM> seg(init);

seg.apply(1, 4, AM::F({1, 2, 3})); // 1 + 2i + 3i^2 を代入
assert(seg.all_prod().val == 77);

seg.apply(2, 5, AM::F({0, 0, 0})); // 0 を代入
assert(seg.all_prod().val == 16);
seg.apply(0, 5, AM::id());         // 恒等操作
assert(seg.all_prod().val == 16);
```

区間左端基準の $Q(i-l)$ は、絶対添字 $i$ の多項式へ展開して渡す。

## 詳細なドキュメント

### ActedMonoidPolyUpdSum

```cpp
ActedMonoidPolyUpdSum<T, D>
```

`D` は扱う多項式の最大次数。
`M = MonoidIndexPowerSum<T, D>`、`S = M::S`。
`S` の構築・メンバは [MonoidIndexPowerSum](../index_power_sum.md) を参照。

`MF = MonoidUpdFlag<array<T, D + 1>>`、`F = MF::S`。
`F` は係数列 `val` と、恒等操作を表す `is_id` を持つ。

##### 制約

- $D \geq 0$
- `T` は $0$, $1$, 整数からの変換、加算、乗算を持つ可換半環
- 整数型では、保持する値・区間和・添字の累乗和が `T` に収まる
- 整数型以外では、作用の積和を `T` で計算できる

#### F のコンストラクタ

```cpp
(1) F()
(2) F(array<T, D + 1> val)
(3) F(bool is_id, array<T, D + 1> val)
```

- (1)：恒等操作を構築する。
- (2)：係数列 `val` の多項式を代入する操作を構築する。全係数 $0$ の場合は $0$ 代入。
- (3)：恒等操作かどうかと係数列を指定する。

##### 計算量

- $O(D+1)$

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

`f.is_id` が真なら `x` を返す。
それ以外では `x.val` を $\sum_{k=0}^{D} f.val[k]\cdot x.power\_sum[k]$ に置き換える。
`power_sum` は維持する。

##### 計算量

- $O(D+1)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を作用させる合成を返す。`f.is_id` が真なら `g`、それ以外なら `f`。

##### 計算量

- $O(D+1)$

#### id

```cpp
F id()
```

恒等操作を返す。

##### 計算量

- $O(D+1)$
