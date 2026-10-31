このドキュメントには AI による作成・変更が含まれています。

## 概要

実行時に行数・列数を決める行列。成分を行優先の1本の `vector` に保持する。
`a(i, j)`、`a.at(i, j)`、`a[i][j]` で成分にアクセスできる。
四則演算と行列ベクトル積は環・半環、行基本変形と連立方程式は体上で使う。

## 使用例

```cpp
using mint = modint998244353;
Matrix<FieldAddSubMulDiv<mint>> a(vvc<mint>{{1, 1}, {1, 0}});
auto b = a.pow(5);
assert(b[0][1] == 5);
auto solution = a.solve(vc<mint>{3, 2});
assert(solution && solution->first[0] == 2 && solution->first[1] == 1);

vc<mint> v{1, 2};
assert((a * v == vc<mint>{3, 1}));
assert((v * a == vc<mint>{3, 1}));
```

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) Matrix<F, int BS = 32>(int n, int m, S diag = F::e0(), S non_diag = F::e0())
(2) Matrix<F, int BS = 32>(vvc<S> a)
```

`F` は体（一部の操作は環や半環でよい）、`BS` は行列積の際に用いるブロックサイズ。

- (1)：$n \times m$ の行列を作る。対角成分と非対角成分の初期値を指定できる。
- (2)：$a$ の要素を成分とする行列を作る。

#### shape

```cpp
template <class I = ll> pair<I, I> shape() const
```

行列の (行数, 列数) を返す。0行の行列も構築時の列数を保持する。

#### 成分・行・配列へのアクセス

```cpp
reference operator()(int i, int j)
reference at(int i, int j)
auto row(int i)
auto operator[](int i)
vvc<T> to_vvi<T = S>() const
const vc<S>& content() const
```

`operator()` と `at` は成分を返す。`at` は添字を `assert` で確認する。const版は読み取り用の参照を返す。`S=bool` は `vector<bool>` の参照型を使う。
`row(i)` と `operator[]` は元の成分を参照する行ビューを返す。要素アクセス、反復、同じ長さの行・配列の代入ができる。
行の反復には `for (auto row : a)` を使う。`swap_rows(i, j)` は2行の成分を交換する。
`to_vvi` は二次元配列へのコピー、`content` は行優先の保存配列への読み取り用参照。`vvc<S>` への変換もコピーを返す。`PRINTV(a.to_vvi())` で各行を出力できる。
行列の形は構築時に指定する。行を追加する操作は、新しい形の行列へ成分を移して行う。

##### 制約

- 成分の添字は行数・列数の範囲内。
- 行への代入は代入先と同じ長さ。

##### 計算量

- 成分・行ビュー・`content`：$O(1)$。
- 行の代入・`swap_rows`：$O(m)$。
- `to_vvi`：$O(nm)$。

#### 四則演算

- スカラーとの `*`, `/`：$O(nm)$
- 列ベクトル `a * v`、行ベクトル `v * a`（`vc<S>`）：$O(nm)$
- 行列との `+`, `-`：$O(nm)$
- 行列との `*`：$O(nmk)$

行列積は両辺の疎さを調べる。右辺が疎なら非零成分だけを列挙し、左辺が疎なら左辺の零を飛ばして各成分を一度ずつ走査する。通常の32 bit modintの密な積は、右辺を転置して剰余をまとめた内積を使う。小さい正方行列では、両辺が上三角なら三角形の範囲だけを計算する。
右辺の疎な経路の時間は、左辺の第 $t$ 列の非零数を $d_t$、右辺の第 $t$ 行の非零数を $e_t$ として $O(nm+mk+nk+\sum_t d_t e_t)$。ここでは右辺の列数を $k$ とする。

`a` が $n\times m$ 行列のとき、`a * v` は長さ $m$ の列ベクトルから長さ $n$ の列ベクトル、`v * a` は長さ $n$ の行ベクトルから長さ $m$ の行ベクトルを求める。ベクトルの向きは左右の位置で決まる。係数の積も式の順序に従い、`a * v` は `F::mul(a[i][j], v[j])`、`v * a` は `F::mul(v[i], a[i][j])` を使う。

##### 制約

- `a * v` は `v.size() == m`、`v * a` は `v.size() == n`

#### pow

```cpp
Matrix<F, BS> pow(int k)
```

行列の $k$ 乗を返す。

`F = FieldAddSubMulDiv<mint>` で通常の 32 bit modint を使い、法が素数の場合は、行列の構造と多項式による計算を使って行列積の回数を削減する。環・半環を指定した場合は二分累乗で計算する。

##### 制約

- $k \geq 0$
- 行列は正方行列である

##### 計算量

- $O(n^3 \log k)$

#### row_reduction

```cpp
tuple<Matrix<F, BS>, I, F::S> row_reduction(bool rref = false)
```

行基本変形を行う。

- `rref` が `true` のときは簡約行列にしたもの、`false` のときは階段行列にしたもの
- rank
- $n=m$ のときの行列式

の組を返す。

##### 制約

- `F` は体

##### 計算量

- $O(nm\min(n,m))$

#### rank

```cpp
I rank()
```

行列の rank を返す。

##### 計算量

- $O(nm\min(n,m))$

#### det

```cpp
F::S det()
```

行列の行列式を返す。

##### 制約

- 行列は正方行列である

##### 計算量

- $O(n^3)$
- mod が十分大きい modint で行列が疎行列であるときは、非零要素の個数を $k$ として $O(n^2 + nk)$

#### inv

```cpp
optional<Matrix<F, BS>> inv() const
```

逆行列を返す。存在しなければ `nullopt`。$0\times0$ 行列では値ありの空行列を返す。

##### 制約

- 行列は正方行列である

##### 計算量

- $O(n^3)$

#### solve

```cpp
optional<pair<vc<F::S>, vvc<F::S>>> solve(const vc<F::S>& b) const
```

$A\boldsymbol{x} = \boldsymbol{b}$ の解を求め、解のひとつ $\boldsymbol{x}_0$ と解空間の基底 $\lbrace\boldsymbol{c}_i\rbrace_i$ のペアを返す。解がなければ `nullopt`。すなわち、解は $\boldsymbol{x} = \boldsymbol{x}_0 + \sum_i \lambda_i \boldsymbol{c}_i$ の形で表されるベクトル全体。

##### 制約

- $\boldsymbol{b}$ は $n$ 次元ベクトル

##### 計算量

- $O(nm \min(n, m) + m^2)$


## 32 bit modint の積に関する補足

ベクトルとの積は各行の `dot_product<F>` で計算する。
行列積 $A\in S^{n\times m}$、$B\in S^{m\times k}$ では、
[`dot_product` の高速化対象](../dot_product.md)で $n,m,k\ge16$、
かつ $A$ の非零要素が半分以上のとき、$B$ の転置を保持して内積を計算する。
この経路では返り値に加えて $O(mk)$ の作業領域を使う。
`BS` は従来のブロック計算経路に対して適用される。


### 通常の 32 bit modint の掃き出しで使う作業領域

`F = FieldAddSubMulDiv<mint>` で、`mint` が `static_modint32` または `dynamic_modint32` のとき、行数・列数がともに 32 以上なら内部で 64 bit 整数の作業配列を使う。`row_reduction` の入力コピーに加えて、行数を $n$、列数を $m$ として約 $8nm$ byte と $O(n)$ の作業領域をヒープに確保する。`rank`・`det`・`inv`・`solve` からこの掃き出しを呼ぶ場合も同様（`inv`・`solve` では拡大行列の大きさで数える）。
