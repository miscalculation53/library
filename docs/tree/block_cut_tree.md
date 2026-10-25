このドキュメントには AI による作成・変更が含まれています。

## 概要

無向グラフの二重連結成分と関節点からなる森を構築する。ブロックに含まれる関節点との間に辺を張る。元のグラフの各連結成分が1本の木になり、孤立点も1ノードの木として残る。

ブロック数を $b$、関節点数を $a$ とすると、ノード番号は次のようになる。

| ノード番号 | 対応 |
|---|---|
| `[0, b)` | `BiconnectedComponents` と同じ番号のブロック |
| `[b, b+a)` | 元の頂点番号の昇順に並べた関節点 |

元の頂点は、関節点なら関節点ノード、それ以外なら属する唯一のブロックノードへ写す。2頂点間の森上のパスにある関節点は、元のグラフでその2頂点を分離する関節点に対応する。分離点を数えるときは両端自身を除く。

自己ループ・孤立点の扱いは [BiconnectedComponents](../graph/biconnected_components.md) と共通。

## 使用例

```cpp
#include "tree/block_cut_tree.hpp"
#include "tree/add_virtual_root.hpp"

vc<pair<int, int>> es{{0, 1}, {1, 2}, {2, 0}, {2, 3}};
GraphUndirected<> g(5, es);
BiconnectedComponents bcc(g);
BlockCutTree bct(bcc);  // BlockCutTree bct(g); と直接構築してもよい
const auto &forest = bct.forest();
int u = bct.node_id(0), v = bct.node_id(3);

// 仮想根を追加すると、非連結の場合も RootedTree で扱える。
vc<pair<int, int>> tree_edges;
for (const auto &e : forest.edges()) tree_edges.eb(e.from, e.to);
auto tree = add_virtual_root(bct.size(), tree_edges);
int lca = tree.lca(u, v);
```

## 詳細なドキュメント

### BlockCutTree

#### コンストラクタ

```cpp
BlockCutTree();
explicit BlockCutTree(const BiconnectedComponents &bcc);
template <class Cost, bool is_erasable>
explicit BlockCutTree(const GraphUndirected<Cost, is_erasable> &g);
```

デフォルト構築は空の森を作る。分解結果を渡すと、そのブロック番号を保って構築する。グラフを渡す形式では LowLink とブロックの頂点一覧だけを構築し、BCT に使わない元の辺一覧の生成を省く。元のグラフや分解結果のコピー・参照は保持しない。

##### 計算量

- $O(n+m)$。$n$ は元の頂点数、$m$ は存在する辺数。

#### size / num_of_blocks / forest

```cpp
template <class I = ll> I size() const;
template <class I = ll> I num_of_blocks() const;
const GraphUndirected<> &forest() const;
```

森のノード数、ブロック数、森を表す重みなし無向グラフを返す。

##### 計算量

- 各 $O(1)$。

#### node_id / vertex_ids

```cpp
int node_id(int v) const;
const vc<int> &vertex_ids() const;
```

元の頂点 `v` に対応するノード番号と、全頂点の対応表を返す。孤立点を含め、すべての元の頂点がノードに対応する。

##### 制約

- `node_id(v)` では `v` は元のグラフの頂点番号。

##### 計算量

- 各 $O(1)$。

#### is_articulation_node / articulation_vertex

```cpp
bool is_articulation_node(int node) const;
optional<int> articulation_vertex(int node) const;
```

ノードが関節点を表すかを返す。`articulation_vertex()` は関節点ノードに対応する元の頂点を返し、ブロックノードでは `nullopt` を返す。

##### 制約

- `0 <= node < size()`。

##### 計算量

- 各 $O(1)$。
