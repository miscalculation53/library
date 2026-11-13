このドキュメントには AI による作成・変更が含まれています。

## 概要

非零成分を行ごとの [CSR](../../ds/csr.md) に保存する疎行列。各行の列番号は昇順で、同じ位置の成分を加算し、零成分を除く。

ベクトルとの積、転置、行・列のスケーリング、疎行列・密行列との積を扱う。`rank`・`det` は、密行列へ展開する計算と既存の [BBLA](bbla.md) を入力の形状・非零数から自動選択する。`solve` は BBLA を使う。BBLA は有限体などの正確な体演算と、十分大きい集合からの独立な乱数を前提とする。

疎な因子の積が密になる場合にも、積の作用を合成して BBLA に渡せる。例えば $M=ADB^{\mathsf T}$ なら $Mv=A(D(B^{\mathsf T}v))$ と計算する。小さい密行列として積を保持したい場合は `multiply_dense` を使う。

次元が小さい行列や、掃き出しで疎性が保たれる行列では、`to_dense().rank()` などの既存の密行列計算が速い場合もある。保存形式と計算方法を用途に応じて選ぶ。

## 使用例

```cpp
#include "math/linalg/sparse_matrix.hpp"

using F = FieldAddSubMulDiv<modint998244353>;
SparseMatrix<F> a(2, 3, {{0, 0, 2}, {0, 2, 1}, {1, 1, 3}});
dump(a);
vc<modint998244353> v{1, 2, 3};
auto av = a * v;                         // {5, 6}
auto gram = a.multiply_dense(a.transpose()); // 2 x 2 の密行列
auto rank = a.rank<int>();
auto solution = a.solve(av);
assert(solution.solvable);
assert(a * solution.solution == av);
```

## 詳細なドキュメント

### SparseMatrix

```cpp
template<class F = FieldAddSubMulDiv<modint998244353>>
class SparseMatrix;
```

`S = F::S` は既定構築・コピー・代入できる型とする。基本演算には半環、スケーリングには可換な乗法、BBLA には体を使う。以下では行数を $n$、列数を $m$、非零成分数を $z$ とする。保存領域は $O(n+z)$。

#### コンストラクタ

```cpp
SparseMatrix();
SparseMatrix(int n, int m);
template<class I = int, class J = int>
SparseMatrix(int n, int m, const vc<tuple<I, J, S>>& entries);
SparseMatrix(int n, int m, initializer_list<tuple<int, int, S>> entries);
template<class I, bool Erasable>
SparseMatrix(int columns, const CSR<pair<I, S>, Erasable>& rows);
template<int BS>
explicit SparseMatrix(const Matrix<F, BS>& matrix);
```

既定構築は $0\times0$、次元のみの指定は零行列。`entries` は `(行, 列, 係数)` の列。CSR からの構築では `rows[i]` を第 $i$ 行の `(列, 係数)` の列とする。重複成分は加算する。削除可能 CSR では有効な成分を使う。

添字の整数型は行と列で独立に選べ、`i128`・`u128` も使える。

##### 制約

- $n,m\geq0$。
- 指定する行番号は $[0,n)$、列番号は $[0,m)$。

##### 計算量

- 零行列：$O(n)$。
- 三つ組・CSR：入力成分数を $t$、第 $i$ 行の入力成分数を $t_i$ として $O(n+t+\sum_i t_i\log t_i)$。各行が列番号順なら $O(n+t)$。
- 密行列から：$O(nm+n)$。

#### shape・size・nnz・成分の参照

```cpp
template<class I = ll> pair<I, I> shape() const;
template<class I = ll> I size() const;
template<class I = ll> I nnz() const;
auto operator[](int i) const;
const CSR<pair<int, S>>& rows() const;
S operator()(int i, int j) const;
```

`shape` は `(行数, 列数)`、`size` は行数、`nnz` は非零成分数を返す。`a[i]` は第 $i$ 行の `(列, 係数)` の読み取り用 range。`a(i,j)` は零成分も含めて係数を返す。

##### 計算量

- 係数取得は $O(\log m)$、ほかは $O(1)$。

#### content

```cpp
pair<pair<int, int>, vc<tuple<int, int, S>>> content() const;
```

`((行数, 列数), 非零成分の列)` を返す。各非零成分は `(行, 列, 係数)` の三つ組で、行番号・列番号順に並ぶ。`LOCAL` では `dump(a)` でこの形式を表示できる。マトロイドの `sparse_linear_representation()` が返す行列にも同様に使える。

##### 計算量

- $O(n+z)$。

#### to_dense・transpose

```cpp
Matrix<F> to_dense() const;
SparseMatrix transpose() const;
```

密行列への変換、転置行列の構築。

##### 計算量

- `to_dense`：$O(nm+n)$。
- `transpose`：$O(n+m+z)$。

#### select_rows

