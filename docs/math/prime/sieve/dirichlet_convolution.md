## 概要

数論関数の Dirichlet 積

$$
(f*g)(n)=\sum_{d\mid n}f(d)g(n/d)
$$

と、その商・逆数・微積分・log・exp・累乗・代入を計算する。
一般の数列に対応し、乗法性が既知の場合は高速化できる。
累積和も扱う場合は [Dirichlet 積と累積和テーブル](dirichlet_prefix_sum.md) を使う。
微積分・log・exp・代入は、先頭の係数をすべて持つ本クラスの演算。
Dirichlet 級数の定数項は添字 $1$ の係数で、単位元は $\varepsilon=[1,0,0,\ldots]$。

## 使用例

```cpp
#include "math/prime/sieve/dirichlet_convolution.hpp"

using R = RingAddSubMul<ll>;
DirichletSeries<R> one(N, zeta_primepower);
DirichletSeries<R> id(N, id_primepower);
auto phi = id / one;
auto tau = one * one;
auto mu = one.inv();
dump(phi); // LOCAL のとき、content() の係数配列を表示する
```

1-indexed 配列から構築することもできる。

```cpp
auto mu = enumerate_multiplicative<R>(N, mobius_primepower);
auto one = enumerate_multiplicative<R>(N, zeta_primepower);
auto identity = (DirichletSeries<R>(mu, true) * DirichletSeries<R>(one, true)).to_vector();
```

微積分・log・exp と係数型の指数による累乗には、逆元を持つ構造を指定する。

```cpp
using R = FieldAddSubMulDiv<modint998244353>;
using D = DirichletSeries<R>;
using mint = R::S;
D a(N);                         // a(1) = 0
a.set_f(2, 1);
a.set_f(3, 1);
auto f = D::unit(N) + a;         // f(1) = 1
auto df = f.diff();              // df(n) = Ω(n) f(n)
auto restored = df.integ(1);     // 積分定数を添字 1 に置く
auto l = f.log();
auto e = l.exp();                // f と同じ係数
auto power = f.pow(1000000000000000000LL);
auto root = f.pow(mint(1) / 2);  // 定数項 1 の平方根
auto squared_indices = a.substitute_power(2); // 添字 n の値を n^2 に移す
auto composed = a.compose_fps(vc<mint>{0, 1, 1, 2, 5, 14});
```

最後の例の外側の FPS は $H(z)=z+z^2+2z^3+5z^4+14z^5$。
`a.compose_fps(c)` は、$a$ を内側として $\sum_j c[j]a^{*j}$ を返す。

例えば $c[k]=\mathrm{Catalan}(k-1)$、$c[0]=0$ とすると、
葉が $k$ 個の順序付き充満二分木の形を数えることになる。
葉の重みが $2,3$ の各 1 種類なら、出力の添字 $x$ は葉の重みの総積が $x$ の木の個数。
添字 $6$ は 2 通り、添字 $12$ は 6 通りになる。
すべての木を数えるには $k\le\lfloor\log_2 N\rfloor$ までの Catalan 数を渡す。
外側の係数が整数で用意できれば、この代入は `RingAddSubMul<ll>` でも使える。

## 詳細なドキュメント

以下では係数の基本演算・コピーを $O(1)$ とする。
多項式環などでは、そのコストを別途掛ける。小さい $K$ の対数は定数として読む。
各演算の追加空間は $O(K+1)$。乗法性を利用する計算法では共有の `LinearSieve` を使い、
篩はそれまでに要求された最大範囲に比例する空間を保持する。
以下 $L=\lfloor\log_2 K\rfloor$ とする（$K=0$ では $L=0$）。
微積分等で必要な $\Omega$ の表も共有され、自動で必要な範囲まで伸びる。

#### 係数環と除算の条件

環はライブラリ共通の形式を使う。

| 意味 | 環のインターフェース |
| --- | --- |
| 要素型 | `R::S` |
| 零・単位元 | `R::e0()`, `R::e1()` |
| 加算・加法逆元・乗算 | `R::add(a,b)`, `R::minus(a)`, `R::mul(a,b)` |

可換環を仮定する。要素型にはコピー・代入・等値比較が必要だが、
通常の算術演算子や `S(0)`・`S(1)`・既定構築による零を要求しない。
例えば `PolynomialRingArray<RingAddSubMul<ll>, 3>` をそのまま指定できる。

積や非負整数乗には乗法逆元が不要で、零因子を持つ環も利用できる。
商 $h*g=f$ は $g(1)$ による除算が必要になるため、次のように扱う。

