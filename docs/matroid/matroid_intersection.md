このドキュメントには AI による作成・変更が含まれています。

## 概要

同じ台集合を持つ二つのマトロイドの、最大要素数の共通独立集合を求める。fundamental circuit から交換グラフを作り、最短増加路に沿って選択を反転する。

交換グラフは [Graph](../graph/graph.md) で保持し、最短増加路の探索と復元には [ShortestPath](../graph/sssp.md) の BFS を使う。重みなし・重み付きのヘッダは、それぞれ必要な API を include できる入口として分け、交換グラフの構築と最短路処理を共有している。

oracle の共通仕様は [マトロイド](matroid.md) を参照。重みを扱う場合は [重み付き交叉](weighted_matroid_intersection.md) を使う。

## 使用例

```cpp
#include "matroid/partition_matroid.hpp"
#include "matroid/matroid_intersection.hpp"

// 二部グラフの辺を台集合とし、両側で次数を1以下にする。
PartitionMatroid left ({0, 0, 1}, {1, 1});
PartitionMatroid right({0, 1, 0}, {1, 1});
auto selected = matroid_intersection(left, right); // {1, 2}
```

## 詳細なドキュメント

#### matroid_intersection

```cpp
template<class M1, class M2>
vc<int> matroid_intersection(M1 first, M2 second,
                            const vc<int>& initial = {});
```

最大共通独立集合の要素番号を昇順に返す。空配列も有効な答え。`initial` を初期解として増加させられる。oracle を値で受け取り、内部の状態を作り直す。

##### 制約

- `first.size() == second.size()`。
- 両方の型が共通 oracle の仕様を満たす。
- `initial` は相異なる有効な要素からなり、両方で独立。

##### 計算量

台集合の要素数を $n$、最大共通独立集合の大きさを $r$ とする。二つの `build` の時間の和を $B$、`circuit` 一回の時間の最大値を $C$ とする。circuit の返却にかかる時間も $C$ に含める。

- 時間 $O(r(B+nC+nr))$。
- 追加メモリ $O(nr)$。oracle の領域は別。

