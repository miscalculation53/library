このドキュメントには AI による作成・変更が含まれています。

## 概要

同じ体上で線形表現された2つのマトロイドについて、共通独立集合の要素数を最大化する乱択アルゴリズム。
各表現の列数と要素番号を揃えて使う。`linear_matroid_intersection_size` は最大要素数、`linear_matroid_intersection_restore` は要素番号の集合を返す。返る集合は常に両方で独立であり、最大性には乱択による誤りの確率がある。

列行列を $A,B$ とすると、独立な変数を持つ行列

$$
A\operatorname{diag}(x_0,\ldots,x_{n-1})B^{\mathsf T}
$$

の階数が最大共通独立集合の要素数に一致する。
乱数を代入して階数を求め、要素を両方の表現で縮約できるか確かめながら解を復元する。
階数計算を繰り返す単純な実装で、入力の階数が小さい場合に適する。

Cauchy–Binet の公式で各小行列式を展開すると、同じ列集合を選んだ $A,B$ の小行列式の積が係数になる。
異なる列集合は異なる単項式に対応するため、$k$ 次の非零な小行列式が存在することと、大きさ $k$ の共通独立集合が存在することが同値になる。

適用例の参考として、分割・グラフ的マトロイドを扱う [ABC399 G 公式解説](https://atcoder.jp/contests/abc399/editorial/12546?lang=ja) がある。

`F` は入力の線形表現から推論する。`sample` を省略すると、[random_modint](../math/modint/random.md) で零を含む体の元から一様に選ぶ。既定の `LinearMatroid<>` は `modint998244353` を使う。その他の体も、その体のまま計算する。

## 使用例

```cpp
#include "matroid/linear_matroid_intersection.hpp"

LinearMatroid<> a(2, {{1, 0}, {1, 0}, {0, 1}});
LinearMatroid<> b(2, {{1, 0}, {0, 1}, {0, 1}});
auto ids = linear_matroid_intersection_restore(a, b, 2);
int count = linear_matroid_intersection_size(a, b, 2);
assert(count == 2);
// この入力の最大要素数は2。
```

## 詳細なドキュメント

#### linear_matroid_intersection_restore

```cpp
template<class F>
vc<int> linear_matroid_intersection_restore(const LinearMatroid<F>& first,
    const LinearMatroid<F>& second, int trials = 1);

template <class F, class Sample>
vc<int> linear_matroid_intersection_restore(
    const LinearMatroid<F>& first,
    const LinearMatroid<F>& second,
    Sample&& sample, int trials = 1)
```

`sample()` を指定する場合は、体の要素 `F::S` に変換できる値を返す関数とする。
`trials` 回それぞれ独立に階数評価と解の復元を行い、最も大きい解の要素番号を昇順で返す。
入力の `build` の状態は使わず、列ベクトルを読み取る。

返る集合の独立性は、縮約時の正確な零判定で保証する。
乱数の分布にかかわらず実行可能な集合が返り、乱択の失敗は要素数の過小評価として現れる。

##### 制約

- 2つのマトロイドの要素数が等しく、同じ体を使う。
- `F` と `S` は [LinearMatroid](linear_matroid.md) の条件を満たす。
- `trials >= 1`。
- `sample` を省略する場合、`F::S` は modint。

##### 乱数と誤りの確率

各呼び出しで体の $q$ 個の要素から独立に一様に選ぶとする。
入力の階数を $r_A,r_B$、$r=\min(r_A,r_B)$ とすると、1回の試行で最大要素数を得られない確率は

$$
\min\left(1,\frac{(n+1)r}{q}\right)
$$

以下。独立な `trials` 回の試行では、この上界の `trials` 乗以下となる。
小さい体では上の保証が自明になる場合がある。元の表現の標数を保つ十分な大きさの拡大体へ係数を埋め込むと、誤りの確率を小さくできる。
決定的な計算が必要なら、`LinearMatroid` を [matroid_intersection](matroid_intersection.md) に渡す。

##### 計算量

元の次元を $d_A,d_B$、要素数を $n$ とする。
最初の行空間の圧縮に

$$
O(d_A n\min(d_A,n)+d_B n\min(d_B,n)+n)
$$

時間を使う。その後は `trials` を $t$ として

$$
O\left(t n r_A r_B\left(n+\min(r_A,r_B)\right)\right)
$$

時間。

#### linear_matroid_intersection_size

```cpp
template<class F>
int linear_matroid_intersection_size(const LinearMatroid<F>& first,
    const LinearMatroid<F>& second, int trials = 1);

template <class F, class Sample>
int linear_matroid_intersection_size(
    const LinearMatroid<F>& first,
    const LinearMatroid<F>& second,
    Sample&& sample, int trials = 1)
```

最大共通独立集合の要素数を求める。復元を省き、`trials` 回の階数評価の最大値を返す。
制約と乱数の条件は復元版と共通。返る値は常に真の最大要素数以下となる。
誤りの確率の上界は $\min(1,r/q)^{trials}$。

##### 計算量

行空間の圧縮は復元版と共通。その後は $O(t r_A r_B(n+\min(r_A,r_B)))$ 時間。

## 疎な線形表現

`LinearMatroid<F>` の代わりに [SparseMatrix<F>](../math/linalg/sparse_matrix.md) を渡せる。行が座標、列が台集合の要素。関数名・`sample`・`trials` と、返す番号の意味は共通。

全て零の行を除き、形状と対応する両列の非零数から次の経路を自動選択する。選択基準は `SparseMatrix::rank`、`det`、線形マトロイド parity と共通。

- 密評価：疎な因子から $ADB^{\mathsf T}$ を密行列として作り、その rank を求める。
- BBLA：$ADB^{\mathsf T}$ の作用と転置作用を合成して渡す。

$A,B$ の行数を $d_A,d_B$、共通の列数を $n$、非零数の和を $s$、列 $e$ の非零数を $a_e,b_e$、$t=\mathrm{trials}$ とする。次を定める。

$$
T=d_A+d_B+n+s,\qquad h=\min(d_A,d_B),\qquad P=\sum_e a_e b_e.
$$

##### 計算量

| 経路 | rank 評価1回の時間 | 入力を含む領域 |
|---|---|---|
| 密評価 | $O(T+d_A d_B h+P)$ | $O(T+d_A d_B)$ |
| BBLA | $O(hT)$ | $O(T)$ |

両経路の費用の見積もりを定数倍の範囲で比較する。

$$
R=\min(T+d_A d_B h+P,hT)
$$

を使い、サイズ計算と復元の計算量を次で表せる。零行の除去で実際の次元が小さくなった場合も、この上界は有効。

| 関数 | 前処理 | 前処理後の時間 |
|---|---|---|
| `linear_matroid_intersection_size` | $O(T)$ | $O(tR)$ |
| `linear_matroid_intersection_restore` | $O(T)$ | $O(tnR)$ |

復元は deletion self-reduction を使う。要素の区間をまとめて削除できるか調べ、削除できなければ区間を二分する。削除は対角係数を零にするだけなので、元の疎な表現を保てる。最後の残存要素数が目標サイズと一致した試行を採用する。

各 rank 判定が正確なら、最適サイズを $k$ として、rank 評価は最初の1回と $O(k\log n)$ 回。この場合の復元時間は、前処理後で $O(tR+tkR\log n)$。最終的に残る最適解を一つ固定すると、削除できない区間はその解と交わる。各段の区間は互いに素なので高々 $k$ 個となる。過小評価を含む最悪の呼出し回数は1試行 $O(n)$。

##### 乱数の条件

どちらの経路も多項式評価の失敗で過小評価する場合がある。返した集合の実行可能性は保たれる。密評価は rank 自体を正確に求め、BBLA は rank 推定の失敗も加わる。

密評価の場合、$q$ 元体から独立な一様乱数を選ぶと、サイズ計算の1試行の失敗確率は $\min(1,h/q)$ 以下、復元は $\min(1,2nh/q)$ 以下。独立な $t$ 試行では、それぞれ上界の $t$ 乗以下となる。BBLA の場合は、これに [BBLA](../math/linalg/bbla.md) の条件・失敗も加わる。

BBLA には十分大きい体と適切な sampler を使う。例えば $\mathbf F_2$ では、試行回数を増やしても正しい rank を得られない例がある。binary matroid の独立性を保つには、標数2の十分大きい拡大体を使う。`trials` は処理全体を繰り返す回数で、入力表現の再生成は含まない。