- **一般の環**：$g(1)=1_R$ または $-1_R$ なら `R::inv` がなくても計算できる。
  両方が乗法的な場合や、$f(1)=1_R$ の関数の逆数はここに含まれる。
- **`R::inv(a)` を持つ構造**：$g(1)$ が可逆なら利用でき、逆元を一度計算して使い回す。
  通常の modint には `FieldAddSubMulDiv<mint>` を指定する。
  法が合成数の場合も、逆元を取る要素が法と互いに素なら使える。
- **通常の整数環 `RingAddSubMul<int/ll/...>`**：各段階で整数の `/` を使う。
  各除算が割り切れ、途中の値も型の範囲に収まることを保証する。
  整数の逆数は $f(1)=1$ または $-1$ のとき利用できる。

任意の環について、要素型の `/` や `inv()` から演算を推測することはない。
整数での割り切れる商は上記の通常の整数環に限った扱い。
`FieldAddSubMulDiv<ll>` は整数の切り捨て除算を逆元に使ってしまうため、指定しない。

スカラー除算も同じ条件。$K=0$ の数列同士の商・逆数では係数の除算を行わない。

`integ`・`log`・`exp`・係数型の指数の `pow` には `R::inv` が必要。
これらでは $1_R,2\cdot1_R,\ldots,L\cdot1_R$ がすべて可逆であることを要求する。
素数 mod $p$ なら $p>L$ で十分で、$p>K$ は要求しない。
通常の整数環で割り切れる場合に対応した商とは異なり、整数の切り捨て除算では代用できない。
`diff`・`substitute_power`・`compose_fps`・整数乗には逆元は不要。

小さい整数の逆元列は `R` ごとの共有キャッシュに保存する。
`integ`・`log`・`exp`・高速な `pow` が必要としたとき、足りない範囲だけ
[inv_many](../../modint/inv_many.md) で追加計算する。modint も同じ方法を使う。

- 既に必要な範囲がある場合、逆元計算・列の再構築を行わない。
- 新たに必要になった整数を $\Delta L$ 個追加する場合、償却 $O(\Delta L)+I_R$。
  $I_R$ は `R::inv` 1 回の時間。追加 1 回につき逆元計算は 1 回だけ。
- 初期状態で $0,1$ の位置を持ち、$L\le1$ では逆元計算を行わない。
- `pow` 内の log と exp は同じ表を使うので、最初の log が範囲を伸ばした場合も exp で再計算しない。

共有キャッシュの空間は、要求された最大 $L$ に比例する。
動的な法は `R::mod()`、なければ `R::S::mod()` を確認し、前回の呼び出しから変わっていれば
キャッシュを初期化する。それ以外の方法で演算を変更する構造は、異なる `R` の型で区別する。
通常の商・逆数・スカラー除算で必要になる除数の逆元は、この整数逆元列とは別に各演算内で求める。

### DirichletSeries

```cpp
template <class R>
struct DirichletSeries;
```

$f(1),\ldots,f(K)$ を保持し、演算を組み合わせるためのクラス。
公開する型 `S` は `typename R::S`。係数配列・範囲・乗法性フラグは非公開。

#### コンストラクタ

```cpp
explicit DirichletSeries(int K = 0);
explicit DirichletSeries(vc<S> values, bool multiplicative = false);

template <class F>
DirichletSeries(int K, const F &f_primepower, bool completely_multiplicative = false);
```

- `K` のみ：零関数を作る。デフォルト構築では $K=0$。
- `values`：1-indexed 配列から作る。$K=\lvert\mathrm{values}\rvert-1$ とし、
  空配列は `{R::e0()}` に正規化する。添字 $0$ は `R::e0()` にする。
- `K, f_primepower`：素数冪での値から乗法的関数を作る。
  コールバックは [enumerate_multiplicative](enumerate_multiplicative.md) と共通。
  `completely_multiplicative=true` の場合は完全乗法的な関数を作る。

環はクラス側で一度だけ指定し、用意済みラムダには型引数を付けない。

##### 制約

- $0\le K$。
- 乗法性・完全乗法性を指定する場合は、その性質を呼び出し側で保証する。
  $K\ge1$ なら $f(1)=1_R$ が必要。素数冪での値は零でもよい。
- コールバックは対応する列挙関数の制約を満たす。

##### 計算量

- 零関数・配列からの構築：$O(K+1)$。
- 素数冪からの構築：対応する列挙関数と同じ。

#### unit

```cpp
static DirichletSeries unit(int K);
```

Dirichlet 積の単位元 $\varepsilon$ を作る。乗法性フラグは `true`。

