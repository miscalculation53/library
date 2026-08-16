## 概要

区間 `chmin`・`chmax`・加算と、区間最小値・最大値の取得を通常の遅延セグメント木で処理する作用つきモノイド。

作用は $f(x)=\operatorname{clamp}(x,lo,hi)+bias$ で表す。$f$ は単調非減少なので、任意の列 $a$ について次が成り立つ。

$$
\min_i f(a_i)=f(\min_i a_i),\qquad
\max_i f(a_i)=f(\max_i a_i)
$$

したがって和を取得しない部分問題では Segment Tree Beats は不要で、`LazySegmentTree` を利用できる。

## 使用例

```cpp
using AM = ActedMonoidClampMinMax<ll>;
using F = AM::F;
LazySegmentTree<AM> seg(a);

seg.apply(l, r, F::chmin(x));
seg.apply(l, r, F::chmax(y));
seg.apply(l, r, F::add(z));
auto [mn, mx] = seg.prod(l, r);
```

## 詳細なドキュメント

### ActedMonoidClampMinMax

```cpp
ActedMonoidClampMinMax<T, infty = INF>
```

要素型 `S` は区間最小値 `mn` と区間最大値 `mx` を持つ。作用型 `F` は `MonoidClampAdd<T, infty>::S`。

#### S のコンストラクタ

```cpp
(1) S(T x)
(2) S(T mn, T mx)
```

(1) は値 `x` の一点、(2) は最小値 `mn`、最大値 `mx` の区間情報を構築する。

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

単位元 `{infty, -infty}` を返す。

##### 制約

- `-infty` は取り得るすべての値以下、`infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(const F& f, S x)
```

`x.mn`, `x.mx` に `f` を作用させた区間情報を返す。

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を作用させる合成を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

作用の恒等関数を返す。

##### 計算量

- $O(1)$

### ActedMonoidClampMin / ActedMonoidClampMax

```cpp
ActedMonoidClampMin<T, infty = INF>
ActedMonoidClampMax<T, infty = INF>
```

最小値だけ、または最大値だけを保持する省メモリ版。作用型 `F` は `MonoidClampAdd<T, infty>::S`。

#### op

```cpp
S op(S a, S b)
```

`ActedMonoidClampMin` は `min(a, b)`、`ActedMonoidClampMax` は `max(a, b)` を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

`ActedMonoidClampMin` は `infty`、`ActedMonoidClampMax` は `-infty` を返す。

##### 制約

- `-infty` は取り得るすべての値以下、`infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(const F& f, S x)
```

単位元でなければ `f(x)` を返す。

##### 制約

- 実際の列の値は `-infty`, `infty` と異なる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を作用させる合成を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

作用の恒等関数を返す。

##### 計算量

- $O(1)$
