## 概要

代数的構造（ビット演算）

## 詳細なドキュメント

### GroupXor

```cpp
GroupXor<T>
```

XOR を演算とする群。`S = T`。

#### op

```cpp
S op(S x, S y)
```

`x ^ y` を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

$0$ を返す。

##### 計算量

- $O(1)$

#### inv

```cpp
S inv(S x)
```

XOR に関する逆元 `x` 自身を返す。

##### 計算量

- $O(1)$

#### pow

```cpp
S pow(S x, I k)
```

`x` を `k` 回 XOR した値を返す。`k` が奇数なら `x`、偶数なら $0$ を返す。

##### 制約

- $k \geq 0$

##### 計算量

- $O(1)$

### MonoidAnd

```cpp
MonoidAnd<T>
```

AND を演算とするモノイド。

#### op

```cpp
S op(S x, S y)
```

`x & y` を返す。

##### 制約

- `T` は `&` が使える整数型

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

`numeric_limits<T>::max()` を返す。

##### 計算量

- $O(1)$

#### pow

```cpp
S pow(S x, I k)
```

`x` を `k` 回 AND した値を返す。`k = 0` なら単位元、それ以外なら `x` を返す。

##### 制約

- $k \geq 0$

##### 計算量

- $O(1)$

### MonoidOr

```cpp
MonoidOr<T>
```

OR を演算とするモノイド。

#### op

```cpp
S op(S x, S y)
```

`x | y` を返す。

##### 制約

- `T` は `|` が使える整数型

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

$0$ を返す。

##### 計算量

- $O(1)$

#### pow

```cpp
S pow(S x, I k)
```

`x` を `k` 回 OR した値を返す。`k = 0` なら単位元、それ以外なら `x` を返す。

##### 制約

- $k \geq 0$

##### 計算量

- $O(1)$

### エイリアス

```cpp
(1) RingXorAnd<T>
(2) SemiRingOrAnd<T>
(3) SemiRingAndOr<T>
```

- (1)：加法を XOR、乗法を AND とする環。
- (2)：加法を OR、乗法を AND とする半環。
- (3)：加法を AND、乗法を OR とする半環。

各演算の計算量は $O(1)$。
