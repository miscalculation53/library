## 概要

HLD を含む根つき木クラス。親子関係、部分木、LCA、パスの分解を扱う。

木の構造と辺の値を分けて管理する。構築には端点の組または親配列を渡し、辺の値は `edge_to_vertex_values` で子側の頂点に対応付ける。`depth` と `dist` は辺数に基づく。同じ木でも、根ごとに別のインスタンスを構築する。

## 使用例

```cpp
vc<pair<int, int>> es = {{0, 1}, {1, 2}, {1, 3}};
vc<tuple<int, int, ll>> weighted_edges = {{0, 1, 5}, {1, 2, 7}, {1, 3, 11}};
RootedTree tree(4, es, 0);

auto values = tree.edge_to_vertex_values(weighted_edges, 0LL);
// values == {0, 5, 7, 11}：頂点番号順

auto depths = tree.weighted_depths(values);
int u = 2, v = 3, a = tree.lca(u, v);
ll distance = (depths[u] - depths[a]) + (depths[v] - depths[a]);  // 18

// HLD の区間クエリには行きがけ順の配列を使う。
FenwickTree<GroupAddSub<ll>> fw(tree.reordered_vertex_values(values));
ll sum = 0;
tree.path_query(u, v, [&](int l, int r, bool) {
  sum += fw.sum(l, r);
}, true);  // 辺の値を集計する
```

木 DP は `bottom_up_vertices()` で子から親へ処理できる。

```cpp
for (int v : tree.bottom_up_vertices()) {
  for (int c : tree.children(v)) {
    // dp[c] を使って dp[v] を更新する。
  }
}
```

## 詳細なドキュメント

### RootedTree

#### コンストラクタ

```cpp
(1) RootedTree()
(2) RootedTree(int n, const vc<I>& par)
(3) RootedTree(int n, const vc<P>& es, int root)
```

- (1)：デフォルトコンストラクタ。構築済みの木を代入して使う。
- (2)：親配列から構築する。根は `par[root] < 0` または `par[root] == root` で表す。
- (3)：端点の組 `(u, v)` の列と根から構築する。

##### 制約

- (2), (3)：$n \geq 1$
- (2)：`par` は長さ $n$ で、ちょうど一つの根を持つ木を表す
- (3)：`es` は長さ $n-1$ で、頂点 $0,\ldots,n-1$ の無向木を表す
- (3)：$0 \leq \mathrm{root} < n$

##### 計算量

- (1)：$O(1)$
- (2), (3)：$O(n)$

#### size, root

```cpp
(1) I size<I = ll>() const
(2) int root() const
```

- (1)：頂点数を返す。
- (2)：根の頂点番号を返す。

##### 計算量

- $O(1)$

#### depth, subtree_size

```cpp
(1) I depth<I = ll>(int v) const
(2) I subtree_size<I = ll>(int v) const
```

- (1)：根から `v` までの辺数を返す。
- (2)：`v` 自身を含む部分木の頂点数を返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(1)$

#### parent

```cpp
int parent(int v) const
```

`v` の親の頂点番号を返す。

##### 制約

- $0 \leq v < n$
- $v \neq \mathrm{root}()$

##### 計算量

- $O(1)$

#### children, light_children, heavy_child, head

```cpp
(1) auto children(int v) const
(2) auto light_children(int v) const
(3) int heavy_child(int v) const
(4) int head(int v) const
```

- (1)：子の頂点番号を走査する範囲を返す。
- (2)：light edge でつながる子を走査する範囲を返す。
- (3)：heavy child を返す。葉では `-1`。
- (4)：`v` が属する heavy path の先頭を返す。

(1), (2) の範囲には `empty()` と `to_v()` がある。`to_v()` は頂点番号の配列を返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- 範囲の取得・(3), (4)：$O(1)$
- 範囲の走査・`to_v()`：含まれる頂点数に比例

#### parent_child, cut_and_subtree_size

```cpp
(1) pair<int, int> parent_child(int u, int v) const
(2) pair<I, I> cut_and_subtree_size<I = ll>(int u, int v) const
```

- (1)：辺の端点を `(親, 子)` の順に返す。
- (2)：辺を切断したときの、`u` 側と `v` 側の連結成分の頂点数を返す。

##### 制約

- $0 \leq u,v < n$
- $(u,v)$ は木の辺

##### 計算量

- $O(1)$

#### preorder, postorder, preorder_select

```cpp
(1) int preorder(int v) const
(2) int postorder(int v) const
(3) int preorder_select(int i) const
```

- (1)：HLD の行きがけ順での位置を返す。
- (2)：`v` の部分木に対応する半開区間の右端を返す。
- (3)：行きがけ順で `i` 番目の頂点を返す。

`v` の部分木は `[preorder(v), postorder(v))` に対応する。

##### 制約

- (1), (2)：$0 \leq v < n$
- (3)：$0 \leq i < n$

