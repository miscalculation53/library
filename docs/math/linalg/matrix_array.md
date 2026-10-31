このドキュメントには AI による作成・変更が含まれています。

## 概要

行数・列数をコンパイル時に固定する行列。`array` に格納し、小さい行列の積やセグメント木の要素として使う。
`F` は要素型 `F::S` と演算を定める。基本の演算は [Matrix](matrix.md) と共通。

成分は `a(i, j)`、`a.at(i, j)`、`a[i][j]`、行は `a.row(i)` で参照できる。`shape<I=ll>()` は固定された行数・列数を返す。
行列積は動的サイズ版と同じ処理を共有し、両辺の疎さと小さい上三角行列を利用する。

## 使用例

```cpp
using mint = modint998244353;
using M = MatrixArray<FieldAddSubMulDiv<mint>, 2, 2>;
M a;
a[0] = {1, 1};
a[1] = {1, 0};
assert(a.pow(5)[0][1] == 5);
auto result = a.solve(array<mint, 2>{3, 2});
assert(result && result->first[0] == 2 && result->first[1] == 1);

array<mint, 2> v{1, 2};
assert((a * v == array<mint, 2>{3, 1}));
assert((v * a == array<mint, 2>{3, 1}));
```

## 詳細なドキュメント

### MatrixArray<F, n, m, BS = 32>

`S=F::S`、`Vm=array<S,m>`、`Vn=array<S,n>` とする。行 `a[i]` は `array<S,m>`。

#### コンストラクタ、零行列・単位行列

```cpp
(1) MatrixArray(S diag = F::e0(), S non_diag = F::e0())
(2) MatrixArray(const array<array<S, m>, n>& a)
(3) static MatrixArray e0()
(4) static MatrixArray e1()
```

(1) は対角成分を `diag`、それ以外を `non_diag` で初期化する。(2) は要素をコピーする。
`e0` は零行列、`e1` は対角成分が乗法単位元の行列。

##### 制約

- $n,m\ge0$、$BS>0$。
- `F` は利用する演算に応じた半環・環・体の演算を持つ。

##### 計算量

- $O(nm)$。

#### 四則演算、行列とベクトルの積

```cpp
(1) M operator+(const M& b), operator-(const M& b)
(2) M operator*(S x), operator/(S x)
(3) Vn operator*(const Vm& v)
(4) friend Vm operator*(const Vn& v, const M& a)
(5) MatrixArray<F, n, p> operator*(const MatrixArray<F, m, p>& b)
```

加減算、スカラー倍・除算、行列とベクトルまたは行列の積を返す。複合代入も使える。
スカラー除算には `F::inv`、減算には加法逆元が必要。

`a * v` は長さ $m$ の列ベクトルから長さ $n$ の列ベクトル、`v * a` は長さ $n$ の行ベクトルから長さ $m$ の行ベクトルを求める。ベクトルの向きは左右の位置で決まる。係数の積も式の順序に従い、`a * v` は `F::mul(a[i][j], v[j])`、`v * a` は `F::mul(v[i], a[i][j])` を使う。

##### 計算量

- (1), (2), (3), (4)：$O(nm)$。
- (5)：$O(nmp)$。

#### pow

```cpp
M pow(T k) const
```

正方行列の $k$ 乗を返す。$k=0$ なら単位行列。

##### 制約

- $n=m$、$k\ge0$。

##### 計算量

- $O(n^3\log k)$。構造を利用する経路では [Matrix](matrix.md#pow) と同じ。

#### row_reduction, rank, det

```cpp
(1) tuple<M, I, S> row_reduction<I=ll>(bool rref = false) const
(2) I rank<I=ll>() const
(3) S det() const
```

(1) は `(階段行列, 階数, 行列式)`。`rref=true` なら簡約階段行列にする。
行列式は正方行列の場合に使う。(2) は階数、(3) は行列式。

##### 制約

- `F` は体。
- `det` は $n=m$。

##### 計算量

- $O(nm\min(n,m))$。

#### inv, solve

```cpp
(1) optional<M> inv() const
(2) optional<pair<Vm, vc<Vm>>> solve(const Vn& b) const
```

(1) は逆行列、(2) は $Ax=b$ の `(解のひとつ, 解空間の基底)`。
対象が存在すれば値あり、存在しなければ `nullopt` を返す。

##### 制約

- `F` は体。
- `inv` は $n=m$。

##### 計算量

- (1)：$O(n^3)$。
- (2)：$O(nm\min(n,m)+m^2)$。

## 32 bit modint の積に関する補足

`Matrix` と同じ条件で `dot_product<F>` を使う。
$n\times m$ 行列と $m\times k$ 行列の密な積では、
返り値に加えて転置用の $O(mk)$ の作業領域をヒープに確保する。
切替条件は [Matrix](matrix.md) を参照。


### 通常の 32 bit modint の掃き出しで使う作業領域

`F = FieldAddSubMulDiv<mint>` で、`mint` が `static_modint32` または `dynamic_modint32` のとき、行数・列数がともに 32 以上なら内部で 64 bit 整数の作業配列を使う。`row_reduction` の入力コピーに加えて、約 $8nm$ byte と $O(n)$ の作業領域をヒープに確保する。`rank`・`det`・`inv`・`solve` からこの掃き出しを呼ぶ場合も同様（`inv`・`solve` では拡大行列の大きさで数える）。


### 累乗

`pow(k)` は [Matrix の累乗](matrix.md#pow) と同じ条件で、行列の構造と多項式を使った計算を選ぶ。指数が 0 の場合は、固定した行列サイズ全体の単位行列を返す。
