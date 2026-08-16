## 概要

最小値の個数も持つモノイド

## 詳細なドキュメント

### MonoidMinCount

```cpp
MonoidMinCount<T, U = ll, infty = INF>
```

最小値と、その最小値の出現数を持つモノイド。要素型 `S` は `mn`, `cnt` を持つ。

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

#### pow

```cpp
S pow(const S& a, I k)
```

`a` を `k` 回 `op` でまとめた値を返す。`k = 0` なら単位元、それ以外なら `{a.mn, a.cnt * k}` を返す。

##### 制約

- $k \geq 0$

##### 計算量

- $O(1)$

### MonoidMaxCount

```cpp
MonoidMaxCount<T, U = ll, infty = INF>
```

最大値と、その最大値の出現数を持つモノイド。要素型 `S` は `mx`, `cnt` を持つ。

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

#### pow

```cpp
S pow(const S& a, I k)
```

`a` を `k` 回 `op` でまとめた値を返す。`k = 0` なら単位元、それ以外なら `{a.mx, a.cnt * k}` を返す。

##### 制約

- $k \geq 0$

##### 計算量

- $O(1)$
