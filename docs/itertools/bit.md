このドキュメントには AI による作成・変更が含まれています。

## 概要

部分集合・上位集合全探索

## 使用例

```cpp
#include "itertools/bit.hpp"

vc<ull> masks;
for (ull mask : bsubsets(0b101ULL)) masks.push_back(mask);
assert(masks == vc<ull>({5, 4, 1, 0}));
masks.clear();
for (ull mask : bsupsets(3, 0b001ULL)) masks.push_back(mask);
assert(masks == vc<ull>({1, 3, 5, 7}));
```

## 詳細なドキュメント

#### bsubsets

次のように書くと、$y \subseteq x$ なる $y$ を値の大きい方から列挙できる。

```cpp
fec(y : bsubsets(x))
```

##### 計算量

- $1$ 回のイテレーションに $O(1)$

$y \subseteq x \subseteq [n]$ なる $(x, y)$ の個数は $3^n$ であることは有名。この列挙を $O(3^n)$ で行うのが主な用途。

#### bsupsets

次のように書くと、$y \subseteq x \subseteq [n]$ なる $x$ を値の小さい方から列挙できる。

```cpp
fec(x : bsupsets(n, y))
```

##### 計算量

- $1$ 回のイテレーションに $O(1)$

$y \subseteq x \subseteq [n]$ なる $(x, y)$ の個数は $3^n$ であることは有名。この列挙を $O(3^n)$ で行うのが主な用途。
