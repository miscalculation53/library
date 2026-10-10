## 概要

UnionFindで管理する成分の併合履歴から、親配列を作る。元の頂点が葉になり、異なる成分を併合するたびに、その2成分の根を子にした親ノードを作る。未接続の成分が残る場合は森林になる。

元の頂点数を $n$、完成したノード数を $V$ とする。葉の番号は $0,\ldots,n-1$、内部ノードは作成順に $n,n+1,\ldots$。親の番号は子より大きく、$n>0$ なら $V\leq 2n-1$。メモリは $O(n)$。

親配列を [RootedTree](rooted_tree.md) に渡し、子の列挙・木DP・LCA・祖先検索を行う。森林は [add_virtual_root](add_virtual_root.md) で仮想根を追加できる。

併合時刻などの情報は、`merge` が返すノード番号に対応する配列で管理する。辺を重みの昇順に処理すると、2頂点のLCAに対応する併合時刻が、その2頂点が初めて連結する閾値になる。

## 使用例

```cpp
#include "graph/tree/merge_tree.hpp"
#include "graph/tree/rooted_tree.hpp"

int n = 4;
MergeTree merges(n);
vc<tuple<int, int, ll>> edges = {{0, 1, 2}, {2, 3, 5}, {1, 2, 8}, {0, 3, 10}};
vc<ll> weight(n, 0);
for (auto [u, v, w] : edges) {
  int node = merges.merge(u, v);
  if (node != -1) weight.push_back(w);
}
auto par = merges.parents();
RootedTree tree(par.size(), par);
assert(weight[tree.lca(0, 3)] == 8);

// 木の操作にはRootedTreeを使う。
vc<int> leaf_count(tree.size(), 1);
for (int v : tree.bottom_up_vertices()) {
  if (v < n) continue;
  leaf_count[v] = 0;
  for (int child : tree.children(v)) leaf_count[v] += leaf_count[child];
}
auto [child, parent] = tree.first_ancestor(0, [&](int x) { return leaf_count[x] >= 3; });
assert(child == 4 && parent == 6);
assert(weight[parent] == 8);
```

## 詳細なドキュメント

### MergeTree

#### コンストラクタ

```cpp
explicit MergeTree(int n = 0)
```

$n$ 個の葉を作る。併合の順番や、併合に付随する情報は利用側で管理する。

##### 制約

- $0\leq n$

##### 計算量

- $O(n+1)$

#### merge

```cpp
int merge(int u, int v)
```

元の頂点 `u`, `v` の成分を併合し、新しい親ノードの番号を返す。同じ成分の場合は `-1` を返す。新しいノードの番号は $n,n+1,\ldots$ の順に増える。

##### 制約

- $0\leq u,v<n$

##### 計算量

- ならし $O(\alpha(n))$

#### parents

```cpp
vc<int> parents() const
```

完成した親配列のコピーを返す。各成分の根の親は `-1`。

`RootedTree` に直接渡す場合は、元の頂点が1個以上あり、全頂点が連結していること。森林の場合は `add_virtual_root(par.size(), par)` で根を追加する。

##### 計算量

- $O(V+1)$
