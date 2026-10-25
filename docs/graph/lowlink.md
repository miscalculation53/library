このドキュメントには AI による作成・変更が含まれています。

## 概要

無向グラフの DFS 順序・LowLink・橋・関節点を求める。非連結グラフ、多重辺、自己ループ、重み付きグラフ、辺削除後の `GraphUndirected<Cost, true>` に対応する。重みは参照しない。

親配列と各頂点の走査位置で DFS を進める。子から戻るときに `low` を親へ伝播し、橋・関節点の条件を判定する。親辺だけを持つ頂点は発見時に結果を確定する。親への辺は元の辺番号で識別する。再帰や探索スタック用の別配列を使わず、構築時点の結果を保存する。

## 使用例

```cpp
#include "graph/lowlink.hpp"

vc<pair<int, int>> es{{0, 1}, {1, 2}, {2, 0}, {2, 3}};
GraphUndirected<> g(5, es);
LowLink lowlink(g);
auto cuts = lowlink.articulations();  // {2}
auto bridges = lowlink.bridges();     // {3}: 元の辺番号
bool b = lowlink.is_bridge(2, 3);     // true
auto p = lowlink.parent(0);           // nullopt: DFS 木の根
```

## 詳細なドキュメント

### LowLink

#### コンストラクタ

```cpp
LowLink();
template <class Cost, bool is_erasable>
explicit LowLink(const GraphUndirected<Cost, is_erasable> &g);
```

デフォルト構築は頂点数0の結果を作る。グラフを渡すと全連結成分を探索する。以下、$n$ は頂点数、$m$ は構築時に存在する辺数。

##### 計算量

- $O(n+m)$。

#### size / order / low / parent

```cpp
template <class I = ll> I size() const;
int order(int v) const;
int low(int v) const;
optional<pair<int, int>> parent(int v) const;
```

`size()` は頂点数。`order(v)` は全グラフを通じて重複しない0始まりの DFS 行きがけ順。`low(v)` は、DFS 木で $v$ の子孫へ下り、後退辺を高々1本使って到達できる頂点の `order` の最小値。

`parent(v)` は DFS 木の `(親頂点, 親との辺番号)` を返す。各 DFS 木の根では `nullopt`。

##### 制約

- 頂点を受け取る操作では `0 <= v < size()`。

##### 計算量

- 各 $O(1)$。

#### dfs_order / articulations / bridges

```cpp
const vc<int> &dfs_order() const;
const vc<int> &articulations() const;
const vc<int> &bridges() const;
```

`dfs_order()` は頂点を行きがけ順に並べた列。`articulations()` は関節点の頂点番号を昇順に並べた列。`bridges()` は橋の元の辺番号の列で、順序は任意。

関節点・橋は、その頂点・辺を除くと連結成分数が増えるものをいう。自己ループと多重辺を構成する辺は橋にならない。

##### 計算量

- 各 $O(1)$。保存済みの配列を参照で返す。

#### is_articulation / is_bridge

```cpp
bool is_articulation(int v) const;
bool is_bridge(int u, int v) const;
template <class Cost> bool is_bridge(const Edge<Cost> &e) const;
```

頂点が関節点か、2頂点を結ぶ辺が橋かを返す。`Edge` を渡す形式は `e.from`, `e.to` を使う。橋がその2頂点間に存在すれば `true`。

##### 制約

- 頂点番号は `[0, size())`。

##### 計算量

- 各 $O(1)$。

二辺連結成分は [two_edge_connected_component_ids](two_edge_connected_components.md)、二重連結成分は [BiconnectedComponents](biconnected_components.md) で求められる。
