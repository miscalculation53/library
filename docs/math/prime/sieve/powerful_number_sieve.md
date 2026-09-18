## 概要

乗法的関数を $f=g*h$ と分解し、すべての素数 $p$ で $h(p)=0$ となる場合に、
$F(N)=\sum_{n=1}^N f(n)$ を powerful number 篩で求める。
powerful number は、素因数の指数がすべて $2$ 以上の正整数。$1$ も含める。

入力は $G(x)=\sum_{n=1}^x g(n)$ を返す関数と、素数冪での値 $h(p^e)$。
環には可換環 `R` を指定し、除算は使わない。
これらを定数時間で評価できる場合、$O(\sqrt N)$ 時間で求められる。

戻り値は **$F(N)$ 一点**であり、商集合上の累積和テーブルではない。
既存の [DirichletPrefixSum](dirichlet_prefix_sum.md) を $G$ に使うこともできる。
一般の [multiplicative_prefix_sum](multiplicative_prefix_sum.md) のように、任意の素数上の和から復元する関数ではなく、
素数で一致する扱いやすい $g$ を見つける必要がある。

## 使用例

$f(p^e)=p^{e-1}$ の和。$g(n)=1$ とすると、$h(p)=0$、$e\ge2$ では $h(p^e)=p^{e-1}-p^{e-2}$。

```cpp
#include "math/prime/sieve/powerful_number_sieve.hpp"

using R = RingAddSubMul<ll>;
auto getG = [](ll x) { return x; };
auto h = [](auto &pp) { return pp.pe / pp.p - pp.pe / pp.p / pp.p; };
ll answer = powerful_number_sieve<R>(N, getG, h);
```

既知の累積和テーブルを使う場合：

```cpp
auto answer = powerful_number_sieve<R>(G.n(), [&](ll x) { return G.F(x); }, h);
```

## 詳細なドキュメント

#### powerful_number_sieve

```cpp
template <class R, class GetG, class GetPrimePower>
typename R::S powerful_number_sieve(
  ll N, const GetG &getG, const GetPrimePower &h_primepower);
```

次の値を返す。

$$
\sum_{\substack{1\le d\le N\\d\text{ は powerful}}}
h(d)G(\lfloor N/d\rfloor).
$$

$h(1)=1_R$、$h(p)=0_R$ は暗黙に定め、$h$ の残りの値を乗法性で復元する。
`h_primepower` は $e\ge2$ の値だけを定義すればよい。
$G$ 自体は任意の数列 $g$ の累積和でよく、$g$ の乗法性は計算の正当性には不要。
$f=g*h$ が目的の関数であることは呼び出し側で保証する。

- `getG(x)` または `getG(x, R{})` で累積和を返す。
  呼ばれる $x$ は正の商集合 $Q_N=\{\lfloor N/i\rfloor:1\le i\le N\}$ に含まれる。
  同じ $x$ に複数回呼ぶことがあり、内部では累積和のメモ化をしない。
- `h_primepower(q)` または `h_primepower(q, R{})` で $h(p^e)$ を返す。
  `q` は `const PrimePower<ll>&` で、`.p`, `.e`, `.pe` を持つ。
  `auto &q` のラムダも受け付け、この場合も const な参照になる。
- 両形式が呼べる場合は単引数形式を優先する。
- `h_primepower` は $e\ge2,\ p^e\le N$ のすべての素数冪に一度ずつ呼ぶ。
  素数の昇順、同じ素数では指数の昇順に評価し、結果を保存する。
  $e=0,1$ や $p^e>N$ では呼ばない。
- 途中の重みが零なら、その先の異なる素数を掛ける列挙を省く。
  $h(p^e)=0$ でも、$h(p^{e+1})$ 以降は独立に調べる。
- $N=0$ では $0_R$ を返し、コールバックは呼ばない。

powerful number の一覧は保存しない。追加空間は $O(\sqrt N)$。
素数の準備には `LinearSieve::primes(floor(sqrt(N)))` を使い、共有テーブルを再利用する。
共有テーブルがそれより大きい場合も、使う素数は $p\le\lfloor\sqrt N\rfloor$ に限る。
`getG` 側の前計算・キャッシュに必要な時間と空間は別途必要。

##### 制約

