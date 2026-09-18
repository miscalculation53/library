## 概要

局所因子の積

$$
\widetilde A(s)=\widetilde B(s)
\prod_i\left(1+\sum_j a_{i,j}d_{i,j}^{-s}\right)
$$

の係数の累積和を作る。$B$ は既知の数列で、省略すると Dirichlet 積の単位元。
添字 $d_{i,j}$ は正整数で、素数冪でなくてもよい。
因子ごとに「何も選ばない」または「項を一つ選ぶ」と解釈できる。
同じ因子の項同士は掛けず、異なる因子からの選択を掛け合わせる。

結果は [DirichletPrefixSum](dirichlet_prefix_sum.md)。
正の商集合 $Q_N=\{\lfloor N/i\rfloor:1\le i\le N\}$ 上の累積和と $F(0)=0$ を持ち、
全体の和は `result.F(N)` で取得する。
係数には可換環 `R` を使う。環での除算は使わず、整数環・合成数法・多項式環でも使える。

乗法的なコスト $c$ と重み $w$ に対する

$$
S(N)=\sum_{c(x)\le N}w(x)
$$

は、素数 $p$ の局所因子に $(d,a)=(c(p^e),w(p^e))$ を入れた場合。
通常の乗法的関数の和は $c(x)=x$、Totient Bound は $c(x)=\varphi(x),w(x)=1$。
ただし、出力の係数 $A(m)=\sum_{c(x)=m}w(x)$ 自体は一般には乗法的でない。

通常の乗法的関数には [multiplicative_prefix_sum](multiplicative_prefix_sum.md) の専用計算法を使う。
一般の入力に対する計算量は因子数・項数・コストに依存する。

## 使用例

```cpp
#include "math/prime/sieve/euler_product_prefix_sum.hpp"

using R = RingAddSubMul<ll>;
vvc<pair<ll, R::S>> factors = {
  {{2, 1}, {6, 3}},   // 1 + 2^(-s) + 3 * 6^(-s)
  {{3, -1}, {4, 2}},  // 1 - 3^(-s) + 2 * 4^(-s)
};
auto result = euler_product_prefix_sum<R>(N, factors);
ll answer = result.F(N);
```

既知の数列 $B$ を掛けた状態から始める場合は、その累積和を `seed` に渡す。
この場合の環は `seed` から推論する。

```cpp
auto result = euler_product_prefix_sum(seed, factors);
```

大きい因子のまとめ方と Totient Bound への適用は、末尾のアルゴリズムを参照。

## 詳細なドキュメント

以下では環の演算・要素のコピーを $O(1)$ とする。
正の商の個数を $D=\lvert Q_N\rvert=O(\sqrt N)$、因子数を $M$、入力項数を $I$ とする。

#### factors の形式

`factors` は `vvc<pair<ll, S>>` 型で、内側の配列一つが局所因子一つを表す。
`(d,a)` で $a\,d^{-s}$ を表し、先頭の $1$ は暗黙に加わる。
因子の項から一つを選ぶか、何も選ばないかを表す。

同じコストに複数の選択肢がある場合は、別の項として入れてもよい。
**$d=1$ の追加項も許す。** 例えば `{{1, 1}, {2, -1}}` は $2-2^{-s}$。
このため最終的な係数 $A(1)$ が $1_R$ でない場合や零になる場合も扱う。
入力の保存には全体で $O(M+I)$ 空間を使う。

#### euler_product_prefix_sum

```cpp
template <class R>
DirichletPrefixSum<R> euler_product_prefix_sum(
  const DirichletPrefixSum<R> &seed, const vvc<pair<ll, typename R::S>> &factors);

template <class R>
DirichletPrefixSum<R> euler_product_prefix_sum(
  ll N, const vvc<pair<ll, typename R::S>> &factors);
```

`seed` が表す数列 $B$ に全局所因子を掛け、係数の累積和を返す。
`N` を渡す版では $B=\varepsilon$（Dirichlet 積の単位元）とする。
入力を変更しない。

- `seed` には任意の正しい累積和を渡せる。
- 因子の順番・因子内の項の順番は自由。零係数や重複する $d$ も許す。
- $d>N$ の項は無視する。
- 出力は同じ $N$ の商集合上の `DirichletPrefixSum<R>` で、乗法性フラグは常に `false`。
  Dirichlet 積・和・商などにそのまま接続できる。
- $N=0$ では空のテーブルを返す。

追加空間は $O(D+M)$。入力の局所因子の保存に使う空間は別途必要。
`seed` の構築をライブラリが自動で行うことはない。

##### 制約

