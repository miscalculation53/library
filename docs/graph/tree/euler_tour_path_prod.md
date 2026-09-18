## 概要

木のオイラーツアーとセグ木でパスの群積クエリを処理する。頂点値・辺の値を扱い、非可換な群にも対応する。

## 使用例

```cpp
using G = GroupAddSub<ll>;
vc<int> par = {-1, 0, 1};
vc<tuple<int, int, ll>> es = {{0, 1, 5}, {1, 2, 7}};
EulerTour<false> tree(3, par);
auto values = tree.edge_to_vertex_values(es, G::e());
PathProd<G> paths(3, par, values, true);  // 辺モード

assert(paths.prod(0, 2) == 12);
assert(paths.prod(1, 1) == 0);
paths.set(2, 10);  // 辺 (1, 2) の値を更新
assert(paths.prod(2) == 15);
```

## 詳細なドキュメント

### PathProd

```cpp
PathProd<G, need_lca = true, DS1, DS2, RMQ>
```

`need_lca = false` は根からの `prod(v)` 用に LCA の前計算を省略する。二頂点間の `prod(u, v)` には `need_lca = true` を指定する。

#### コンストラクタ

```cpp
(1) PathProd<G>(int n, const vc<I>& par, const vc<T>& values = {}, bool edge = false)
(2) PathProd<G>(int n, const vc<P>& es, int root, const vc<T>& values = {}, bool edge = false)
```

- (1)：親配列 `par` から構築する。
- (2)：無向木の辺集合 `es` と根 `root` から構築する。
- `edge = false`：`values[v]` を頂点 `v` の値として扱う。
- `edge = true`：`values[v]` を辺 `(parent(v), v)` の値として扱う。根の保存値は入力にかかわらず `G::e()` にする。
- `values` は頂点番号順。辺の tuple 列からは `edge_to_vertex_values(es, G::e())` で作れる。
- `values` を省略した場合は、全要素を `G::e()` で初期化する。辺モードの空初期値は `{}, true` と指定する。

##### 制約

- `G` は群
- $n \geq 1$
- (1)：`par` の長さは $n$ で、ちょうど一つの根を持つ木を表す
- (2)：`es` の長さは $n-1$ で、頂点 $0,\ldots,n-1$ の木を表す
- `values` は空、または長さ $n$

##### 計算量

- `need_lca = false`：$O(n)$
- `need_lca = true`：`EulerTour` の RMQ 構築に加えて $O(n)$
- デフォルトの `RMQ` では $O(n \log n)$

#### get

```cpp
S get(int v)
```

頂点モードでは頂点 `v`、辺モードでは辺 `(parent(v), v)` の現在値を返す。辺モードの根では `G::e()` を返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(1)$

#### set

```cpp
void set(int v, const S& x)
```

頂点モードでは頂点 `v`、辺モードでは辺 `(parent(v), v)` の値を `x` に変更する。

##### 制約

- $0 \leq v < n$
- 辺モードの更新対象は根以外の頂点

##### 計算量

- $O(\log n)$

#### prod

```cpp
(1) S prod(int v)
(2) S prod(int u, int v)
```

- (1)：根から `v` までの値を、パスの順に積んだ値を返す。
- (2)：`u` から `v` までの値を、パスの順に積んだ値を返す。

頂点モードでは両端を含む頂点値、辺モードではパス上の辺の値を集計する。辺モードでは根や LCA に載せた値を除き、同じ頂点間の結果は `G::e()`。

各辺は両方向で同じ値を使い、非可換な演算では通過順に積む。逆向きの通過でも辺の値そのものを使う。

##### 制約

- $0 \leq u, v < n$
- (2)：`need_lca = true`

##### 計算量

- $O(\log n)$
