## 概要

添字の一次式 $c+di$ の区間代入と、区間最小値・最大値の取得を処理する作用つきモノイド。
係数は定数項から順に `{c, d}` と並べる。

- `ActedMonoidAriUpdMin`：最小値を `.mn` で取得する。
- `ActedMonoidAriUpdMax`：最大値を `.mx` で取得する。
- `ActedMonoidAriUpdMinMax`：最小値を `.mn`、最大値を `.mx` で取得する。

構築時は `S(i, a[i])` を並べた列を渡す。`set` でも `S(i, val)` として添字を保持する。
代入後の最小値は、公差が非負なら区間左端、負なら右端の値になる。
最大値はその反対側の端の値になる。最小値と最大値を同時に持つ場合も、両端で一次式を評価すればよい。

`infty` の省略時は [型ごとの既定の無限大](../../utils/default_infty.md) を使う。値や関数を明示した場合は、その指定を使う。

整数の積和は unsigned で計算し、最終結果を `T` に戻す。整数型以外は `T` の演算を使う。

## 使用例

```cpp
#include "algebra/acted_monoid/ariupd_min.hpp"
#include "ds/segtree/lazy_segtree.hpp"

using AM = ActedMonoidAriUpdMin<ll>;
vc<AM::S> init;
for (int i = 0; i < 6; i++) init.emplace_back(i, 100);
LazySegmentTree<AM> seg(init);

int l = 1, r = 5;
ll a = 10, d = -3;
seg.apply(l, r, AM::F({a - d * l, d})); // 10, 7, 4, 1 を代入
assert(seg.prod(l, r).mn == 1);

seg.apply(2, 4, AM::F({0, 0})); // 0 を代入
assert(seg.all_prod().mn == 0);
```

最大値取得：

```cpp
using AM = ActedMonoidAriUpdMax<ll>;
LazySegmentTree<AM> seg(vc<AM::S>{{0, 4}, {1, 1}, {2, 7}});
seg.apply(0, 3, AM::F({10, -3})); // 10, 7, 4 を代入
assert(seg.all_prod().mx == 10);
```

最小値・最大値の取得：

```cpp
using AM = ActedMonoidAriUpdMinMax<ll>;
LazySegmentTree<AM> seg(vc<AM::S>{{0, 4}, {1, 1}, {2, 7}});
seg.apply(0, 3, AM::F({10, -3})); // 10, 7, 4 を代入
auto result = seg.all_prod();
assert(result.mn == 4 && result.mx == 10);
```

## 詳細なドキュメント

### ActedMonoidAriUpdMin

```cpp
ActedMonoidAriUpdMin<T, infty = nullptr>
```

要素型 `S` は区間の端点 `l`, `r` と最小値 `mn` を持つ。
`MF = MonoidUpdFlag<array<T, 2>>`、`F = MF::S`。
`F` は係数列 `val = {c, d}` と、恒等操作を表す `is_id` を持つ。

##### 制約

- `T` は整数からの変換、加算、乗算、大小比較を持ち、一次式 $c+di$ の大小関係が公差の符号と整合する
- 整数型では、各位置への代入結果が `T` に収まる
- 整数型以外では、作用の積和を `T` で計算できる
- `infty` は取り得るすべての値以上

#### S のコンストラクタ

```cpp
(1) S()
(2) S(int i, T val)
(3) S(int l, int r, T mn)
```

- (1)：`l = r = 0`, `mn = infty` の空区間を構築する。
- (2)：添字 `i`、値 `val` の一点、すなわち区間 $[i,i+1)$ を構築する。
- (3)：区間 $[l,r)$ とその最小値 `mn` を指定して構築する。

##### 制約

- (2)：$i \geq 0$
- (3)：$0 \leq l \leq r$。空区間では `mn = infty`

##### 計算量

- $O(1)$

#### F のコンストラクタ

```cpp
(1) F()
(2) F(array<T, 2> val)
(3) F(bool is_id, array<T, 2> val)
```

- (1)：恒等操作を構築する。
- (2)：`val = {c, d}` として、各位置 `i` へ $c+di$ を代入する操作を構築する。
- (3)：恒等操作かどうかと係数列を指定する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

最小値と、含まれる添字の左右端をまとめる。空区間は単位元として扱う。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

空区間を返す。

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(const F& f, S x)
```

恒等操作または空区間なら `x` を返す。
それ以外では、`f.val[1]` が非負なら `x.l`、負なら `x.r - 1` で一次式を評価し、`x.mn` に代入する。
端点は維持する。

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を作用させる合成を返す。`f.is_id` が真なら `g`、それ以外なら `f`。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

恒等操作を返す。

##### 計算量

- $O(1)$

### ActedMonoidAriUpdMax

```cpp
ActedMonoidAriUpdMax<T, infty = nullptr>
```

要素型 `S` は区間の端点 `l`, `r` と最大値 `mx` を持つ。
作用型 `F`、そのコンストラクタ、`composition`、`id` は `ActedMonoidAriUpdMin` と共通で、すべて $O(1)$。

##### 制約

- 型と演算の条件は `ActedMonoidAriUpdMin` と共通
- `-infty` は取り得るすべての値以下

#### S のコンストラクタ

```cpp
(1) S()
(2) S(int i, T val)
(3) S(int l, int r, T mx)
```

- (1)：`l = r = 0`, `mx = -infty` の空区間を構築する。
- (2)：添字 `i`、値 `val` の一点を構築する。
- (3)：区間 $[l,r)$ とその最大値 `mx` を指定して構築する。

##### 制約

- (2)：$i \geq 0$
- (3)：$0 \leq l \leq r$。空区間では `mx = -infty`

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

最大値と、含まれる添字の左右端をまとめる。空区間は単位元として扱う。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

空区間を返す。

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(const F& f, S x)
```

恒等操作または空区間なら `x` を返す。
それ以外では、`f.val[1]` が非負なら `x.r - 1`、負なら `x.l` で一次式を評価し、`x.mx` に代入する。
端点は維持する。

##### 計算量

- $O(1)$

### ActedMonoidAriUpdMinMax

```cpp
ActedMonoidAriUpdMinMax<T, infty = nullptr>
```

要素型 `S` は区間の端点 `l`, `r` と最小値 `mn`、最大値 `mx` を持つ。
作用型 `F`、そのコンストラクタ、`composition`、`id` は `ActedMonoidAriUpdMin` と共通で、すべて $O(1)$。

##### 制約

- 型と演算の条件は `ActedMonoidAriUpdMin` と共通
- `-infty` は取り得るすべての値以下、`infty` は取り得るすべての値以上

#### S のコンストラクタ

```cpp
(1) S()
(2) S(int i, T val)
(3) S(int l, int r, T mn, T mx)
```

- (1)：`l = r = 0`, `mn = infty`, `mx = -infty` の空区間を構築する。
- (2)：添字 `i`、値 `val` の一点を構築する。`mn` と `mx` はともに `val`。
- (3)：区間 $[l,r)$ とその最小値 `mn`、最大値 `mx` を指定して構築する。

##### 制約

- (2)：$i \geq 0$
- (3)：$0 \leq l \leq r$。空区間では `mn = infty`, `mx = -infty`、それ以外では `mn <= mx`

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

最小値・最大値と、含まれる添字の左右端をまとめる。空区間は単位元として扱う。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

空区間を返す。

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(const F& f, S x)
```

恒等操作または空区間なら `x` を返す。
それ以外では、`x.l` と `x.r - 1` で一次式を評価し、小さい方を `x.mn`、大きい方を `x.mx` に代入する。
端点は維持する。

##### 計算量

- $O(1)$
