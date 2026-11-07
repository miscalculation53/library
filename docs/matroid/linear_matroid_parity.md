このドキュメントには AI による作成・変更が含まれています。

## 概要

体上の列ベクトルを互いに素なペアに分け、選んだペアに含まれるすべての列が線形独立になるよう、ペア数を最大化する乱択アルゴリズム。
返るペアの集合は常に実行可能であり、最大性には乱択による誤りの確率がある。

ペア $i$ の列を $a_i,b_i$ とすると、独立な変数を持つ交代行列

$$
\sum_i x_i\left(a_i b_i^{\mathsf T}-b_i a_i^{\mathsf T}\right)
$$

の階数の半分が最適なペア数に一致する。
乱数を代入して階数を求め、独立なペアの縮約と残りの階数評価を繰り返して復元する。
標数2でも対角成分を0に保つ交代行列を使う。
階数計算を繰り返す単純な実装で、入力の階数が小さい場合に適する。

階数と最適ペア数の関係は Lovász の階数公式による。

## 使用例

```cpp
#include "matroid/linear_matroid_parity.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
LinearMatroid<F> m(3, {{1, 0, 0}, {0, 1, 0},
                      {0, 1, 0}, {0, 0, 1}});
mt19937_64 rng(1234567);
uniform_int_distribution<uint> distribution(0, mint::mod() - 1);
auto sample = [&] { return mint(distribution(rng)); };
auto pair_ids = linear_matroid_parity(m, sample, 2);
int count = linear_matroid_parity_size(m, sample, 2);
assert(count == 1);
// ペア0は列0,1、ペア1は列2,3。この入力の最大ペア数は1。
```

## 詳細なドキュメント

#### linear_matroid_parity

```cpp
template <class F, class Sample>
vc<int> linear_matroid_parity(
    const LinearMatroid<F>& matroid,
    const vc<pair<int,int>>& pairs,
    Sample&& sample, int trials = 1)

template <class F, class Sample>
vc<int> linear_matroid_parity(
    const LinearMatroid<F>& matroid,
    Sample&& sample, int trials = 1)
```

`sample()` は体の要素 `F::S` を返す関数。
`trials` 回それぞれ独立に階数評価と解の復元を行い、最も大きい解のペア番号を昇順で返す。
明示的な `pairs` を省略すると、列 $(0,1),(2,3),\ldots$ をペアとする。
明示的な `pairs` では、ペアに含まれない列も入力できる。
入力の `build` の状態は使わず、列ベクトルを読み取る。

返る集合の独立性は、縮約時の正確な零判定で保証する。
乱数の分布にかかわらず実行可能な集合が返り、乱択の失敗はペア数の過小評価として現れる。

##### 制約

- `F` と `S` は [LinearMatroid](linear_matroid.md) の条件を満たす。
- 各ペアの2つの列は異なり、各列は高々1つのペアに現れる。
- 明示的な `pairs` の各要素番号は有効な範囲にある。
- `pairs` を省略するとき、列数は偶数。
- `trials >= 1`。

##### 乱数と誤りの確率

各呼び出しで体の $q$ 個の要素から独立に一様に選ぶとする。
ペア数を $m$、入力全体の階数を $r$ とすると、1回の試行で最大ペア数を得られない確率は

$$
\min\left(1,\frac{(m+1)r}{q}\right)
$$

以下。独立な `trials` 回の試行では、この上界の `trials` 乗以下となる。
小さい標数の表現では、十分な大きさの拡大体へ係数を埋め込むと誤りの確率を小さくできる。

##### 計算量

元の次元を $d$、列数を $n$、ペア数を $m$、入力全体の階数を $r$ とする。
最初の行空間の圧縮に $O(dn\min(d,n))$ 時間を使う。
その後は `trials` を $t$ として $O(tmr^2(m+r))$ 時間。

#### linear_matroid_parity_size

```cpp
template <class F, class Sample>
int linear_matroid_parity_size(
    const LinearMatroid<F>& matroid,
    const vc<pair<int,int>>& pairs,
    Sample&& sample, int trials = 1)

template <class F, class Sample>
int linear_matroid_parity_size(
    const LinearMatroid<F>& matroid,
    Sample&& sample, int trials = 1)
```

最大ペア数を求める。復元を省き、`trials` 回の階数評価の半分の最大値を返す。
制約と乱数の条件は復元版と共通。返る値は常に真の最大ペア数以下となる。
誤りの確率の上界は $\min(1,r/q)^{trials}$。

##### 計算量

行空間の圧縮は復元版と共通。その後は $O(t r^2(m+r))$ 時間。

## 参考文献

- Lovász の階数公式の解説：[Cheung・Lau・Leung, Algebraic Algorithms for Linear Matroid Parity Problems, Theorem 4.1](https://cs.uwaterloo.ca/~lapchi/papers/parity.pdf)。
- 任意標数での Pfaffian による定式化と交代行列の扱い：[Iwata・Kobayashi, A Weighted Linear Matroid Parity Algorithm, Sections 2, 9](https://arxiv.org/pdf/1905.13371v1)。
