## 概要

乗法的関数の $f(1), \dots, f(n)$ を列挙する。

有名な乗法的関数については、$f(p^e)$ の値を返すラムダ式を用意している。

## 使用例

```cpp
#include "math/prime/sieve/enumerate_multiplicative.hpp"

using R = RingAddSubMul<ll>;
auto phi = enumerate_multiplicative<R>(N, totient_primepower);
auto mu = enumerate_multiplicative<R>(N, mobius_primepower);
auto squares = enumerate_completely_multiplicative<R>(N, pow_primepower(2));
```

自作のラムダ式を使う例（環を省略することもできる）：

```cpp
auto tau_squared = enumerate_multiplicative(N, [](const auto &q) -> ll {
  return ll(q.e + 1) * (q.e + 1);
});
```

### 有名な乗法的関数

| 関数名 | 関数 $f(n)$ | 完全乗法的 |
| --- | --- | --- |
| `e` | $\varepsilon(1)=1$、$n\ge2$ では $0$ | ○ |
| `zeta` | $1$ | ○ |
| `id` | $n$ | ○ |
| `pow` | $n^k$ | ○ |
| `pow_inv` | $n^{-k}$。非可逆の場合は下記参照 | ○ |
| `mobius` | メビウス関数 $\mu(n)$ | |
| `divisor_count` | 約数個数 $\tau(n)$ | |
| `divisor_sum` | 約数総和 $\sigma(n)$ | |
| `divisor_k` | 約数の $k$ 乗和 $\sigma_k(n)$ | |
| `totient` | オイラーのファイ関数 $\varphi(n)$ | |

## 詳細なドキュメント

以下では、環の演算・要素のコピーを $O(1)$ とする。
多項式環などでは、そのコストを別途掛ける。

#### enumerate_multiplicative

```cpp
template <class R, class F>
vc<typename R::S> enumerate_multiplicative(int n, const F &f_primepower);

template <class F>
auto enumerate_multiplicative(int n, const F &f_primepower);
```

$f(1),\ldots,f(n)$ を長さ $n+1$ の 1-indexed 配列で返す。
`a[0]=R::e0()`、$n\ge1$ なら `a[1]=R::e1()`。
$n=0$ では `{R::e0()}` を返す。

コールバックは `f_primepower(q)` または `f_primepower(q, R{})` の形式で、
$f(p^e)$ を返す。両方呼べる場合は単引数を優先する。
第 2 引数は環を表すオブジェクトであり、単位元の値ではない。
`q` は `PrimePower<int>` で、次を持つ。

- `q.p`：素数 $p$。
- `q.e`：指数 $e$。
- `q.pe`：素数冪 $p^e$。

各素数冪 $2\le p^e\le n$ について昇順に一度ずつ呼ぶ。$p^0$ では呼ばない。
素数冪は const な参照で渡す。ラムダの引数は `auto q`、`const auto &q`、`auto &q` のいずれでもよい。
`auto &q` と書いた場合も、`q` は const な参照になる。
`const PrimePower<ll> &` を受け取るコールバックも、変換コンストラクタにより使える。
篩の準備・拡張は自動で行う。

環を省略した場合は、戻り値から参照・const を除いた型を `S` として
`RingAddSubMul<S>` を使う。任意の環を戻り値の型だけから推論するわけではない。
推論のためにコールバックを実行することはない。

##### 制約

- $0\le n$。
- `R` は可換環。結果・中間値を要素型で表現できる。
- コールバックは const な状態で呼べ、返り値を `R::S` に代入できる。
- 二引数形式では `R{}` を構築できる。
- 環を省略する場合は単引数形式を使う。

##### 計算量

- $O(n+1)$ に、各素数冪でのコールバック評価時間の合計を加えたもの。

#### enumerate_completely_multiplicative

```cpp
template <class R, class F>
vc<typename R::S> enumerate_completely_multiplicative(int n, const F &f_primepower);

template <class F>
auto enumerate_completely_multiplicative(int n, const F &f_primepower);
```

完全乗法的関数の $f(1),\ldots,f(n)$ を返す。
コールバックは素数についてだけ呼ぶ（`q.e=1`）。
配列の形式・環の省略・コールバックの形式は `enumerate_multiplicative` と同じ。

##### 制約

- `enumerate_multiplicative` と同じ制約を満たす。
- $f$ が完全乗法的であることを呼び出し側で保証する。

##### 計算量

- コールバックが $O(t)$ 時間なら $O(n+1+\pi(n)t)$。


##### 制約

- 正しい素数冪 `q=(p,e,p^e)` を渡す。$e\ge0$。
- $0\le k$。
- `pow_inv_primepower` では `R::S` が通常の modint で、`R::inv` を持ち、
  環の演算が modint の演算と一致する。
- 結果だけでなく中間値も要素型で表現できる。

##### 計算量

整数 $a$ を環に写す時間を $C(a)$ とする。
通常の数値型の `RingAddSubMul`・`FieldAddSubMulDiv`、および `RingXorAnd` では $C(a)=O(1)$。
一般の環では $C(a)=O(\log(|a|+1))$。

| ラムダ式 | 素数冪一つあたりの時間 |
| --- | --- |
| `e_primepower`, `zeta_primepower`, `mobius_primepower` | $O(1)$ |
| `id_primepower`, `divisor_sum_primepower`, `totient_primepower` | 整数変換を $O(1)$ 回 |
| `divisor_count_primepower` | $O(1+C(e+1))$ |
| `pow_primepower(k)` | $O(C(p^e)+\log(k+1)+1)$ |
| `pow_inv_primepower(k)` | 上記に逆元計算時間を加えたもの |
| `divisor_k_primepower(k)` | $O(C(p)+\log(k+1)+e+1)$ |

## アルゴリズム

共有の `LinearSieve` で最小素因数の冪 $p^e$ を求め、
$f(n)=f(p^e)f(n/p^e)$ から順に復元する。
完全乗法的な場合は $f(n)=f(p)f(n/p)$ を使う。

整数の環への埋め込みには二分法による加算を使う。
`RingFromGroupMonoid`・`FieldFromGroupGroup` では加法群の `pow` を利用できるため、
通常の数値型や XOR の環では定数時間になる。

`divisor_sum_primepower` は $p^e+(p^e-1)/(p-1)$ の各項を環に写してから加える。
中間計算で $p^{e+1}$ を作らず、環での除算も使わない。
`divisor_k_primepower` も乗算と加算だけで計算する。
