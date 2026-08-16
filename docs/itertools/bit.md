## 概要

部分集合・上位集合全探索

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