##### 制約

- $0\le K$。

##### 計算量

- $O(K+1)$。

#### n, is_multiplicative

```cpp
int n() const;
bool is_multiplicative() const;
```

それぞれ最大の添字 $K$、乗法性が保証されているかを返す。

##### 計算量

- $O(1)$。

#### f

```cpp
const S &f(int i) const;
```

係数 $f(i)$ の読取り専用参照を返す。
`a` が非 `const` でも乗法性フラグを変更しない。
`a.f(i).val()` や配列型の係数の `a.f(i)[j]` も読取りに使える。
添字 $0$ は零の番兵として読み取れる。

##### 制約

- $0\le i\le K$。

##### 計算量

- $O(1)$。

#### set_f

```cpp
void set_f(int i, const S &value);
```

係数 $f(i)$ を `value` に変更し、乗法性フラグを解除する。同じ値を指定した場合も解除する。
加算する場合は `a.set_f(i, R::add(a.f(i), value))` と書く。

##### 制約

- $1\le i\le K$。

##### 計算量

- $O(1)$。

#### to_vector

```cpp
vc<S> to_vector() const &;
vc<S> to_vector() &&;
```

長さ $K+1$ の 1-indexed 係数配列を返す。添字 $0$ は `R::e0()`。
通常はコピーを返し、返した配列の編集は元の数列に影響しない。
一時オブジェクトや `std::move(a).to_vector()` では配列を移動し、移動元を $K=0$ の零関数にする。
追加空間はコピー時に $O(K+1)$、移動時に $O(1)$。

##### 計算量

- コピー時：$O(K+1)$。
- 移動時：$O(1)$。

#### content

```cpp
vc<S> content() const;
```

デバッグ用に、`to_vector()` と同じ 1-indexed 係数配列のコピーを返す。
添字 $0$ の零も含む。`LOCAL` のとき `dump(a)` でこの配列を表示できる。
`dump(a.content())` と書けば配列だけを表示する。
元の係数・範囲・乗法性フラグは変更しない。

##### 計算量

- $O(K+1)$。

#### convolve, operator*

```cpp
DirichletSeries convolve(const DirichletSeries &g) const;
DirichletSeries operator*(const DirichletSeries &g) const;
DirichletSeries &operator*=(const DirichletSeries &g);
```

Dirichlet 積を求める。乗法性と非零項の数から計算法を自動選択する。
両方の入力が乗法的な場合だけ、出力も乗法的と記録する。

##### 制約

- `n()` が等しい。

##### 計算量

- 両方が乗法的：$O(K+1)$。
- 一方が乗法的：$O(K\log\log K+1)$。
- 一般：$O(K\log K+1)$。

#### convolve_sparse

```cpp
DirichletSeries convolve_sparse(const DirichletSeries &g) const;
```

非零項だけを掛ける計算法で Dirichlet 積を求める。
結果は長さ $K+1$ の通常の係数配列として保持する。
乗法性フラグの扱いは `convolve` と同じ。

##### 制約

- `n()` が等しい。

##### 計算量

- 非零係数の添字対 $(i,j)$ で $ij\le K$ を満たす個数を $P$ として $O(K+1+P)$。
- 非零項が少なくても配列の初期化・走査が必要なので、$O(P)$ にはならない。

#### operator/, inv

```cpp
DirichletSeries operator/(const DirichletSeries &g) const;
DirichletSeries &operator/=(const DirichletSeries &g);
DirichletSeries inv() const;
```

Dirichlet 商は $h*g=f$ を満たす $h$、逆数は $f*h=\varepsilon$ を満たす $h$ を求める。
商は両方の入力が乗法的な場合だけ、逆数は入力が乗法的な場合に出力も乗法的と記録する。

##### 制約

- 二項演算では `n()` が等しい。
- 除数の $f(1)$ が上記の除算の条件を満たす。$K=0$ では除算を行わない。

##### 計算量

- 商の分子・分母が乗法的：$O(K+1)$。
- 商の分母のみ乗法的：$O(K\log\log K+1)$。
- その他の商：$O(K\log K+1)$。
- 逆数：乗法的なら $O(K+1)$、一般には $O(K\log K+1)$。
- 必要な逆元計算の時間を別途加える。

#### pow（整数指数）

```cpp
DirichletSeries pow(ll exponent) const;
```

Dirichlet 積の非負整数乗を返す。指数 $0$ では単位元を返す。
負の指数には `inv().pow(...)` を使う。
通常の modint の `FieldAddSubMulDiv<mint>` で $f(1)=1_R$ かつ $1,\ldots,L$ が可逆なら、
指数の大きさに依存しない高速版を自動で使う。それ以外でも除算不要の整数乗を利用できる。
入力が乗法的なら結果も乗法的と記録する。指数 $0$ の結果も乗法的と記録する。

