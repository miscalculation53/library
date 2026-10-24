このドキュメントには AI による作成・変更が含まれています。

## 概要

作用つきモノイド：更新・和取得

## 使用例

```cpp
#include "algebra/acted_monoid/upd_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"

using AM = ActedMonoidUpdSum<ll>;
LazySegmentTree<AM> seg(vc<ll>{1, 2, 3});
seg.apply(0, 2, AM::F(5));
assert(seg.all_prod().val == 13);
```

## 詳細なドキュメント

### ActedMonoidUpdSum

```cpp
ActedMonoidUpdSum<T, L = ll>
```

区間代入と区間和取得を処理する作用つきモノイド。

要素型 `S` は区間長 `len` と総和 `val` を持つ。作用型 `F` は恒等作用かを表す `is_id` と代入値 `val` を持つ。

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
(2) F(T val)
(3) F(bool is_id, T val)
```

- (2)：各要素を `val` に代入する作用を構築する。
- (3)：`is_id` が `true` なら恒等作用、`false` なら `val` への代入を構築する。

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

空区間を表す `{0, T{}}` を返す。

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

`f.is_id` なら `x`、そうでなければ全要素を `f.val` に代入した `{x.len, f.val * x.len}` を返す。

##### 制約

- 積が `T` に収まる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を作用させる合成を返す。`f` が恒等作用なら `g`、そうでなければ `f`。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

恒等作用 `{true, T{}}` を返す。

##### 計算量

- $O(1)$
