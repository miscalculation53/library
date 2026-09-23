## 概要

二項係数の prefix sum

$$
S(n,k) = \sum_{i=0}^{k} \binom{n}{i}
$$

を、$n,k$ を $1$ ずつ増減しながら管理する。`l = n`, `r = k`, `sum = S(n,k)` を保持し、[Mo](../../ds/mo/mo.md) には平面上の点 $(n,k)$ として渡す。

$S(n,-1)=0$、$k \geq n$ では $S(n,k)=2^n$ とする。以下、法を $p$ とする。

更新には $S(n+1,k)=2S(n,k)-\binom{n}{k}$ を使う。[Binomial](binomial.md) のテーブルを必要な $n$ まで前計算すると、各更新は $O(1)$ 時間。

クエリを順次受け取りたい場合は [BinomialPrefixSum](binomial_prefix_sum.md) を使う。

## 使用例

```cpp
#include "math/modint/binomial_prefix_sum_slider.hpp"
#include "ds/mo/mo.hpp"

using mint = modint998244353;
vc<pair<int, int>> queries = {{5, 2}, {3, 3}, {0, 0}}; // (n, k)
BinomialPrefixSumSlider<mint> slider(5);
vc<mint> ans(queries.size());
mo(5, queries, slider, [&](int q) { ans[q] = slider.sum; });
// ans = {16, 8, 1}
```

## 詳細なドキュメント

### BinomialPrefixSumSlider

`mint` は素数を法とする modint 型、$p>2$ とする。dynamic modint の法は、このオブジェクトを使う間固定する。

#### コンストラクタ

```cpp
BinomialPrefixSumSlider<mint> slider(int max_n = 0)
```

`l = r = 0`, `sum = 1` で初期化し、`Binomial<mint>::reserve(max_n)` を呼ぶ。`max_n` より大きい $n$ に移動する場合は、必要に応じてテーブルを追加で計算する。

##### 制約

- $0 \leq \mathrm{max\_n} < p$

##### 計算量

- $O(\mathrm{max\_n}+1)$

#### lpp / lmm / rpp / rmm

```cpp
void lpp()
void lmm()
void rpp()
void rmm()
```

| 関数 | 更新 |
| --- | --- |
| `lpp()` | $n \leftarrow n+1$ |
| `lmm()` | $n \leftarrow n-1$ |
| `rpp()` | $k \leftarrow k+1$ |
| `rmm()` | $k \leftarrow k-1$ |

`sum` も更新後の $S(n,k)$ に更新する。

##### 制約

- 更新後も $0 \leq n < p$, $k \geq -1$

##### 計算量

- `Binomial` のテーブルが必要な $n$ まで計算済みなら $O(1)$
- テーブルの追加計算は、到達した $n$ の最大値までの総和で $O(\max(n)+1)$

#### set

```cpp
void set(int nl, int nr)
```

現在の状態を $(n,k)=(\mathrm{nl},\mathrm{nr})$ に移動する。`set(n, k)` で $S(n,k)$ を求める。

##### 制約

- $0 \leq \mathrm{nl} < p$
- $\mathrm{nr} \geq -1$

##### 計算量

移動前の状態を $(n,k)$ として、

- $O(1+|\mathrm{nl}-n|+|\mathrm{nr}-k|)$。テーブルの追加計算を除く。
