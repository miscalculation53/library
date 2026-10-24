このドキュメントには AI による作成・変更が含まれています。

## 概要

作用つきモノイド：一次関数作用・和取得

## 使用例

```cpp
#include "algebra/acted_monoid/affine_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"

using AM = ActedMonoidAffineSum<ll>;
LazySegmentTree<AM> seg(vc<ll>{1, 2, 3});
seg.apply(0, 3, AM::F(2, 1));  // x を 2x+1 にする。
assert(seg.all_prod().val == 15);
```

## 詳細なドキュメント

### ActedMonoidAffineSum

```cpp
ActedMonoidAffineSum<T, L = ll>
```

区間への一次関数作用 $x \mapsto bx+c$ と区間和取得を処理する作用つきモノイド。

要素型 `S` は区間長 `len` と総和 `val`、作用型 `F` は係数 `b`, `c` を持つ。

制約：

- `T` は $0$, $1$, 加算、乗算を持ち、計算結果を保持できる

`L` は区間長の整数型で、既定値は `ll`。各区間の長さを保持できる型を選ぶ。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T val)
(3) S(L len, T val)
```

- (2)：長さ $1$、総和 `val` の要素を構築する。
- (3)：長さ `len`、総和 `val` の要素を構築する。

##### 計算量

- $O(1)$

#### F のコンストラクタ

```cpp
(1) F()
(2) F(T b, T c)
```

(2) は作用 $x \mapsto bx+c$ を構築する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(S a, S b)
```

二つの区間を連結した `{a.len + b.len, a.val + b.val}` を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

空区間を表す `{0, 0}` を返す。

##### 計算量

- $O(1)$

#### pow

```cpp
S pow(const S& x, ll k)
```

区間 `x` を `k` 回連結した積を返す。制約と計算量は [MonoidLenSum::pow](../min_max_sum.md#pow) と共通で、$O(1)$。

#### mapping

```cpp
S mapping(F f, S x)
```

`x` の各要素に $f(t)=bt+c$ を作用させ、`{x.len, f.b * x.val + f.c * x.len}` を返す。

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を作用させる合成 $f \circ g$ を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

恒等関数 `{1, 0}` を返す。

##### 計算量

- $O(1)$
