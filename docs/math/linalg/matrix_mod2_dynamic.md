このドキュメントには AI による作成・変更が含まれています。

## 概要

$\mathbb F_2$ 上の実行時サイズ行列。`MatrixMod2Dynamic` は各行を64ビットのワードに分け、全行のワード列を1本の `vector` に保持する。
加減算、行列・ベクトル積、累乗、行基本変形、階数、行列式、逆行列、連立方程式を扱う。固定サイズ版は [MatrixMod2](matrix_mod2.md)。

## 使用例

```cpp
MatrixMod2Dynamic a(vc<string>{"110", "011"});
MatrixMod2Dynamic b(vc<string>{"10", "01", "11"});
assert((a * b).to_vstr() == (vc<string>{"11", "10"}));
a(0, 2) = true;
a.row(1).reset();

MatrixMod2Dynamic identity(65, 65, 1);
auto inverse = identity.inv();
assert(inverse && *inverse == identity);
auto solution = identity.solve(DynamicBitset(65));
assert(solution && solution->second.empty());
```

## 詳細なドキュメント

### MatrixMod2Dynamic

行数を $n$、列数を $m$、ワード幅を $w=64$、$W_x=\lceil x/w\rceil$ とする。`V=DynamicBitset`。

#### コンストラクタ・変換

```cpp
MatrixMod2Dynamic()
MatrixMod2Dynamic(int n, int m, int diag = 0, int non_diag = 0)
MatrixMod2Dynamic(const vvc<T>& a)
MatrixMod2Dynamic(const vc<string>& a)
vvc<T> to_vvi<T = ll>() const
vc<string> to_vstr() const
pair<I, I> shape<I = ll>() const
size_t size() const
bool empty() const
```

既定構築は0行0列。寸法指定では対角成分を `diag`、その他を `non_diag` とする。
文字列の第 `j` 文字が第 `j` 列。`shape` は行数・列数、`size` は行数、`empty` は行数が0かを返す。0行の行列も列数を保持する。

##### 制約

- $n,m\ge0$。
- `diag`、`non_diag`、配列の成分は0または1。
- 配列の各行の長さが等しい。

##### 計算量

- 寸法指定：$O(nW_m+\min(n,m))$。
- 配列・文字列との変換：$O(nm+nW_m)$。
- `shape`、`size`、`empty`：$O(1)$。

#### 成分・行へのアクセス

```cpp
reference operator()(int i, int j)
reference at(int i, int j)
bool test(int i, int j) const
M& set(int i, int j, bool value = true)
M& flip(int i, int j)
auto row(int i)
auto operator[](int i)
void swap_rows(int i, int j)
```

`operator()` と `at` はビットへの参照を返す。const版は `bool`。`at` は添字を `assert` で確認する。
行は元のワード列を参照するビュー。`a[i][j]`、行への同じ長さの `DynamicBitset`・行ビューの代入、行の XOR も使える。
行ビューは `size`、`test`、`set`、`reset`、`flip`、`any`、`none`、`all`、`count`、`set_range`、`reset_range`、`flip_range`、`find_first`、`find_next` に対応する。検索結果は整数の位置で、右番兵は列数 `m`。
`DynamicBitset` への変換はコピーを返す。行の反復には `for (auto row : a)` を使う。行列の形は構築時に指定する。

##### 制約

- 成分・行の添字は有効範囲内。
- 行の代入・XORは同じ長さ。

##### 計算量

- 成分アクセス・行ビュー取得：$O(1)$。
- 行のコピー・XOR・交換・全ビット操作・集計：$O(W_m)$。
- 区間操作：区間長を $l$ として $O(1+l/w)$。
- ビット検索：$O(W_m)$。

#### 演算・累乗

```cpp
M operator+(const M& b) const
M operator-(const M& b) const
M operator*(const M& b) const
DynamicBitset operator*(const DynamicBitset& v) const
DynamicBitset operator*(const DynamicBitset& v, const M& a)
M pow(T k) const
```

加減算・行列積・列ベクトル積・行ベクトル積・非負整数乗。複合代入も使える。
列ベクトル積の戻り値の長さは行数、行ベクトル積の戻り値の長さは列数。

##### 制約

- 加減算は同じ形。行列積は左辺の列数と右辺の行数が一致する。
- 列ベクトルの長さは列数、行ベクトルの長さは行数。
- `pow` は $n=m$、$k\ge0$。

##### 計算量

- 加減算：$O(nW_m)$。
- 行列積：左辺の非零数を $z$、右辺の列数を $p$ として $O(nW_m+nW_p+zW_p)$。
- 列ベクトル積：$O(nW_m)$。
- 行ベクトル積：ベクトルの1の個数を $z$ として $O(W_n+W_m+zW_m)$。
- 累乗：$O(n^2W_n\log k)$。

#### row_reduction・rank・det

```cpp
M row_reduction(bool rref = false) const
I rank<I = ll>() const
I det<I = ll>() const
```

階段行列、階数、行列式。`rref=true` は簡約階段行列。元の行列は保持する。

##### 制約

- `det` は $n=m$。

##### 計算量

- $O(nm+n\min(n,m)W_m)$。

#### inv・solve

```cpp
optional<M> inv() const
optional<pair<DynamicBitset, vc<DynamicBitset>>> solve(const DynamicBitset& b) const
```

逆行列、および $Ax=b$ の `(解のひとつ, 解空間の基底)`。存在しなければ `nullopt`。

##### 制約

- `inv` は $n=m$。
- `solve` は `b.size() == n`。

##### 計算量

- `inv`：$O(n^2+n^2W_n)$。
- `solve`：$O(nm+n\min(n,m)W_{m+1}+m^2)$。
