このドキュメントには AI による作成・変更が含まれています。

## 概要

無向グラフからすべての橋を取り除いたときの、各頂点の連結成分番号を返す。成分番号は `[0, k)` に詰める。非連結グラフ、多重辺、自己ループ、重み付きグラフ、辺削除後のグラフに対応する。

成分の頂点一覧は `GroupIndexRaw`、成分を縮約した森は `contracted_graph` と組み合わせて得られる。

## 使用例

```cpp
#include "graph/two_edge_connected_components.hpp"
#include "graph/contraction.hpp"
#include "ds/group_index.hpp"

vc<pair<int, int>> es{{0, 1}, {1, 2}, {2, 0}, {2, 3}};
GraphUndirected<> g(5, es);
auto ids = two_edge_connected_component_ids<int>(g);
GroupIndexRaw groups(ids);             // {0,1,2}, {3}, {4}
auto forest = contracted_graph(g, ids); // 成分を結ぶ橋だけが残る

LowLink lowlink(g);
auto same_ids = two_edge_connected_component_ids<int>(lowlink);
```

## 詳細なドキュメント

#### two_edge_connected_component_ids

```cpp
template <class I = ll, class Cost, bool is_erasable>
vc<I> two_edge_connected_component_ids(const GraphUndirected<Cost, is_erasable> &g);

template <class I = ll>
vc<I> two_edge_connected_component_ids(const LowLink &lowlink);
```

各頂点の二辺連結成分番号を返す。`I` は返す配列の要素型。空グラフでは空配列を返す。成分番号の並びは任意。

グラフを渡す形式は内部で `LowLink` を構築する。計算済みの `LowLink` を渡すと、その DFS 木と橋の情報を再利用する。

無向 DFS では、根と親辺が橋である頂点で `low(v) == order(v)` となる。その頂点から新しい成分を始め、残りの頂点は親の成分番号を引き継ぐ。

##### 制約

- `I` はすべての成分番号を表現できる整数型。

##### 計算量

- グラフを渡す場合は $O(n+m)$。$n$ は頂点数、$m$ は存在する辺数。
- `LowLink` を渡す場合は $O(n)$。
