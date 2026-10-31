このドキュメントには AI による作成・変更が含まれています。

## 概要

$\mathbb F_2$ 上の固定サイズ行列。`MatrixMod2<N, M>` は `N` 行 `M` 列を `array<bitset<M>, N>` に保持する。`M` の省略時は `N` とする。
加減算は XOR、積と行基本変形はビット演算で処理する。実行時サイズ版は [MatrixMod2Dynamic](matrix_mod2_dynamic.md)。

## 使用例

```cpp
MatrixMod2<2> a(vc<string>{"11", "10"});
auto inverse = a.inv();
assert(inverse && a * *inverse == MatrixMod2<2>::e1());
auto solution = a.solve(bitset<2>(3));
assert(solution && solution->first[0] && !solution->first[1]);

MatrixMod2<3, 2> tall;
tall(0, 0) = 1;
tall.at(2, 1) = 1;
bitset<3> column = tall * bitset<2>(3);
bitset<2> row = bitset<3>(5) * tall;
assert(column.to_ulong() == 5 && row.to_ulong() == 3);
```

## 詳細なドキュメント

### MatrixMod2<N, M = N>

行数を $n=N$、列数を $m=M$、ワード幅を $w=64$、$W_x=\lceil x/w\rceil$ とする。
`Vn=bitset<N>`、`Vm=V=bitset<M>`。

#### コンストラクタ・変換

```cpp
MatrixMod2(int diag = 0, int non_diag = 0)
MatrixMod2(const array<bitset<M>, N>& a)
MatrixMod2(const vvc<T>& a)
MatrixMod2(const vc<string>& a)
vvc<T> to_vvi<T = ll>() const
vc<string> to_vstr() const
static M e0(), e1()
```

対角成分を `diag`、その他を `non_diag` として構築する。二次元配列・文字列からの構築は添字と成分を保つ。文字列の第 `j` 文字が第 `j` 列。
`e0` は零行列、`e1` は対角成分が1の行列。変換関数は成分のコピーを返す。

##### 制約

- `diag`、`non_diag`、配列の成分は0または1。
- 配列・文字列の形は $n\times m$。

##### 計算量

- 対角成分指定：$O(nW_m+\min(n,m))$。
- 配列・文字列との変換：$O(nm+nW_m)$。

#### 成分・行へのアクセス

```cpp
pair<I, I> shape<I = ll>() const
reference operator()(int i, int j)
reference at(int i, int j)
bool test(int i, int j) const
M& set(int i, int j, bool value = true)
M& flip(int i, int j)
bitset<M>& row(int i)
```

`shape` は行数・列数、`operator()` と `at` はビットへの参照を返す。const版は `bool`。`at` は添字を `assert` で確認する。
`row(i)`、`a[i]` は行への参照。`a[i][j]` や `a[i] ^= a[k]` も使える。

##### 制約

- $0\le i<n$、$0\le j<m$。

##### 計算量

- $O(1)$。

#### 演算・累乗

```cpp
M operator+(const M& b) const
M operator-(const M& b) const
MatrixMod2<N, K> operator*(const MatrixMod2<M, K>& b) const
bitset<N> operator*(const bitset<M>& v) const
bitset<M> operator*(const bitset<N>& v, const M& a)
M pow(T k) const
```

行列積・列ベクトル積・行ベクトル積の形はテンプレート引数から決まる。加減算と積の複合代入も使える。積の複合代入の右辺は `MatrixMod2<M, M>`。
`pow(0)` は単位行列を返す。

##### 制約

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

階段行列、階数、行列式を返す。`rref=true` は簡約階段行列。元の行列は保持する。

##### 制約

- `det` は $n=m$。

##### 計算量

- $O(nm+n\min(n,m)W_m)$。

#### inv・solve

```cpp
optional<M> inv() const
optional<pair<bitset<M>, vc<bitset<M>>>> solve(const bitset<N>& b) const
```

逆行列、および $Ax=b$ の `(解のひとつ, 解空間の基底)` を返す。存在しなければ `nullopt`。

##### 制約

- `inv` は $n=m$。

##### 計算量

- `inv`：$O(n^2+n^2W_n)$。
- `solve`：$O(nm+n\min(n,m)W_{m+1}+m^2)$。

## 内部計算

階数・逆行列の高速経路は、検証した GNU libstdc++ 15 の `bitset` 配置を仮定する。サイズとコピー特性の検査が配置検証を補助する。他の構成では標準の `bitset` 操作を使う。
