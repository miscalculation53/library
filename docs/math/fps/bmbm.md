このドキュメントには AI による作成・変更が含まれています。

## 概要

Berlekamp–Massey で線形漸化式を復元して Bostan–Mori で第 $k$ 項を求めるという一連の流れをライブラリにしたもの。

必要な項数：線型漸化的数列が $d$ 個の係数で表される線形漸化式を持つことがわかっているとき、先頭 $2d + 1$ 項がわかっていれば正しい線形漸化式が復元できる。

## 使用例

```cpp
using mint = modint998244353;
auto x = bmbm(vc<mint>{0, 1, 1, 2, 3, 5}, 10, false);
assert(x == 55);
```

## 詳細なドキュメント

#### linear_recurrence_gf

```cpp
pair<FormalPowerSeries<mint>, FormalPowerSeries<mint>>
linear_recurrence_gf(const vc<mint>& a, const vc<mint>& c)
```

$d = |c| - 1$ として、初期項 $a_0, \dots, a_{d-1}$ と漸化式 $a_i = \sum_{j=1}^d c_j a_{i-j}$ の母関数を表す組 $(p, q)$ を返す。$c_0$ は任意の値でよい。

$q$ の定数項は $1$ で、高次のゼロ係数を除いて返す。$p$ は元の漸化式の長さ $d$ を使って求めるため、$c_d = 0$ の場合も初期項を保つ。

##### 制約

- $|c| \geq 1$
- $0 \leq d \leq |a|$

##### 計算量

$n = |a|$ として

- $O((n+d) \log(n+d))$

#### bmbm

```cpp
mint bmbm(const vc<mint>& a, ll k, bool show_coefs = true)
```

数列 $a_0, a_1, \dots$ が十分な項数の与えられた線形漸化的数列であることを仮定して、$a$ の第 $k$ 項を推測する。`show_coefs` を `true` にすると、復元した線形漸化式の係数を dump する。

##### 制約

- $k \geq 0$

##### 計算量

$n = \lvert a \rvert$ として

- $O(n^2 + n \log n \log k)$
