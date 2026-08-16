## 概要

代数的構造（$2 \times 2$ 行列の積）

## 詳細なドキュメント

### GroupMatMul22

```cpp
GroupMatMul22<mint>
```

$2 \times 2$ 行列を行優先の `array<mint, 4>` で持ち、行列積を演算とする群。

```cpp
using G = GroupMatMul22<mint>;
G::S a = {a00, a01, a10, a11};
```

#### op

```cpp
S op(const S& a, const S& b)
```

行列積 $ab$ を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

単位行列 `{1, 0, 0, 1}` を返す。

##### 計算量

- $O(1)$

#### inv

```cpp
S inv(const S& a)
```

行列 `a` の逆行列を返す。

##### 制約

- `mint` は四則演算と逆元計算を持つ
- `a` の行列式が逆元を持つ

##### 計算量

- `mint` の逆元計算 $1$ 回
