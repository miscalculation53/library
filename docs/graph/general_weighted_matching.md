このドキュメントには AI による作成・変更が含まれています。

## 概要

一般無向グラフで、未マッチの頂点を許し、選んだ辺の重みの総和を最大にする。[GraphUndirected](graph.md) の整数重み・重みなし・辺削除可能な形式に対応する。

Edmonds の主双対法で交互木を再利用する。各連結成分の辺密度に応じて、最小スラック候補を配列で管理する方式と、次のイベントを区間木で管理する方式を切り替える。

## 使用例

```cpp
#include "graph/general_weighted_matching.hpp"

vc<tuple<int, int, ll>> edges{
    {0, 1, 6}, {1, 2, 11}, {2, 3, 6}, {0, 1, 4}};
GraphUndirected<ll> g(4, edges);
auto ids = general_weighted_matching(g);
// 辺番号 0 と 2。重みの総和は 12。
```

## 詳細なドキュメント

#### general_weighted_matching

```cpp
template <class Cost, bool erasable>
vc<int> general_weighted_matching(const GraphUndirected<Cost, erasable> &g);
```

最大重みマッチングを、グラフ構築時の辺番号の配列で返す。返す順序と同じ重みの解の選び方は任意。空配列も有効な解になる。

現在残っている正の重みの辺を使う。自己ループは読み飛ばし、同じ頂点対を結ぶ辺は最大重みの一本を使う。重みなしグラフでは各辺の重みを $1$ とする。

孤立頂点を除き、正の重みの辺からなる連結成分ごとに解く。現在の切り替えでは、辺密度が 6.25% 以上で、平均次数が $4.5\sqrt v$ 以上の成分を配列版で解く。$v$ は成分の頂点数。残りの成分は区間木版で解く。

##### 制約

- `Cost` は `i128`・`u128` を含む整数型、または `void`
- 各成分の正の最大辺重みの $4$ 倍が `u128` に収まる。`ull` に収まる場合は `ull` で内部計算する

##### 計算量

- 準備は $O(n+m\alpha(n))$。$n$ は入力の頂点数、$m$ は現在の辺数、$\alpha$ は逆 Ackermann 関数
- 頂点数 $v$、辺数 $e$ の成分あたり、配列版は $O(v^3)$、区間木版は $O(v^2(v+e)\log(v+e))$。$e$ は多重辺をまとめた後の辺数

追加メモリは全体で $O(n+m)$。配列版は成分あたり $O(v^2)$ の表を使い、選択される場合は $v^2=O(e)$ を満たす。区間木版の候補は実在する辺と Blossom に対応する。

参考文献

- [Edmonds, Maximum Matching and a Polyhedron With 0,1-Vertices](https://nvlpubs.nist.gov/nistpubs/jres/69B/jresv69Bn1-2p125_A1b.pdf)（最大重みマッチングの主双対法）
- [Gabow, Data Structures for Weighted Matching and Extensions to b-matching and f-factors](https://arxiv.org/html/1611.07541v1) §2（スラックに基づく双対更新とイベント）
- [Boost.Graph 1.90.0 のアルゴリズム解説](https://www.boost.org/doc/libs/1_90_0/libs/graph/doc/maximum_weighted_matching.html#algorithm-description)（増加路が通る交互木だけを解除する方針）