- `R` は可換環。結果・中間値を要素型で表現できる。
- 各項のコストが $1\le d$。
- 与える因子・項は有限個。各因子の $d\le N$ の項はすべて含める。
  例えば $c(x)=1,w(x)=1$ のような無限の和を、この有限な入力にはできない。

$O(N^{2/3})$ 時間・$O(\sqrt N)$ 空間で使える代表的な条件は次の通り。

- 素数で添字付けした因子で、コストが $c(p^e)=\Theta(p^e)$。
  比較の定数は $p,e,N$ に依存しない。少数のコスト $1$ の項などは許す。
- 大きい因子を `seed` に集約し、その $[2,\lfloor\sqrt N\rfloor]$ の係数が零。
- 明示する因子・項は合計 $O(\sqrt N)$ 個で、各コスト・重みを $O(1)$ 時間で評価できる。

通常の乗法的関数と $c=\varphi$ はこの場合に当たる。
任意の因子や任意の `seed` で同じ計算量になるわけではない。

##### 計算量

各因子を処理する直前の係数が $[2,L_i)$ で零と分かっているとする。
実装が利用する $L_i$ は次のように入力から決まる。$K=\lfloor\sqrt N\rfloor$ と置く。

- $L_0$：`seed` の $[2,K]$ にある最初の非零係数の添字。なければ $K+1$。
  `seed` を省略した場合も $L_0=K+1$。
- $m_i$：因子 $i$ に含まれる $2\le d\le N$ の非零項の最小コスト。なければ $+\infty$。
- 因子を $m_i$ の降順に並べたとき、$L_i=\min(L_0,m_1,\ldots,m_{i-1})$。
  最初の因子では $L_1=L_0$。

時間計算量は

$$
O\left(
D+I+M\log(M+1)
+\log(D+1)\sum_{\substack{i,j\\d_{i,j}\le N,\ a_{i,j}\ne0}}
\left(1+\min\left\{\sqrt{\frac{N}{d_{i,j}}},\frac{N}{d_{i,j}L_i}\right\}\right)
\right).
$$

`seed` の準備時間は別途加える。環の演算に加え、零判定・整数の除算と平方根を $O(1)$ とする。
$N=0$ では $O(1)$。

処理順を追わずに評価したい場合は、$L_i$ を $\min(L_0,m_i)$ に置き換えても上界になる。
零区間を使わず、各項を $1+\sqrt{N/d_{i,j}}$ で抑えてもよい。
打ち消しなどによって実際にはさらに速くなる場合があるが、その効果はこの評価に含めない。
上記の素数の因子に対する条件を満たす場合は、この和を評価すると $O(N^{2/3})$ となる。

## アルゴリズム

### seed を渡す場合と後から掛ける場合

数学的には `euler_product_prefix_sum(seed, factors)` と
`euler_product_prefix_sum<R>(N, factors) * seed` は同じ結果になる。
後者は全保持位置の Dirichlet 積を一回追加するため、一般の上界として $O(N^{2/3})$ 時間が加わる。
実際の Dirichlet 積は疎さも利用するので、常にその時間が必要なわけではない。
最終的な一点の和だけが必要なら `dirichlet_convolution_sum` の $O(\sqrt N)$ 時間でよい。

`seed` を渡すと一回の積を省けるが、局所因子の処理時間も変わる。
`seed` の $[2,K]$ の係数がすべて零なら、$L_i$ は単位元から始める場合と同じであり、
上の更新回数の評価を保ったまま積を省ける。
一方、例えば添字 $2$ の係数が非零ならすべての $L_i=2$ となる。
素数 $p\le\sqrt N$ にコスト $p$ の項が一つずつある場合、
零区間による短縮を失った評価は $O(N^{3/4})$ となり、後から掛ける方がよい場合もある。

### $N^{2/3}$ より速くなる例

素因数の指数がすべて $2$ 以上である整数（powerful number）の個数を考える。
このような整数は $a^2b^3$（$b$ は squarefree）と一意に表せるので、対応する級数は

$$
\zeta(2s)\prod_p(1+p^{-3s})
$$

となる。`seed` には平方数の指示関数、局所因子には $p^3\le N$ の項 $(p^3,1)$ を渡す。

```cpp
using R = RingAddSubMul<ll>;
DirichletPrefixSum<R> squares(N, [](ll x) { return iroot(x, 2); });
const int limit = iroot(N, 3);
vvc<pair<ll, R::S>> factors;
for (ll p : LinearSieve::primes(limit)) {
  factors.push_back({{p * p * p, 1}});
}
auto result = euler_product_prefix_sum(squares, factors);
```

零区間を使わない評価でも、項ごとの $\sqrt{N/p^3}$ の和は

