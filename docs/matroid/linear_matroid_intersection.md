このドキュメントには AI による作成・変更が含まれています。

## 概要

同じ体上で線形表現された2つのマトロイドについて、共通独立集合の要素数を最大化する乱択アルゴリズム。
各表現の列数と要素番号を揃えて使う。返る要素番号の集合は常に両方で独立であり、最大性には乱択による誤りの確率がある。

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

## 使用例

```cpp
#include "matroid/linear_matroid_intersection.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
LinearMatroid<F> a(2, {{1, 0}, {1, 0}, {0, 1}});
LinearMatroid<F> b(2, {{1, 0}, {0, 1}, {0, 1}});
mt19937_64 rng(1234567);
uniform_int_distribution<uint> distribution(0, mint::mod() - 1);
auto sample = [&] { return mint(distribution(rng)); };
auto ids = linear_matroid_intersection(a, b, sample, 2);
int count = linear_matroid_intersection_size(a, b, sample, 2);
assert(count == 2);
// この入力の最大要素数は2。
```

## 詳細なドキュメント

#### linear_matroid_intersection

```cpp
template <class F, class Sample>
vc<int> linear_matroid_intersection(
    const LinearMatroid<F>& first,
    const LinearMatroid<F>& second,
    Sample&& sample, int trials = 1)
```

`sample()` は体の要素 `F::S` を返す関数。
`trials` 回それぞれ独立に階数評価と解の復元を行い、最も大きい解の要素番号を昇順で返す。
入力の `build` の状態は使わず、列ベクトルを読み取る。

返る集合の独立性は、縮約時の正確な零判定で保証する。
乱数の分布にかかわらず実行可能な集合が返り、乱択の失敗は要素数の過小評価として現れる。

##### 制約

- 2つのマトロイドの要素数が等しく、同じ体を使う。
- `F` と `S` は [LinearMatroid](linear_matroid.md) の条件を満たす。
- `trials >= 1`。

##### 乱数と誤りの確率

各呼び出しで体の $q$ 個の要素から独立に一様に選ぶとする。
入力の階数を $r_A,r_B$、$r=\min(r_A,r_B)$ とすると、1回の試行で最大要素数を得られない確率は

$$
\min\left(1,\frac{(n+1)r}{q}\right)
$$

以下。独立な `trials` 回の試行では、この上界の `trials` 乗以下となる。
小さい標数の表現では、十分な大きさの拡大体へ係数を埋め込むと誤りの確率を小さくできる。
決定的な計算が必要なら、`LinearMatroid` を [matroid_intersection](matroid_intersection.md) に渡す。

##### 計算量

元の次元を $d_A,d_B$、要素数を $n$ とする。
最初の行空間の圧縮に

$$
O(d_A n\min(d_A,n)+d_B n\min(d_B,n))
$$

時間を使う。その後は `trials` を $t$ として

$$
O\left(t n r_A r_B\left(n+\min(r_A,r_B)\right)\right)
$$

時間。

#### linear_matroid_intersection_size

```cpp
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
