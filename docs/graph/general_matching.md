このドキュメントには AI による作成・変更が含まれています。

## 概要

一般無向グラフの最大マッチングを Micali–Vazirani 法で求める。[GraphUndirected](graph.md) の重み付き・重みなし・辺削除可能な形式に対応する。多重辺を扱い、自己ループは読み飛ばす。重みは使わない。追加メモリは $O(n+m)$。

初期マッチングの両端の頂点番号には、それぞれ `i128`・`u128` を含む任意の整数型を使える。

## 使用例

```cpp
#include "graph/general_matching.hpp"

GraphUndirected<> g(4, vc<pair<int, int>>{
    {0, 1}, {1, 2}, {2, 0}, {2, 3}});
auto matching = general_matching(g);
// {{0, 1}, {2, 3}}

matching = general_matching(g, vc<pair<int, int>>{{1, 2}});
// 初期マッチングを組み替え、同じ最大辺数を求める。
```

## 詳細なドキュメント

#### general_matching

```cpp
template <class Cost, bool erasable, class I = int, class J = int>
vc<pair<int, int>> general_matching(
    const GraphUndirected<Cost, erasable> &g,
    const vc<pair<I, J>> &initial = {});
```

最大マッチングを頂点対 `(u, v)` の配列で返す。各対は `u < v` を満たし、配列は `u` の昇順。各頂点は高々一度現れ、最大解が複数ある場合の組合せは任意。空配列も有効な答えになる。

`initial` は探索開始時のマッチング。グラフと初期対応は変更せず、削除可能グラフでは現在残っている辺を使う。

##### 制約

- `initial` の両端が異なる有効な頂点番号で、現在のグラフの辺で結ばれている
- `initial` に同じ頂点が高々一度現れる

##### 計算量

- $O((n+m)\sqrt n\alpha(n))$。$n$ は頂点数、$m$ は辺数、$\alpha$ は逆 Ackermann 関数
- 最初の貪欲な追加で全頂点がマッチする場合は $O(n+m)$

通常の UnionFind を使うため、集合併合の $\alpha(n)$ を含む。

参考：[Vazirani, A Proof of the MV Matching Algorithm, v1](https://arxiv.org/abs/2012.03582v1)、Sections 2–4, 8。