##### 制約

- $\mathrm{exponent}\ge0$。

##### 計算量

- 上記の高速版：一般の入力では $O(K\log K+1)$、乗法的なら $O(K+1)$。
  整数逆元列を伸ばす場合だけ $I_R$ を別途加える。
- それ以外：`convolve` の時間を $T(K)$ として $O(T(K)\log(\mathrm{exponent}+1)+K+1)$。
- 指数 $0,1$ では $O(K+1)$。

#### diff

```cpp
DirichletSeries diff() const;
```

添字 $i$ の係数を $\Omega(i)f(i)$ にする。$\Omega$ は重複を含めた素因数の個数。
出力の添字 $1$ は零。範囲は変えず、乗法性フラグを解除する。
これは積の微分公式を満たす形式的な演算で、$s$ に関する通常の解析的な微分とは異なる。

##### 計算量

- $O(K+1)$。

#### integ

```cpp
DirichletSeries integ(const S &constant = R::e0()) const;
```

添字 $i>1$ の係数を $f(i)/\Omega(i)$ にし、添字 $1$ を `constant` にする。
範囲は変えず、乗法性フラグを解除する。
`f.diff().integ(f.f(1))` は元の係数に一致する。

##### 制約

- $K>0$ なら $f(1)=0_R$。
- `R::inv` を持ち、$1,\ldots,L$ が係数環で可逆。

##### 計算量

- $O(K+1)$。整数逆元列を伸ばす場合だけ $I_R$ を別途加える。

#### log

```cpp
DirichletSeries log() const;
```

Dirichlet 積に関する形式的な対数を返す。係数ごとの対数ではない。
出力の添字 $1$ は零。乗法的な入力でも出力の乗法性フラグは解除する。

##### 制約

- $K>0$ なら $f(1)=1_R$。
- `R::inv` を持ち、$1,\ldots,L$ が係数環で可逆。

##### 計算量

- 乗法的：$O(K+1)$。
- 一般：$O(K\log K+1)$。
- 整数逆元列を伸ばす場合だけ $I_R$ を別途加える。

#### exp

```cpp
DirichletSeries exp() const;
```

Dirichlet 積に関する形式的な指数関数を返す。$K>0$ なら出力の添字 $1$ は単位元。
非零係数が素数冪だけにある場合を自動判定し、その場合は結果を乗法的と記録する。
それ以外では結果の乗法性フラグを解除する。

##### 制約

- $K>0$ なら $f(1)=0_R$。
- `R::inv` を持ち、$1,\ldots,L$ が係数環で可逆。

##### 計算量

- 素数冪以外の係数がすべて零：$O(K+1)$。
- 一般：$O(K\log K+1)$。
- 整数逆元列を伸ばす場合だけ $I_R$ を別途加える。

#### pow（係数型の指数）

```cpp
// T が S と同じ非整数型であるときだけ有効。
template <class T, enable_if_t<is_same_v<T, S> && !is_integral_ext<T>, int> = 0>
DirichletSeries pow(const T &exponent) const;
```

$\exp_*(\mathrm{exponent}\cdot\log_*f)$ を返す。
例えば `f.pow(mint(1) / k)` で定数項 $1$ の $k$ 乗根を求められる。
整数リテラルには整数乗が選ばれる。こちらを明示的に選ぶときは係数型 `S` の値を渡す。
入力が乗法的なら結果も乗法的と記録する。指数 $0$ は単位元、指数 $1$ は入力を返す。

##### 制約

- $K>0$ なら $f(1)=1_R$。
- `R::inv` を持つ。
- 指数が $0_R,1_R$ 以外なら $1,\ldots,L$ が係数環で可逆。
  $k$ 乗根をこの方法で求める場合は $k$ も可逆。

##### 計算量

- 乗法的：$O(K+1)$。
- 一般：$O(K\log K+1)$。
- 整数逆元列を伸ばす場合だけ $I_R$ を別途加える。
- 指数 $0_R,1_R$ では $O(K+1)$。

#### substitute_power

```cpp
DirichletSeries substitute_power(ll d) const;
```

母関数 $F(s)$ を $F(ds)$ にする。係数 $f(i)$ を添字 $i^d$ に移す。
出力の上限は $K$ のままで、それを超える項は捨てる。添字 $1$ の係数は保つ。
入力の乗法性フラグを引き継ぐ。

