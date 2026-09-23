## 概要

多項式、あるいは形式的冪級数を適当な次数で打ち切ったもの、を扱うライブラリ。

設計の参考にしたもの： https://github.com/KentaroMatsushita/icpc_library/blob/main/src/FPS/poly.hpp

`vc<mint>` を継承しており、同じコンストラクタやメンバ関数が使える。

2変数の係数表には [FormalPowerSeries2D](fps_2d.md) を使う。

以下（`vc<mint>` にない）メンバ関数のドキュメント。`FormalPowerSeries<mint>` を `fps` と書く。

打ち切り長 `n` は係数の個数を表し、$0$ 次から $n-1$ 次まで（$\bmod x^n$）を求める。`inv`, `div`, `log`, `exp`, `pow`, `sqrt` とその疎な場合の専用関数では `n` を必ず指定する。成功時の結果は末尾の $0$ も含めてちょうど `n` 要素になり、`n = 0` なら空配列になる。各演算の数学的な前提は `n = 0` でも必要。

入力の保存範囲外の係数は $0$ として扱う。`resized(n)` は切り詰め・ゼロ埋めによって長さを揃えたコピーを返す。

## 使用例

```cpp
using fps = FormalPowerSeries<modint998244353>;
fps f{1, 1};
auto a = f.inv(5);                 // 1 - x + x^2 - x^3 + x^4
auto b = f.pow(2, 4);              // {1, 2, 1, 0}
auto c = fps{1}.div(fps{1, -1}, 5); // {1, 1, 1, 1, 1}
auto d = (f * f).resized(2);        // {1, 2}
auto e = f.resized(4);              // {1, 1, 0, 0}
auto g = f.resized(4).rev();        // {0, 0, 1, 1}
```

### 中身

https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE

#### ニュートン法

https://atcoder.jp/contests/abc260/editorial/4434

$F$ が $A(F) = 0$ を満たしているとする。$\hat{F} = F \bmod x^d$ とすると

