このドキュメントには AI による作成・変更が含まれています。

## 概要

連結成分への分解

## 使用例

```cpp
#include "graph/connected_components.hpp"

GraphUndirected<> g(4, vc<pair<int, int>>{{0, 1}, {2, 3}});
auto ids = connected_component_ids(g);
assert(ids[0] == ids[1] && ids[2] == ids[3]);
assert(ids[0] != ids[2]);
```

## 詳細なドキュメント

#### connected_component_ids

```cpp
vc<I> connected_component_ids<I = ll>(const GraphUndirected<Cost> &g)
```

無向グラフを受け取り、各頂点の連結成分の番号を返す。

##### 計算量

- $O(n + m)$
