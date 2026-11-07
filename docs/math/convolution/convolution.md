このドキュメントには AI による作成・変更が含まれています。

## 概要

ACL の畳み込みにだいたい準拠。

NTT の長さが短くなる場合は、入力の両端に並ぶ `0` を除いた有効区間で計算する。サイズ境界では、可能な場合に高次係数を別計算し、半分の長さの巡回積から復元する。結果の長さと係数の位置は元の入力に合わせる。

## 使用例

```cpp
using mint = modint998244353;
auto c = convolution(vc<mint>{1, 2}, vc<mint>{3, 4});
assert((c == vc<mint>{3, 10, 8}));
```

## 詳細なドキュメント

#### convolution_point_get

```cpp
T convolution_point_get(const vc<T> &a, const vc<T> &b, int p)
```

$a$ と $b$ を畳み込んだときの $p$ 番目の要素を返す。長さが足りないときは $0$ を返す。

##### 計算量

- $O(\lvert a \rvert + \lvert b \rvert)$

#### ntt, intt

```cpp
(1) void ntt(vc<mint> &a)
(2) void intt(vc<mint> &a)
```

$a$ に対し NTT / INTT を行う（破壊的に変更する）。`intt` は正規化前の値を返すため、`intt(ntt(a))` は元の配列の $\lvert a\rvert$ 倍になる。元に戻す際は `mint(a.size()).inv()` を各係数に掛ける。
静的な 32 bit・64 bit の modint を使え、法に合わせて中間計算の型を選ぶ。
大きい法では非平方元から 2 冪位数の根を作る。

##### 制約

- `mint` は static_modint、法は素数
- $a$ の長さは $2$ べき
- $\lvert a \rvert = 2^k$ として、`mint::mod() - 1` は $2$ で $k$ 回以上割り切れる

##### 計算量

- $O(\lvert a \rvert \log \lvert a \rvert)$

#### ntt_convolution

```cpp
template<class mint>
vc<mint> ntt_convolution(const vc<mint> &a, const vc<mint> &b)
```

`math/convolution/convolution_ntt.hpp` から使える、NTT 用の畳み込み。係数と結果の形式は `convolution` と同じ。空の入力では空配列を返す。

##### 制約

- `mint` は静的 modint、法は素数。
- 空配列を除き、`ntt_ok<mint>(a.size() + b.size() - 1)` が成り立つ。

##### 計算量

- $O((n+m)\log(n+m))$。小さい入力と疎な入力の直接計算は `convolution` と共通。

#### convolution

```cpp
(1) vc<mint> convolution(const vc<mint> &a, const vc<mint> &b)
(2) template<int mod = 998244353, class T>
    vc<T> convolution(const vc<T> &a, const vc<T> &b)
```

- (1)：$\lvert a \rvert = n, \lvert b \rvert = m$ とする。畳み込みを計算し、長さ $n+m-1$ の vector を返す。ただし、$n = 0$ または $m = 0$ のときは空の vector を返す。
- (2)：同じだが、vector の要素は `ll` などの整数で、`mod` をテンプレート引数で指定する。

##### 制約

- $n + m - 1 \leq 2^{26}$
  - ただし、ここまで大きいとそもそも TLE する

##### 計算量

- $O((n+m)\log(n+m))$
  - NTT が可能**でない**場合、定数倍が $3$ 倍程度つく。
  - サイズが小さい場合や $a$ と $b$ のどちらかが疎な場合、$a, b$ の非零要素の個数を $n^{\prime}, m^{\prime}$ として $O(n + m + \min(n^{\prime}m, nm^{\prime}))$ の実装に切り替える。
  - 疎な入力では、零を飛ばす直接計算の演算数と、有効区間に対する NTT の推定計算量を比較して切り替える。
  - 両方が疎な場合は非零項同士を列挙し、$O(n+m+n^{\prime}m^{\prime})$ で求める。

