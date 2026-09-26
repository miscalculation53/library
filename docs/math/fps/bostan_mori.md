## 概要

$\displaystyle [x^k] \frac{p(x)}{q(x)}$ を求めるアルゴリズム。ただし $[x^0]q(x) \neq 0$ とする。線形漸化的数列の第 $k$ 項を求めるなどの応用がある。

https://qiita.com/ryuhe1/items/da5acbcce4ac1911f47a

$\dfrac{P(x)}{Q(x)} = \dfrac{P(x) Q(-x)}{Q(x) Q(-x)} = \dfrac{U_\text{e}(x^2) + xU_\text{o}(x^2)}{V(x^2)}$ と書ける（$Q(x)Q(-x)$ は偶多項式なので）。すると

- $k$ が偶数のとき、$[x^k] \dfrac{P(x)}{Q(x)} = [x^k] \dfrac{U_\text{e}(x^2)}{V(x^2)} = [x^{k/2}]\dfrac{U_\text{e}(x)}{V(x)}$
- $k$ が奇数のとき、$[x^k] \dfrac{P(x)}{Q(x)} = [x^k] \dfrac{xU_\text{o}(x^2)}{V(x^2)} = [x^{(k-1)/2}]\dfrac{U_\text{o}(x)}{V(x)}$

となるので、添字を半分ずつ小さくできる。

この実装では、次数 $d \ge 64$ のとき、残りの添字 $t$ が $d$ 以下になったら FPS 除算に切り替える。分母の逆数を $t+1$ 項求め、分子との積の第 $t$ 係数を内積で取り出す。これにより、末尾の約 $\log_2 d$ 回の Bostan–Mori の反復を省ける。最初から $k \le d$ なら、次数によらず直接 FPS 除算で求める。

NTT を直接使える法と、任意の法の両方に適用する。[速度比較](bostan_mori_benchmark.md)も参照。

## 使用例

```cpp
using mint = modint998244353;
using fps = FormalPowerSeries<mint>;

// Fibonacci 数列の第 10 項
fps p = {0, 1}, q = {1, -1, -1};
assert(bostan_mori(p, q, 10) == 55);
```

## 詳細なドキュメント

#### bostan_mori

```cpp
template <class mint>
mint bostan_mori(const FormalPowerSeries<mint>& p,
                 const FormalPowerSeries<mint>& q, ll k)
```

$[x^k] \dfrac{p(x)}{q(x)}$ を求める。$k < 0$ のときは $0$ を返す。

##### 制約

- $[x^0] q(x) \neq 0$

##### 計算量

$k \ge 0$、$q(x)$ の次数を $d \ge 1$ として

- $O\!\left(d\log(d+1)\left(1+\log\left(1+\frac{k}{d}\right)\right)\right)$

$\deg p \ge d$ の場合は、最初の多項式除算の計算量が加わる。$d=0$ の場合は $O(|p|)$。
