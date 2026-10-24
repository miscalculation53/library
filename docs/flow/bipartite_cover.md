このドキュメントには AI による作成・変更が含まれています。

## 概要

二部グラフの最小頂点被覆・最大独立集合・最小辺被覆を構成する。既存の [bipartite_matching](bipartite_matching.md) を使い、追加の最大流は行わない。重みを無視し、削除可能グラフでは残っている辺を使う。

最小頂点被覆の大きさは最大マッチングの大きさに等しい。その補集合が最大独立集合となる。孤立点がある場合、辺被覆は存在しない。孤立点がなければ最小辺被覆の大きさは $n-\lvert M\rvert$。

一般の無向グラフにも、最大マッチングから最小辺被覆を作る関数を用意している。[参考：最小点被覆・最大独立集合・最小辺被覆](https://qiita.com/drken/items/7f98315b56c95a6181a4)

## 使用例

```cpp
#include "flow/bipartite_cover.hpp"

GraphUndirected<> g(4, vc<pair<int, int>>{{0, 2}, {0, 3}, {1, 2}});
vc<bool> color{0, 0, 1, 1};
auto matching = bipartite_matching(g, color);
auto cover = bipartite_vertex_cover(g, color, matching); // 頂点番号
auto independent = bipartite_independent_set(g, color, matching);
auto edge_cover = minimum_edge_cover(g, matching);     // 辺番号、失敗は nullopt
assert(cover.size() == matching.size());
assert(edge_cover && edge_cover->size() == 4 - matching.size());
```

## 詳細なドキュメント

#### bipartite_vertex_cover / bipartite_independent_set

```cpp
template <class Cost, bool is_erasable, class Color>
vector<int> bipartite_vertex_cover(
    const GraphUndirected<Cost, is_erasable>& g, const vector<Color>& color);
vector<int> bipartite_vertex_cover(
    const GraphUndirected<Cost, is_erasable>& g, const vector<Color>& color,
    const vector<pair<int,int>>& matching);

// 同じ引数で呼べる。
vector<int> bipartite_independent_set(...);
```

それぞれ最小頂点被覆・最大独立集合の頂点番号を、頂点番号順で返す。色0の未マッチ頂点から交互路を辿り、「到達しない色0」と「到達する色1」を頂点被覆に選ぶ。孤立点は独立集合に含まれる。

`matching` を省略した場合は、Hopcroft–Karp 法で求める。

##### 制約

- `color.size() == g.size()`。
- `Color` を整数リテラル `0`・`1` と比較でき、各要素が0または1。
- 各辺の両端の色が異なる。
- 指定する `matching` は現在のグラフの**最大**マッチング。各頂点対の向きは任意。

##### 計算量

- `matching` あり：$O(n+m)$。
- 省略時：$O(n+m\sqrt{k})$。$k$ は孤立点を除いた両側の頂点数の小さい方。

#### minimum_edge_cover / bipartite_edge_cover

```cpp
template <class Cost, bool is_erasable>
optional<vector<int>> minimum_edge_cover(
    const GraphUndirected<Cost, is_erasable>& g,
    const vector<pair<int,int>>& matching);

template <class Cost, bool is_erasable, class Color>
optional<vector<int>> bipartite_edge_cover(
    const GraphUndirected<Cost, is_erasable>& g, const vector<Color>& color);
```

最小辺被覆の辺番号を返す。マッチングの辺に、未マッチの各頂点に接続する辺を1本ずつ足す。孤立点がある場合は `nullopt`。頂点数0では空の辺被覆を返す。辺番号は元のグラフの番号。

`minimum_edge_cover` は一般の無向グラフでも使える。`bipartite_edge_cover` は内部で最大二部マッチングを求める。

##### 制約

- `matching` は現在のグラフの最大マッチング。頂点対の向きは任意で、自己ループを含まない。
- `bipartite_edge_cover` の彩色の条件は `bipartite_vertex_cover` と同じ。

##### 計算量

- `minimum_edge_cover`：$O(n+m)$。
- `bipartite_edge_cover`：$O(n+m\sqrt{k})$。
