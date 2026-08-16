## 概要

作用つきモノイド：加算・最小値・最小値の個数取得

## 詳細なドキュメント

### ActedMonoidAddMinCount

```cpp
ActedMonoidAddMinCount<T, U = ll, infty = INF>
```

区間加算と、区間最小値およびその出現数の取得を処理する作用つきモノイド。

要素型 `S` は最小値 `mn` と個数 `cnt` を持ち、作用型 `F` は `T`。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T mn, U cnt = 1)
```

(2) は最小値 `mn` とその個数 `cnt` を持つ要素を構築する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

`a.mn`, `b.mn` の小さい方を返す。等しい場合は `cnt` を加算する。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

単位元 `{infty, 0}` を返す。

##### 制約

- `infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(F f, S x)
```

最小値に `f` を加えた `{f + x.mn, x.cnt}` を返す。

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

### ActedMonoidAddMaxCount

```cpp
ActedMonoidAddMaxCount<T, U = ll, infty = INF>
```

区間加算と、区間最大値およびその出現数の取得を処理する。要素型 `S` は `mx`, `cnt` を持つ。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T mx, U cnt = 1)
```

(2) は最大値 `mx` とその個数 `cnt` を持つ要素を構築する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

`a.mx`, `b.mx` の大きい方を返す。等しい場合は `cnt` を加算する。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

単位元 `{-infty, 0}` を返す。

##### 制約

- `-infty` は取り得るすべての値以下

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(F f, S x)
```

`{f + x.mx, x.cnt}` を返す。

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
