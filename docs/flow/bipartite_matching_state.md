このドキュメントには AI による作成・変更が含まれています。

## 概要

無向グラフと $0/1$ 彩色から二部マッチングの状態を作り、指定した左頂点からの交互路探索と増加を行う。探索に失敗した場合も到達集合を取得できる。

左頂点を順に追加する割当や [transversal matroid](../matroid/transversal_matroid.md) の circuit の計算に使える。一括で最大マッチングだけを求める場合は [bipartite_matching](bipartite_matching.md) を使う。

隣接先の配列は `i128`・`u128` を含む任意の整数型に対応する。削除可能な CSR では有効な要素だけを取り込む。

## 使用例

```cpp
#include "flow/bipartite_matching_state.hpp"

GraphUndirected<> g(5, vc<pair<int, int>>{{0, 1}, {2, 1}, {2, 3}, {4, 3}});
vc<bool> color{false, true, false, true, false};
BipartiteMatchingState matching(g, color);
assert(matching.augment(0));
assert(matching.augment(1));
auto search = matching.search(*matching.left_index(4));
assert(!search.free_right);
// search.left は内部番号 {2, 1, 0}。元の左頂点は {4, 2, 0}。
auto pairs = matching.matching(); // 元の頂点番号による {{0, 1}, {2, 3}}。
```

## 詳細なドキュメント

### BipartiteMatchingState

#### コンストラクタ

```cpp
template<class Cost, bool is_erasable, class Color>
BipartiteMatchingState(
    const GraphUndirected<Cost, is_erasable>& graph, const vc<Color>& color);
BipartiteMatchingState(int right_vertices, CSR<int> adjacency);
template<class I = int>
BipartiteMatchingState(int right_vertices, const vvc<I>& adjacency);
template<class I = int>
BipartiteMatchingState(int right_vertices, initializer_list<vc<I>> adjacency);
template<class I, bool erasable>
BipartiteMatchingState(int right_vertices, const CSR<I, erasable>& adjacency);
```

グラフと彩色から構築する場合、色 $0$ を左、色 $1$ を右とし、それぞれ元の頂点番号の昇順に内部番号を付ける。孤立点も保持する。辺の重み・番号は使わず、多重辺に対応する。削除可能グラフでは構築時点で有効な辺を使う。隣接関係と頂点番号の対応をコピーして保持する。

隣接列からも構築できる。この場合、左頂点は列の添字、右頂点は列内の値で、それぞれ内部番号と一致する。重複する隣接先に対応する。

初期状態のマッチングは空。探索・対応配列・交互グラフは内部番号を使う。元の頂点番号との変換は以下の関数で行える。以下、左右の頂点数を $n_L,n_R$、重複を含む辺数を $m$ とする。

##### 制約

- グラフからの構築：`color.size() == graph.size()`、各値を整数リテラル `0`・`1` と比較でき、値が $0$ または $1$。辺の両端の色が異なる。
- 隣接列からの構築：`right_vertices >= 0`、隣接先は `0..right_vertices-1`。

##### 計算量

- $O(n_L+n_R+m)$。

#### 状態の取得

```cpp
int left_size() const;
int right_size() const;
int matching_size() const;
const CSR<int>& adjacency() const;
const vc<int>& left_mate() const;
const vc<int>& right_mate() const;
```

左右の頂点数、対応数、隣接先、左右の対応配列を返す。対応配列の未対応位置は `-1`。

##### 計算量

- $O(1)$。

#### 頂点番号の変換 / matching

```cpp
int left_vertex(int u) const;
int right_vertex(int v) const;
optional<int> left_index(int vertex) const;
optional<int> right_index(int vertex) const;
vc<pair<int, int>> matching() const;
```

`left_vertex`・`right_vertex` は有効な内部番号から元の頂点番号を返す。`left_index`・`right_index` は元の頂点番号から対応する内部番号を返し、その側に属する頂点が存在するときに値を持つ。

`matching()` は現在の対応を `(元の左頂点番号, 元の右頂点番号)` の配列で返す。隣接列から構築した場合は、それぞれ左右の内部番号になる。

##### 計算量

- 番号変換：$O(1)$。
- `matching()`：$O(n_L)$。

#### search

```cpp
struct SearchResult {
  vc<int> left, right, parent_right;
  optional<int> free_right;
};
template<bool CollectRight = true>
SearchResult search(int source) const;
```

未対応の左頂点から交互路を探索する。到達した左右の頂点を探索順に返し、`left` の先頭は `source`。`parent_right[v]` は右頂点 `v` に到達した直前の左頂点で、未到達なら `-1`。

未対応の右頂点に到達した時点で、その番号を `free_right` に返す。この場合の到達集合は途中までの探索結果。失敗時は `free_right == nullopt` で、到達集合は完全なものになる。現在のマッチングは保持する。

`CollectRight=false` では `right` の保存を省く。左の到達集合と探索経路だけを使う場合に指定できる。`parent_right` はこの場合も保持する。

##### 制約

- `source` は有効な未対応の左頂点。

##### 計算量

- $O(n_L+n_R+m)$。

#### augment

```cpp
bool augment(int source);
```

`search(source)` で増加路が得られれば対応を組み替え、対応数を一つ増やして `true` を返す。失敗時は状態を保って `false` を返す。制約・計算量は `search` と同じ。

#### clear / unmatch

```cpp
void clear();
int unmatch(int u);
```

`clear` は全対応を外す。`unmatch` は有効な左頂点 `u` の対応を一つ外し、外した対応数を返す。頂点と辺は保持する。

##### 計算量

- `clear`：$O(n_L+n_R)$。
- `unmatch`：$O(1)$。

#### maximize

```cpp
int maximize();
```

現在の対応から全左頂点を対象に増加し、最大マッチングの対応数を返す。

##### 計算量

- $O(n_L(n_L+n_R+m))$。

#### alternating_graph

```cpp
GraphDirected<> alternating_graph() const;
```

左頂点を `0..n_L-1`、右頂点を `n_L..n_L+n_R-1` とした交互グラフを返す。各辺を左から右へ向け、対応に使う頂点対には右から左への辺も加える。

最大マッチングに対するこのグラフの到達領域と SCC は [DM 分解](dm_decomposition.md) の材料になる。DM 分解の全体構造に対し、`search` は指定した一つの未対応左頂点からの到達可能性を扱う。

##### 計算量

- $O(n_L+n_R+m)$。
