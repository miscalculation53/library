## 概要

無向グラフの二部グラフ判定を行い、二部グラフなら $2$ 彩色、それ以外なら奇数本の辺からなる単純閉路を返す。
非連結グラフ・多重辺・自己ループに対応する。自己ループは長さ $1$ の奇閉路として扱う。

## 使用例

```cpp
GraphUndirected<> g(4, vc<pair<int, int>>{{0, 1}, {1, 2}});
auto [ok, color, cycle] = bipartite_coloring(g);
// ok == true、color == {0, 1, 0, 0}、cycle は空

GraphUndirected<> triangle(3, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 0}});
auto [ok2, color2, cycle2] = bipartite_coloring(triangle);
// ok2 == false、color2 は空、cycle2 は三角形の辺列
for (const auto &e : cycle2)
  PRINT(e.from, e.to, e.index);
```

## 詳細なドキュメント

#### bipartite_coloring

```cpp
template <class Cost, class I = ll>
tuple<bool, vc<I>, vc<Edge<Cost>>> bipartite_coloring(const GraphUndirected<Cost> &g)
```

戻り値は `(二部グラフかどうか, 彩色, 奇閉路)`。

- 二部グラフなら `(true, color, 空の辺列)`。`color` は長さ $n$ で、各要素は `0` または `1`。各辺の両端は異なる色になる。各連結成分で頂点番号が最小の頂点を `0` に彩色する。
- 奇閉路があれば `(false, 空の彩色, cycle)`。`cycle` は奇数本の辺を巡回順に並べた列で、`cycle[i].to == cycle[(i + 1) % cycle.size()].from` を満たす。各頂点は一度ずつ通る。

辺の `from`・`to` は進行方向に合わせ、`index`・`cost` は元のグラフの値を保つ。
重みは判定に影響せず、`Cost = void` では `Edge<void>` の辺列を返す。
頂点数 $0$ の場合は `(true, 空の彩色, 空の辺列)` を返す。

BFS で彩色し、同色の頂点を結ぶ辺を見つけたら、親辺を共通祖先まで辿って奇閉路を復元する。

##### 制約

- `I` は $-1, 0, 1$ を表現できる整数型。

##### 計算量

- $O(n + m)$。$n$ は頂点数、$m$ は辺数。