$F \equiv \hat{F} - \dfrac{A(\hat{F})}{A'(\hat{F})} \pmod {x^{2d}}$

が成り立つ。これを利用して精度を倍々にしていく。

$A(F)$ をいい感じに定めるとニュートン法の式が導出できる。$A(F)$ の定め方が悪いと何の意味もない式が得られることがあるので注意。（$(g\ の式) - f$ で定めるといいのかな？）

**inv**

$g = f^{-1}$ を求めたい。$A(g) = \dfrac{1}{g} - f$ とすると $A'(g) = -\dfrac{1}{g^2}$ であるから

$g \equiv \hat{g} - \dfrac{\frac{1}{\hat{g}}-f}{-\frac{1}{\hat{g}^2}} = 2\hat{g} - f\hat{g}^2 \pmod {x^{2d}}$

**exp**

$g = \exp f$ を求めたい。$A(g) = \log g - f$ とすると $A'(g) = \dfrac{1}{g}$ であるから

$g \equiv \hat{g} - \dfrac{\log \hat{g} - f}{\frac{1}{\hat{g}}} = \hat{g}(1 - \log \hat{g} + f) \pmod {x^{2d}}$

なお exp の実装上は普通にニュートン法をやると定数倍が悪い。https://arxiv.org/pdf/1301.5804.pdf を読んで実装した（中身はあまり理解してない）。

#### log

$\log f = \displaystyle \int \frac{f'}{f}$

#### pow

$[x^0] f = 1$ のとき、$f^k = \exp \log f^k = \exp(k \log f)$

$[x^0] f \coloneqq a_0 \neq 0$ のとき、$f^k = \left(\dfrac{f}{a_0}\right)^k \cdot a_0^k$

$f = x^\ell g, [x^0] g \neq 0$ のとき、$f^k = x^{k\ell} g^k$

sqrt も $1/2$ 乗だと思って同様に

#### 疎なとき

乗算は定義通り、除算は乗算の逆演算と考える、log は $f'/f$ の積分。

exp, pow は疎な微分方程式を使う。$[x^0]a = 1$ を満たす $a(x), b(x)$ が与えられたとき

$a(x) F'(x) + b(x) F(x) = 0, [x^0] F(x) = 1$

を満たす $F$ が一意に存在し、$F$ の長さ $n$ までを $O(nk)$ 時間で計算できる（$k$ は $a, b$ の非零係数の個数）。

- $F = \exp f$ のとき $F' = f' \exp f = f' F$ なので $a = 1, b = -f'$ とすればよい。
- $F = f^k$ のとき $F' = k f' f^{k-1}$ より $fF' = kf'F$ なので $a = f, b = -kf'$ とすればよい（$[x^0] f = 1$ となるように適当に調整は必要）。

ほかにもたとえば $F = f^n g^m$ や $F = \exp(f/g)$ などが疎な微分方程式を使って計算できる（ https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE#toc45 ）。必要に応じて対数微分法 $(\log F)' = \dfrac{F'}{F}$ を使うと導出しやすい。疎な微分方程式は `diff_eq` として実装している。（素直にやると $O(nk_1k_2)$ になるやつが $O(n(k_1+k_2))$ になるというのもあるらしいが一旦放置で……）

## 詳細なドキュメント

#### 便利な操作

```cpp
(1) int sz()
(2) void shrink()
(3) mint get(int i)
(4) fps resized(int n)
(5) fps rev(int d = -1)
(6) int cnt_nz()
(7) tuple<bool, int, mint> nz_front()
(8) vc<pair<int, mint>> nz()
```

- (1) 保存している係数の個数を返す。末尾の $0$ も数える。
- (2) 多項式として見て、末尾の余計な $0$ を削除して最高次を非零にする。
- (3) $[x^i] f(x)$ を返す。`[]` や `at()` と違って $i$ が範囲外のときも使える（$0$ を返す）。
- (4) $[x^0]f,\ldots,[x^{n-1}]f$ を格納した長さ $n$ のコピーを返す。短い入力は末尾を $0$ で埋める。元の配列はそのまま保つ。
- (5) 全体を逆順にしたコピーを返す。$d \ge 0$ を指定すると `resized(d).rev()` と同じ結果を返す。
- (6) 非零な係数の個数を返す。
- (7) 非零な係数がなければ、$(\mathrm{false}, -1, 0)$ を返す。非零な係数があれば、最初の非零な係数が $i$ 次の $a_i$ であるとして $(\mathrm{true}, i, a_i)$ を返す。
- (8) 非零な係数の (次数, 係数) のペアをすべて格納した vector を返す。

##### 制約

- `resized(n)`：$n \ge 0$

##### 計算量

- `resized(n)`：$O(n)$

#### eval

```cpp
mint eval(mint x)
```

多項式として見て、$x$ を代入したときの値 $f(x)$ を返す（形式的冪級数に対しては代入が定義できないことに注意）。

##### 計算量

- $O(n)$

#### 四則演算

非自明な挙動のみを書く。疎なときに高速化できるものは自動的に高速化する。

- `fps` どうしの `+`, `-`：長さが大きいほうに合わせる。
- `fps` どうしの `*`：それぞれの長さが $n, m$ のとき、結果の長さは $n + m - 1$ となる。一方が空配列なら空配列を返す。
- `fps` と定数の `*`, `/`：各係数に定数を掛ける・定数で割る。配列の長さは保つ。
- `fps` どうしの形式的冪級数としての除算には `div(g, n)` を使う。
- `fps div_poly(g)`：多項式としての除算を行う。
- `pair<fps, fps> divmod(g)`：多項式としての除算を行い、商とあまりを返す。
- `%`：多項式としての剰余を返す。
- `circular_mod(n)`：$\bmod \ x^n - 1$ をとる。

##### 計算量

- `+`, `-` は $O(n)$
- それ以外は $O(n \log n)$
- `fps` どうしの `*` は、疎なときは一方の非零要素の個数を $k$ として $O(nk)$
- 定数との `*`, `/` は $O(n)$

#### inv, div

```cpp
(1) fps inv(int n)
(2) fps div(const fps &g, int n)
(3) fps div_sparse(const fps &g, int n)
(4) fps div_sparse_destructive(const fps &g, int n)
```

- (1)：$f^{-1} \bmod x^n$ を返す。
- (2), (3)：$f/g \bmod x^n$ を返す。(2) は疎な場合の高速化を自動で選び、(3) は疎な場合の算法を使う。
- (4)：(3) と同じ計算で自身を更新し、更新後の値を返す。

いずれも結果の長さはちょうど $n$。更新する場合は `f = f.div(g, n)` とも書ける。

##### 制約

- $n \ge 0$
- (1)：$[x^0]f \neq 0$
- (2), (3), (4)：$[x^0]g \neq 0$

##### 計算量

入力と出力の長さの最大値を $N$、分母の非零係数の個数を $c$ とする。

- (1), (2)：$O(N \log N)$。疎な場合は $O(N(c+1))$
- (3), (4)：$O(N(c+1))$

#### シフト

- `f << k`：$f(x) \cdot x^k$、長さ $d + k$
- `f >> k`：$f(x) / x^k$ の $0$ 次以上の部分、長さ $\max(0, d - k)$

##### 計算量

- $O(n)$

#### egf, ogf

```cpp
(1) fps egf()
(2) fps ogf()
```

(1)：$\displaystyle f(x) = \sum_{i=0}^n a_i x^i$ に対し、各係数 $a_i$ を $i!$ で割った $\displaystyle g(x) = \sum_{i=0}^n \frac{a_i}{i!} x^i$ を返す。

(2)：$\displaystyle f(x) = \sum_{i=0}^n a_i x^i$ に対し、各係数 $a_i$ に $i!$ を掛けた $\displaystyle g(x) = \sum_{i=0}^n i! a_i x^i$ を返す。

##### 制約

- (1)：$n < \bmod$

##### 計算量

- $O(n)$

#### 微積分

```cpp
(1) fps diff()
(2) fps integ()
```

- (1)：$\displaystyle f(x) = \sum_{i=0}^n a_i x^i$ の微分 $\displaystyle f'(x) = \sum_{i=1}^n i a_i x^{i-1}$ を返す。
- (2)：$\displaystyle f(x) = \sum_{i=0}^n a_i x^i$ の積分 $\displaystyle \int f(x) \mathrm{d}x = \sum_{i=0}^n \frac{a_i}{i+1} x^{i+1}$ を返す（定数項は $0$ とする）。

##### 制約

- (2)：$n < \bmod$

##### 計算量

- $O(n)$

#### log, exp, pow, sqrt

```cpp
(1) fps log(int n)
(2) fps exp(int n)
(3) fps pow(ll k, int n)
(4) pair<bool, fps> sqrt(int n)
```

いずれも、疎なときに高速化できるものは自動的に高速化する。成功時の結果の長さはちょうど $n$。

- (1)：$\displaystyle \log f(x) = \sum_{i=1}^\infty \frac{(-1)^{i-1}}{i} (f(x) - 1)^i$ の先頭 $n$ 項を返す。
- (2)：$\displaystyle \exp f(x) = \sum_{i=0}^\infty \frac{f(x)^i}{i!}$ の先頭 $n$ 項を返す。
- (3)：$f(x)^k$ の先頭 $n$ 項を返す。$f^0 = 1$ とする。
- (4)：形式的冪級数としての平方根 $g(x)^2 = f(x)$ が存在するとき、$(\mathrm{true}, g\bmod x^n)$ を返す。存在しないときは $(\mathrm{false}, 空)$ を返す。存在判定には入力全体を使う。

平方根の先頭非零項が $x^s$ の項であるとき、先頭 $n$ 項の計算には入力の $n+s-1$ 次までの係数を使う場合がある。例えば $f=x^4$ に対する `sqrt(3)` は $\pm x^2$ を返す。入力の保存範囲外は $0$ として計算する。

##### 制約

- $0 \le n \le \bmod$、法は素数
- (1)：$[x^0]f = 1$
- (2)：$[x^0]f = 0$
- (3)：$k < 0$ のとき $[x^0]f \neq 0$
- (4)：法は奇素数

##### 計算量

入力と出力の長さの最大値を $N$、入力の非零係数の個数を $c$ とする。

- $O(N \log N)$。疎な場合は $O(N(c+1))$
- `pow` はさらに $O(\log(|k|+1))$、`sqrt` はさらに $O(\log^2 \bmod)$

#### exp_sparse, pow_sparse, sqrt_sparse

```cpp
(1) fps exp_sparse(int n)
(2) fps pow_sparse(ll k, int n)
(3) pair<bool, fps> sqrt_sparse(int n)
```

疎な場合の算法を使う。計算結果、長さ、制約はそれぞれ `exp(n)`, `pow(k, n)`, `sqrt(n)` と同じ。

##### 計算量

入力と出力の長さの最大値を $N$、入力の非零係数の個数を $c$ とする。

- $O(N(c+1))$
- `pow_sparse` はさらに $O(\log(|k|+1))$、`sqrt_sparse` はさらに $O(\log^2 \bmod)$

#### mul_bin, div_bin

```cpp
(1) fps mul_bin(int k, mint c)
(2) fps mul_bin_destructive(int k, mint c)
(3) fps div_bin(int k, mint c, int n)
(4) fps div_bin_destructive(int k, mint c, int n)
```

- (1), (2)：$f(x)(1+cx^k)$ を求める。結果の長さは入力の長さ $+k$。
- (3), (4)：$f(x)/(1+cx^k)\bmod x^n$ を求める。結果の長さはちょうど $n$。
- (2), (4) は自身を更新し、更新後の値を返す。(1), (3) は元の配列を保つ。

##### 制約

- $k \ge 0$
- (3), (4)：$n \ge 0$。$k=0$ のとき $1+c \neq 0$

##### 計算量

入力の長さを $s$ とする。

- (1), (2)：$O(s+k)$
- (3), (4)：$O(s+n)$

#### pow_mod

```cpp
fps pow_mod(ll k, fps g)
```

$f(x)^k \bmod g(x)$ を返す。

##### 計算量

- $O(n \log n \log k)$
