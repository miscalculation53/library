## 概要

木のオイラーツアー

## 詳細なドキュメント

### EulerTour

根付き木のオイラーツアーを構築し、深さ、部分木、親子関係、パス、LCA などを扱う。

`need_lca = true` の場合は、オイラーツアー上の RMQ も構築する。

#### コンストラクタ

```cpp
(1) EulerTour<need_lca = true, RMQ>()
(2) EulerTour<need_lca = true, RMQ>(int n, const vc<I>& par)
(3) EulerTour<need_lca = true, RMQ>(int n, const vc<P>& es, int root)
```

- (1)：デフォルトコンストラクタ。
- (2)：親配列 `par` から構築する。根は `par[root] < 0` または `par[root] == root` で表す。
- (3)：無向木の辺集合 `es` と根 `root` から構築する。

##### 制約

- $n \geq 1$
- (2)：`par` の長さは $n$ で、ちょうど一つの根を持つ木を表す
- (3)：`es` の長さは $n-1$ で、各辺の端点は $[0,n)$ にあり、全体が木である
- (3)：$0 \leq \mathrm{root} < n$
- `need_lca = true` の場合、`RMQ` は `ull` の区間最小値を処理できる

##### 計算量

RMQ の長さ $2n-1$ に対する構築時間を $T_{\mathrm{RMQ}}(n)$ として、

- 時間：$O(n + T_{\mathrm{RMQ}}(n))$

デフォルトの `RMQ` では $O(n \log n)$。

#### size

```cpp
I size<I = ll>()
```

頂点数 $n$ を返す。

##### 計算量

- $O(1)$

#### root

```cpp
int root()
```

根の頂点番号を返す。

##### 計算量

- $O(1)$

#### depth

```cpp
I depth<I = ll>(int v)
```

頂点 `v` の深さを返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(1)$

#### subtree_size

```cpp
I subtree_size<I = ll>(int v)
```

`v` を根とする部分木の頂点数を返す。`v` 自身も含む。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(1)$

#### cut_and_subtree_size

```cpp
pair<I, I> cut_and_subtree_size<I = ll>(int u, int v)
```

辺 $(u,v)$ を切ったときの、`u` 側と `v` 側の連結成分の頂点数をこの順に返す。

##### 制約

- $0 \leq u,v < n$
- $(u,v)$ は木の辺

##### 計算量

- $O(1)$

#### parent

```cpp
int parent(int v)
```

頂点 `v` の親を返す。

##### 制約

- $0 \leq v < n$
- $v \neq \mathrm{root}()$

##### 計算量

- $O(1)$

#### path_to_root

```cpp
vc<int> path_to_root(int v)
```

`v` から根までのパス上の頂点を、`v` から根へ向かう順に返す。両端を含む。

##### 制約

- $0 \leq v < n$

##### 計算量

- 時間：$O(\mathrm{depth}(v))$

#### path

```cpp
vc<int> path(int u, int v)
```

`u` から `v` までのパス上の頂点を順に返す。両端を含む。

##### 制約

- $0 \leq u,v < n$

##### 計算量

- 時間：$O(\mathrm{depth}(u) + \mathrm{depth}(v))$

#### children

```cpp
auto children(int v)
```

頂点 `v` の子を走査できる範囲を返す。`to_v()` で vector に変換できる。

##### 制約

- $0 \leq v < n$

##### 計算量

- 範囲の取得：$O(1)$
- 全要素の走査・`to_v()`：$O(\deg_{\mathrm{child}}(v))$

#### parent_child

```cpp
pair<int, int> parent_child(int u, int v)
```

辺 $(u,v)$ の端点を `(親, 子)` の順に返す。

##### 制約

- $0 \leq u,v < n$
- $(u,v)$ は木の辺

##### 計算量

- $O(1)$

#### reordered_edge_info

```cpp
vc<EdgeInfo> reordered_edge_info(
  const vc<P>& es,
  const vc<EdgeInfo>& edge_info)
```

`edge_info[i]` が `es[i]` の情報であるとき、長さ $n$ の配列 `res` を返す。根以外の頂点 `v` について、`res[v]` は辺 `(parent(v), v)` の情報になる。`res[root()]` は未定義。

##### 制約

- `es` の長さは $n-1$
- `edge_info` の長さは $n-1$ 以上
- `es` はこの `EulerTour` と同じ木の辺集合

##### 計算量

- 時間：$O(n)$

#### lca

```cpp
int lca(int u, int v)
```

`u` と `v` の最小共通祖先を返す。

##### 制約

- `need_lca = true`
- $0 \leq u,v < n$

##### 計算量

- `RMQ::prod` $1$ 回
- デフォルトの `RMQ` では $O(1)$

#### dist

```cpp
I dist<I = ll>(int u, int v)
```

`u` と `v` の間の辺数を返す。

##### 制約

- `need_lca = true`
- $0 \leq u,v < n$

##### 計算量

- `lca` $1$ 回
- デフォルトの `RMQ` では $O(1)$

#### is_ancestor

```cpp
bool is_ancestor(int u, int v)
```

`u` が `v` の祖先なら `true` を返す。`u == v` の場合も `true`。

##### 制約

- $0 \leq u,v < n$

##### 計算量

- $O(1)$
