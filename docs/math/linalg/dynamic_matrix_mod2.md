## 概要

列数を実行時に決める $\mathbb{F}_2$ 上の行列。

各行を `DynamicBitset` として持ち、加減算、積、累乗、行基本変形、rank、行列式、逆行列、連立方程式を処理する。最大列数をテンプレート引数で指定する版は `MatrixMod2`。速度の優劣は操作と設定した最大列数に依存し、`DynamicMatrixMod2` は実際に必要な列数だけを保持する。

## 使用例

```cpp
DynamicMatrixMod2 a(vc<string>{"110", "011"});
DynamicMatrixMod2 b(vc<string>{"10", "01", "11"});
auto c = a * b;

DynamicBitset v(2);
v.set(0);
auto [ok, sol, basis] = a.solve(v);
```

行列を `vc<string>` から作る場合は、文字列の $j$ 文字目が第 $j$ 列に対応する。`solve` に渡す `DynamicBitset` は `test(i)` が方程式の第 $i$ 行に対応する。

## 詳細なドキュメント

### DynamicMatrixMod2

#### コンストラクタ

```cpp
DynamicMatrixMod2(int n, int m, int diag = 0, int non_diag = 0)
DynamicMatrixMod2(const vvc<T> &a)
DynamicMatrixMod2(const vc<string> &a)
```

第1形式は $n\times m$ 行列を作り、対角成分を `diag`、それ以外を `non_diag` にする。第2、第3形式は与えた二次元配列から作る。

##### 制約

- $n,m\geq 0$
- 各成分は $0$ または $1$
- 二次元配列の各行の長さは等しい

##### 計算量

- $O(nm/w)$（二次元配列から作る場合は $O(nm)$）

#### test / set / flip

```cpp
bool test(int i, int j)
DynamicMatrixMod2 &set(int i, int j, bool value = true)
DynamicMatrixMod2 &flip(int i, int j)
```

$(i,j)$ 成分を取得または変更する。

##### 計算量

- $O(1)$

#### to_vvi / to_vstr

```cpp
vvc<T> to_vvi<T>()
vc<string> to_vstr()
```

通常の二次元配列へ変換する。

##### 計算量

- $O(nm)$

#### 加減算

```cpp
a + b; a - b; a += b; a -= b;
```

同じ大きさの行列を加減算する。$\mathbb{F}_2$ 上では加算と減算は等しい。

##### 計算量

- $O(nm/w)$

#### 乗算

```cpp
a * b; a *= b;
```

行列積を計算する。

##### 計算量

- $O(nmk/w)$。左辺が $n\times m$、右辺が $m\times k$ の場合

#### pow

```cpp
DynamicMatrixMod2 pow(T k)
```

正方行列の $k$ 乗を返す。

##### 制約

- $k\geq 0$

##### 計算量

- $O(n^3\log k/w)$

#### row_reduction

```cpp
DynamicMatrixMod2 row_reduction(bool rref = false)
```

`rref` が `false` なら階段行列、`true` なら簡約階段行列を返す。

##### 計算量

- $O(nm\min(n,m)/w)$

#### rank

```cpp
I rank<I = ll>()
```

行列の rank を返す。

##### 計算量

- $O(nm\min(n,m)/w)$

#### det

```cpp
I det<I = ll>()
```

正方行列の行列式を返す。

##### 計算量

- $O(n^3/w)$

#### inv

```cpp
pair<bool, DynamicMatrixMod2> inv()
```

逆行列が存在しなければ `{false, {}}`、存在すれば `{true, inverse}` を返す。

##### 計算量

- $O(n^3/w)$

#### solve

```cpp
tuple<bool, DynamicBitset, vc<DynamicBitset>> solve(const DynamicBitset &b)
```

$Ax=b$ の解が存在しなければ `{false, {}, {}}` を返す。存在する場合は、解のひとつと解空間の基底を返す。

##### 計算量

- $O(nm\min(n,m)/w+m^2)$
