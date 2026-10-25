このドキュメントには AI による作成・変更が含まれています。

## 概要

想定用途：連結成分や強連結成分への分解などと組み合わせて使う。

## 使用例

```cpp
#include "graph/contraction.hpp"

GraphDirected<> g(3, vc<pair<int, int>>{{0, 1}, {1, 2}, {0, 2}});
auto h = contracted_graph(g, vc<int>{0, 0, 1});
assert(h.size() == 2 && h.num_of_edges() == 1);
```

## 詳細なドキュメント

#### contracted_graph

```cpp
template <bool is_directed, class Cost, class I>
Graph<is_directed, Cost> contracted_graph(const Graph<is_directed, Cost>& g, const vector<I>& ids);
```

グラフ $g$ を、頂点 $v$ が縮約後に $\mathrm{ids}[v]$ の頂点になるように縮約したグラフを返す。ただし、コストは特に考えず、縮約後のグラフには自己ループや多重辺を含まないようにする。

頂点を成分ごとに CSR へまとめ、成分ごとに行先を記録して重複を除く。縮約後の辺の順序は任意。

##### 制約

- `ids.size() == g.size()`。
- `ids` の各要素が非負。

##### 計算量

- $O(n+m+k)$。$k$ は縮約後の頂点数で、空配列なら0、それ以外は `max(ids)+1`。成分番号が詰まっている場合は $O(n+m)$。
