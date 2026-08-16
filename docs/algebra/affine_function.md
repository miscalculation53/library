## 概要

一次関数を合成で扱うモノイドと群。

## 詳細なドキュメント

### MonoidAffineFunction

```cpp
MonoidAffineFunction<T>
```

一次関数 $f(x)=bx+c$ を合成で扱うモノイド。要素型 `S` は係数 `b`, `c` を持つ。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T b, T c)
```

(2) は一次関数 $x \mapsto bx+c$ を構築する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& f, const S& g)
```

合成 $f \circ g$ を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

恒等関数 $x \mapsto x$、すなわち `{1, 0}` を返す。

##### 計算量

- $O(1)$

### GroupAffineFunction

```cpp
GroupAffineFunction<mint>
```

一次関数 $f(x)=ax+b$ を合成で扱う群。要素型 `S` は係数 `a`, `b` を持つ。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(mint a, mint b)
(3) S(const pair<T, U>& ab)
```

- (2)：一次関数 $x \mapsto ax+b$ を構築する。
- (3)：`ab.first`, `ab.second` を係数として構築する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& f, const S& g)
```

合成 $f \circ g$ を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

恒等関数 $x \mapsto x$、すなわち `{1, 0}` を返す。

##### 計算量

- $O(1)$

#### inv

```cpp
S inv(const S& f)
```

$f(x)=ax+b$ の逆関数 $f^{-1}(x)=a^{-1}x-ba^{-1}$ を返す。

##### 制約

- `f.a` が乗法逆元を持つ

##### 計算量

- `mint` の逆元計算 $1$ 回
