## 概要

作用つきモノイド：更新・最小値／最大値取得

## 詳細なドキュメント

### ActedMonoidUpdMin

```cpp
ActedMonoidUpdMin<T, infty = INF, idnty = INF + 1>
```

区間代入と区間最小値取得を処理するための作用つきモノイド。`S = F = T`。

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

`f == idnty` なら `x`、そうでなければ `f` を返す。

##### 制約

- 代入する値は `idnty` と異なる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を作用させる合成を返す。`f != idnty` なら `f`、そうでなければ `g`。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

作用の単位元 `idnty` を返す。

##### 計算量

- $O(1)$

### ActedMonoidUpdMinMax

```cpp
ActedMonoidUpdMinMax<T, infty = INF, idnty = INF + 1>
```

区間代入と、区間最小値・最大値の取得を処理する作用つきモノイド。要素型 `S` は公開メンバ `mn`, `mx` を持ち、作用型 `F` は `T`。`idnty` を作用の単位元として使用する。

`ActedMonoidClampMinMax` と `F::set` でも同じ操作を処理できるが、この構造体は代入専用なので作用型が小さく、作用と作用の合成も単純になる。

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

`f != idnty` なら区間 `x` の最小値と最大値をともに `f` に更新する。`f == idnty` の場合と空区間は変更しない。

##### 制約

- 代入する値は `idnty` と異なる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(F f, F g)
```

先に `g`、次に `f` を適用する合成を返す。`f != idnty` なら `f`、そうでなければ `g` を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

更新作用の単位元 `idnty` を返す。

##### 計算量

- $O(1)$

### ActedMonoidUpdMax

```cpp
ActedMonoidUpdMax<T, infty = INF, idnty = INF + 1>
```

区間代入と区間最大値取得を処理するための作用つきモノイド。各関数の計算量は $O(1)$。

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

`f == idnty` なら `x`、そうでなければ `f` を返す。

##### 制約

- 代入する値は `idnty` と異なる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(F f, F g)
```

`f == idnty` なら `g`、そうでなければ `f` を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

作用の単位元 `idnty` を返す。

##### 計算量

- $O(1)$
