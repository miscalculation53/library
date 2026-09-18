## 概要

乗法的関数 $f$ の $F(x)=\sum_{i\le x}f(i)$ を商集合
$Q_N=\{\lfloor N/j\rfloor:1\le j\le N\}$ 上で計算する。
戻り値は [DirichletPrefixSum](dirichlet_prefix_sum.md)。全体の和は `result.F(N)`。
$F(0)=0$ も取得できる。

完全乗法的関数の既知の累積和から素数上の和を求める `prime_prefix_sum` と、
素数上の和 $P(x)=\sum_{p\le x}f(p)$ および素数冪での値から $F(x)$ を求める
`multiplicative_prefix_sum` を提供する。
素数上の和を別の方法で用意できるなら、前者は不要。
複数の素数上の和の線形結合も、そのまま後者に渡せる。
特に $f(p)$ が $p$ の多項式である必要はない。

環での除算は不要。整数環・多項式環・小さい標数や零因子のある環も使える。
時間は自動選択で $O(N^{2/3})$、空間は $O(\sqrt N)$。
素数で一致する関数 $g$ の累積和を簡単に計算でき、一点の和だけが必要な場合は
[powerful_number_sieve](powerful_number_sieve.md) も使える。
コストも乗法的な関数にする $\sum_{c(x)\le N}w(x)$ などの一般化には
[euler_product_prefix_sum](euler_product_prefix_sum.md) を使う。

## 使用例

```cpp
#include "math/prime/sieve/multiplicative_prefix_sum.hpp"
#include "math/modint/modint.hpp"

using mint = modint998244353;
using R = RingAddSubMul<mint>;

DirichletPrefixSum<R> one(N, zeta_prefix_sum);
DirichletPrefixSum<R> id(N, id_prefix_sum);
auto pi = prime_prefix_sum(one);       // π(x)
auto prime_id = prime_prefix_sum(id);  // Σ_{p<=x} p

auto phi = multiplicative_prefix_sum(prime_id - pi, totient_primepower);
mint answer = phi.F(N);

// f(p^e) = a*e + b*p の場合。整数の係数・戻り値は mint に変換される。
ll a = 2, b = 3;
auto weighted = multiplicative_prefix_sum(a * pi + b * prime_id,
  [&](auto &pp) { return a * pp.e + b * pp.p; });
mint weighted_answer = weighted.F(N);
```

環は入力テーブルから推論する。ラムダに型引数を付ける必要はない。
`multiplicative_prefix_sum.hpp` 一つを読み込み、上記の二関数だけを使う。
前計算境界や手法の指定は不要。3/4 乗版・2/3 乗版は `internal` に置く。

## 詳細なドキュメント

以下では環の演算・要素のコピー・整数の環への埋め込み・素数冪の評価を $O(1)$ とする。
入力の準備時間や、係数の演算自体にかかるコストは別途加える。
素数上の和を $d$ 本計算する場合は、その分の時間が必要。
小さい $N$ の対数・分母は定数として読む。$N=0$ では $O(1)$ 時間で空のテーブルを返す。
各関数の追加空間は $O(\sqrt N)$。

#### prime_prefix_sum

```cpp
template <class R>
DirichletPrefixSum<R> prime_prefix_sum(const DirichletPrefixSum<R> &a);
```

完全乗法的関数 $f$ の累積和 $a$ から、$P(x)=\sum_{p\le x}f(p)$ を求める。
入出力は同じ $N$ の商集合。出力の乗法性フラグは `false`。
入力の乗法性フラグの指定は不要。

##### 制約

- `a` が完全乗法的関数の実際の累積和である。
  完全乗法性を呼び出し側で保証し、$N\ge1$ なら $a.F(1)=1_R$。
- 結果・中間値を要素型で表現できる。

##### 計算量

- $O(N^{2/3})$。$N$ に応じて内部の計算法を自動選択する。

#### multiplicative_prefix_sum

```cpp
template <class R, class GetPrimePower>
DirichletPrefixSum<R> multiplicative_prefix_sum(
  const DirichletPrefixSum<R> &prime_sum, const GetPrimePower &f_primepower);
```

素数上の和 $P(x)=\sum_{p\le x}f(p)$ と素数冪での値 $f(p^e)$ から、
乗法的関数の累積和 $F(x)$ を求める。
入出力は同じ $N$ の商集合。出力の乗法性フラグは `true`。

コールバックは `f_primepower(q)` または `f_primepower(q, R{})` の形式で、
$f(q.pe)$ を返す。両方呼べる場合は単引数を優先する。
`q` には **`PrimePower<ll>`** を渡す。
const な参照で渡すため、ラムダの引数を `auto &q` と書いても使える。この場合も `q` は const な参照になる。
用意済みの [素数冪のラムダ](enumerate_multiplicative.md) や、`const auto &q` で受けるラムダも使える。

##### 制約

- $f$ は乗法的で、$f(1)=1_R$。零になる素数冪も許す。
- `prime_sum` が求めたい $f$ の素数上の和である。$N\ge1$ なら $P(1)=0_R$。
  コールバックの $f(p)$ と一致することを呼び出し側で保証する。
- コールバックは const な状態で呼べ、返り値を `R::S` に代入できる。
  二引数形式では `R{}` を構築できる。
- 結果・中間値を要素型で表現できる。

##### 計算量

- $O(N^{2/3})$。$N$ に応じて内部の計算法を自動選択する。

## アルゴリズム

### 基本の 3/4 乗版と自動選択

素数を順に消す Lucy DP で素数上の和を求め、逆順に素数冪の寄与を戻す DP で乗法的関数を復元する。
自動版は $N<10^6$ なら基本 DP、それ以上なら Fenwick Tree を使う 2/3 乗版を選ぶ。
境界は [modint での実測](../../../../benchmark/multiplicative_prefix_sum.md) に基づく目安。
両方を実行して速さを調べるわけではなく、環や実行環境ごとの最速を保証するものではない。

### 2/3 乗への高速化

素数を $N^{1/6},N^{1/3}$ で分ける。
小素数では通常の DP を行い、中間の素数では $x\gtrsim N^{2/3}$ の商位置だけを直接更新する。
それ以下の位置は、最小素因数がその素数である数を一度ずつ列挙し、
累積和への変更を商集合の座標に圧縮した Fenwick Tree に蓄積する。

列挙される数は小さい素因数を持たないため $O(N^{2/3}/\log N)$ 個であり、
各変更に $O(\log N)$ かけても全体 $O(N^{2/3})$ となる。
乗法的関数を復元する側では、まず $N^{1/3}$ より大きい素因数だけの寄与を作る。
この場合の素因数は重複込みで高々二つなので、素数上の和から直接求められる。
その後、中間の素数と小素数を戻す。

環での加減算・乗算しか使わず、素数冪での値が零でもよい。
従来と同じ入力から同じ累積和テーブルを返すため、扱える関数の条件は変わらない。
参考：[rsk0315「素数の数え上げと乗法的関数の和」](https://rsk0315.github.io/slides/prime-counting.pdf)
の高速化と付録（掲載の擬似コードをそのまま転記せず、商集合の境界に合わせて実装）。
