## 行列

### コンストラクタ

```cpp
(1) Matrix<F, int BS = 32>(int n, int m, S diag = F::e0(), S non_diag = F::e0())
(2) Matrix<F, int BS = 32>(vvc<S> a)
```

`F` は体（一部の操作は環や半環でよい）、`BS` は行列積の際に用いるブロックサイズ。

- (1)：$n \times m$ の行列を作る。対角成分と非対角成分の初期値を指定できる。
- (2)：$a$ の要素を成分とする行列を作る。

### メンバ関数

#### shape

```cpp
pair<int, int> shape()
```

行列の (行数, 列数) を返す。 

#### 四則演算

- スカラーとの `*`, `/`：$O(nm)$
- ベクトル（`vc<S>`）との `*`：$O(nm)$
- 行列との `+`, `-`：$O(nm)$
- 行列との `*`：$O(nmk)$

#### pow

```cpp
Matrix<F, BS> pow(int k)
```

行列の $k$ 乗を返す。

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
pair<bool, Matrix<F, BS>> inv()
```

(逆行列が存在するか, 存在する場合逆行列) を返す。

##### 制約

- 行列は正方行列である

##### 計算量

- $O(n^3)$

#### solve

```cpp
tuple<bool, vc<F::S>, vvc<F::S>> solve(vc<F::S> b)
```

$A\boldsymbol{x} = \boldsymbol{b}$ の解を求め、

- 解が存在するかどうか
- 解が存在する場合の解のひとつ $\boldsymbol{x}_0$
- 解空間の基底 $\{\boldsymbol{c}_i\}_i$

を返す。すなわち、解は $\boldsymbol{x} = \boldsymbol{x}_0 + \sum_i \lambda_i \boldsymbol{c}_i$ の形で表されるベクトル全体。

##### 制約

- $\boldsymbol{b}$ は $n$ 次元ベクトル

##### 計算量

- $O(nm \min(n, m) + m^2)$

