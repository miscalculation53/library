このドキュメントには AI による作成・変更が含まれています。

## 概要

$\mathbb F_2$ 上の行列を `bitset` の行で保持する。加減算は XOR、行演算はワード単位で進める。
`MatrixMod2<C>` の `C` は列の固定容量、`m` は実際に使う列数。

## 使用例

```cpp
MatrixMod2<64> a(vc<string>{"11", "10"});
assert(a.rank() == 2);
auto inverse = a.inv();
assert(inverse && (a * *inverse).to_vstr() == (vc<string>{"10", "01"}));
auto result = a.solve(bitset<2>(3));
assert(result && result->first[0] && !result->first[1]);
```

## 詳細なドキュメント

### MatrixMod2<C>

行数を $n$、論理列数を $m$、ワードのビット数を $w$ とする。`V=bitset<C>`。
各行の $m$ 列目以降のビットは $0$ に保つ。

#### コンストラクタ、変換

```cpp
(1) MatrixMod2(int n, int m, int diag = 0, int non_diag = 0)
(2) MatrixMod2(const vvc<T>& a)
(3) MatrixMod2(const vc<string>& a)
(4) vvc<T> to_vvi<T=ll>() const
(5) vc<string> to_vstr() const
```

(1) は対角成分とその他の成分を指定する。(2), (3) は $0,1$ の配列、または `'0','1'` の文字列から構築する。
(4), (5) は論理列数までの成分を配列・文字列に変換する。

##### 制約

- $n\ge0$、$0\le m\le C$。
- 入力の各行の長さが等しく、各成分は $0$ または $1$。

##### 計算量

- $O(nm+nC/w)$。

#### 四則演算、pow

```cpp
(1) M operator+(const M& b), operator-(const M& b)
(2) MatrixMod2<K> operator*(const MatrixMod2<K>& b) const
(3) M pow(T k) const
```

加減算、行列積、非負整数乗を返す。複合代入も使える。$k=0$ なら単位行列。

##### 制約

- 加減算は同じ形、積は左の列数と右の行数が一致する。
- `pow` は $n=m$、$k\ge0$。

##### 計算量

- (1)：$O(nC/w)$。
- (2)：右の固定列容量を $K$ として $O(nC/w+nmK/w)$。左の行のコピーとビット走査を含む。
- (3)：$O(n^2C\log k/w)$。

#### row_reduction, rank, det

```cpp
(1) M row_reduction(bool rref = false) const
(2) I rank<I=ll>() const
(3) int det() const
```

(1) は階段行列、`rref=true` なら簡約階段行列を返す。(2) は階数、(3) は行列式。
元の行列は保持する。`det` は正方行列で使う。

##### 計算量

- (1), (2)：$O(nm+n\min(n,m)C/w)$。
- (3)：$O(n^2+n^2C/w)$。

#### inv, solve

```cpp
(1) optional<M> inv() const
(2) optional<pair<V, vc<V>>> solve(const bitset<R>& b) const
```

(1) は正方行列の逆行列、(2) は $Ax=b$ の `(解のひとつ, 解空間の基底)`。
対象が存在しなければ `nullopt`。`solve` の右辺は先頭 $n$ ビットを使う。

##### 制約

- `inv` は $n=m$。
- `solve` は $n\le R$。

##### 計算量

- (1)：$O(n^2+n^2C/w)$。
- (2)：$O(nm+n\min(n,m)C/w+mC/w+m^2)$。

## 内部計算

高速経路は、検証した GNU libstdc++ 15 構成の `bitset` 配置を仮定する。型のサイズとコピー特性の検査は、その配置検証を補助する。高速経路の有効化条件を満たさない構成では `bitset` の行演算を使う。
