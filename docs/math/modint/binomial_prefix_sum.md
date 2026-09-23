## 概要

二項係数の prefix sum $\displaystyle S(n,k)=\sum_{i=0}^{k}\binom{n}{i}$ をオンラインで求める。

ブロック幅 $B$ をテンプレート引数で指定し、クエリに必要な範囲まで自動で前計算する。$n$ の上限を決めずに使える。

到達した $n$ の最大値を $N$ とすると、追加する前計算は合計 $O(N+N^2/B+1)$ 時間、計算済みの範囲へのクエリは $O(B)$ 時間。メモリは $O(N+(N/B+1)^2)$。

標準は $B=512$。$B$ を小さくするとクエリが速くなり、前計算の時間とメモリが増える。$B$ を $\sqrt{N}$ 程度に選ぶと、前計算 $O(N\sqrt{N}+1)$ 時間、クエリ $O(\sqrt{N}+1)$ 時間、メモリ $O(N+1)$ となる。

$n,k$ が $B$ の倍数である点の答えを保存し、クエリごとに近くの点から [BinomialPrefixSumSlider](binomial_prefix_sum_slider.md) で移動する。すべてのクエリが事前に分かる場合は、slider と [Mo](../../ds/mo/mo.md) の組み合わせも使える。

## 使用例

```cpp
#include "math/modint/binomial_prefix_sum.hpp"

using mint = modint998244353;
BinomialPrefixSum<mint> bs; // ブロック幅 512。明示するなら <mint, 512>
assert(bs.sum(5, 2) == 16); // 1 + 5 + 10
assert(bs.sum(5, -1) == 0);
assert(bs.sum(5, 10) == 32);
bs.reserve(1000); // 必要な範囲をまとめて前計算することもできる
assert(bs.sum(1001, 1) == 1002); // さらに大きい n にも自動で拡張する
```

## 詳細なドキュメント

### BinomialPrefixSum

`mint` は素数を法とする modint 型、法 $p$ は $p>2$ とする。dynamic modint の法は、このオブジェクトを使う間固定する。

テンプレート引数 $B$ は正の整数で、標準は $512$。コンパイル時定数なので除算の定数最適化が効き、$2$ のべき乗ではシフトを使える。

#### コンストラクタ

```cpp
BinomialPrefixSum<mint, B> bs(int n = 0)
```

$n$ まで前計算して初期化する。省略すると $n=0$ で初期化し、その後はクエリに合わせて拡張する。

##### 制約

- $0 \leq n < p$

##### 計算量

- $O(n+n^2/B+1)$

#### reserve

```cpp
void reserve(int n)
```

$n$ までのクエリを処理するための表と、[Binomial](binomial.md) のテーブルを前計算する。計算済みの部分を再利用し、追加分だけ計算する。`sum` からも自動で呼ばれる。

##### 制約

- $0 \leq n < p$

##### 計算量

$T$ 回呼び出し、引数の最大値を $N$ として、

- 合計 $O(T+N+N^2/B)$

#### sum

```cpp
mint sum(int n, int k)
```

$S(n,k)$ を返す。$k<0$ なら $0$、$k \geq n$ なら $2^n$ を返す。$k \geq 0$ の場合、必要に応じて `reserve(n)` で前計算を追加する。

##### 制約

- $0 \leq n < p$

##### 計算量

- 前計算の追加を除いて $O(B)$
- $k<0$ の場合は $O(1)$
- $k \geq n$ の場合は、前計算の追加を除いて $O(1)$
