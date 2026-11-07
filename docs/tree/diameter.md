このドキュメントには AI による作成・変更が含まれています。

## 概要

木の直径の端点・長さ・頂点列と、各頂点からの最大距離を最小にする中心を求める。重みなしと非負の重み付きの木に対応する。

`endpoint_groups()` は、直径の両端を選ぶための頂点集合への分割を返す。異なる集合から1頂点ずつ選ぶと直径の両端になる。色別の組の数え上げなどに使える。

`virtual_center_tree()` は、直径の中点を根にして木の構造を扱うための機能。中点が元の頂点に重なる場合は元の木を根付け直し、辺の途中にある場合はその辺だけを仮想根で分割する。重み0の辺も元の接続関係を保つ。

## 使用例

```cpp
#include "tree/diameter.hpp"

int n = 4;
vc<pair<int, int>> edges{{0, 1}, {1, 2}, {1, 3}};
Diameter diameter(n, edges);
auto [a, b] = diameter.endpoints();
assert(diameter.length() == 2);
assert(diameter.path_vertex().front() == a);
assert(diameter.path_vertex().back() == b);
assert(diameter.centers() == vc<int>{1});
assert(diameter.endpoint_groups() == vvc<int>({{0}, {2}, {3}}));

// 同色の直径端点の組を数える。color[v] は [0,n) の整数。
vc<int> color{0, 1, 0, 0};
vc<ll> seen(n), count(n);
ll answer = 0;
for (const auto &group : diameter.endpoint_groups()) {
  vc<int> touched;
  for (int v : group) {
    if (count[color[v]]++ == 0) touched.push_back(color[v]);
  }
  for (int c : touched) {
    answer += seen[c] * count[c];
    seen[c] += count[c];
    count[c] = 0;
  }
}
assert(answer == 3);
```

重み付きの木では、親辺の重みの2倍値も受け取る。

```cpp
Diameter diameter(3, vc<tuple<ll, ll, ll>>{{0, 1, 2}, {1, 2, 8}});
auto [tree, costs2] = diameter.virtual_center_tree();
auto depths2 = tree.weighted_depths(costs2);
assert(tree.root() == 3 && tree.size() == 4);
assert(depths2 == vc<ll>({10, 6, 10, 0}));
assert(tree.path_vertex(0, 2) == vc<int>({0, 1, 3, 2}));
assert(diameter.endpoint_groups() == vvc<int>({{0}, {2}}));
```

中点が頂点に重なる場合は、元の頂点数と接続関係を保つ。

```cpp
vc<pair<int, int>> edges{{0, 1}, {1, 2}};
Diameter diameter(3, edges, vc<ll>{3, 3});
auto [tree, costs2] = diameter.virtual_center_tree();
assert(tree.root() == 1 && tree.size() == 3);
assert(costs2 == vc<ll>({6, 0, 6}));
assert(tree.path_vertex(0, 2) == vc<int>({0, 1, 2}));

Diameter zero(3, edges, vc<ll>{0, 0});
auto [zero_tree, zero_costs2] = zero.virtual_center_tree();
assert(zero_tree.root() == 0 && zero_tree.size() == 3);
assert(zero_tree.path_vertex(0, 2) == vc<int>({0, 1, 2}));
assert(zero.endpoint_groups() == vvc<int>({{0}, {1}, {2}}));
```

## 詳細なドキュメント

### Diameter

```cpp
template <class Cost = void>
struct Diameter;
```

`Cost = void` は重みなし。距離の型 `Dist` は重みなしなら `ll`、重み付きなら `Cost`。結果を自身で保持し、構築時の入力は構築後に変更・破棄できる。使用メモリは $O(n)$。

#### コンストラクタ

```cpp
(1) Diameter(int n, const vc<Pair>& es)
(2) Diameter(int n, const vc<Pair>& es, const vc<Cost>& costs)
(3) Diameter(int n, const vc<tuple<I, I, Cost>>& es)
```

- (1)：重みなしの直径を求める。`Diameter<>` を推論する。
- (2)：`costs[i]` を辺 `es[i]` の重みとして直径を求める。`Diameter<Cost>` を推論する。
- (3)：各要素を `(u, v, cost)` とする重み付きの辺の列から直径を求める。`Diameter<Cost>` を推論する。

##### 制約

