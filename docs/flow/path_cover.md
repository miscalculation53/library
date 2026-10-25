このドキュメントには AI による作成・変更が含まれています。

## 概要

有向グラフの頂点・辺を、最少本数のパス・ウォークで分割・被覆する。

- パスは頂点を重複して通らない。ウォークは重複してよい。
- 分割は対象をちょうど1回ずつ、被覆は1回以上使う。
- 頂点1個の長さ0の経路を許す。頂点が対象なら孤立点も必要になる。辺が対象なら孤立点のための経路は作らない。
- DAG ではパスとウォークが一致する。

重み・辺の削除可否によらず `GraphDirected` を受け取る。削除可能グラフでは現在の辺を使い、経路の辺には元の端点・重み・辺番号を保持した `Edge<Cost>` を返す。

## 分類

$n$ は頂点数、$m$ は辺数、$T(N,M)$ は既存 [MaxFlow](maxflow.md) の計算量。構成の出力長を $L$ とする。

| 番号 | グラフ | 経路 | 対象・回数 | 実装・解法 |
|---|---|---|---|---|
| (1) | DAG | パス | 頂点をちょうど1回 | `dag_vertex_path_partition`。二部マッチング、$n-\lvert M\rvert$。$O(n+m\sqrt n)$ |
| (2) | DAG | パス | 頂点を1回以上 | `dag_vertex_path_cover`。頂点分割後の残余グラフで最大流 |
| (3) | 一般 | ウォーク | 頂点を1回以上 | `vertex_walk_cover_size`。SCC 縮約後に (2) |
| (4) | DAG | パス | 辺をちょうど1回 | `dag_edge_path_partition`。入辺・出辺を頂点ごとに繋ぐ。$O(n+m)$ |
| (5) | DAG | パス | 辺を1回以上 | `dag_edge_path_cover`。下限1の既知の実行可能流から最大流で流し戻す |
| (6) | 一般 | ウォーク | 辺を1回以上 | `edge_walk_cover_size`。(5) と同じネットワークを使い、弱連結成分ごとに0本を補正 |
| (7) | 一般 | ウォーク | 辺をちょうど1回 | `edge_walk_partition`。Euler 路分解。$O(n+m)$ |

本数だけを求める `_size` 関数も用意する。(3), (6) は本数のみを返す。

### 表についての補足

