## 概要

多項式係数の線形漸化式

$$\sum_{j=0}^{r} c_j(n)a_{n+j}=0\qquad(r\ge1)$$

の推定と項計算を行う。`c[j][k]` は $c_j(x)$ の $x^k$ の係数で、$r$ を漸化式の位数と呼ぶ。

`guess_p_recursive` は既知の初期項から、指定した位数・係数次数の範囲で漸化式の候補を探す。
得られるのは与えた全項に整合する候補であり、後続項への適用には元の数列でも漸化式が成立することが必要。
推定に使わなかった項で照合するとよい。

`p_recursive_extend` は順に項を生成し、`p_recursive_nth` は [多項式行列の連続積](polynomial_matrix_product.md) で指定項を高速に求める。
位数・係数次数を定数とすると、後者の計算量は通常 $O(\sqrt{N}\log N)$（$N$ は進める項数）。

項計算では、既知の接頭辞の末尾から漸化式を適用する。
必要な各遷移で $c_r(n)\ne0$ を前提とし、違反時は `assert` で停止する。
$c_r(n)=0$ となる特異点では別途求めた項を初期項に追加し、その後から計算を再開できる。
渡した初期項は既知の値として扱い、漸化式との整合性の検査は省略する。

## 使用例

```cpp
#include "math/fps/p_recursive.hpp"
using mint = modint998244353;
using P = FormalPowerSeries<mint>;

vc<mint> factorial(20, 1);
for (int i = 1; i < 20; ++i) factorial[i] = factorial[i - 1] * i;
auto guessed = guess_p_recursive(factorial); // 位数・係数次数の上限はともに 10
// guessed = {{-1, -1}, {1}}: -(n+1)a[n] + a[n+1] = 0
assert(!guessed.empty());
mint fact = p_recursive_nth(factorial, guessed, 1'000'000);

// Catalan 数: (n+2)C[n+1] - (4n+2)C[n] = 0
vc<P> c = {{-2, -4}, {2, 1}};
vc<mint> a = {1};
auto terms = p_recursive_extend(a, c, 6);
assert(terms == (vc<mint>{1, 1, 2, 5, 14, 42}));
mint value = p_recursive_nth(a, c, 1'000'000);
```

## 詳細なドキュメント

以下、$r=|c|-1$, $d=\max_j\deg c_j$ とし、定数・零多項式だけの場合は $d=0$ とする。
計算量は体上の演算を $O(1)$ として数える。

#### guess_p_recursive

```cpp
template <class mint>
vc<FormalPowerSeries<mint>> guess_p_recursive(
    const vc<mint>& a, int max_order = 10, int max_degree = 10);
```

$N=|a|$, $R=\texttt{max\_order}$, $D=\texttt{max\_degree}$、法を $p$ とする。
$r=1,\ldots,R$ の順に、次数 $d\le D$ かつ $d<p$ の漸化式を探す。
各候補は $N-r\ge(r+1)(d+1)$ を満たす範囲で探索し、$0\le n<N-r$ のすべての式を使う。
同じ位数では係数多項式の最大次数が最小の候補を優先する。候補がなければ空配列を返す。

各位数で、初期項数に見合う次数

$$d_r=\min\left(D,p-1,\left\lfloor\frac{N-r}{r+1}\right\rfloor-1\right)$$

まで探索する。係数を次数順に並べた同次連立方程式を解くことで、$0$ 次から $d_r$ 次までを一度の消去で調べる。
上限を広げると探索対象も広がり、以前の上限で候補が得られた入力では引き続き候補が得られる。
探索対象が増えるため、返す候補は変わる場合がある。

デフォルトの上限はともに $10$。小さい位数・次数の漸化式を試すための設定で、必要に応じて引き上げる。
例えば Catalan 数の先頭 $10$ 項なら、`guess_p_recursive(a)` と `guess_p_recursive(a, 10, 10)` のどちらでも位数 $1$・次数 $1$ の漸化式が得られる。

返り値は長さ $r+1$ で、$c_r$ は非零、その最高次係数は $1$。
各多項式の末尾の零は除き、零多項式は空配列で表す。
同じ位数・次数に複数の候補がある場合はそのうち一つを返す。
全係数に共通する因子も残し、その根における元の漸化式の制約を保存する。

定数係数に限定する場合は `max_degree = 0`。
この場合の専用アルゴリズムには [Berlekamp–Massey](berlekamp_massey.md) がある。

##### 制約

- `mint` は素数を法とする modint
- $R\ge1$, $D\ge0$

##### 計算量

実際に探索する位数の最大値を $S\le R$、位数 $r$ の未知係数数を $C_r=(r+1)(d_r+1)$ とする。

- 位数 $r$ 一つの探索は $O((N-r)C_r^2)$
- 全体で $O\left(\sum_{r=1}^{S}(N-r)C_r^2\right)\subseteq O\bigl(NS^3(D+1)^2\bigr)$

$N=O(C_r)$ 程度の初期項を使う場合、一つの位数の探索は $O(C_r^3)$ となる。

#### p_recursive_extend

```cpp
template <class mint>
vc<mint> p_recursive_extend(
    vc<mint> a, const vc<FormalPowerSeries<mint>>& c, int size);
```

先頭 `size` 項を返す。`size <= a.size()` の場合はその長さに切り詰める。
各項には

$$a_{n+r}=-\frac{\sum_{j=0}^{r-1}c_j(n)a_{n+j}}{c_r(n)}$$

を用いる。

##### 制約

- `mint` は素数を法とする modint
- $|c|\ge2$, $\texttt{size}\ge0$
- 項を追加する場合は $|a|\ge r$
- 追加する各項について、$|a|-r\le n<\texttt{size}-r$ で $c_r(n)\ne0$

##### 計算量

- $N=\max(0,\texttt{size}-|a|)$ として $O\bigl(|a|+Nr(d+1)\bigr)$

#### p_recursive_nth

```cpp
template <class mint>
mint p_recursive_nth(
    const vc<mint>& a, const vc<FormalPowerSeries<mint>>& c, ll k);
```

$0$-indexed の第 $k$ 項を返す。`k < a.size()` の場合は `a[k]`。
それ以外では、$l=|a|-r$ から $k-r$ までの遷移を行う。

状態 $(a_n,\ldots,a_{n+r-1})^\mathsf{T}$ の遷移行列について、分母 $c_r(n)$ と多項式行列の分子をそれぞれまとめて計算する。
法を $p$ とすると係数の評価値は周期 $p$ を持つため、$k\ge p$ も周期の積の累乗で処理する。

##### 制約

- `mint` は素数を法とする modint で、[畳み込み](../convolution/convolution.md) に対応するもの
- $|c|\ge2$, $k\ge0$
- 未知の項を求める場合は $|a|\ge r$
- 必要な各遷移 $|a|-r\le n\le k-r$ で $c_r(n)\ne0$
- 計算する範囲で、対象の数列が与えた漸化式を満たす

##### 計算量

- 既知の項は $O(1)$
- $d=0$ は $N=k-|a|+1$ として $O(r^3\log(N+1))$
- $d>0$ は $r$ 次正方行列の [polynomial_matrix_product](polynomial_matrix_product.md) と同じ。通常、$T=\min(N,p)$, $L=\sqrt{dT}$ として $O\bigl(r^2d^2+r^2M(L)+r^3L+r^3\log(N+1)\bigr)$。$M(L)$ は長さ $O(L)$ の畳み込みの計算量

短い区間や補間に十分な点が取れない場合の計算量は、リンク先の直接計算の項を参照。