$$
\sum_{p\le N^{1/3}}\sqrt{N/p^3}
\le\sqrt N\sum_{m=2}^{\infty}m^{-3/2}=O(\sqrt N).
$$

因子の準備・`seed` の構築を含めて、全保持位置を $O(\sqrt N\log N)$ 時間・$O(\sqrt N)$ 空間で計算できる。
これはこの関数による上界であり、この問題専用の最速計算法を主張するものではない。
一点の個数だけなら、squarefree な $b\le N^{1/3}$ を列挙して
$\sum_b\lfloor\sqrt{N/b^3}\rfloor$ を取る方法もある。

一意表現の参考：[Tsz Ho Chan, Arithmetic Progressions Among Powerful Numbers](https://cs.uwaterloo.ca/journals/JIS/VOL26/Chan/chan33.pdf)。

### 大きい因子をまとめる

通常はすべての素数を列挙せず、最小の非自明コストが $\sqrt N$ 以下の因子だけを明示する。
残りの因子は、非自明な項を二つ選ぶとコストが $N$ を超えるため、$[1,N]$ 上では

$$
\prod_{i\in\mathrm{tail}}(1+A_i)=1+\sum_{i\in\mathrm{tail}}A_i
$$

となる。この累積和を `seed` として渡す。
境界は **最小コスト $>\lfloor\sqrt N\rfloor$**。等号では二つ選べる場合がある。

`seed` の構築方法をライブラリが推測することはない。
素数上の和が必要なら [prime_prefix_sum](multiplicative_prefix_sum.md) などで先に用意する。
任意の重みをブラックボックスで渡すだけで、大素数の集約まで自動で高速化できるわけではない。

例えば $N=100$ で、変形後の $p=13,17$ の因子を掛けると、
それぞれの非定数項の添字は $12,13$ と $16,17$。
両方から一項ずつ選んだ添字は最小でも $12\cdot16=192>100$ なので、
$100$ 以下には、片方からだけ選んだ四項と定数項だけが残る。
大きい因子を一つだけ選んだ寄与を `seed` に保持し、小さい因子との積はその後の処理で計算する。

### Totient Bound の全累積和

以下はすべての保持位置で $\#\{x:\varphi(x)\le t\}$ を返す。
素数冪のコストは $(p-1)p^{e-1}$、重みは $1$。
この形のままでも、`seed` の準備を含めて $O(N^{2/3})$ 時間・$O(\sqrt N)$ 空間。
明示する素数を $p\le K+1$、$K=\lfloor\sqrt N\rfloor$ に限り、
それより大きい素数の部分は、正の $t\le K$ で $1$、$t>K$ で
$1+\pi(t+1)-\pi(K+1)$ という累積和にまとめる。

```cpp
#include "math/prime/sieve/multiplicative_prefix_sum.hpp"
#include "math/prime/sieve/euler_product_prefix_sum.hpp"
#include "math/prime/large/primality_test.hpp"

using R = RingAddSubMul<ll>;
const int root = int(iroot(N, 2));
auto pi = prime_prefix_sum(DirichletPrefixSum<R>(N, zeta_prefix_sum));
vvc<pair<ll, R::S>> factors;
ll small_primes = 0;
for (int p : LinearSieve::primes(root + 1)) {
  ++small_primes;
  vc<pair<ll, R::S>> factor;
  for (ll cost = p - 1; cost <= N;) {
    factor.emplace_back(cost, 1);
    if (cost > N / p) break;
    cost *= p;
  }
  factors.push_back(move(factor));
}
DirichletPrefixSum<R> tail(N, [&](ll t) {
  return t <= root ? 1LL : 1 + pi.F(t) + is_prime(t + 1) - small_primes;
});
auto count = euler_product_prefix_sum(tail, factors);
```

`pi.F(t) + is_prime(t+1)` で $\pi(t+1)$ を求める。
`pi.F(t+1)` は商集合から外れることがあるため呼ばない。
$p=\lfloor\sqrt N\rfloor+1$ が素数ならコストは $\lfloor\sqrt N\rfloor$ なので、明示する側に含める。
$\varphi(2)=1$ も通常の局所因子の項として処理する。

重みを変えるときは局所因子と大素数側の両方を変える。
例えば各非自明な選択の重みを多項式の $z$ にすると
$\sum_{\varphi(x)\le t}z^{\omega(x)}$ が得られる。
`PolynomialRingArray<R, D>` を使えば、異なる素因数の個数が $D$ 未満の各場合を同時に集計できる。

### Totient Bound の一点の和

[Totient Bound](https://yukicoder.me/problems/no/1322) では、次の変形で素数計数を省ける。

$$
\sum_{x\ge1}\varphi(x)^{-s}
=\zeta(s)\prod_p\left(1+(p-1)^{-s}-p^{-s}\right).
$$

$p\le\lfloor\sqrt N\rfloor+1$ の因子を `{{p-1,1},{p,-1}}` として渡す。
大素数側の累積和は $t\le\lfloor\sqrt N\rfloor$ で $1$、それより大きい $t$ で $1+[t+1\text{ が素数}]$。
最後に `dirichlet_convolution_sum(product, zeta)` で一点の和を求める。
[verify](../../../../verify/yukicoder/totient_bound.test.cpp) はこの形。

この変形後の `product` 自体は $\varphi$ の逆像の個数テーブルではない。
変形は $O(N^{2/3})$ にするための必須条件ではない。
変形により各素数の明示項は二つになり、大素数側の準備には素数計数の代わりに素数判定だけを使える。
全累積和は、上の直接構築のほか `product * zeta` でも求められる。
この積も $O(N^{2/3})$ 時間・$O(\sqrt N)$ 空間なので、
全体の計算量は変わらない。一点の和だけなら、上記の $O(\sqrt N)$ 時間の関数を使う。

### 局所因子の更新

商集合の隣り合う境界の間の **係数和** を保持し、Fenwick Tree で区間和を取る。
因子は最小の非自明コストが大きい順に処理する。
係数の既知の零区間 $[2,L)$ を使い、コスト $d$ の項を掛ける更新を二つに分ける。

- 小さい元の添字 $v\le B$：係数を読み、コスト $dv$ を含むブロックへ加える。
- 大きい元の添字 $v>B$：更新先のブロック $[l+1,r]$ ごとに、
  元の区間 $(\max(B,\lfloor l/d\rfloor),\lfloor r/d\rfloor]$ の係数和を取る。

$v=1$ の寄与は別に加える。同じ局所因子の全項を読み終えてから更新を反映するので、
同じ因子の選択肢を二つ選んでしまうことはない。$d=1$ でも更新前の値を読む。

環の演算を $O(1)$ とする。正の商の個数を $D=O(\sqrt N)$、因子数を $M$、入力項数を $I$ と置く。
各有効項 $(d,a)$ について、実装の境界は

$$
B=\min\left(\lfloor N/d\rfloor,
\max\left(L-1,\left\lfloor\sqrt{N/d}\right\rfloor\right)\right).
$$

$Q(d,B)$ を $\lfloor x/d\rfloor>B$ を満たす保持位置 $x$ の個数とする。
このような位置は $x\ge d(B+1)$ なので、$Q(d,B)\le N/(d(B+1))$。
元の添字の走査回数と合わせると

$$
1+(B-L+1)_++Q(d,B)
=O\left(1+\min\left\{\sqrt{N/d},\frac{N}{dL}\right\}\right)
$$

となる。$L\le\sqrt{N/d}$ なら平方根で分割し、$L>\sqrt{N/d}$ なら零区間を飛ばして
$N/(dL)$ 個以下の大きい保持位置だけを調べるためである。
各走査・変更先に Fenwick Tree の $O(\log(D+1))$ を掛け、入力の走査と因子の整列を加えると、
API に記載した計算量が得られる。添字 $1$ の寄与が各項の定数 $1$ に対応する。
追加空間は $O(D+M)$。入力の保存に $O(M+I)$ を使う。

### 素数の因子に対する計算量の評価

API に記載した $c(p^e)=\Theta(p^e)$ などの条件を仮定する。
主な $e=1$ の更新数は、$p\le N^{1/3}$ でおよそ $\sqrt{N/p}$、それ以上でおよそ $N/p^2$。
これは $d=\Theta(p)$ と $L=\Omega(p)$ を代入した

$$
\log N\sum_{p\le O(\sqrt N)}
\min\left\{\sqrt{N/p},\frac{N}{p^2}\right\}=O(N^{2/3})
$$

という評価に対応する。$e\ge2$ の項も、同じ素数の中で平方根側は $p^{-1/2}$ 倍、
もう一方は $p^{-1}$ 倍ずつ減る幾何級数で抑えられる。
各項の定数時間と入力の走査・整列は、仮定の入力サイズでは $O(\sqrt N\log N)$ に収まる。

任意の因子や任意の `seed` で同じ計算量になるわけではない。
例えば全整数で添字付けした $c(m^e)=m^e$ の因子では、同じ評価は $O(N^{2/3}\log N)$。

更新分割の参考：[Naszt による一般化された Euler 積の篩](https://www.luogu.com.cn/article/2jbt2rrb)。
実装では区間の境界・更新の同時性・コスト $1$・任意の初期数列を明示的に扱っている。
