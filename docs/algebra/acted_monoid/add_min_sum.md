## 概要

区間加算と、区間最小値または最大値および区間和の取得を処理する作用つきモノイド。

## 使用例

```cpp
using AM = ActedMonoidAddMinSum<ll>;
LazySegmentTree<AM> seg(a);

seg.apply(l, r, x);
auto res = seg.prod(l, r);
cout << res.mn << ' ' << res.sum << '\n';
```

## 詳細なドキュメント

### ActedMonoidAddMinSum

```cpp
ActedMonoidAddMinSum<T, infty = INF>
```

要素型 `S` は公開メンバ `mn`, `sum`, `len` を持ち、作用型 `F` は `T`。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T x, int len = 1)
(3) S(T mn, T sum, int len)
```

- (2)：値 `x` が `len` 個並ぶ区間情報を構築する。
- (3)：最小値 `mn`、総和 `sum`、長さ `len` を直接指定する。

##### 制約

- `len >= 0`
- `x * len` が `T` に収まる

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

`a`, `b` を連結した区間の最小値、総和、長さを返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

`{infty, 0, 0}` に相当する空区間を返す。

##### 制約

- `infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(F f, S x)
```

区間 `x` の各要素に `f` を加えた区間情報を返す。空区間は変更しない。

##### 制約

- 値、総和および中間計算が `T` に収まる

##### 計算量

- $O(1)$

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

加算作用の単位元 `T{}` を返す。

##### 計算量

- $O(1)$

### ActedMonoidAddMinMaxSum

```cpp
ActedMonoidAddMinMaxSum<T, infty = INF>
```

区間最小値と最大値を同時に取得する版。要素型 `S` は公開メンバ `mn`, `mx`, `sum`, `len` を持ち、作用型 `F` は `T`。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T x, int len = 1)
(3) S(T mn, T mx, T sum, int len)
```

- (2)：値 `x` が `len` 個並ぶ区間情報を構築する。
- (3)：最小値 `mn`、最大値 `mx`、総和 `sum`、長さ `len` を直接指定する。

##### 制約

- `len >= 0`
- `x * len` が `T` に収まる

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

`a`, `b` を連結した区間の最小値、最大値、総和、長さを返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

`{infty, -infty, 0, 0}` に相当する空区間を返す。

##### 制約

- `-infty` は取り得るすべての値以下、`infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(F f, S x)
```

区間 `x` の各要素に `f` を加えた区間情報を返す。空区間は変更しない。

##### 制約

- 値、総和および中間計算が `T` に収まる

##### 計算量

- $O(1)$

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

加算作用の単位元 `T{}` を返す。

##### 計算量

- $O(1)$

### ActedMonoidAddMaxSum

```cpp
ActedMonoidAddMaxSum<T, infty = INF>
```

要素型 `S` は公開メンバ `mx`, `sum`, `len` を持ち、作用型 `F` は `T`。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T x, int len = 1)
(3) S(T mx, T sum, int len)
```

- (2)：値 `x` が `len` 個並ぶ区間情報を構築する。
- (3)：最大値 `mx`、総和 `sum`、長さ `len` を直接指定する。

##### 制約

- `len >= 0`
- `x * len` が `T` に収まる

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

`a`, `b` を連結した区間の最大値、総和、長さを返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

`{-infty, 0, 0}` に相当する空区間を返す。

##### 制約

- `-infty` は取り得るすべての値以下

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(F f, S x)
```

区間 `x` の各要素に `f` を加えた区間情報を返す。空区間は変更しない。

##### 制約

- 値、総和および中間計算が `T` に収まる

##### 計算量

- $O(1)$

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

加算作用の単位元 `T{}` を返す。

##### 計算量

- $O(1)$
