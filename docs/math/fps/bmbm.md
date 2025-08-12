## BMBM

Berlekamp–Massey で線形漸化式を復元して Bostan–Mori で第 $k$ 項を求めるという一連の流れをライブラリにしたもの。

必要な項数：線型漸化的数列が $d$ 個の係数で表される線形漸化式を持つことがわかっているとき、先頭 $2d + 1$ 項がわかっていれば正しい線形漸化式が復元できる。

#### linear_recurrence

```cpp
mint linear_recurrence(vc<mint> a, vc<mint> c, ll k)
```

$a_0, \dots, a_{n-1}$ と $c_1, \dots, c_d$ を与える（$c_0$ は適当な値でよい）。すると、$a_i = \sum_{j=1}^d c_j a_{i-j}$ で定まる無限数列 $a$ の第 $k$ 項を返す。

##### 制約

- $1 \leq d \leq n$
- $k \geq 0$

##### 計算量

- $O(d \log d \log k)$

#### bmbm

```cpp
mint bmbm(vc<mint> a, ll k, bool show_coefs = true)
```

数列 $a_0, a_1, \dots$ が十分な項数の与えられた線形漸化的数列であることを仮定して、$a$ の第 $k$ 項を推測する。`show_coefs` を `true` にすると、復元した線形漸化式の係数を dump する。

##### 制約

- $k \geq 0$

##### 計算量

$n = \lvert a \rvert$ として

- $O(n^2 + n \log n \log k)$
