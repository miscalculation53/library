## 概要

2変数の形式的冪級数 $f(x,y)=\sum_{i,j\ge0}f_{i,j}x^iy^j$ を扱う。
`FormalPowerSeries2D<mint>` は `vvc<mint>` を継承し、`f[i][j]` が $x^iy^j$ の係数を表す。
行ごとの長さは任意で、省略した係数は $0$ として扱う。

打ち切り長 `(h, w)` は $0\le i<h$, $0\le j<w$ の長方形を表す。
`mul`, `inv`, `div`, `log`, `exp`, `pow` は $\bmod (x^h,y^w)$ で計算し、結果をちょうど `h` 行 `w` 列で返す。
`h == 0` なら空配列、`w == 0` なら `h` 個の空行になる。
`h == 0` の場合、列数は配列に保存されず、`shape()` は `(0, 0)` を返す。

乗算は [Kronecker 置換](https://flintlib.org/doc/gr_poly.html) で1変数の畳み込みに帰着する。
逆数と `exp` は全次数に対する精度を倍増させる Newton 法を用いる。
乗算・逆数・除算・`log`・`exp` は、出力係数数を $N=hw$ として $O(N\log(N+1))$ 時間。

[2変数有理母関数の推定](rational_gf_2d.md) が返す分子・分母を、そのままコンストラクタへ渡せる。

## 使用例

```cpp
#include "math/fps/fps_2d.hpp"
using mint = modint998244353;
using F = FormalPowerSeries2D<mint>;

F q{{1, -1}, {-1}};                 // 1-x-y
auto a = q.inv(5, 6);               // [x^i y^j] = C(i+j, i)
assert(a[2][3] == 10);
auto b = F{{1}}.div(q, 5, 6);       // P/Q を展開する
assert(a == b);

F f{{0, 1}, {1}};                   // x+y
auto e = f.exp(5, 6);               // [x^i y^j] = 1/(i!j!)
assert(e.log(5, 6) == f.resized(5, 6));
auto square = f.mul(f, 3, 3);        // (x+y)^2 を 3 行 3 列で返す
auto cube = f.pow(3, 4, 4);
auto dx = cube.diff_x();
```

推測した母関数との組み合わせ：

```cpp
#include "math/fps/rational_gf_2d.hpp"

auto gf = guess_rational_gf_2d<mint>(a);
assert(gf);
auto [p, denominator] = *gf;
auto more = F(p).div(denominator, 50, 60);
```

## 詳細なドキュメント

以下、`F = FormalPowerSeries2D<mint>`、法を $p$ とする。
`mint` は素数を法とする modint で、[畳み込み](../convolution/convolution.md) に対応するものを使う。
長さ $O(N)$ の畳み込みの計算量を $M(N)=O(N\log(N+1))$ と書く。
打ち切り長を指定する演算の計算量は $h,w\ge1$ の場合を示す。
空の結果の生成は $O(h+1)$。

### FormalPowerSeries2D

#### コンストラクタ

```cpp
F();
F(int h, int w);
F(const vvc<mint>& a);
F(vvc<mint>&& a);
F f{{1, 2}, {}, {3, 4, 5}};
```

引数なしなら空配列、`(h, w)` なら `h` 行 `w` 列の零を作る。
`vvc<mint>` のコンストラクタ・代入・配列操作も使える。

##### 制約

- $h,w\ge0$

##### 計算量

- `(h, w)` は $O(h+hw)$
- コピーは $O(H+L)$。$H$ は入力行数、$L$ は格納係数数
- ムーブは $O(1)$

#### 係数と形状

```cpp
pair<int, int> shape() const;
mint get(int i, int j) const;
F resized(int h, int w) const;
void shrink();
F transposed() const;
mint eval(mint x, mint y) const;
```

- `shape()`：行数と、全行の長さの最大値を返す。末尾の零も数える。
- `get(i, j)`：保存範囲外・負の添字を $0$ として係数を返す。
- `resized(h, w)`：切り詰めと零埋めで長方形にそろえたコピーを返す。
- `shrink()`：各行末の零と、末尾の空行を除く。零多項式は空配列になる。
- `transposed()`：$x,y$ を交換し、`shape()` が `(H, W)` なら `W` 行 `H` 列で返す。
- `eval(x, y)`：保存した係数を多項式として評価する。

##### 制約

- `resized` は $h,w\ge0$

##### 計算量

行数を $H$、最大行長を $W$、格納係数数を $L$ とする。

- `shape` は $O(H)$、`get` は $O(1)$
- `resized` は $O(h+hw)$
- `shrink`, `eval` は $O(H+L)$
- `transposed` は $O(H+W+HW)$

#### 四則演算

```cpp
F operator-() const;
F operator+(const F& g) const;
F operator-(const F& g) const;
F operator*(const F& g) const;
F operator*(mint c) const;
F operator/(mint c) const;
F mul(const F& g, int h, int w) const;
F div(const F& g, int h, int w) const;
```

対応する `+=`, `-=`, `*=` と、スカラーによる `/=` も使える。
`c * f` も使える。

加減算では行数と各行の長さを二つの入力の大きい方にそろえる。
`f * g` は多項式としての積全体を返す。
入力の形状が `(H, W)`, `(A, B)` なら、結果は `(H+A-1, W+B-1)` の長方形。
いずれかの行数・最大行長が $0$ なら空配列を返す。

`mul(g, h, w)` は積の必要な長方形だけを返す。
`div(g, h, w)` は形式的冪級数としての $f/g$ を返す。
スカラー演算は保存形状を保つ。

##### 制約

- `mul`, `div` は $h,w\ge0$
- `div` は `g.get(0, 0) != 0`
- スカラー除算は $c\ne0$
- 使用する畳み込みの長さが対応範囲に収まる

##### 計算量

- 加減算・スカラー演算は入力の行数と格納係数数の和に比例
- 積全体は $O(M((H+A-1)(W+B-1)))$
- `mul`, `div` は $O(M(hw))$

#### inv

```cpp
F inv(int h, int w) const;
```

$1/f$ を求める。$g\leftarrow g(2-fg)$ で、全次数が $m$ 未満の精度を $2m$ 未満へ伸ばす。
長方形全体を得るため、精度が $h+w-1$ に達するまで更新する。

##### 制約

- $h,w\ge0$
- `get(0, 0) != 0`。空の出力でも必要

##### 計算量

- $O(M(hw))$

#### 偏微分・積分

```cpp
F diff_x() const;
F diff_y() const;
F integ_x() const;
F integ_y() const;
```

対応する変数で微分・積分する。積分定数に当たる $x^0$ 行または $y^0$ 列はすべて $0$。
入力の形状が `(H, W)` なら、出力は次の長方形となる。

| 演算 | 行数 | 列数 |
| --- | --- | --- |
| `diff_x` | $\max(0,H-1)$ | $W$ |
| `diff_y` | $H$ | $\max(0,W-1)$ |
| `integ_x` | $H+1$ | $W$ |
| `integ_y` | $H$ | $W+1$ |

##### 制約

- `integ_x` は $H<p$
- `integ_y` は $W<p$

##### 計算量

- $O(H+W+HW)$

#### log, exp

```cpp
F log(int h, int w) const;
F exp(int h, int w) const;
```

それぞれ $\log f$, $\exp f$ を求める。
`log` は Euler 作用素 $E=x\partial_x+y\partial_y$ による $E(\log f)=E(f)/f$ を用い、各係数を全次数 $i+j$ で割る。
`exp` は $g\leftarrow g(1+f-\log g)$ で全次数に対する精度を倍増させる。

##### 制約

- $h,w\ge0$
- `log` は `get(0, 0) == 1`、`exp` は `get(0, 0) == 0`。空の出力でも必要
- $h,w\ge1$ の場合、$h+w-2<p$

##### 計算量

- $O(M(hw))$

#### pow

```cpp
F pow(ll k, int h, int w) const;
```

$f^k$ を二分累乗で求める。$k<0$ の場合は逆数を累乗する。
$k=0$ なら $1$ を返し、零多項式の $0$ 乗も $1$ とする。
指数には `long long` の全範囲を指定できる。

##### 制約

- $h,w\ge0$
- $k<0$ の場合は `get(0, 0) != 0`。空の出力でも必要

##### 計算量

- $O(M(hw)\log(|k|+2))$
