## 概要

https://37zigen.com/lagrange-interpolation/

$n$ 個の点 $(x_i, y_i)$ が与えられたとき、$f(x_i) = y_i \ (0 \leq i \leq n-1)$ を満たす高々 $n-1$ 次の多項式 $f(x)$ を求めるアルゴリズム。

$\displaystyle f(x) = \sum_{i=0}^{n-1} y_i \prod_{\substack{0\leq j\leq n-1\\\\  j\neq i}} \frac{x - x_j}{x_i - x_j}$

と書ける。さらに $\displaystyle g(x) \coloneqq \prod_{i=0}^{n-1} (x - x_i)$ とおくと $\displaystyle g'(x) = \sum_{i=0}^{n-1} \prod_{\substack{0\leq j\leq n-1\\\\  j\neq i}} (x - x_j)$, $\displaystyle g'(x_i) = \prod_{\substack{0\leq j\leq n-1\\\\  j\neq i}} (x_i - x_j)$ であるから、$f(x)$ は次のように書き換えられる。

$\displaystyle f(x) = g(x) \sum_{i=0}^{n-1} \frac{y_i}{g'(x_i)} \cdot \frac{1}{x - x_i}$

結局、計算手順としては次のようになる：

- 評価点の積木を構築し、根から $g(x)$ を得る。
- 同じ積木で $g'(x_i)$ を多点評価する。
- 葉に $y_i/g'(x_i)$ を置き、積木の分母を共有して分子だけを併合する。

各段で積木と NTT 結果を再利用する。詳しい比較は [FPS の高速化とコスト評価](../../../benchmark/fps_optimization.md) を参照。

## 使用例

```cpp
using mint = modint998244353;
auto f = interpolation(vc<mint>{0, 1, 2}, vc<mint>{1, 6, 17}); // {1, 2, 3}
```

## 詳細なドキュメント

#### interpolation

```cpp
FormalPowerSeries<mint> interpolation(const vc<mint>& xs, const vc<mint>& ys)
```

$f(x_i) = y_i \ (0 \leq i \leq n-1)$ を満たす高々 $n-1$ 次の多項式 $f(x)$ を求める。

##### 制約

- $\lvert \mathrm{xs} \rvert = \lvert \mathrm{ys} \rvert$
- 評価点 $x_i$ は相異なる。係数は体上で計算する。

##### 計算量

- $O(n \log^2 n)$
