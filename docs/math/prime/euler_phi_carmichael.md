## オイラーのファイ関数・カーマイケル関数

### 概要

オイラーのファイ関数 $\phi(n)$ は、$1$ 以上 $n$ 以下で $n$ と互いに素な整数の個数。

$n = p_1^{e_1} \cdots p_k^{e_k}$ とするとき $\phi(n) = n\prod_i\frac{p-1}{p} = \prod_i p_i^{e_i-1}(p_i-1)$

オイラーの定理：$\gcd(a, n) = 1$ を満たす $a$ に対し $a^{\phi(n)} \equiv 1 \pmod n$

カーマイケル関数 $\lambda(n)$ は、$\gcd(a, n) = 1$ を満たす $a$ すべてに対し $a^e \equiv 1 \pmod n$ を満たす最小の $e$（乗法群 $(\mathbb{Z}/n\mathbb{Z})^\times$ の元の位数の最大値）。

$\lambda(1) = 1$

$\lambda(2) = 1, \lambda(2^2) = 2, \lambda(2^e) = 2^{e-2} \: (e \geq 3)$

$\lambda(p^e) = p^{e-1}(p-1) \: (p \geq 3)$

$n = p_1^{e_1} \cdots p_k^{e_k}$ とするとき $\lambda(n) = \mathrm{lcm}(\lambda(p_1^{e_1}), \dots, \lambda(p_k^{e_k}))$

### ライブラリの使い方

```cpp
(1) ll euler_phi(vc<PrimePower<ll>> fac)
(2) ll carmichael(vc<PrimePower<ll>> fac)
```

$n$ の素因数分解形のみを `fac` に渡す。

#### 制約

- `fac` は素因数分解形として正しい
- `fac` が表す整数は `ll` に収まる

#### 計算量

- (1)：$O\left(\dfrac{\log n}{\log\log n}\right)$
- (2)：$O\left(\dfrac{\log^2 n}{\log\log n}\right)$

ただし、`fac` の前計算に $O(n^{1/4})$ かかることに注意。