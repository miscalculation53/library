このドキュメントには AI による作成・変更が含まれています。

## 概要

体上の列ベクトルを互いに素なペアに分け、選んだペアに含まれるすべての列が線形独立になるよう、ペア数を最大化する乱択アルゴリズム。
`linear_matroid_parity_size` は最大ペア数、`linear_matroid_parity_restore` は選んだペア番号を返す。復元版の返す集合は常に実行可能であり、最大性には乱択による誤りの確率がある。

ペア $i$ の列を $a_i,b_i$ とすると、独立な変数を持つ交代行列

$$
\sum_i x_i\left(a_i b_i^{\mathsf T}-b_i a_i^{\mathsf T}\right)
$$

の階数の半分が最適なペア数に一致する。
乱数を代入して階数を求める。復元版は階数評価を繰り返す。
標数2でも対角成分を0に保つ交代行列を使う。
階数計算を繰り返す単純な実装で、入力の階数が小さい場合に適する。

階数と最適ペア数の関係は Lovász の階数公式による。

密な入力には `Matrix<F, BS>` または `LinearMatroid<F>` を渡す。行が座標、列が要素に対応し、`F` は入力から推論する。

`sample` は省略でき、[random_modint](../math/modint/random.md) で零を含む体の元から一様に選ぶ。`F::S` が modint の場合に使える。乱数エンジンは `mt`。

`pairs` の両端の要素番号には、それぞれ `i128`・`u128` を含む任意の整数型を使える。

## 使用例

```cpp
#include "matroid/linear_matroid_parity.hpp"

using F = FieldAddSubMulDiv<modint998244353>;
Matrix<F> m{{1, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 1}};
auto pair_ids = linear_matroid_parity_restore(m, 2);
int count = linear_matroid_parity_size(m, 2);
assert(count == 1);
// ペア0は列0,1、ペア1は列2,3。この入力の最大ペア数は1。
```

## 詳細なドキュメント

#### linear_matroid_parity_restore

```cpp
template <class F, int BS, class I = int, class J = int>
vc<int> linear_matroid_parity_restore(const Matrix<F, BS>& matroid,
    const vc<pair<I, J>>& pairs, int trials = 1);
template <class F, int BS>
vc<int> linear_matroid_parity_restore(const Matrix<F, BS>& matroid, int trials = 1);

template <class F, int BS, class Sample, class I = int, class J = int>
vc<int> linear_matroid_parity_restore(
    const Matrix<F, BS>& matroid,
    const vc<pair<I, J>>& pairs,
    Sample&& sample, int trials = 1);

template <class F, int BS, class Sample>
vc<int> linear_matroid_parity_restore(
    const Matrix<F, BS>& matroid,
    Sample&& sample, int trials = 1);
```

`sample()` を指定する場合は、体の要素 `F::S` に変換できる値を返す関数とする。mutable な関数オブジェクトや move-only な関数オブジェクトも渡せる。
`trials` 回それぞれ独立に階数評価と解の復元を行い、最も大きい解のペア番号を昇順で返す。
明示的な `pairs` を省略すると、列 $(0,1),(2,3),\ldots$ をペアとする。
明示的な `pairs` では、ペアに含まれない列も入力できる。
`LinearMatroid<F>` を渡す場合も、保持する全列を使う。

返る集合の独立性は、縮約時の正確な零判定で保証する。
乱数の分布にかかわらず実行可能な集合が返り、乱択の失敗はペア数の過小評価として現れる。

##### 制約

- `F` と `S` は [LinearMatroid](linear_matroid.md) の条件を満たす。
- 各ペアの2つの列は異なり、各列は高々1つのペアに現れる。
- 明示的な `pairs` の各要素番号は有効な範囲にある。
- `pairs` を省略するとき、列数は偶数。
- `trials >= 1`。
- `sample` を省略する場合、`F::S` は modint。

##### 乱数と誤りの確率

各呼び出しで体の $q$ 個の要素から独立に一様に選ぶとする。
ペア数を $m$、入力全体の階数を $r$ とすると、1回の試行で最大ペア数を得られない確率は

$$
\min\left(1,\frac{(m+1)r}{q}\right)
$$

以下。独立な `trials` 回の試行では、この上界の `trials` 乗以下となる。
小さい体では、十分な大きさの拡大体へ係数を埋め込むと誤りの確率を小さくできる。binary matroid の既定表現の $\mathbb F_2$ では $q=2$ であり、上の保証が自明になる場合がある。元の表現の標数を保つ拡大体を使い、必要なら `sample` を指定する。

##### 計算量

元の次元を $d$、列数を $n$、ペア数を $m$、入力全体の階数を $r$ とする。
最初の行空間の圧縮・ペア検査に $O(dn\min(d,n)+n+m)$ 時間を使う。
その後は `trials` を $t$ として $O(tmr^2(m+r))$ 時間。

#### linear_matroid_parity_size

```cpp
template <class F, int BS, class I = int, class J = int>
int linear_matroid_parity_size(const Matrix<F, BS>& matroid,
    const vc<pair<I, J>>& pairs, int trials = 1);
template <class F, int BS>
int linear_matroid_parity_size(const Matrix<F, BS>& matroid, int trials = 1);

template <class F, int BS, class Sample, class I = int, class J = int>
int linear_matroid_parity_size(
    const Matrix<F, BS>& matroid,
    const vc<pair<I, J>>& pairs,
    Sample&& sample, int trials = 1);

template <class F, int BS, class Sample>
int linear_matroid_parity_size(
    const Matrix<F, BS>& matroid,
    Sample&& sample, int trials = 1);
```

