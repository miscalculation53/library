## 概要

区間更新と、区間最小値または最大値および区間和の取得を処理する作用つきモノイド。

## 使用例

```cpp
using AM = ActedMonoidUpdMinSum<ll>;
LazySegmentTree<AM> seg(a);

seg.apply(l, r, AM::F(x));
auto res = seg.prod(l, r);
cout << res.mn << ' ' << res.sum << '\n';
```

## 詳細なドキュメント

### ActedMonoidUpdMinSum

```cpp
ActedMonoidUpdMinSum<T, infty = INF>
```

要素型 `S` は公開メンバ `mn`, `sum`, `len` を持つ。作用型 `F` は、単位元かを表す `is_id` と更新値 `val` を持つ。

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

#### F のコンストラクタ

```cpp
(1) F()
(2) F(T val)
(3) F(bool is_id, T val)
```

- (1)：単位元を構築する。
- (2)：区間を `val` に更新する作用を構築する。
- (3)：`is_id` と `val` を直接指定する。

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
S mapping(const F& f, S x)
```

`f` が更新作用なら、区間 `x` のすべての要素を `f.val` に更新した区間情報を返す。`f` が単位元の場合と空区間は変更しない。

##### 制約

- `f.val * x.len` が `T` に収まる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を適用する合成を返す。`f` が更新作用なら `f`、単位元なら `g` を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

更新作用の単位元を返す。

##### 計算量

- $O(1)$

### ActedMonoidUpdMinMaxSum

```cpp
ActedMonoidUpdMinMaxSum<T, infty = INF>
```

区間最小値と最大値を同時に取得する版。要素型 `S` は公開メンバ `mn`, `mx`, `sum`, `len` を持つ。作用型 `F` は、単位元かを表す `is_id` と更新値 `val` を持つ。

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

#### F のコンストラクタ

```cpp
(1) F()
(2) F(T val)
(3) F(bool is_id, T val)
```

- (1)：単位元を構築する。
- (2)：区間を `val` に更新する作用を構築する。
- (3)：`is_id` と `val` を直接指定する。

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
S mapping(const F& f, S x)
```

`f` が更新作用なら、区間 `x` のすべての要素を `f.val` に更新した区間情報を返す。`f` が単位元の場合と空区間は変更しない。

##### 制約

- `f.val * x.len` が `T` に収まる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を適用する合成を返す。`f` が更新作用なら `f`、単位元なら `g` を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

更新作用の単位元を返す。

##### 計算量

- $O(1)$

### ActedMonoidUpdMaxSum

```cpp
ActedMonoidUpdMaxSum<T, infty = INF>
```

要素型 `S` は公開メンバ `mx`, `sum`, `len` を持つ。作用型 `F` は、単位元かを表す `is_id` と更新値 `val` を持つ。

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

#### F のコンストラクタ

```cpp
(1) F()
(2) F(T val)
(3) F(bool is_id, T val)
```

- (1)：単位元を構築する。
- (2)：区間を `val` に更新する作用を構築する。
- (3)：`is_id` と `val` を直接指定する。

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
S mapping(const F& f, S x)
```

`f` が更新作用なら、区間 `x` のすべての要素を `f.val` に更新した区間情報を返す。`f` が単位元の場合と空区間は変更しない。

##### 制約

- `f.val * x.len` が `T` に収まる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を適用する合成を返す。`f` が更新作用なら `f`、単位元なら `g` を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

更新作用の単位元を返す。

##### 計算量

- $O(1)$
