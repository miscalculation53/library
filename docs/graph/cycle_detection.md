このドキュメントには AI による作成・変更が含まれています。

## 概要

有向・無向グラフのサイクルを検出する。自己ループ・多重辺・重み付き辺に対応する。ヒープ上の配列で探索し、長い経路も扱う。追加メモリは $O(n)$。

## 使用例

```cpp
#include "graph/cycle_detection.hpp"

GraphDirected<> g(3, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 0}});
auto cycle = cycle_detection(g);
assert(cycle && cycle->size() == 3);
for (const auto& e : *cycle) assert(0 <= e.index && e.index < 3);
```

## 詳細なドキュメント

#### cycle_detection

```cpp
optional<vc<Edge<Cost>>> cycle_detection(const Graph<is_directed, Cost>& g)
```

無向または有向グラフ $g$ のサイクルを検出し、巡回順の辺列を返す。サイクルがなければ `nullopt`。

返す辺は巡回順に並び、`from`・`to` はたどる向き、`index`・`cost` は元の辺の値になる。複数のサイクルがある場合は任意の一つを返す。自己ループは長さ $1$、異なる無向多重辺の往復は長さ $2$ のサイクルとして扱う。

##### 計算量

- $O(n + m)$