##### 制約

- $d\ge1$。

##### 計算量

- 出力配列の初期化を含めて $O(K+1)$。

#### compose_fps

```cpp
DirichletSeries compose_fps(const vc<S> &c) const;
```

通常の FPS $H(z)=\sum_j c[j]z^j$ に、入力の Dirichlet 級数 $f$ を代入する。
戻り値は $\sum_j c[j]f^{*j}$。`c` は **0-indexed** で、欠けた項は零とみなす。
空配列なら零関数を返す。$L$ 次を超える係数は参照しない。
出力の範囲は $K$ のままで、乗法性フラグを解除する。

##### 制約

- $K>0$ なら $f(1)=0_R$。
- 係数環の加算・乗算だけでよく、逆元は不要。

##### 計算量

- $O(K\log K+1)$。外側の係数列を用意する時間は含めない。

#### 加減算・スカラー演算

```cpp
DirichletSeries operator-() const;
DirichletSeries operator+(const DirichletSeries &g) const;
DirichletSeries operator-(const DirichletSeries &g) const;
DirichletSeries operator*(const S &a) const;
DirichletSeries operator/(const S &a) const;
```

係数ごとの加減算・スカラー倍・スカラー除算。
`+=`, `-=`, `*=`, `/=` と、左からのスカラー倍 `a * f` にも対応する。
これらの演算では乗法性フラグを解除する。

##### 制約

- 数列同士の二項演算では `n()` が等しい。
- スカラー除算では `a` が上記の除算の条件を満たす。

##### 計算量

- $O(K+1)$。スカラー除算では必要な逆元計算の時間を別途加える。

## アルゴリズム

両方が乗法的なら素数冪で畳み込み、それ以外の値を乗法性から復元する。
片方が乗法的なら、その関数を素数ごとの局所因子に分解し、順番に掛ける。
一般の場合は積 $ij\le K$ を列挙する。

`convolve_sparse` は非零係数だけを抽出し、その添字の組で $ij\le K$ を満たすものを列挙する。
`convolve` と `*` は、両方が乗法的なら線形時間の積を選ぶ。
それ以外では実際の非零項から組数 $P$ を求め、乗法性に応じた計算法の見積もりより
十分少なければ非零項だけを列挙する。判定は性能上のヒューリスティックで、結果の値には影響しない。

### 微積分・log・exp・累乗

$\Omega(ab)=\Omega(a)+\Omega(b)$ から、$\mathcal Df(i)=\Omega(i)f(i)$ は
$\mathcal D(f*g)=(\mathcal Df)*g+f*(\mathcal Dg)$ を満たす。
$\Omega$ の値は `LinearSieve::Omega` で必要な範囲まで自動で前計算する。

log は $\mathcal D\log_*f=(\mathcal Df)/f$ により、微分・Dirichlet 除算・積分で計算する。
exp は $b=\exp_*a$ に対して

$$
b(1)=1_R,\qquad
\Omega(n)b(n)=\sum_{\substack{d\mid n\\d>1}}\Omega(d)a(d)b(n/d)
$$

を添字の小さい順に計算する。
乗法的な入力の log は素数ごとの局所因子の log の和なので、素数冪だけに値が現れる。
逆に素数冪だけに値がある入力の exp は乗法的になり、素数冪での値から全体を復元できる。
この二つの場合は、素数ごとの短い FPS の計算を合わせても $O(K+1)$ になる。

小さい整数は環の加算で作る。逆元列の不足分に対して `inv_many<R>` を呼び、
不足分の積の逆元を一度求めて、後ろから個々の逆元を復元する。
計算済みの値は `R` ごとに保存し、次の演算でも再利用する。

### FPS の代入

$f(1)=0$ なら $f^{*j}(n)=0\ (n<2^j)$ なので、外側の FPS は $L$ 次まであれば十分。
Horner 法で $c_j\varepsilon+f*(\cdots)$ を計算するとき、
外側を $K$ まで求めるには内側を $K/2$ まで、その内側を $K/4$ まで求めればよい。
各段階の積の上限を縮めることで、合計は
$\sum_{j\ge0}O((K/2^j)\log(K/2^j))=O(K\log K+1)$ になる。

### 参考

- [ABC428 G 公式解説](https://atcoder.jp/contests/abc428/editorial/14241)：$\Omega$ を使った微分と log。
- [UBC Math 539, Problem Set 0](https://personal.math.ubc.ca/~lior/teaching/1516/539_W16/PS/539_PS0.pdf)：形式的 Dirichlet 級数の代入と log・exp の恒等式。