##### 計算量

- $O(1)$

#### top_down_vertices, bottom_up_vertices, dfs_ordered_vertices

```cpp
(1) const vc<int>& top_down_vertices() const
(2) vc<int> bottom_up_vertices() const
(3) pair<string, vc<int>> dfs_ordered_vertices() const
```

- (1)：親が子より先に現れる行きがけ順の頂点列を返す。
- (2)：(1) の逆順の頂点列を返す。
- (3)：DFS の移動方向を表す長さ $2(n-1)$ の括弧列 `bp` と、長さ $2(n-1)+1$ の頂点列 `vs` を返す。移動 `vs[i] → vs[i+1]` は `bp[i] == '('` なら子向き、`')'` なら親向き。

##### 計算量

- (1)：$O(1)$
- (2), (3)：$O(n)$

#### path_to_root, path

```cpp
(1) vc<int> path_to_root(int v) const
(2) vc<int> path(int u, int v) const
```

- (1)：`v` から根までのパス上の頂点列を返す。
- (2)：`u` から `v` までのパス上の頂点列を返す。

いずれも移動順に並べ、両端を含む。

##### 制約

- $0 \leq u,v < n$

##### 計算量

- 返す頂点数に比例

#### lca, dist, is_ancestor

```cpp
(1) int lca(int u, int v) const
(2) I dist<I = ll>(int u, int v) const
(3) bool is_ancestor(int u, int v) const
```

- (1)：最小共通祖先を返す。
- (2)：パス上の辺数を返す。
- (3)：`u` が `v` の祖先なら `true`。`u == v` も含む。

##### 制約

- $0 \leq u,v < n$

##### 計算量

- (1), (2)：$O(\log n)$
- (3)：$O(1)$

#### la, jump

```cpp
(1) int la(int v, int k) const
(2) int jump(int u, int v, int k) const
```

- (1)：`v` から親方向へ $k$ 辺進んだ頂点を返す。
- (2)：`u` から `v` へのパスを $k$ 辺進んだ頂点を返す。

該当する頂点が存在する場合はその番号、パスの長さを超えた場合は `-1` を返す。

##### 制約

- $0 \leq u,v < n$
- $k \geq 0$

##### 計算量

- $O(\log n)$

#### edge_to_vertex_values

```cpp
vc<T> edge_to_vertex_values(
  const vc<tuple<I, I, T>>& es, const T& root_value = T{}) const
```

`(u, v, value)` の列を、頂点番号順の長さ $n$ の配列に変換する。各辺の値を子側の頂点に置き、根には `root_value` を置く。辺の並び順と端点の向きは任意。

数値の和では `root_value = 0`、群の積ではその単位元を指定する。HLD 順に並べる場合は、結果を `reordered_vertex_values` に渡す。

##### 制約

- `es` はこの木の各辺をちょうど一度含む
- `T` はコピー構築・コピー代入ができる
- `root_value` を省略する場合、`T{}` が構築できる

##### 計算量

- $O(n)$（値のコピーを $O(1)$ とする）

#### weighted_depths

```cpp
vc<T> weighted_depths(vc<T> costs) const
```

`costs[v]` を辺 `(parent(v), v)` の重みとして、根から各頂点までの重みの合計を返す。入力・出力ともに頂点番号順。根の入力値は計算上 `0` に置き換える。

##### 制約

- `costs` の長さは $n$
- `T` は `0` の代入と加算を扱え、累積計算の値を表現できる

##### 計算量

- $O(n)$（値の演算を $O(1)$ とする）

#### reordered_vertex_values

```cpp
vc<T> reordered_vertex_values(const vc<T>& vertex_values) const
```

頂点番号順の値を HLD の行きがけ順に並べる。返り値の `preorder(v)` 番目が `vertex_values[v]` になる。

##### 制約

- `vertex_values` の長さは $n$

##### 計算量

- $O(n)$（値のコピーを $O(1)$ とする）

#### subtree_interval

```cpp
pair<int, int> subtree_interval(int v, bool edge = false) const
```

部分木に対応する HLD 順の半開区間を返す。`edge = true` では子側に載せた辺の値を扱い、部分木内部の辺に対応する `[preorder(v)+1, postorder(v))` を返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(1)$

#### path_query

```cpp
void path_query(int u, int v, F f, bool edge = false) const
```

パスを HLD 順の半開区間に分解し、`u` から `v` への順に `f(l, r, isrev)` を呼ぶ。`isrev = true` の区間は添字の降順、それ以外は昇順に値を集計する。

`edge = false` は両端を含む頂点値、`edge = true` は子側に載せた辺の値を扱う。辺モードでは LCA の値を除く。`u == v` の辺パスは空で、`f` の呼び出しは $0$ 回。

##### 制約

- $0 \leq u,v < n$

##### 計算量

- $O(\log n)$ に、$O(\log n)$ 回の `f` の実行時間を加えた時間