- $n \geq 1$
- `es` は頂点 $0,\ldots,n-1$ の無向木を表す辺の列。端点の型は整数型
- (2)：`costs` の長さは $n-1$
- (2)(3)：各重みは非負
- (2)(3)：`Cost` は加減算・大小比較・等値比較ができ、距離と中間値が十分な余裕を持って収まる

##### 計算量

- $O(n)$（重みの演算を $O(1)$ とする）

#### endpoints, length

```cpp
(1) pair<int, int> endpoints() const
(2) Dist length() const
```

- (1)：直径の端点 `(a, b)` を返す。複数の直径がある場合はその一つ。長さ0の直径では同一頂点を両端とする場合がある。
- (2)：直径の長さを返す。重みなしなら辺数、重み付きなら重みの合計。

##### 計算量

- $O(1)$

#### path_vertex

```cpp
vc<int> path_vertex() const
```

`a` から `b` までの頂点列を移動順に返す。両端を含む。`a == b` なら `{a}`。

##### 計算量

- 返す頂点数に比例

#### centers

```cpp
vc<int> centers() const
```

各頂点からの最大距離を最小にする頂点を、頂点番号順にすべて返す。重みなしなら1頂点または隣接する2頂点。重み付きでは重み0の辺により3頂点以上になることもある。中心は元の頂点から選ぶ。

##### 計算量

- 重みなし：$O(\log n)$
- 重み付き：$O(n)$（重みの演算を $O(1)$ とする）

#### endpoint_groups

```cpp
vvc<int> endpoint_groups() const
```

直径端点の候補を、空でない集合 $V_1,\ldots,V_k$ に分割して返す。相異なる元の2頂点が直径の両端になることと、それらが異なる集合に属することが同値になる。

集合は互いに素で、各集合内は頂点番号の昇順。集合は最小要素の昇順に並ぶ。どの集合にも属さない頂点は直径端点の候補から外れる。

重み0の辺も扱える。直径0では各頂点を1要素の集合にし、単一頂点からは `{{0}}` を返す。

##### 計算量

- $O(n)$（重みの演算を $O(1)$ とする）

#### virtual_center_tree

```cpp
(1) RootedTree virtual_center_tree() const                     // Cost = void
(2) pair<RootedTree, vc<Dist>> virtual_center_tree() const      // 重み付き
```

元の頂点番号を保ち、直径の中点を根とする木を返す。

- 中点が元の頂点に重なる：頂点数は $n$。重み0により該当頂点が複数ある場合は最小番号の頂点を根にし、元の辺をそのまま使う。直径0の場合は頂点0が根になる。
- 中点が辺の途中：その辺だけを取り除き、両端と仮想根 $n$ をつなぐ。頂点数は $n+1$。

返す木の `root()`・`size()` で根と頂点数を取得できる。元の2頂点間の `path_vertex` の結果から仮想根 $n$ を除くと、元の木と同じ頂点列になる。元の頂点は $[0,n)$ にある。

(1) は重みなし用。元の直径を $D$ とすると最大の深さは $\lceil D/2\rceil$。中点が辺内の場合、その辺をまたぐ2頂点間の辺数が1増える。

(2) は重み付き用で、`(tree, costs2)` を返す。`costs2` の長さは `tree.size()`。`costs2[v]` は `(tree.parent(v), v)` の辺の重みの2倍値で、根の値は0。中点が辺の途中にある場合は、両端までの距離を分割後の辺に割り当てる。整数重みでも半整数の中点を正確に扱える。

元の2頂点間の重み付き距離は、返した `costs2` で計算すると元の距離の2倍になる。根からの重み付き深さの2倍値は `tree.weighted_depths(costs2)` で求まり、その最大は元の直径 $D$。`tree.depth`・`tree.dist` は辺数に基づく。

正の重みの場合、相異なる元の2頂点が直径の両端になる条件は、両方の深さの2倍値が $D$ で、根の異なる子の部分木に属すること。重み0の辺がある場合の端点分類には `endpoint_groups()` を使う。中心を根にした木は、元の枝内部の構造を使う DP などに利用できる。

##### 制約

- (2)：2倍した距離と中間値も `Dist` に十分な余裕を持って収まる

##### 計算量

- $O(n)$（重みの演算を $O(1)$ とする）
