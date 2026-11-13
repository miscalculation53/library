このドキュメントには AI による作成・変更が含まれています。

## 概要

無向グラフの辺を台集合とし、各連結成分が高々 $1$ 個の閉路を持つ辺集合を独立とする bicircular matroid。独立集合は pseudoforest とも呼ばれる。自己ループ・多重辺に対応する。要素番号は入力の辺の添字。

追加で従属になる成分から葉を順に除き、唯一の fundamental circuit を取り出す。閉路間を結ぶパスも circuit の一部として扱う。

連結成分の管理には [`UnionFind`](../ds/uf/uf.md)、葉除去の隣接関係には [`GraphUndirected`](../graph/graph.md) を使う。自己ループは次数 $2$ として扱う。

線形表現は、このヘッダの `linear_representation()` メンバから取得できる。乱数生成関数は省略できる。既定の係数体は `modint998244353`。引数・条件は [線形表現](linear_representation.md) を参照。

`build` の要素番号の配列には、`i128`・`u128` を含む任意の整数型を使える。返す要素番号は `int`。

## 使用例

```cpp
BicircularMatroid m(2, {{0, 0}, {1, 1}, {0, 1}});
m.build({0, 1});
auto c = m.circuit(2); // 0, 1 を含む列
m.build({0});
assert(!m.circuit(2)); // 片方の成分が木なら結合できる
```

## 詳細なドキュメント

### BicircularMatroid

#### コンストラクタ

```cpp
template<class I = int, class J = int>
BicircularMatroid(int vertices, const vc<pair<I, J>>& edges);
```

頂点数と辺列を指定して構築する。両端の頂点番号にはそれぞれ任意の整数型を使える。初期状態の独立集合は空集合。以下、頂点数を $v$、辺数を $n$ とする。

##### 制約

- `vertices >= 0`。
- 各辺の両端は `0..vertices-1`。

##### 計算量

- $O(v+n)$。

#### size

```cpp
int size() const;
```

台集合の要素数、すなわち辺数を返す。

##### 計算量

- $O(1)$。

#### build

```cpp
template<class I = int>
void build(const vc<I>& independent);
```

現在の独立集合 $I$ を置き換え、各連結成分の閉路の有無を記録する。

##### 制約

- 各要素は有効な辺番号で、互いに異なる。
- 各連結成分の辺数が頂点数以下。

##### 計算量

- $O(v+n+\lvert I\rvert\alpha(v))$。

#### circuit

```cpp
optional<vc<int>> circuit(int e) const;
```

$I+e$ が pseudoforest なら `nullopt`、従属なら fundamental circuit から `e` を除いた辺番号列を返す。返す列の順序は未規定。

##### 制約

- `e` は有効な辺番号で、$I$ の外にある。

##### 計算量

- $O(v+\lvert I\rvert)$。

#### sparse_linear_representation

`linear_representation` と同じ引数・係数体で、行を座標、列を要素とする `SparseMatrix<F>` を返す。構築の条件と計算量は [線形表現](linear_representation.md) を参照する。
