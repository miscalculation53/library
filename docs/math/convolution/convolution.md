## 畳み込み

ACL の畳み込みにだいたい準拠。

#### ntt, intt

```cpp
(1) void ntt(vc<mint> &a)
(2) void intt(vc<mint> &a)
```

$a$ に対し NTT / INTT を行う（破壊的に変更する）。

##### 制約

- `mint` は static_modint
- $a$ の長さは $2$ べき
- $\lvert a \rvert = 2^k$ として、`mint::mod()` は $2$ で $k$ 回以上割り切れる

##### 計算量

- $O(\lvert a \rvert \log \lvert a \rvert)$


#### convolution

```cpp
(1) vc<mint> convolution(vc<mint> a, vc<mint> b)
(2) vc<ll> convolution<mod = 998244353> convolution(vc<ll> a, vc<ll> b)
```

- (1)：$\lvert a \rvert = n, \lvert b \rvert = m$ とする。畳み込みを計算し、長さ $n+m-1$ の vector を返す。ただし、$n = 0$ または $m = 0$ のときは空の vector を返す。
- (2)：同じだが、vector の要素は `ll` などの整数で、`mod` をテンプレート引数で指定する。

##### 制約

- $n + m - 1 \leq 2^{26}$
  - ただし、ここまで大きいとそもそも TLE する

##### 計算量

- $O((n+m)\log(n+m))$
  - NTT が可能**でない**場合、定数倍が $3$ 倍程度つく。

#### convolution64

```cpp
vc<ull> convolution64(vc<ull> a, vc<ull> b)
```

畳み込みを $\bmod \ 2^{64}$ で計算する。

##### 制約

- $n + m - 1 \leq 2^{25}$
  - ただし、ここまで大きいとそもそも TLE する

##### 計算量

- $O((n+m)\log(n+m))$
  - 定数倍は NTT の $5$ 倍程度つく。

#### convolution_4e18

```cpp
vc<ll> convolution_4e18(vc<ll> a, vc<ll> b)
```

畳み込みの最終結果が $4.2 \times 10^{18}$ 程度に収まる場合に、結果を `ll` で計算する。

ACL の `convolution_ll` は `ll` の範囲を全部できるようにしている代わりに mod が $3$ つになっている。欲しくなったらこれも作るかも。

##### 制約

- $n+m-1 \leq 2^{25}$
  - ただし、ここまで大きいとそもそも TLE する

##### 計算量

- $O((n+m)\log(n+m))$
  - 定数倍は NTT の $2$ 倍程度つく。
