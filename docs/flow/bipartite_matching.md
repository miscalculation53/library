このドキュメントには AI による作成・変更が含まれています。

## 概要

無向二部グラフと $0/1$ 彩色から、最大マッチングを頂点対の配列として求める。彩色には `vc<int>`・`vc<ll>`・`vc<bool>` などを渡せる。重み付き・重みなし・辺削除可能な [GraphUndirected](../graph/graph.md) に対応する。重みは使わず、多重辺も扱える。

Hopcroft–Karp 法を使う。孤立点を除いて頂点数が少ない側から探索し、まず貪欲に長さ $1$ の増加路を処理する。その側の頂点がすべてマッチしたら、その時点で返す。それ以外は両側の残次数を更新し、次数 $1$・$2$ の頂点を優先して初期対応を作り直す。その後、多重辺を除いた片側の行先だけを CSR に配置する。増加路が残る場合は、線形の走査予算内で任意長の増加路を先に処理し、完全探索で増加路を持たないと確定した領域を再利用する。最後は BFS と反復 DFS で最短増加路をまとめて処理する。長い交互路も明示的なスタックで扱う。追加メモリは $O(n+m)$。

## 使用例

```cpp
#include "flow/bipartite_matching.hpp"
#include "graph/bipartite_coloring.hpp"

GraphUndirected<> g(5, vc<pair<int, int>>{{0, 2}, {0, 3}, {1, 2}});
vc<bool> color{false, false, true, true, true};
auto matching = bipartite_matching(g, color);
// {{0, 3}, {1, 2}}。常に色0 -> 色1の向き。

// bipartite_coloring が返す vc<ll> もそのまま使える。
auto [ok, coloring, cycle] = bipartite_coloring(g);
if (ok) matching = bipartite_matching(g, coloring);
```

## 詳細なドキュメント

#### bipartite_matching

```cpp
template <class Cost, bool is_erasable, class Color>
vc<pair<int, int>> bipartite_matching(
    const GraphUndirected<Cost, is_erasable> &g,
    const vc<Color> &color);
```

最大マッチングを `(色0の頂点, 色1の頂点)` の配列で返す。各頂点は高々一度現れ、各頂点対は現在のグラフの辺で結ばれる。解と配列の順序は任意。頂点数 $0$、辺数 $0$ の場合は空配列を返す。

グラフ・彩色は呼出し中に参照し、変更しない。削除可能グラフでは現在残っている辺を使う。元のグラフの頂点番号を返し、辺の重み・番号は使わない。

##### 制約

- `color.size() == g.size()`
- `Color` の値を整数リテラル `0`・`1` と比較でき、各要素が `0` または `1`
- 各辺の両端の色が異なる

##### 計算量

- $O(n+m\sqrt{k})$。$n$ は頂点数、$m$ は辺数、$k$ は孤立点を除いた両側の頂点数の小さい方
- 最初の貪欲法で小さい側の全頂点がマッチする場合は $O(n+m)$
