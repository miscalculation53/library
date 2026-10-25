このドキュメントには AI による作成・変更が含まれています。

## 概要

https://noshi91.github.io/algorithm-encyclopedia/polynomial-interpolation-geometric#noredirect

導出は ↑ を参照（結構手間）。

実装上は $r^n \equiv 1$ となる場合にちょっと注意。

## 使用例

```cpp
using mint = modint998244353;
auto f = interpolation_geo(mint(1), mint(2), vc<mint>{2, 3, 5});
assert(f.eval(3) == 4); // f(x) = 1 + x
```

## 詳細なドキュメント

#### interpolation_geo

```cpp
FormalPowerSeries<mint> interpolation_geo(mint a, mint r, vc<mint> ys)
```

$f(a r^i) = y_i \ (0 \leq i \leq n-1)$ を満たす、高々 $n-1$ 次の多項式 $f(x)$ を求める。

##### 制約

- $r^i \not\equiv 1 \ (1 \leq i \leq n-1)$

##### 計算量

- $O(n \log n)$
