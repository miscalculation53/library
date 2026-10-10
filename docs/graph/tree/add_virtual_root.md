## 概要

森に仮想根を1頂点追加し、各成分の根をつないだ `RootedTree` を返す。親配列と無向辺の列の両方から構築できる。

元の頂点数を $n$ とすると、元の頂点番号 $0,\ldots,n-1$ を保ち、仮想根の番号を $n$ にする。返す木の頂点数は $n+1$。空の森からは仮想根だけの木を作る。メモリは $O(n)$。

親配列では指定された各根を、辺の列では各成分の最小番号の頂点を仮想根につなぐ。同じ成分内のLCAは元の根に基づき、異なる成分のLCAは仮想根になる。

頂点の値や辺の値は利用側で管理する。値の配列にも仮想根や追加した辺に対応する値を用意する。

`first_ancestor` の条件は仮想根を含めた経路で単調にする。仮想根まで到達したら該当なしと扱う用途では、条件を `v == tree.root() || pred(v)` とし、返り値のペアの親側が仮想根かどうかで判定できる。

## 使用例

```cpp
#include "graph/tree/add_virtual_root.hpp"

vc<int> par = {-1, 0, -1, 2, 2};
auto tree = add_virtual_root(5, par);
assert(tree.size() == 6 && tree.root() == 5);
assert(tree.lca(3, 4) == 2);
assert(tree.lca(1, 3) == 5);

vc<pair<int, int>> edges = {{1, 0}, {3, 2}, {2, 4}};
auto from_edges = add_virtual_root(5, edges);
assert(from_edges.parent(0) == 5 && from_edges.parent(2) == 5);
assert(from_edges.lca(3, 4) == 2);
```

## 詳細なドキュメント

#### add_virtual_root

```cpp
template <class T>
RootedTree add_virtual_root(int n, const vc<T>& forest)
```

`T` が整数型なら親配列、その他の場合は端点の組の列として扱う。辺の列は `pair`、2要素の `tuple`、2要素の `array` などを使える。

##### 制約

- $0\leq n$
- 親配列の場合：長さは $n$。各根は `forest[v] < 0` または `forest[v] == v` で表し、それ以外の親は $[0,n)$ の頂点。根の自己参照以外に閉路がない
- 辺の列の場合：各端点は $[0,n)$。無向グラフが森である

##### 計算量

- 親配列：$O(n+1)$
- 辺の列：辺数を $m$ として $O(n+m+1)$

返した木の操作は [RootedTree](rooted_tree.md) を参照。
