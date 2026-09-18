## 概要

数論関数 $f$ の累積和 $F(x)=\sum_{i=1}^x f(i)$ を、根を取った商集合

$$
Q_{N,D}=\{\lfloor(N/i)^{1/D}\rfloor:1\le i\le N\}
$$

上で保持し、加減算・Dirichlet 積・商・逆数・非負整数乗を計算する。
$F(0)=0$ も取得できる。乗法的でない数列にも対応する。
`DirichletPrefixSum<R, D=1>` の形で次数を指定する。省略時は従来の商集合。
固定した $D$ に対し、積・商・逆数は
**$O(N^{2/(2D+1)})$ 時間・$O(N^{1/(D+1)})$ 空間**。
保持する境界の指定は不要。乗法性を仮定しない計算量で、隠れた対数因子はない。

| 次数 | 保持する位置 | 積・商・逆数の時間 | 空間 |
| --- | --- | --- | --- |
| `D=1` | $\lfloor N/i\rfloor$ | $O(N^{2/3})$ | $O(N^{1/2})$ |
| `D=2` | $\lfloor\sqrt{N/i}\rfloor$ | $O(N^{2/5})$ | $O(N^{1/3})$ |
| `D=3` | $\lfloor\sqrt[3]{N/i}\rfloor$ | $O(N^{2/7})$ | $O(N^{1/4})$ |

`n()` は根を取る前の $N$。最大の保持位置は $X=\lfloor N^{1/D}\rfloor$ であり、
`D>1` で通常 `F(N)` は取得できない。
素数冪での値から未知の累積和を構築する場合は
[素数上の和・乗法的関数の和](multiplicative_prefix_sum.md) を使う。そちらと
[一般化した Euler 積の和](euler_product_prefix_sum.md) の出力は引き続き `D=1`。

## 使用例

```cpp
#include "math/prime/sieve/dirichlet_prefix_sum.hpp"

using R = RingAddSubMul<ll>;

DirichletPrefixSum<R> one(N, zeta_prefix_sum, true);
DirichletPrefixSum<R> id(N, id_prefix_sum, true);

auto phi = id / one;                  // φ * 1 = id
ll answer = phi.F(N);                 // sum_{i=1}^N φ(i)
ll half = phi.F(N / 2);               // sum_{i=1}^{floor(N/2)} φ(i)
ll divisors = dirichlet_convolution_sum(one, one); // sum_{i=1}^N τ(i)
dump(phi); // LOCAL のとき、content() の「商の値 -> 累積和」を表示する
```

