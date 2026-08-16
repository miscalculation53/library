## 概要

https://37zigen.com/lagrange-interpolation/

$n$ 個の点 $(x_i, y_i)$ が与えられたとき、$f(x_i) = y_i \ (0 \leq i \leq n-1)$ を満たす高々 $n-1$ 次の多項式 $f(x)$ を求めるアルゴリズム。

$\displaystyle f(x) = \sum_{i=0}^{n-1} y_i \prod_{\substack{0\leq j\leq n-1\\\\  j\neq i}} \frac{x - x_j}{x_i - x_j}$

と書ける。さらに $\displaystyle g(x) \coloneqq \prod_{i=0}^{n-1} (x - x_i)$ とおくと $\displaystyle g'(x) = \sum_{i=0}^{n-1} \prod_{\substack{0\leq j\leq n-1\\\\  j\neq i}} (x - x_j)$, $\displaystyle g'(x_i) = \prod_{\substack{0\leq j\leq n-1\\\\  j\neq i}} (x_i - x_j)$ であるから、$f(x)$ は次のように書き換えられる。

$\displaystyle f(x) = g(x) \sum_{i=0}^{n-1} \frac{y_i}{g'(x_i)} \cdot \frac{1}{x - x_i}$

結局、計算手順としては次のようになる：

- $g(x)$ を多項式の総積で $O(n \log^2 n)$ 時間で計算する。
- 各 $g'(x_i)$ を多点評価で $O(n \log^2 n)$ 時間で計算する。
- $f(x)$ を有理式の総和で $O(n \log^2 n)$ 時間で計算する。分母は $g(x)$ であるから分子を答えとすればよい。

## 詳細なドキュメント

#### interpolation

```cpp
FormalPowerSeries<mint> interpolation(vc<mint> xs, vc<mint> ys)
```

$f(x_i) = y_i \ (0 \leq i \leq n-1)$ を満たす高々 $n-1$ 次の多項式 $f(x)$ を求める。

##### 制約

- $\lvert \mathrm{xs} \rvert = \lvert \mathrm{ys} \rvert$

##### 計算量

- $O(n \log^2 n)$