```cpp
template<class I = int>
SparseMatrix select_rows(const vc<I>& indices) const;
```

指定した行を入力順に取り出す。行番号の重複にも対応し、出力の列数は元と同じ。`I` は `i128`・`u128` を含む整数型。各番号は $[0,n)$ とする。

##### 計算量

- 指定行数を $k$、出力の非零数を $z^{\prime}$ として $O(k+z^{\prime})$。

#### ベクトルとの積

```cpp
vc<S> operator*(const vc<S>& v) const;
friend vc<S> operator*(const vc<S>& v, const SparseMatrix& a);
```

それぞれ $Av$、$v^{\mathsf T}A$ を返す。入力ベクトルの長さはそれぞれ $m$、$n$。

##### 計算量

- $O(n+m+z)$。

#### スケーリング

```cpp
SparseMatrix& scale_rows(const vc<S>& scale);
SparseMatrix& scale_columns(const vc<S>& scale);
SparseMatrix& operator*=(const S& scale);
SparseMatrix operator*(const S& scale) const;
```

指定した係数を各行・各列・全成分に掛ける。`scale_rows`・`scale_columns` の配列長はそれぞれ $n$、$m$。零になった成分は除く。

##### 計算量

- $O(n+z)$。

#### 行列との積

```cpp
template<int BS> Matrix<F, BS> operator*(const Matrix<F, BS>& b) const;
template<int BS> friend Matrix<F, BS> operator*(const Matrix<F, BS>& a, const SparseMatrix& b);
SparseMatrix operator*(const SparseMatrix& b) const;
Matrix<F> multiply_dense(const SparseMatrix& b) const;
```

通常の行列積。積の内側の次元は等しくする。密行列との積は密行列を返す。疎行列同士では `operator*` が疎行列、`multiply_dense` が密行列を返す。

$A$ を $n\times m$、$B$ を $m\times p$ とする。疎行列の非零数を $z_A,z_B$、$W=\sum_k\mathrm{nnz}(A_{:,k})\mathrm{nnz}(B_{k,:})$ とする。

##### 計算量

- 疎行列 $A$ × 密行列 $B$：$O(np+pz_A)$。
- 密行列 $A$ × 疎行列 $B$：$O(nm+np+nz_B)$。
- 疎行列同士の `multiply_dense`：$O(np+n+z_A+W)$。
- 疎行列同士の `operator*`：積の非零数を $z_C$ として $O(n+p+z_A+W+z_C\log p)$。

#### rank・det・solve

```cpp
template<class I = ll, class Sample = decltype(bbla::random_sample_mint<S>) &>
I rank(int trials = 2, Sample&& sample = bbla::random_sample_mint<S>) const;
template<class Sample = decltype(bbla::random_sample_mint<S>) &>
S det(Sample&& sample = bbla::random_sample_mint<S>) const;
template<class Sample = decltype(bbla::random_sample_mint<S>) &>
bbla::LinearSolution<S> solve(const vc<S>& b, Sample&& sample = bbla::random_sample_mint<S>) const;
```

`rank`・`det` は、密行列の生成・掃き出しと BBLA の行列作用の費用を見積もって経路を選ぶ。`solve` は行列作用と転置作用を BBLA に渡す。`sample` の既定値は modint 用の非零一様乱数。独自の体では適切な sampler を指定する。mutable な sampler・move-only な sampler も参照して使う。

密評価では rank を正確に返す。BBLA による `rank` は Monte Carlo 法で、失敗時には過小評価し、`trials` 回の最大を返す。`det` はどちらの経路でも返り値が正しく、BBLA を選んだ場合は Las Vegas 法となる。密評価では `sample` を呼ばない。

`solve` の `b` の長さは $n$。解があれば `solvable == true` と解ベクトル、解がなければ `solvable == false` と証明ベクトル $y$ を返す。証明は $A^{\mathsf T}y=0$ かつ $y^{\mathsf T}b\ne0$ を満たす。返す解・証明は検算済み。零ベクトルも有効な解になる。

BBLA を使う場合、小さい体では確率保証が弱くなり、`det`・`solve` の再試行が進まない場合がある。乱数と返り値の詳細な条件は [BBLA](bbla.md) を参照する。

##### 制約

- 係数演算が体をなす。
- `rank` の `trials > 0`。
- `det` は $n=m$。
- BBLA を使う場合、`sample` は十分大きい集合から独立に体の元を返し、非零元を正の確率で返す。

##### 計算量

$d=\min(n,m)$ とする。入力の保存に加え、密評価の領域は $O(nm)$、BBLA の領域は $O(n+m)$。

- `rank`：$O(\min(nmd,\mathrm{trials}\cdot d(n+m+z)))$。密評価は一度だけ計算する。
- `det`：密評価は $O(n^3)$、BBLA は1試行 $O(n^2+nz)$。
- `solve`：1試行 $O(d(n+m+z))$。
