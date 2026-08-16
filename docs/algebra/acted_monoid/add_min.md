## 概要

作用つきモノイド：加算・最小値／最大値取得

## 詳細なドキュメント

### ActedMonoidAddMin

```cpp
ActedMonoidAddMin<T, infty = INF>
```

区間加算と区間最小値取得を処理するための作用つきモノイド。`S = F = T`。

#### op

```cpp
S op(S a, S b)
```

`min(a, b)` を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

モノイドの単位元 `infty` を返す。

##### 制約

- `infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(F f, S x)
```

`f + x` を返す。

##### 制約

- 加算結果が `T` に収まる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を作用させる合成 `f + g` を返す。

##### 制約

- 加算結果が `T` に収まる

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

作用の単位元 `T{}` を返す。

##### 計算量

- $O(1)$

### ActedMonoidAddMinMax

```cpp
ActedMonoidAddMinMax<T, infty = INF>
```

区間加算と、区間最小値・最大値の取得を処理する作用つきモノイド。要素型 `S` は公開メンバ `mn`, `mx` を持ち、作用型 `F` は `T`。

`ActedMonoidClampMinMax` でも同じ操作を処理できるが、この構造体は加算専用なので作用型が小さく、作用と作用の合成も単純になる。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T x)
(3) S(T mn, T mx)
```

- (2)：値 `x` の一点を構築する。
- (3)：最小値 `mn`、最大値 `mx` の区間情報を構築する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

`a`, `b` を連結した区間の最小値と最大値を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

`{infty, -infty}` に相当する空区間を返す。

##### 制約

- `-infty` は取り得るすべての値以下、`infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(F f, S x)
```

区間 `x` の最小値と最大値に `f` を加える。空区間は変更しない。

##### 制約

- 加算結果が `T` に収まる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を加える合成 `f + g` を返す。

##### 制約

- 加算結果が `T` に収まる

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

加算作用の単位元 `T{}` を返す。

##### 計算量

- $O(1)$

### ActedMonoidAddMax

```cpp
ActedMonoidAddMax<T, infty = INF>
```

区間加算と区間最大値取得を処理するための作用つきモノイド。各関数の計算量は $O(1)$。

#### op

```cpp
S op(S a, S b)
```

`max(a, b)` を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

モノイドの単位元 `-infty` を返す。

##### 制約

- `-infty` は取り得るすべての値以下

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(F f, S x)
```

`f + x` を返す。

##### 制約

- 加算結果が `T` に収まる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を作用させる合成 `f + g` を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

作用の単位元 `T{}` を返す。

##### 計算量

- $O(1)$