(1), (2), (4), (5) の最大流・最小費用循環流への帰着は成立する。(3) は先に SCC を縮約し、(6) は下記の補正を行う。[頂点分割と流し戻しの説明](https://noshi91.hatenablog.com/entry/2024/05/20/050731)、[一般グラフの辺ウォーク被覆](https://atcoder.jp/contests/abc374/editorial/11102)も参照。

(2) は推移閉包上の二部マッチングでも解ける。この実装は元の辺のまま最大流を使い、推移閉包による辺数の増加を避ける。

(6) を SCC 縮約して (5) に帰着する際は、次の2点を保つ必要がある。

- **成分間の多重辺の本数を保存する**。2つの SCC の間に元の辺が2本あると、その2本を1本のウォークで両方通ることはできない。`contracted_graph` の重複辺を消す処理をそのまま使うと本数が変わる。
- 成分間の辺を持たず、**内部辺だけを持つ SCC は1本必要**。元の辺がない孤立点は0本。

実装は元のグラフを直接使い、弱連結成分ごとに、求めた流量が0でも辺があれば1へ補正する。

(3)(イ) を、元のグラフに頂点通過量の下限1を置くだけで循環流化する方法として使うと不十分。`a -> c <- b` の各頂点に自己ループがある例では、各自己ループの循環で費用0にできる。一方、`a` と `b` を同じウォークが通ることはできず、実際には2本必要になる。弱連結成分ごとの「0なら1」補正でも足りない。**先に SCC 縮約し、その DAG で (2) を解く**形なら成立する。

### 最大流を使わずに解けるもの

(1) は既存の Hopcroft–Karp 法を使う。(4) の答えは

$$\sum_v\max(0,\mathrm{outdeg}(v)-\mathrm{indeg}(v)).$$

各頂点で入辺と出辺を $\min(\mathrm{indeg},\mathrm{outdeg})$ 組ずつ繋げば達成できる。DAG なので繋いだ辺は有限のパスに分かれる。

さらに (7) も解ける。辺を持つ弱連結成分 $C$ ごとに

$$\max\left(1,\sum_{v\in C}\max(0,\mathrm{outdeg}(v)-\mathrm{indeg}(v))\right)$$

を足したものが答え。入出次数の不足を仮想辺で補って Euler 閉路を作り、仮想辺の位置で切ると構成できる。

頂点被覆・辺被覆については [bipartite_cover](bipartite_cover.md) を参照。最大マッチングを使った最小辺被覆の大きさは $n-\lvert M\rvert$ であり、これは**最小値**。

## 使用例

```cpp
#include "flow/path_cover.hpp"

GraphDirected<> g(5, vc<pair<int,int>>{{0,2}, {1,2}, {2,3}, {2,4}});
auto partition = dag_vertex_path_partition(g);
auto cover = dag_vertex_path_cover(g);
assert(partition.size() == 3);
assert(cover.size() == 2); // 頂点2を複数のパスで使える
for (const auto& [start, path] : cover) {
  int v = start;
  for (const auto& e : path) {
    assert(e.from == v);
    v = e.to;
  }
}

GraphDirected<> isolated(1, vc<pair<int,int>>{});
auto [start, path] = dag_vertex_path_partition(isolated).front();
assert(start == 0 && path.empty()); // 頂点0だけのパス

GraphDirected<> cycle(2, vc<pair<int,int>>{{0,1}, {1,0}});
assert(vertex_walk_cover_size(cycle) == 1);
assert(edge_walk_partition(cycle).size() == 1);
```

## 詳細なドキュメント

#### 経路の表現

```cpp
vc<Edge<Cost>>                   // 辺の分割・被覆の各経路
pair<int, vc<Edge<Cost>>>        // 頂点の分割・被覆の各経路: (始点, 辺列)
```

辺列は辿る順に並ぶ。各辺の `from`, `to`, `cost`, `index` は入力グラフの値を保つ。辺が対象なら各経路は1辺以上を含む。

頂点が対象なら始点を併せて返し、辺列が空のときはその頂点だけのパスを表す。頂点列は始点に各辺の `to` を順に加えて得られる。経路集合内の順序は任意。辺列には [rev_path](../graph/graph.md) も使える。

#### DAG の分割・被覆

```cpp
template <class Cost, bool is_erasable>
vc<pair<int, vc<Edge<Cost>>>> dag_vertex_path_partition(const GraphDirected<Cost,is_erasable>& g);
template <class Cost, bool is_erasable>
vc<pair<int, vc<Edge<Cost>>>> dag_vertex_path_cover(const GraphDirected<Cost,is_erasable>& g);
template <class Cost, bool is_erasable>
vvc<Edge<Cost>> dag_edge_path_partition(const GraphDirected<Cost,is_erasable>& g);
template <class Cost, bool is_erasable>
vvc<Edge<Cost>> dag_edge_path_cover(const GraphDirected<Cost,is_erasable>& g);

// それぞれの本数だけを返す。
int dag_vertex_path_partition_size(...);
int dag_vertex_path_cover_size(...);
int dag_edge_path_partition_size(...);
int dag_edge_path_cover_size(...);
```

それぞれ (1), (2), (4), (5) を解く。頂点が対象なら全頂点、辺が対象なら残っている全辺を分割・被覆する。

被覆では、既知の初期流から $T\to S$ の最大流を1回求めて経路数を減らす。初期流は、頂点被覆なら各頂点の単独パス、辺被覆なら各辺を1回ずつ使い、入出次数差を始点・終点へ接続したもの。最小費用流や一般の下限付きフローの実行可能性判定を経ずに解く。

##### 制約

- 入力グラフが DAG。

##### 計算量

- `dag_vertex_path_partition` と `_size`：$O(n+m\sqrt n)$。
- `dag_edge_path_partition` と `_size`：$O(n+m)$。
- `dag_vertex_path_cover_size`：$O(n+m+T(2n+2,m+3n))$。
- `dag_edge_path_cover_size`：$O(n+m+T(n+2,m+2n))$。
- 被覆の構成は、それぞれの `_size` の計算量に $O(n+m+L)$ を足したもの。

#### 一般の有向グラフのウォーク

```cpp
template <class Cost, bool is_erasable>
int vertex_walk_cover_size(const GraphDirected<Cost,is_erasable>& g);
int edge_walk_cover_size(const GraphDirected<Cost,is_erasable>& g);
int edge_walk_partition_size(const GraphDirected<Cost,is_erasable>& g);
vvc<Edge<Cost>> edge_walk_partition(const GraphDirected<Cost,is_erasable>& g);
```

それぞれ (3), (6), (7) を解く。(7) の構成では、各辺がちょうど1回現れ、頂点の重複を許す。

##### 計算量

- `vertex_walk_cover_size`：$O(n+m+T(2k+2,r+3k))$。$k$ は SCC 数、$r$ は縮約後の異なる辺の数。
- `edge_walk_cover_size`：$O(n+m+T(n+2,m+2n))$。
- `edge_walk_partition_size`・`edge_walk_partition`：$O(n+m)$。