- $0\le N$。
- `R` は可換環。結果・中間値を `R::S` で表現できる。
- 要素に零との等値・不等値比較を行える。
- コールバックは const な状態で呼べ、返り値を `R::S` に変換できる。
- 二引数形式では `R{}` を構築できる。

##### 計算量

環の演算・比較・要素のコピーを $O(1)$ とする。
$\mathcal P_N$ を $N$ 以下の powerful number 全体の集合、
素数冪でのコールバックの時間を $T_h(p,e)$、累積和の評価時間を $T_G(x)$ とすると、上界は

$$
O\left(
\sqrt N
+\sum_{\substack{p\text{ は素数},\ e\ge2\\p^e\le N}}T_h(p,e)
+\sum_{d\in\mathcal P_N}T_G(\lfloor N/d\rfloor)
\right).
$$

零による枝刈りの効果は含めない。$N=0$ では $O(1)$。
$T_h=O(1)$ とし、$T_G(x)=O(x^\alpha)$、$\alpha\ge0$ を固定定数とすると、

| 累積和の評価時間 | 全体の時間 |
| --- | --- |
| $\alpha<1/2$ | $O(\sqrt N)$ |
| $\alpha=1/2$ | $O(\sqrt N\log N)$ |
| $\alpha>1/2$ | $O(N^\alpha)$ |

## アルゴリズム

### 分解と列挙

$f=g*h$ なら、和の順番を変えて

$$
F(N)=\sum_{d=1}^N h(d)G(\lfloor N/d\rfloor)
$$

となる。$h$ は乗法的で $h(p)=0$ なので、指数 $1$ の素因数を持つ $d$ の寄与は零。
残りの powerful number だけを列挙する。

実装では素数を昇順に選び、選んだ素数を指数 $2$ 以上で掛ける DFS を行う。
状態には $\lfloor N/d\rfloor$、次に選べる素数の位置、$h(d)$ を保持する。
同じ素因数の集合と指数を重複して列挙せず、値 $h(d)$ も局所因子の積から定数回の環演算で更新できる。
素数の準備には既存の `LinearSieve` を使う。

### $g,h$ の求め方

$f,g$ が乗法的で $f(p)=g(p)$ なら、$f=g*h$ の $h$ は $h(p)=0$ を満たす。
素数を一つ固定し、$f_e=f(p^e),\ g_e=g(p^e),\ h_e=h(p^e)$ と書くと、

$$
h_0=1,\qquad h_1=0,\qquad
h_e=f_e-g_e-\sum_{j=2}^{e-1}h_jg_{e-j}\quad(e\ge2).
$$

この漸化式、または局所生成関数 $\sum_e h_et^e=(\sum_e f_et^e)/(\sum_e g_et^e)$ から求める。
`powerful_number_sieve` に渡すのは、元の $f_e$ ではなく、この $h_e$。
$g$ が完全乗法的なら $h_e=f_e-g(p)f_{e-1}$ と簡単になる。

### 計算量の導出

powerful number は $a^2b^3$（$b$ は squarefree）と一意に表せる。
従って

$$
\lvert\mathcal P_N\rvert
\le\sum_{b\le N^{1/3}}\left\lfloor\sqrt{N/b^3}\right\rfloor
\le\sqrt N\sum_{b\ge1}b^{-3/2}=O(\sqrt N).
$$

DFS の各状態が powerful number 一つに対応し、素数冪の前計算も $O(\sqrt N)$ に収まる。
$T_G(x)=O(x^\alpha)$ の場合は、

$$
\sum_{d\in\mathcal P_N}(N/d)^\alpha
\le N^\alpha\sum_{b\le N^{1/3}}b^{-3\alpha}
\sum_{a\le\sqrt{N/b^3}}a^{-2\alpha}
$$

を評価すればよい。$\alpha<1/2$ では内側の和が
$O((N/b^3)^{(1-2\alpha)/2})$ となり、$O(\sqrt N\sum_b b^{-3/2})$。
$\alpha=1/2$ では $a$ の逆数和から $\log N$ が付き、
$\alpha>1/2$ では $a,b$ の両方の級数が収束する。

参考：[kyopro_friends による powerful number 篩の解説](https://atcoder.jp/contests/abc370/editorial/10882)、
[baihacker, The prefix-sum of multiplicative function: powerful number sieve](https://baihacker.github.io/main/2020/The_prefix-sum_of_multiplicative_function_powerful_number_sieve.html)。
