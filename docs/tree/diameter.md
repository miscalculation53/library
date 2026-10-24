このドキュメントには AI による作成・変更が含まれています。

## 概要

木の直径の両端点と、片方の端点を根とする木を求める。

## 使用例

```cpp
#include "tree/diameter.hpp"

vc<pair<int, int>> edges{{0, 1}, {1, 2}, {1, 3}};
auto [a, b, rooted] = tree_diameter(4, edges);
assert(rooted.depth(b) == 2);
auto [u, v, weighted, distances] =
    tree_diameter_weighted(4, edges, vc<ll>{2, 3, 4});
assert(distances[v] == 7);
```

## 詳細なドキュメント

#### tree_diameter

```cpp
tuple<int, int, RootedTree> tree_diameter(int n, const vc<Pair>& es)
```

辺数に基づく直径を求め、`(a, b, ga)` を返す。`a`, `b` は直径の両端点、`ga` は `a` を根とする木。

##### 制約

- $n \geq 1$
- `es` は端点の組の列で、頂点 $0,\ldots,n-1$ の無向木を表す

##### 計算量

- $O(n)$

#### tree_diameter_weighted

```cpp
tuple<int, int, RootedTree, vc<Cost>> tree_diameter_weighted(
  int n, const vc<Pair>& es, const vc<Cost>& costs)
```

`costs[i]` を辺 `es[i]` の重みとして、`(a, b, ga, depths)` を返す。`a`, `b` は直径の両端点、`ga` は `a` を根とする木、`depths[v]` は `a` から `v` までの重みの合計。直径の長さは `depths[b]`。

##### 制約

- $n \geq 1$
- `es` は端点の組の列で、頂点 $0,\ldots,n-1$ の無向木を表す
- `costs` の長さは $n-1$
- 各辺の重みは非負
- `Cost` は重みの累積計算に必要な値を表現できる

##### 計算量

- $O(n)$（重みの演算を $O(1)$ とする）
