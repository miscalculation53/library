## 概要

各成分が多項式の正方行列 $A(x)$ に対し、連続する整数での評価値の積

$$A(r-1)A(r-2)\cdots A(l)$$

を計算する。右側の行列から順に作用するので、$v_{i+1}=A(i)v_i$ の遷移に使える。
多項式係数の漸化式への適用は [p_recursive](p_recursive.md) を参照。

ブロック積を評価値で保持し、[評価点シフト](shift_of_sampling_points.md) によってブロック長を倍々に伸ばす。
これは [Bostan–Gaudry–Schost の手法](https://mathexp.eu/bostan/publications/BoGaSc04.pdf) に基づく。
法 $p$ を越える長さの区間は、$A(x+p)=A(x)$ を利用して一周期の積を累乗する。

## 使用例

```cpp
#include "math/fps/polynomial_matrix_product.hpp"
using mint = modint998244353;
using P = FormalPowerSeries<mint>;

vvc<P> a = {{{1, 1}}}; // A(x) = [x+1]
auto prod = polynomial_matrix_product(a, 0, 10);
assert(prod[0][0] == 3628800); // 10!
```

## 詳細なドキュメント

#### polynomial_matrix_product

```cpp
template <class mint>
Matrix<FieldAddSubMulDiv<mint>> polynomial_matrix_product(
    const vvc<FormalPowerSeries<mint>>& a, ll l, ll r);
```

$A(r-1)\cdots A(l)$ を返す。空区間では単位行列を返す。
多項式は昇冪順の係数列で、零多項式には空配列も使える。
特異行列を含む場合も計算できる。

成分の最大次数を $d>0$、処理する区間長を $T\le p$ とする。
$dB^2\ge T$ を満たす最小の $2$ 冪 $B$ をブロック長に選ぶ。
長さ $s$ のブロック積 $Q_s(x)=A(x+s-1)\cdots A(x)$ は各成分が $ds$ 次以下なので、

$$Q_{2s}(x)=Q_s(x+s)Q_s(x)$$

を $d(2s)+1$ 点で評価することにより倍長のブロックへ更新する。
最後に $Q_B(l),Q_B(l+B),\ldots$ と端数の行列を順に掛ける。

##### 制約

- `mint` は素数を法とする modint で、[畳み込み](../convolution/convolution.md) に対応するもの
- `a` は正方行列（空行列も可）
- $0\le l\le r$

##### 計算量

行列の一辺を $m$、$N=r-l$、$p=\texttt{mint::mod()}$、$T=\min(N,p)$ とする。
体上の演算を $O(1)$ として数え、$M(s)$ は長さ $O(s)$ の畳み込みの計算量とする。

- 定数行列は $O(m^3\log(N+1))$
- $d>0$, $L=dB=O(\sqrt{dT})$ として、通常は $O\bigl(m^2d^2+m^2M(L)+m^3L+m^3\log(N+1)\bigr)$
- $T\le32$、$d\ge T$、または $dB\ge p$ の場合は直接計算を用い、$O\bigl(T(m^2(d+1)+m^3)+m^3\log(N+1)\bigr)$

通常の計算量の $m^2d^2$ は、最初の $d+1$ 点を Horner 法で評価する部分。
位数・次数が定数で $N<p$ なら $O(\sqrt{N}\log N)$。
