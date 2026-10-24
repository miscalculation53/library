このドキュメントには AI による作成・変更が含まれています。

## 概要

部分グラフへの分解

## 使用例

```cpp
#include "graph/subgraphs.hpp"

GraphUndirected<> g(4, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 3}});
auto [parts, local_ids] = subgraphs(g, vc<int>{0, 0, 1, 1});
assert(parts.size() == 2);
assert(parts[0].size() == 2 && parts[0].num_of_edges() == 1);
assert(local_ids == vc<int>({0, 1, 0, 1}));
```

## 詳細なドキュメント

#### subgraphs

```cpp
pair<vc<Graph<is_directed, Cost>>, vc<I>> subgraphs(Graph<is_directed, Cost> g, vc<I> ids)
```

無向 or 有向 グラフ $g$ の頂点 $v$ が $\mathrm{ids}[v]$ 番目の部分グラフに属するように分解し、

- 部分グラフの vector `graphs`
- 新しい部分グラフでの頂点番号を表す vector `nvids`

のペアを返す。

もとのグラフの頂点 $u$ が、$l$ 番目の部分グラフの頂点 $i$ に対応しているとすると、

- $u$ から $(l, i)$ は、`l = ids[u]`, `i = nvids[u]`
- $(l, i)$ から $u$ は、`GroupIndex grp(ids)` を用いて `u = grp.idxs(l)[i]`

により取得できる。

##### 計算量

- $O(n + m + \lvert\mathrm{ids}\rvert)$
