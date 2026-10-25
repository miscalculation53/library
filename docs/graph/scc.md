このドキュメントには AI による作成・変更が含まれています。

## 概要

強連結成分分解。明示的なスタックを使うTarjan法により、元のグラフを1回のDFSで走査する。補助空間は $O(n)$。

## 使用例

```cpp
#include "graph/scc.hpp"

GraphDirected<> g(3, vc<pair<int, int>>{{0, 1}, {1, 0}, {1, 2}});
auto ids = scc(g);
assert(ids[0] == ids[1]);
assert(ids[1] < ids[2]);
```

## 詳細なドキュメント

#### scc

```cpp
vc<I> scc(const GraphDirected<Cost> &g)
```

有向グラフを受け取り、各頂点の強連結成分の番号を返す。強連結成分はトポロジカル順に並んでいる。

成分数を $k$ として、番号は $0,\ldots,k-1$。異なる成分を結ぶ辺 $u\to v$ について、成分番号は `ids[u] < ids[v]` となる。

##### 制約

- `I` は成分番号を表現できる整数型。

##### 計算量

- $O(n + m)$