係数は `const auto f = phi.to_series();` で数列を作ると、$1\le i\le\lfloor\sqrt N\rfloor$ の範囲を `f.f(i)` で取得できる。
[Dirichlet Convolution and Prefix Sums](https://judge.yosupo.jp/problem/dirichlet_convolution_and_prefix_sums)
のように商集合上の累積和が入力される場合は、次のように読み込める。
商集合の配列を別に作る必要はない。

```cpp
DirichletPrefixSum<R> a(N);
for (int i = 0; i < a.size(); i++) {
  typename R::S v;
  READ(v);
  a.setF(a.value(i), v); // 商の値の昇順
}
```

`ll` では中間値も含めてオーバーフローしない範囲で使う。
modint なら `using R = FieldAddSubMulDiv<mint>;` とする。
`RingAddSubMul<mint>` は逆元演算を提供しないため、商・逆数で除数の $f(1)$ が
$\pm1$ 以外になる場合には使えない。係数型 `mint` の `.inv()` を自動で使うことはない。
環は `DirichletPrefixSum<R>` 側で一度だけ指定し、ラムダには型引数を付けない。

積が $N$ 以下の順序付き因数分解（各因子は $2$ 以上、空列を含む）は、
Dirichlet 級数 $1/(2-\zeta)$ の累積和として求められる。

```cpp
using R = FieldAddSubMulDiv<mint>;
auto e = DirichletPrefixSum<R>::unit(N);
DirichletPrefixSum<R> one(N, zeta_prefix_sum);
auto ordered_factorizations = (mint(2) * e - one).inv();
auto answer = ordered_factorizations.F(N);
```

平方因子を持たない整数の個数は、`D=2` で Möbius 関数の累積和を作ると求められる。
`mobius.F(x)` が保持するのは $\mu$ の累積和であり、平方因子を持たない整数の個数そのものではない。

```cpp
using R = RingAddSubMul<ll>;
auto mobius = DirichletPrefixSum<R, 2>(N, zeta_prefix_sum, true).inv();
ll answer = 0;
for (ll l = 1, root = iroot(N, 2); l <= root; ) {
  ll q = N / l / l;
  ll r = iroot(N / q, 2);
  answer += q * (mobius.F(r) - mobius.F(l - 1));
  l = r + 1;
}
```

全体で $O(N^{2/5})$ 時間・$O(N^{1/3})$ 空間。
検証コードは [Counting Squarefrees](../../../../verify/yosupo/counting_squarefrees.test.cpp)。

## 詳細なドキュメント

係数環・除算の条件は [Dirichlet 積・商・逆数](dirichlet_convolution.md) と共通。
以下では $D\ge1$ を固定定数とし、64 bit 整数の演算・整数根、環の基本演算・要素のコピーを $O(1)$ とする。
また、$X=\lfloor N^{1/D}\rfloor$、$B=\lfloor N^{1/(D+1)}\rfloor$、
$M=\lvert Q_{N,D}\rvert$ とする。$M=O(N^{1/(D+1)})$。
時間計算量は $N\ge1$ に対する評価で、$N=0$ は $O(1)$。
入力の準備・環の逆元計算の時間は別途加える。

#### e_prefix_sum, zeta_prefix_sum, id_prefix_sum

```cpp
auto unit_sum = e_prefix_sum(x, R{});
auto count = zeta_prefix_sum(x, R{});
auto triangular = id_prefix_sum(x, R{});
```

よく使う累積和のラムダ式。
整数値 $a$ を環の値 $a\cdot1_R$ に写して返す。

| 名前 | 返す値 |
| --- | --- |
| `e_prefix_sum` | $x\ge1$ なら $1_R$、$x=0$ なら $0_R$ |
| `zeta_prefix_sum` | $x$ |
| `id_prefix_sum` | $x(x+1)/2$ |

`DirichletPrefixSum<R>(N, id_prefix_sum)` のように、型引数を付けずに渡せる。
`id_prefix_sum` も環での除算は不要で、法が偶数の場合も利用できる。

##### 制約

- $0\le x$。
- 結果・中間値を要素型で表現できる。

##### 計算量

- `e_prefix_sum`：$O(1)$。
- `zeta_prefix_sum`, `id_prefix_sum`：通常の数値型の `RingAddSubMul`・
  `FieldAddSubMulDiv`、および `RingXorAnd` では $O(1)$。一般の環では $O(\log(x+1)+1)$。
- この時間は、構築時のコールバック評価時間に含める。

#### dirichlet_convolution_sum

```cpp
template <class R, int D = 1>
typename R::S dirichlet_convolution_sum(
  const DirichletPrefixSum<R, D> &a, const DirichletPrefixSum<R, D> &b);
```

$(a*b)$ の累積和を $X=\lfloor N^{1/D}\rfloor$ においてだけ求める。
全ての保持位置の結果が不要なら、`(a*b).F(X)` よりこちらを使う。`D` は引数から推論する。
追加空間は $O(1)$。

##### 制約

- 入力の環・次数 `D`・`n()` が一致する。

##### 計算量

- $O(N^{1/(2D)})$。

### DirichletPrefixSum

```cpp
template <class R, int D = 1>
struct DirichletPrefixSum;
```

公開する型 `S` は `typename R::S`。
範囲・累積和配列・乗法性フラグはすべて非公開。
テーブルの空間と各演算の追加空間は $O(N^{1/(D+1)})$ 以下。
アクセス・添字変換・更新の追加空間は $O(1)$。

`D` は正のコンパイル時定数。異なる次数のテーブル同士の積・商は提供しない。

#### コンストラクタ

```cpp
DirichletPrefixSum();
explicit DirichletPrefixSum(ll N);

template <class GetF>
DirichletPrefixSum(ll N, const GetF &getF, bool multiplicative = false);
```

- デフォルト構築：$N=0$ の空のテーブル。
- `N` のみ：$Q_{N,D}$ 上の零関数。
- `N, getF`：$Q_{N,D}$ 上の実際の累積和 `getF(x)` から構築する。
  重複を除いた正の各位置で一度だけ呼ぶ。`getF(0)` は呼ばず、$F(0)=0_R$ とする。

`getF(x, R{})` の形式にも対応する。両方呼べる場合は単引数を優先する。
係数の前計算を別途渡す必要はない。
`multiplicative=true` は、累積和ではなく元の数列 $f$ の乗法性を保証する指定。

デフォルト構築は `vector<DirichletPrefixSum<R>>(count)` や、後から計算結果を代入する用途に使える。
`(N)` の零初期化は、同じ商集合のテーブルを足し合わせる蓄積先や `setF` で埋める DP に使う。
空のテーブルを非空のテーブルへ `+=` することはできないので、蓄積先では `N` を指定する。

##### 制約

- $0\le N$、$D\ge1$。
- コールバックは const な状態で呼べ、返り値を `S` に代入できる。
- 二引数形式では `R{}` を構築できる。
- `multiplicative=true` なら $f$ が乗法的。$N\ge1$ なら $F(1)=f(1)=1_R$。

##### 計算量

- デフォルト構築：$O(1)$。
- 零初期化：$O(N^{1/(D+1)})$。
- コールバックからの構築：$O(N^{1/(D+1)})$ に $M$ 回のコールバック評価時間を加えたもの。

#### unit

```cpp
static DirichletPrefixSum unit(ll N);
```

Dirichlet 積の単位元 $\varepsilon$ の累積和を作る。乗法性フラグは `true`。

##### 制約

- `N` がコンストラクタの制約を満たす。

##### 計算量

- $O(N^{1/(D+1)})$。

#### n, size, is_multiplicative

```cpp
ll n() const;
int size() const;
bool is_multiplicative() const;
```

それぞれ根を取る前の $N$、重複を除いた正の保持位置の個数 $M$、元の数列の乗法性が保証されているかを返す。
`size()` は $0$ を含めない。$N=0$ なら `size()==0`。

##### 計算量

- $O(1)$。

#### index, value

```cpp
int index(ll x) const;
ll value(int i) const;
```

正の保持位置と、その昇順・0 始まりの添字を相互変換する。$0$ は含めない。
例えば $N=10,D=1$ では `value(0..4)` は $1,2,3,5,10$、`index(5)==3`。
$N=100,D=2$ では `value(0..6)` が $1,2,3,4,5,7,10$ となる。
`D=1` は変換用の追加配列を持たない。`D>1` は重複の圧縮用に
$O(N^{1/(D+1)})$ 空間の配列を持つが、変換のたびにメモリを確保することはない。

##### 制約

- `index(x)`：$x\in Q_{N,D}$。集合外では assert。
- `value(i)`：$0\le i<\texttt{size()}$。

##### 計算量

- $O(1)$。

#### contains

```cpp
bool contains(ll x) const;
```

$F(x)$ を保持しているか、すなわち $x\in Q_{N,D}\cup\{0\}$ かを返す。
`contains(0)` は常に `true`。

##### 計算量

- $O(1)$。

#### F

```cpp
const S &F(ll x) const;
```

累積和 $F(x)$ の読取り専用参照を返す。
引数は商の値 $x$ で、商集合内の連番ではない。
最大位置の和は `a.F(iroot(N, D))`、`D=1` なら `a.F(N)` で取得する。
連番 $i$ から取得する場合は `a.F(a.value(i))` と書く。

`a` が非 `const` でも乗法性フラグを変更しない。添字 $0$ は零の番兵として読み取れる。
係数を取得する `f` は提供しない。
係数が必要な場合は `to_series()` で小さい範囲の数列を作るか、
$i,i-1$ の両方を保持している範囲で累積和の差を取る。

##### 制約

- `contains(x)` が `true`。保持していない位置では assert。

##### 計算量

- $O(1)$。

#### setF

```cpp
void setF(ll x, const S &value);
```

累積和 $F(x)$ の 1 点を `value` に変更し、乗法性フラグを解除する。同じ値を指定した場合も解除する。
係数 1 点への加算ではない。
累積和の 1 点に加算する場合は `a.setF(x, R::add(a.F(x), value))` と書く。

##### 制約

- $x\ge 1$ かつ `contains(x)` が `true`。

##### 計算量

- $O(1)$。

#### to_series

```cpp
DirichletSeries<R> to_series() const;
```

$f(1),\ldots,f(B)$ を `DirichletSeries<R>` に変換して返す。
乗法性フラグを引き継ぐ。返す数列は独立しており、その編集は元のテーブルに影響しない。

##### 計算量

- $O(N^{1/(D+1)})$。

#### content

```cpp
map<ll, S> content() const;
```

デバッグ用に、保持位置 $x$ をキー、累積和 $F(x)$ を値とする map を返す。
キーは商集合内の連番ではなく、商の値そのもの。$0\mapsto R::e0()$ も含む。
例えば $N=10,D=1,F(x)=x(x+1)/2$ なら
`{0: 0, 1: 1, 2: 3, 3: 6, 5: 15, 10: 55}` に相当する map を返す。

`LOCAL` のとき `dump(a)` でこの map を表示できる。
`dump(a.content())` と書けば map だけを表示する。
返した map は独立しており、元の累積和・乗法性フラグは変更しない。
デフォルト構築されたテーブルでは $0\mapsto R::e0()$ のみを返す。

##### 計算量

- $O(|Q_{N,D}|+1)=O(N^{1/(D+1)}+1)$。キーの昇順に挿入する。

#### convolve, operator*

```cpp
DirichletPrefixSum convolve(const DirichletPrefixSum &g) const;
DirichletPrefixSum operator*(const DirichletPrefixSum &g) const;
DirichletPrefixSum &operator*=(const DirichletPrefixSum &g);
```

Dirichlet 積の累積和を求める。累積和の点ごとの積ではない。
計算法は自動で選択する。両方の入力が乗法的な場合だけ、出力も乗法的と記録する。
環での除算は不要。

##### 制約

- 環・次数 `D`・`n()` が等しい。

##### 計算量

- $O(N^{2/(2D+1)})$。隠れた対数因子はない。

#### convolve_sparse

```cpp
DirichletPrefixSum convolve_sparse(const DirichletPrefixSum &g) const;
```

非零項だけを列挙する計算法で Dirichlet 積の累積和を求める。
結果の保持形式・乗法性フラグの扱いは `convolve` と同じ。

##### 制約

- 環・次数 `D`・`n()` が等しい。

##### 計算量

- $O(N^{3/(2(D+1))})$ 以下。非零項の数に依存する。
- 直接列挙を強制するため、`convolve` の $O(N^{2/(2D+1)})$ の上界は保証しない。

#### operator/, inv

```cpp
DirichletPrefixSum operator/(const DirichletPrefixSum &g) const;
DirichletPrefixSum &operator/=(const DirichletPrefixSum &g);
DirichletPrefixSum inv() const;
```

Dirichlet 商・逆数の累積和を求める。累積和の点ごとの商・逆数ではない。
商は両方の入力が乗法的な場合だけ、逆数は入力が乗法的な場合に出力も乗法的と記録する。

##### 制約

- 二項演算では環・次数 `D`・`n()` が等しい。
- 除数の $f(1)$ が [係数環と除算の条件](dirichlet_convolution.md#係数環と除算の条件) を満たす。
  一般の数列で $f(1)=1_R$ である必要はない。$N=0$ では係数の除算を行わない。

##### 計算量

- $O(N^{2/(2D+1)})$。隠れた対数因子はない。
- 必要な逆元計算の時間を別途加える。

#### pow

```cpp
DirichletPrefixSum pow(ll exponent) const;
```

Dirichlet 積の非負整数乗の累積和を返す。指数 $0$ では単位元を返す。
負の指数には `inv().pow(...)` を使う。

##### 制約

- $\mathrm{exponent}\ge0$。

##### 計算量

- $O(N^{2/(2D+1)}\log(\mathrm{exponent}+1)+N^{1/(D+1)})$。

#### 加減算・スカラー演算

```cpp
DirichletPrefixSum operator-() const;
DirichletPrefixSum operator+(const DirichletPrefixSum &g) const;
DirichletPrefixSum operator-(const DirichletPrefixSum &g) const;
DirichletPrefixSum operator*(const S &a) const;
DirichletPrefixSum operator/(const S &a) const;
```

数列の加減算・スカラー倍・スカラー除算の累積和を返す。
`+=`, `-=`, `*=`, `/=` と、左からのスカラー倍 `a * f` にも対応する。
これらの演算では乗法性フラグを解除する。

##### 制約

- テーブル同士の二項演算では環・次数 `D`・`n()` が等しい。
- スカラー除算では `a` が係数環と除算の条件を満たす。

##### 計算量

- $O(N^{1/(D+1)})$。スカラー除算では必要な逆元計算の時間を別途加える。

## アルゴリズム

### 商集合の保持と添字変換

$B=\lfloor N^{1/(D+1)}\rfloor$、$L=\lfloor N/(B+1)^D\rfloor$ と置く。
$[1,B]$ の整数は全て $Q_{N,D}$ に属するので、ここを密に持つ。
残りは $x_j=\lfloor(N/j)^{1/D}\rfloor$（$1\le j\le L$）を列挙すればよい。
$L\le B$ なので、全体で $O(N^{1/(D+1)})$ 個。
係数は $f(i)=F(i)-F(i-1)$ から取得する。

`D=1` なら大きい側の値は全て異なる。配列を追加せず、次の式で相互変換する。

$$
\operatorname{index}(x)=
\begin{cases}x-1&x\le B,\\ B+L-\lfloor N/x\rfloor&x>B,\end{cases}
\qquad
\operatorname{value}(i)=
\begin{cases}i+1&i<B,\\ \lfloor N/(B+L-i)\rfloor&i\ge B.\end{cases}
$$

`D>1` は $x_j$ に重複があるため、異なる値の降順配列と、元の $j$ から圧縮後の位置への配列を持つ。
同じ値 $x$ が続く元の添字の終端は $\lfloor N/x^D\rfloor$。
これで `value` は配列参照、`index` は $\lfloor N/x^D\rfloor$ からの配列参照となる。
この変換は内部では集合外の値についても「その値以上の最初の位置」を返せるので、
積・商の区間端点にも使える。公開する `index` は集合内の値のみを受け付ける。
演算結果の構築では座標をコピーし、整数根の列挙をやり直さない。

整数根は浮動小数点の近似を整数演算で補正する。
$(B+1)^D$ が整数型を超える場合も、飽和演算で $L=0$ を求める。
`id_prefix_sum` は整数側で $2$ を約分してから環に写し、整数の `x+1` を直接計算しない。

### 双曲線分割と参照先の閉性

$m=\lfloor\sqrt x\rfloor$ とすると、双曲線分割により

$$
\sum_{uv\le x}a(u)b(v)
=\sum_{i=1}^m\left(a(i)B(\lfloor x/i\rfloor)+b(i)A(\lfloor x/i\rfloor)\right)-A(m)B(m).
$$

$x_j=\lfloor(N/j)^{1/D}\rfloor$ に対して

$$
\left\lfloor\frac{x_j}{a}\right\rfloor
=\left\lfloor\left(\frac{N}{j a^D}\right)^{1/D}\right\rfloor
$$

なので、床除算した参照先も $Q_{N,D}\cup\{0\}$ に属する。
また $m\le\lfloor N^{1/(2D)}\rfloor\le B$ だから、小さい係数も取得できる。
`dirichlet_convolution_sum` は最大位置 $X$ でこの式を一度だけ計算する。

### 積の区間集約

$T=\lfloor N^{1/(2D+1)}\rfloor$ とする。
$x_j\ge(T+1)^2$ の位置では、通常の双曲線分割を行う。
該当する元の添字は $j\le N/(T+1)^{2D}$ なので、合計時間は

$$
O\left(\sum_{j\le N/(T+1)^{2D}}(N/j)^{1/(2D)}\right)
=O\left(\frac{N}{T^{2D-1}}\right).
$$

残りの $x<(T+1)^2$ では、$uv\le x$ の少なくとも片方は $T$ 以下なので、

$$
H(x)=\sum_{a=1}^{T}\bigl(f(a)G(\lfloor x/a\rfloor)+g(a)F(\lfloor x/a\rfloor)\bigr)
-\sum_{\substack{u,v\le T\\uv\le x}}f(u)g(v).
$$

固定した $a$ に対して $\lfloor x_j/a\rfloor$ が同じになる区間をまとめ、
結果配列の差分へ加算する。値が $q$ である区間の元の添字の終端は
$\lfloor N/(aq)^D\rfloor$。`D>1` ではこれを圧縮後の添字へ写す。
固定した $a$ の区間数は $O((N/a^D)^{1/(D+1)})$ なので、全体で
$O((NT)^{1/(D+1)})$。
重複部分は $u,v\le T$ の対を列挙し、$x<uv$ になる位置で差分を更新する。
これは $O(T^2)$ 時間。

従って合計時間は

$$
O\left(\frac{N}{T^{2D-1}}+(NT)^{1/(D+1)}+T^2\right)
=O(N^{2/(2D+1)}).
$$

差分を持つのは保持位置だけで、長さ $N^{2/(2D+1)}$ の配列は作らない。
加減算・乗算だけを使い、乗法性や環での除算は不要。
直接列挙が $O(T^2)$ 回以下の場合はそちらを選ぶので、自動選択でもこの時間上界を保つ。

`D=1` の商集合だけで積を求める発想は
[adamant の記事の追記](https://codeforces.com/blog/entry/117635) を参照。
ここでは固定した $T$ による領域分割を根付きの座標へ拡張している。

### 商・逆数の区間更新

$h*g=a$ を解く。保持位置を昇順に並べ、入力の $A$ の隣接差分を作業配列に持つ。
既知の積の寄与を差し引いてから、各位置の未知の係数和を $g(1)$ で割って確定する。
必要な除算は $g(1)$ によるものだけで、他の係数が零でもよい。

$u,v\ge2$ の寄与を、対角と非対角に分ける。

- $u=v$ の寄与は、$x\ge u^2$ の全保持位置へ区間更新する。
- 非対角 $u<v$ について、$v\le B$ の係数が確定した時点で
  $uv\le x\le\lfloor(N/v)^{1/D}\rfloor$ の範囲へ区間更新する。
- 各保持位置 $x$ では $z=\lfloor N/x^D\rfloor$ と置き、残りの
  $v>\max(u,z)$ を $G(\lfloor x/u\rfloor)-G(\max(u,z))$ などの差でまとめる。
  対称な寄与も同時に加える。未知側の参照先は $\lfloor x/u\rfloor<x$ なので、既に確定している。

非対角の区間更新と、その位置でまとめる領域は $v\le z$ と $v>z$ に対応する。
$z$ は圧縮前の添字であり、`D>1` では逆順の配列添字とは異なる。
各区間更新は作業配列の両端に加減算するだけで $O(1)$。
更新回数は

$$
O\left(\sum_{v\le B}\min\left(v,\frac{N^{1/D}}{v^{1+1/D}}\right)\right)
=O(N^{2/(2D+1)}).
$$

まとめて処理する側も、異なる $z$ について

$$
O\left(\sum_{z\ge1}\min\left((N/z)^{1/(2D)},\frac{N^{1/D}}{z^{1+1/D}}\right)\right)
=O(N^{2/(2D+1)}).
$$

対角と保持位置の走査は $O(N^{1/(D+1)})$。
Fenwick Tree や長さ $N^{2/(2D+1)}$ の配列は使わない。
`D=1` は [adamant の記事の追記](https://codeforces.com/topic/118259/en23) の領域分割に対応する。
積と同様、乗法性を仮定しない。

### 積の計算法の選択と既存ライブラリとの関係

$B$ 以下の係数の積は `DirichletSeries::convolve` に渡す。
一般の場合の $O(B\log B)$ も、固定した $D$ では全体の $O(N^{2/(2D+1)})$ に収まる。
両入力の乗法性が保証されている場合は、ここで乗法的関数用の処理を使う。
大きい位置では、直接の双曲線分割と区間集約を自動で選ぶ。
`convolve_sparse` は密な部分・大きい位置の両方で非零項の直接列挙を強制する。

`D=1` の積は従来の座標計算に特化した実装を使う。
`DirichletSeries` 自体のインターフェースは変わらず、乗法的関数の和や一般化した Euler 積も
従来通り `DirichletPrefixSum<R>` を使う。
実測は [測定記録](../../../../benchmark/dirichlet_prefix_sum.md) を参照。
