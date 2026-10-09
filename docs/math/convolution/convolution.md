## 概要

ACL の畳み込みにだいたい準拠。

## 詳細なドキュメント

#### convolution_point_get

```cpp
T convolution_point_get(vc<T> a, vc<T> b)
```

$a$ と $b$ を畳み込んだときの $p$ 番目の要素を返す。長さが足りないときは $0$ を返す。

##### 計算量

- $O(\lvert a \rvert + \lvert b \rvert)$

#### ntt, intt

```cpp
(1) void ntt(vc<mint> &a)
(2) void intt(vc<mint> &a)
```

$a$ に対し NTT / INTT を行う（破壊的に変更する）。`intt` は正規化前の値を返すため、`intt(ntt(a))` は元の配列の $|a|$ 倍になる。元に戻す際は `mint(a.size()).inv()` を各係数に掛ける。

##### 制約

- `mint` は static_modint、法は素数
- $a$ の長さは $2$ べき
- $\lvert a \rvert = 2^k$ として、`mint::mod() - 1` は $2$ で $k$ 回以上割り切れる

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
  - サイズが小さい場合や $a$ と $b$ のどちらかが疎な場合、$a, b$ の非零要素の個数を $n', m'$ として $O(n + m + \min(n'm, nm'))$ の実装に切り替える。
  - 両方が疎な場合は非零項同士を列挙し、$O(n+m+n'm')$ で求める。

