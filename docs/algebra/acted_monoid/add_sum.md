## 概要

作用つきモノイド：加算・和取得

## 詳細なドキュメント

### ActedMonoidAddSum

```cpp
ActedMonoidAddSum<T>
```

区間加算と区間和取得を処理する作用つきモノイド。

要素型 `S` は区間長 `len` と総和 `val` を持ち、作用型 `F` は `T`。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T val)
(3) S(int len, T val)
```

- (2)：長さ $1$、総和 `val` の要素を構築する。
- (3)：長さ `len`、総和 `val` の要素を構築する。

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

#### mapping

```cpp
S mapping(F f, S x)
```

区間の各要素に `f` を加えた `{x.len, x.val + f * x.len}` を返す。

##### 制約

- 積と加算の結果が `T` に収まる

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

作用の単位元 $0$ を返す。

##### 計算量

- $O(1)$
