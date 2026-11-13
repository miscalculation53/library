このドキュメントには AI による作成・変更が含まれています。

## 概要

無向グラフと $0/1$ 彩色で二部グラフを与え、色 $0$ の頂点を台集合とする。選択した頂点すべてを色 $1$ の頂点へマッチできる集合を独立とする transversal matroid。

独立集合を覆うマッチングを保持し、追加したい左頂点から交互路を探索して fundamental circuit を求める。候補ごとに割当先を持つ要素の選択に使える。

マッチングと交互路探索には [BipartiteMatchingState](../flow/bipartite_matching_state.md) を使う。[DM 分解](../flow/dm_decomposition.md) とも交互グラフを共有する。DM 分解は最大マッチングに対する全体構造を扱い、この oracle は $I+e$ に対応する一つの未対応左頂点からの到達可能性を使う。探索に失敗した場合の到達済み左頂点から `e` を除いたものが `circuit(e)` の返却列になる。

線形表現は、このヘッダの `linear_representation()` メンバから取得できる。乱数生成関数は省略できる。既定の係数体は `modint998244353`。引数・条件は [線形表現](linear_representation.md) を参照。

`build` の要素番号の配列には、`i128`・`u128` を含む任意の整数型を使える。返す要素番号は `int`。

## 使用例

```cpp
#include "matroid/transversal_matroid.hpp"

GraphUndirected<> g(6, vc<pair<int, int>>{{0, 1}, {2, 1}, {2, 3}, {4, 3}});
vc<int> color{0, 1, 0, 1, 0, 0};
TransversalMatroid m(g, color);
assert(m.size() == 4 && m.vertex(2) == 4);
assert(m.element(4) == optional<int>(2));
m.build({0, 1});
auto c = m.circuit(2); // 0, 1 を含む列
assert(m.circuit(3)->empty()); // 割当先がない要素
```

## 詳細なドキュメント

### TransversalMatroid

#### コンストラクタ

```cpp
template<class Cost, bool is_erasable, class Color>
TransversalMatroid(
    const GraphUndirected<Cost, is_erasable>& graph, const vc<Color>& color);
template<class I = int>
TransversalMatroid(int right_vertices, vvc<I> adjacency);
```

グラフと彩色から構築する場合、色 $0$ の頂点を元の頂点番号の昇順に並べ、その順に要素番号 `0..size()-1` を付ける。色 $1$ の頂点も昇順に並べ、線形表現の行座標にする。孤立した色 $0$ の頂点はループとして残る。辺の重み・番号は使わず、多重辺に対応する。削除可能グラフでは構築時点で有効な辺を使い、隣接関係と番号の対応をコピーして保持する。

右頂点数と、任意の整数型を隣接先に使う左頂点ごとの隣接列からも構築でき、この場合の要素番号は `adjacency` の添字。隣接列の重複に対応する。初期状態の独立集合は空集合。

以下、左頂点数を $n$、右頂点数を $v$、隣接列の要素数の総和を $m$ とする。

##### 制約

- グラフからの構築：`color.size() == graph.size()`、各値を整数リテラル `0`・`1` と比較でき、値が $0$ または $1$。辺の両端の色が異なる。
- 隣接列からの構築：`right_vertices >= 0`、各隣接先は `0..right_vertices-1`。

##### 計算量

- $O(n+v+m)$。

#### size

```cpp
int size() const;
```

台集合の要素数、すなわち左頂点数を返す。

##### 計算量

- $O(1)$。

#### vertex / element

```cpp
int vertex(int e) const;
optional<int> element(int vertex) const;
```

`vertex(e)` は有効な要素番号から元の頂点番号を返す。`element(vertex)` は元の頂点番号から要素番号を返し、色 $0$ の頂点が存在するときに値を持つ。隣接列から構築した場合は、要素番号と頂点番号が一致する。

##### 計算量

- $O(1)$。

#### build

```cpp
template<class I = int>
void build(const vc<I>& independent);
```

現在の独立集合 $I$ を置き換え、その全要素を覆うマッチングを構築する。

##### 制約

- 各要素は有効な内部の要素番号で、互いに異なる。
- 指定した左頂点すべてを覆うマッチングが存在する。

##### 計算量

- $O(\lvert I\rvert(n+v+m))$。

#### circuit

```cpp
optional<vc<int>> circuit(int e) const;
```

$I+e$ の全要素を覆えるなら `nullopt`、従属なら fundamental circuit から `e` を除いた内部の要素番号列を返す。隣接先のない要素は値ありの空配列になる。返す列の順序は未規定。

##### 制約

- `e` は有効な内部の要素番号で、$I$ の外にある。

##### 計算量

- $O(n+v+m)$。

#### sparse_linear_representation

`linear_representation` と同じ引数・係数体で、行を座標、列を要素とする `SparseMatrix<F>` を返す。構築の条件と計算量は [線形表現](linear_representation.md) を参照する。
