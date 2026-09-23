## 概要

係数表 $a_{i,j}$ から、2変数の有理母関数

$$F(x,y)=\sum_{i,j\ge0}a_{i,j}x^iy^j=\frac{P(x,y)}{Q(x,y)},\qquad Q(0,0)=1$$

を推測する。1変数の [Berlekamp–Massey](berlekamp_massey.md) と同じく、初期の係数から母関数の式を得る用途に使う。
実装は $QF=P$ の係数比較による [多変数 Padé 近似](https://www.sciencedirect.com/science/article/pii/037704279090166W) で、連立方程式をガウス消去法で解く。
小さい次数の式を探す用途に適する。

- `pade_2d`：分子・分母の全次数の上限をそれぞれ指定して復元する。
- `guess_rational_gf_2d`：分子・分母の全次数の共通上限を小さい順に試す。
- `rational_gf_2d_coefficients`：得られた有理母関数から係数表を生成する。

分子・分母を [FormalPowerSeries2D](fps_2d.md) に渡すと、畳み込みを使った除算や他の2変数 FPS 演算も利用できる。

全次数とは、非零の項 $x^iy^j$ における $i+j$ の最大値。
入力の係数表は長方形で、`a[i][j]` が $x^iy^j$ の係数を表す。
返り値の多項式も同じ係数順で、各行の末尾の零と末尾の空行を除く。零多項式は空配列。

得られるのは与えた全係数に整合する候補。未知の範囲の計算には、元の母関数もその式に一致するという前提が必要になる。
推測に使わなかった係数で照合するとよい。同じ次数で複数の候補がある場合は一つを返す。

元の母関数の分子・分母の全次数がともに $d$ 以下とわかっている場合、$(2d+1)\times(2d+1)$ の係数表は復元に十分。
`max_degree >= d` で推測すると、元と同じ有理関数が得られる。
これは二つの候補の差を通分した分子が全次数 $2d$ 以下になるためで、入力によってはさらに少ない係数でも復元できる。

## 使用例

```cpp
#include "math/fps/rational_gf_2d.hpp"
using mint = modint998244353;

// a[i][j] = C(i+j, i)
vvc<mint> a(6, vc<mint>(6, 1));
for (int i = 1; i < 6; ++i)
  for (int j = 1; j < 6; ++j)
    a[i][j] = a[i - 1][j] + a[i][j - 1];

auto gf = guess_rational_gf_2d(a); // 次数上限のデフォルトは 10
assert(gf);
auto [p, q] = *gf;
// p = {{1}}, q = {{1, -1}, {-1}}: F(x,y) = 1/(1-x-y)
auto extended = rational_gf_2d_coefficients(p, q, 20, 30);
assert(extended[2][3] == 10);

// 分子は定数、分母は全次数 1 以下と指定する場合
auto fixed = pade_2d(a, 0, 1);
assert(fixed);
```

## 詳細なドキュメント

以下、係数型 `T` は正確な四則演算・等値比較ができる体とし、`T(0)`、`T(1)` とデフォルト構築による零を使えるものとする。
例えば素数を法とする modint を使える。計算量は体上の演算を $O(1)$ として数える。

#### pade_2d

```cpp
template <class T>
optional<pair<vvc<T>, vvc<T>>> pade_2d(
    const vvc<T>& a, int numerator_degree, int denominator_degree);
```

$H=|a|$, $W=|a[0]|$、$N=HW$ とする。
分子と分母の項をそれぞれ

$$S_p=\{(i,j):0\le i<H,\ 0\le j<W,\ i+j\le\texttt{numerator\_degree}\},$$
$$S_q=\{(i,j):0\le i<H,\ 0\le j<W,\ i+j\le\texttt{denominator\_degree}\}$$

に限定し、`{p, q}` を返す。分母は `q[0][0] == 1` に正規化する。
1行の入力は $y$ のみ、1列の入力は $x$ のみの有理母関数として扱う。

$P$ の係数を消去し、$(i,j)\notin S_p$ における

$$\sum_{(u,v)\in S_q\setminus\{(0,0)\}}q_{u,v}a_{i-u,j-v}=-a_{i,j}$$

を解く。負の添字の $a$ は $0$ とする。分子は $S_p$ 内の $QF$ の係数から求める。
境界を含めた既知の全係数について $QF=P$ が成立する。

未知係数数は $|S_p|+|S_q|-1$。次の場合は `nullopt` を返す。

- 入力の行数または列数が $0$。
- $N<|S_p|+|S_q|-1$。
- 指定した範囲に整合する候補がない。

方程式に自由変数がある場合は、それらを $0$ にした解を使う。分子・分母の共通因子はそのまま残る。

##### 制約

- `T` は上記の体
- `a` の各行の長さが等しい
- `numerator_degree`, `denominator_degree` はともに $0$ 以上

##### 計算量

- $U=|S_q|$ として $O(NU^2)$

#### guess_rational_gf_2d

```cpp
template <class T>
optional<pair<vvc<T>, vvc<T>>> guess_rational_gf_2d(
    const vvc<T>& a, int max_degree = 10);
```

$d=0,1,\ldots,\texttt{max\_degree}$ の順に `pade_2d(a, d, d)` を試し、最初に得られた候補を返す。
実際の探索は入力内の単項式に限定し、未知係数数が $N$ を超えたところで終了する。
候補が得られなければ `nullopt` を返す。

返す候補は、探索範囲内で $\max(\deg P,\deg Q)$ が最小になる。
上限を広げても、既に得られた候補は同じになる。

##### 制約

- `T` は上記の体
- `a` の各行の長さが等しい
- $\texttt{max\_degree}\ge0$

##### 計算量

実際に試す最大次数を $D$、次数 $d$ の分母の単項式数を $U_d$ とする。

- $O\left(N\sum_{d=0}^{D}U_d^2\right)\subseteq O\bigl(N(D+1)^5\bigr)$

#### rational_gf_2d_coefficients

```cpp
template <class T>
vvc<T> rational_gf_2d_coefficients(
    const vvc<T>& p, const vvc<T>& q, int h, int w);
```

$P/Q$ の $0\le i<h$, $0\le j<w$ の係数を長方形の配列で返す。
`p` と `q` は各行の長さが異なってもよく、省略された係数は $0$ として扱う。
`q[0][0]` は任意の非零値を指定できる。

$$a_{i,j}=\frac{p_{i,j}-\sum_{(u,v)\ne(0,0)}q_{u,v}a_{i-u,j-v}}{q_{0,0}}$$

を行・列の昇順に計算する。負の添字の係数は $0$ とする。
`h == 0` なら空配列、`w == 0` なら `h` 個の空行を返す。

##### 制約

- `T` は上記の体
- $h,w\ge0$
- `q[0][0]` が存在し、非零

##### 計算量

`q` の格納係数数を $L$、出力の範囲内にある定数項以外の非零係数数を $K$ とする。

- $O\bigl(L+h+hw(K+1)\bigr)$
