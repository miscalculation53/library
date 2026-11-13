このドキュメントには AI による作成・変更が含まれています。

## 概要

独立性を判定する関数から、[共通 oracle](matroid.md)を作る。独立集合への追加と一要素ずつの交換を判定し、fundamental circuit を求める。

専用の oracle を用意する前の検証や、小さい入力に使える。

`build` の要素番号の配列には、`i128`・`u128` を含む任意の整数型を使える。返す要素番号は `int`。

## 使用例

```cpp
#include "matroid/independence_matroid.hpp"
#include "matroid/matroid_intersection.hpp"

auto independent = [](const vc<int>& s) { return s.size() <= 2; };
IndependenceMatroid m(5, independent);
auto selected = matroid_intersection(m, m); // 大きさ2
```

## 詳細なドキュメント

### IndependenceMatroid

#### コンストラクタ

```cpp
template<class Independent>
IndependenceMatroid<Independent>(int n, Independent independent);
```

台集合を `0..n-1` とし、独立性判定を値で保持する。初期状態は空集合。

##### 制約

- `n >= 0`。
- `independent` は `const vc<int>&` を受け取って真偽値を返し、`const` な状態でも呼べる。
- 判定がマトロイドの独立集合族を定め、配列の順序に依存しない。

##### 計算量

- $O(n)$。判定関数の移動・コピーに必要な時間は別。

#### size

```cpp
int size() const;
```

台集合の要素数を返す。

##### 計算量

- $O(1)$。

#### build

```cpp
template<class I = int>
void build(const vc<I>& independent);
```

現在の独立集合を置き換える。相異なる有効な要素からなる独立集合を渡す。以下、現在の大きさを $r$、独立性判定一回の時間を $T$ とする。

##### 計算量

- $O(n+T)$。検査を省略したビルドでは $O(n)$。

#### circuit

```cpp
optional<vc<int>> circuit(int e) const;
```

現在の独立集合の外にある有効な要素 `e` について、fundamental circuit から `e` を除いた要素列を返す。追加できる場合は `nullopt`、ループなら値ありの空配列。

##### 計算量

- $O(rT)$。