最大ペア数を求める。復元を省き、`trials` 回の階数評価の半分の最大値を返す。
制約と乱数の条件は復元版と共通。返る値は常に真の最大ペア数以下となる。
誤りの確率の上界は $\min(1,r/q)^{trials}$。

##### 計算量

行空間の圧縮は復元版と共通。その後は $O(t r^2(m+r))$ 時間。

## 参考文献

- Lovász の階数公式の解説：[Cheung・Lau・Leung, Algebraic Algorithms for Linear Matroid Parity Problems, Theorem 4.1](https://cs.uwaterloo.ca/~lapchi/papers/parity.pdf)。
- 任意標数での Pfaffian による定式化と交代行列の扱い：[Iwata・Kobayashi, A Weighted Linear Matroid Parity Algorithm, Sections 2, 9](https://arxiv.org/pdf/1905.13371v1)。

## 疎な線形表現

疎な入力には [SparseMatrix<F>](../math/linalg/sparse_matrix.md) を渡す。行が座標、列が台集合の要素。関数名・ペア指定・`sample`・`trials` と、返す番号の意味は共通。

```cpp
auto a = PartitionMatroid(group, capacity).sparse_linear_representation();
int count = linear_matroid_parity_size(a);
auto pair_ids = linear_matroid_parity_restore(a);
```

全て零の行を除き、入力の形状と各列の非零数から次の経路を自動選択する。選択基準は `SparseMatrix::rank`、`det`、線形マトロイド交叉と共通。

- 密評価：疎な $J$ と転置行列を使って $Q=AJA^{\mathsf T}$ を作り、密行列の rank を求める。元の線形表現は疎なまま保つ。
- BBLA：$Qv=A(J(A^{\mathsf T}v))$ と転置作用を渡して rank を求める。

$d$ を入力の行数、$n$ を列数、$m$ をペア数、$s$ を非零数、$c_e$ を列 $e$ の非零数、$t=\mathrm{trials}$ とする。次を定める。

$$
T=d+n+m+s,\qquad P=\sum_{(a,b)\in\mathrm{pairs}}c_a c_b.
$$

##### 計算量

両経路の rank 評価1回の時間は次の通り。

| 経路 | 時間 | 入力を含む領域 |
|---|---|---|
| 密評価 | $O(T+d^3+P)$ | $O(T+d^2)$ |
| BBLA | $O(dT)$ | $O(T)$ |

密行列の生成費用と rank 計算、BBLA の行列作用の費用を見積もって選ぶ。見積もりの定数倍の範囲で比較するため、

$$
R=\min(T+d^3+P,dT)
$$

を使い、サイズ計算と復元の計算量を次で表せる。零行の除去で実際の次元が小さくなった場合も、この上界は有効。

| 関数 | 前処理 | 前処理後の時間 |
|---|---|---|
| `linear_matroid_parity_size` | $O(T)$ | $O(tR)$ |
| `linear_matroid_parity_restore` | $O(T)$ | $O(tmR)$ |

復元は deletion self-reduction を使う。ペアの区間をまとめて削除できるか調べ、削除できなければ区間を二分する。削除は対角係数を零にするだけなので、元の疎な表現を保てる。最後の残存ペア数が目標サイズと一致した試行を採用する。

各 rank 判定が正確なら、最適ペア数を $k$ として、rank 評価は最初の1回と $O(k\log m)$ 回。従って、この場合の復元時間は、前処理後で $O(tR+tkR\log m)$。最終的に残る最適解を一つ固定すると、削除できない区間はその解と交わる。各段の区間は互いに素なので高々 $k$ 個となる。過小評価がある場合を含む最悪の呼出し回数は1試行 $O(m)$。

##### 乱数の条件

どちらの経路も、多項式評価の失敗で最適値を過小評価する場合がある。返した集合の実行可能性は保たれる。密評価は rank 自体を正確に計算し、BBLA は rank 推定の失敗も加わる。

密評価を選ぶ場合、$q$ 元体から独立な一様乱数を選ぶと、サイズ計算の1試行の失敗確率は $\min(1,d/q)$ 以下、復元は $\min(1,2md/q)$ 以下。独立な $t$ 試行では、それぞれ上界の $t$ 乗以下となる。BBLA を選ぶ場合は、この多項式評価に加えて [BBLA](../math/linalg/bbla.md) の条件・失敗も考慮する。

BBLA には十分大きい体と適切な sampler を使う。例えば $\mathbf F_2$ では、試行回数を増やしても BBLA が正しい rank を得られない例がある。binary matroid の独立性を保つには、標数2の十分大きい拡大体を使う。`trials` は処理全体を繰り返す回数で、生成済みの線形表現自体を作り直すものではない。

## ABC482 G への適用

[ABC482 G](https://atcoder.jp/contests/abc482/tasks/abc482_g) は辺の両端を分割マトロイドのペアにすればよい。個数だけを求める場合は `_size` を使う。$K=\sum_v A_v$ とすると、単純グラフなので $P\leq\sum_{uv\in E}A_u A_v\leq K^2/2$、表現の非零数は高々 $(N-1)K$。密評価で $O(K^3)$ 時間になる。

実装例は [検証コード](../../verify/atcoder/abc482_g_linear_matroid_parity.test.cpp)。
