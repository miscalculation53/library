## 形式的冪級数

多項式、あるいは形式的冪級数を適当な次数で打ち切ったもの、を扱うライブラリ。

### ライブラリ

設計の参考にしたもの： https://github.com/KentaroMatsushita/icpc_library/blob/main/src/FPS/poly.hpp

`vc<mint>` を継承しており、同じコンストラクタやメンバ関数が使える。

以下（`vc<mint>` にない）メンバ関数のドキュメント。`FormalPowerSeries<mint>` を `fps` と書く。

#### 便利な操作

```cpp
(1) int sz()
(2) void shrink()
(3) mint get(int i)
(4) fps pre(int len)
(5) fps rev(int d = -1)
(6) int cnt_nz()
(7) tuple<bool, int, mint> nz_front()
(8) vc<pair<int, mint>> nz()
```

- (1) 係数の個数を返す。これは $(次数 + 1)$ となる。
- (2) 多項式として見て、末尾の余計な $0$ を削除して最高次を非零にする。
- (3) $[x^i] f(x)$ を返す。`[]` や `at()` と違って $i$ が範囲外のときも使える（$0$ を返す）。
- (4) $f$ の高々 $\mathrm{len}$ 次までの部分を返す。
- (5) $f$ の長さ $d$（つまり $d-1$ 次）までの部分（$d$ を指定しない場合は全体）を逆順にしたものを返す。
- (6) 非零な係数の個数を返す。
- (7) 非零な係数がなければ、$(\mathrm{false}, -1, 0)$ を返す。非零な係数があれば、最初の非零な係数が $i$ 次の $a_i$ であるとして $(\mathrm{true}, i, a_i)$ を返す。
- (8) 非零な係数の (次数, 係数) のペアをすべて格納した vector を返す。

#### eval

```cpp
mint eval(mint x)
```

多項式として見て、$x$ を代入したときの値 $f(x)$ を返す（形式的冪級数に対しては代入が定義できないことに注意）。

##### 計算量

- $O(n)$

#### 四則演算

非自明な挙動のみを書く。疎なときに高速化できるものは自動的に高速化する。

- `fps` どうしの `+`, `-`：次数が大きいほうの次数に合わせる。
- `fps` どうしの `*`：それぞれの長さが $n, m$ のとき、結果の長さは $n + m - 1$ となる。
- `fps inv(d = -1)`：形式的冪級数としての逆元を長さ $d$（指定しない場合は同じ長さ）で打ち切ったものを返す。$[x^0] f(x) \neq 0$ が必要。
- `fps` どうしの `/`：形式的冪級数としての除算を行う。次数が大きいほうの次数に合わせる。
- `fps div_poly(g)`：多項式としての除算を行う。
- `pair<fps, fps> divmod(g)`：多項式としての除算を行い、商とあまりを返す。
- `%`：多項式としての剰余を返す。
- `circular_mod(n)`：$\bmod \ x^n - 1$ をとる。

##### 計算量

- `+`, `-` は $O(n)$
- それ以外は $O(n \log n)$
- `*`, `inv`, `/` は、疎なときは一方の非零要素の個数を $k$ として $O(nk)$

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
(1) fps log(d = -1)
(2) fps exp(d = -1)
(3) fps pow(ll k, d = -1)
(4) pair<bool, fps> sqrt(d = -1)
```

いずれも、疎なときに高速化できるものは自動的に高速化する。$\displaystyle f(x) = \sum_{i=0}^n a_i x^i$ とする。

- (1)：$\displaystyle \log f(x) = \sum_{i=1}^\infty \frac{(-1)^{i-1}}{i} (f(x) - 1)^i$ を長さ $d$（指定がなければ $f$ の長さ）で打ち切ったものを返す。$[x^0] f(x) = 1$ が必要。
- (2)：$\displaystyle \exp f(x) = \sum_{i=0}^\infty \frac{f(x)^i}{i!}$ を長さ $d$（指定がなければ $f$ の長さ）で打ち切ったものを返す。$[x^0] f(x) = 0$ が必要。
- (3)：$\displaystyle f(x)^k$ を長さ $d$（指定がなければ $f$ の長さ）で打ち切ったものを返す。$k < 0$ でもよいが、このときは $[x^0] f(x) \neq 0$ が必要。
- (4)：$\displaystyle \sqrt{f(x)}$、つまり $g(x)^2 = f(x)$ となる $g(x)$ を長さ $d$（指定がなければ $f$ の長さ）で打ち切ったものを返す。存在しないときは $(\mathrm{false}, 空)$ を返す。これを使うのはだいたい $[x^0] f(x) = 1$ であるときである（らしい）が、そうでなくても動く。

##### 制約

- (1), (2), (3)：$n < \bmod$

##### 計算量

- $O(n \log n)$
- 疎なときは $O(nc)$、ただし $c$ は非零な係数の個数

#### pow_mod

```cpp
fps pow_mod(ll k, fps g)
```

$f(x)^k \bmod g(x)$ を返す。

##### 計算量

- $O(n \log n \log k)$

---

### 中身

https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE

#### ニュートン法

https://atcoder.jp/contests/abc260/editorial/4434

$F$ が $A(F) = 0$ を満たしているとする。$\hat{F} = F \bmod x^d$ とすると

$F \equiv \hat{F} - \dfrac{A(\hat{F})}{A'(\hat{F})} \pmod {x^{2d}}$

が成り立つ。これを利用して精度を倍々にしていく。

$A(F)$ をいい感じに定めるとニュートン法の式が導出できる。$A(F)$ の定め方が悪いと何の意味もない式が得られることがあるので注意。（$(g\ の式) - f$ で定めるといいのかな？）

##### inv

$g = f^{-1}$ を求めたい。$A(g) = \dfrac{1}{g} - f$ とすると $A'(g) = -\dfrac{1}{g^2}$ であるから

$g \equiv \hat{g} - \dfrac{\frac{1}{\hat{g}}-f}{-\frac{1}{\hat{g}^2}} = 2\hat{g} - f\hat{g}^2 \pmod {x^{2d}}$

##### exp

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
