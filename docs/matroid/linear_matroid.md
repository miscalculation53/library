このドキュメントには AI による作成・変更が含まれています。

## 概要

体上の列ベクトルの線形独立性で定まるマトロイド。要素番号は列の番号に対応する。
独立集合を `build` で設定し、追加する列の fundamental circuit を `circuit` で求める。
共通の oracle 形式を使うため、[組合せ的なマトロイド交叉](matroid_intersection.md)にも渡せる。

係数型は既存の `Matrix<F>` と同じ体のインターフェースを使う。基底の管理と係数の復元には [LinearBasis](../math/linalg/linear_basis.md) を使う。
有限体や有理数など、零判定と四則演算を正確に行える型を使う。

## 使用例

```cpp
#include "matroid/linear_matroid.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
// 各要素を列ベクトルとして渡す。
LinearMatroid<F> m(2, {{1, 0}, {0, 1}, {1, 1}, {0, 0}});
m.build({0, 1});
assert(m.circuit(2) == optional<vc<int>>({0, 1}));
assert(m.circuit(3) == optional<vc<int>>(vc<int>{}));
m.build({0});
assert(!m.circuit(1));
```

## 詳細なドキュメント

### LinearMatroid

#### コンストラクタ

```cpp
LinearMatroid<F>()
LinearMatroid<F>(int dimension, const vvc<S>& columns)
LinearMatroid<F>(int dimension, vvc<S>&& columns)
template <int BS> explicit LinearMatroid<F>(const Matrix<F, BS>& matrix)
```

`S = F::S`。既定構築では次元も要素数も0になる。
`columns[e]` に要素 $e$ の列ベクトルを渡す。`Matrix` からの構築では各列を要素とする。
列配列を非constの右辺値で渡すと所有権を移し、それ以外ではコピーする。
0行の `Matrix` から構築すると列数を要素数として保持し、すべての要素がループになる。

##### 制約

- `dimension >= 0`、すべての列の長さが `dimension`。
- `F` は可換体であり、`e0`, `e1`, `add`, `minus`, `mul`, `inv` を持つ。
- `S` の等値比較が体の等値関係と一致する。

##### 計算量

次元を $d$、要素数を $n$ として、コピーや `Matrix` からの構築は $O(nd)$、列配列の所有権を移す構築は $O(n)$。

#### size / dimension / columns

```cpp
int size() const
int dimension() const
const vvc<S>& columns() const
```

要素数、列ベクトルの次元、列ベクトル全体への読み取り用参照を返す。

##### 計算量

$O(1)$。

#### build

```cpp
void build(const vc<int>& independent)
```

現在の独立集合を設定する。消去済みの基底と、各基底ベクトルの元の列による表現を保持する。

##### 制約

- 要素番号は有効な範囲にあり、重複せず、列ベクトルが線形独立。

##### 計算量

$r = \lvert independent\rvert$ として $O(n+dr^2)$。

#### circuit

```cpp
optional<vc<int>> circuit(int e) const
```

現在の独立集合を $I$ とする。$I+e$ が独立なら `nullopt` を返す。
従属なら fundamental circuit から $e$ を除いた要素番号を、`build` に渡した順序で返す。
ループに対しては値ありの空配列を返す。構築直後の $I$ は空集合。

##### 制約

- `e` は現在の独立集合に含まれない有効な要素番号。

##### 計算量

$O(dr)$。
