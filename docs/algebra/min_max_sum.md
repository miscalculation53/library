## 概要

区間の長さ、和、最小値、最大値のうち必要な情報を持つモノイド。

## 使用例

```cpp
using M = MonoidMinMaxSum<ll>;
M::S x(3), y(5);
auto z = M::op(x, y);
```

## 詳細なドキュメント

### MonoidLenSum

```cpp
MonoidLenSum<T>
```

要素型 `S` は区間長 `len` と総和 `val` を持つ。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T val)
(3) S(int len, T val)
```

(2) は長さ $1$、総和 `val` の要素を構築する。(3) は長さと総和を直接指定する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

長さと総和をそれぞれ加算する。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

空区間 `{0, T{}}` を返す。

##### 計算量

- $O(1)$

### MonoidMinMax

```cpp
MonoidMinMax<T, infty = INF>
```

要素型 `S` は最小値 `mn` と最大値 `mx` を持つ。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T x)
(3) S(T mn, T mx)
```

(1) は空区間、(2) は値 `x` の一点、(3) は最小値と最大値を直接指定した要素を構築する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

最小値の `min` と最大値の `max` を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

空区間 `{infty, -infty}` を返す。

##### 制約

- `-infty` は取り得るすべての値以下
- `infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

### MonoidMinSum / MonoidMaxSum

```cpp
MonoidMinSum<T, infty = INF>
MonoidMaxSum<T, infty = INF>
```

`MonoidMinSum` の要素型 `S` は `mn`, `sum`, `len`、`MonoidMaxSum` は `mx`, `sum`, `len` を持つ。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T x, int len = 1)
(3) S(T mn_or_mx, T sum, int len)
```

(1) は空区間、(2) は値 `x` が `len` 個並ぶ区間、(3) は各情報を直接指定した要素を構築する。

##### 制約

- `len >= 0`
- `x * len` が `T` に収まる

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

最小値または最大値をまとめ、総和と長さを加算する。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

長さ $0$ の空区間を返す。

##### 制約

- `-infty` は取り得るすべての値以下
- `infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

### MonoidMinMaxSum

```cpp
MonoidMinMaxSum<T, infty = INF>
```

要素型 `S` は `mn`, `mx`, `sum`, `len` を持つ。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T x, int len = 1)
(3) S(T mn, T mx, T sum, int len)
```

(1) は空区間、(2) は値 `x` が `len` 個並ぶ区間、(3) は各情報を直接指定した要素を構築する。

##### 制約

- `len >= 0`
- `x * len` が `T` に収まる

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

最小値と最大値をまとめ、総和と長さを加算する。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

長さ $0$ の空区間を返す。

##### 制約

- `-infty` は取り得るすべての値以下
- `infty` は取り得るすべての値以上

##### 計算量

- $O(1)$
