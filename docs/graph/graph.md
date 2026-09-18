## 概要

有向・無向グラフを CSR で管理する。辺の配列から一度に構築し、入力順に辺番号を付ける。

`Cost = void`（既定値）は重みなしグラフを表す。辺の `.cost` は `static constexpr ll` の定数 `1` で、各辺の保存領域を使わない。`Cost = ll` などを指定すると、各辺にその型の重みを保存する。`Cost = bool` は真偽値の重みを保存する型として扱う。

重みなしグラフは [単一始点最短路](sssp.md)・[全点対最短路](apsp.md)・[最小閉路](minimum_cycle.md) にそのまま渡せる。距離や閉路長は `ll`、復元する辺は `Edge<void>` になる。

公開する辺の型は `Edge<Cost>`。`from`, `to`, `cost`, `index` を参照でき、頂点番号 `to` としても扱える。パスは `vc<Edge<Cost>>` で表す。

通常のグラフでは、内部型 `InternalEdge` に `to`, `index` と重みを保存し、走査時に CSR の行から `from` を補う。`is_erasable = true` の場合は、削除に使う `from` を含む `Edge` を保存する。どちらも `out_edges(v)` から `Edge` として走査できる。

`int` が4 bytes、`ll` が8 bytesの環境では、通常のグラフの保存要素は `Cost = void` で8 bytes、`Cost = int` / `bool` で12 bytes、`Cost = ll` で16 bytes。削除可能なグラフの保存要素は、それぞれ12、16、24 bytesになる。

## 使用例

```cpp
vc<pair<int, int>> uv = {{0, 1}, {1, 2}};
GraphUndirected<> g(3, uv);
for (const auto &e : g.out_edges(1)) {
  // e.from == 1、e.to は 0 または 2、e.cost == 1
  int v = e;
}

GraphDirected<ll> weighted(3, vc<tuple<int, int, ll>>{{0, 1, 5}, {1, 2, 7}});
GraphDirected<ll> uniform(3, uv, 9);  // 全辺の重みを9にする

GraphUndirected<void, true> erasable(3, uv);
erasable.erase_edge(0);
```

## 詳細なドキュメント

### Edge

#### コンストラクタ

```cpp
template <class Cost = void> struct Edge;

(1) Edge<Cost>()
(2) Edge<Cost>(int from, int to, Cost cost, int index = -1)  // 重み付き
(3) Edge<void>(int from, int to, int index = -1)             // 重みなし
```

- (1)：`from`, `to`, `index` を `-1` にする。
- (2), (3)：端点と辺番号、重みを指定して辺を作る。`Edge<void>::cost` は常に `1`。

`int` に変換すると `to` を返す。`operator<` は重みを比較し、重みなしの辺同士は同値として扱う。

##### 計算量

- $O(1)$（重みのコピーを $O(1)$ とする）

#### rev, rev_path

```cpp
(1) Edge<Cost> e.rev() const
(2) vc<Edge<Cost>> rev_path(const vc<Edge<Cost>>& path)
```

- (1)：端点を入れ替えた辺を返す。辺番号と重みは引き継ぐ。
- (2)：辺の順序を逆にし、各辺の端点を入れ替えたパスを返す。

##### 計算量

- (1)：$O(1)$
- (2)：$O(\lvert\mathrm{path}\rvert)$

#### dump

`LOCAL` を定義すると、`dump(e)` で `from`, `to`, `cost`, `index` を表示する。重みには `dump` で表示できる任意の型を使える。

```cpp
Edge<double> e(0, 1, 2.5, 7);
dump(e);  // Edge<double>{ from= 0, to= 1, cost= 2.5, index= 7 }
```

独自の重み型は、そのクラス内に `CPP_DUMP_DEFINE_DATA(表示したいメンバ...)` を書くなどして表示方法を定義する。`vc<Edge<Cost>>` や `g.edges()` も同じ形式で表示できる。

### Graph

```cpp
template <bool is_directed, class Cost = void, bool is_erasable = false>
struct Graph;

template <class Cost = void, bool is_erasable = false>
using GraphDirected = Graph<true, Cost, is_erasable>;
template <class Cost = void, bool is_erasable = false>
using GraphUndirected = Graph<false, Cost, is_erasable>;
```

`is_directed = true` は有向、`false` は無向。`is_erasable = true` なら構築後に辺を削除できる。

#### コンストラクタ

```cpp
(1) Graph(int n, const vc<pair<I, I>>& es)
(2) Graph(int n, const vc<pair<I, I>>& es, const Cost& dflt_cost)  // 重み付き
(3) Graph(int n, const vc<tuple<I, I, Cost>>& es)                 // 重み付き
(4) Graph(int n, const vc<Edge<Cost>>& es)
```

頂点数 $n$ と辺の情報からグラフを構築する。(1) は各辺の重みを1、(2) は `dflt_cost` にする。(3), (4) は各辺の重みを引き継ぐ。

辺番号は配列の位置 $0,1,\ldots,m-1$ に設定する。(4) でも入力の `index` をこの番号に振り直す。無向辺の両方向には同じ番号が付き、自己ループは1要素として保存する。多重辺も扱える。

##### 制約

- $n \geq 0$
- 各辺の端点は $0$ 以上 $n$ 未満

##### 計算量

- $O(n+m)$

#### 重み型の自動推論（C++20）

`GraphDirected`・`GraphUndirected` の `<...>` を省略すると、コンストラクタの引数から重み型を推論する。

| 入力 | 推論する重み型 |
| --- | --- |
| `vc<pair<I, I>>` | `void` |
| `vc<tuple<I, I, Cost>>` | `Cost` |
| `vc<pair<I, I>>` と既定重み | 既定重みの型 |
| `vc<Edge<Cost>>` | `Cost`（`Edge<void>` なら `void`） |

```cpp
vc<pair<int, int>> uv = {{0, 1}};
vc<tuple<int, int, ll>> uvw = {{0, 1, 7}};
GraphDirected g(2, uv);        // GraphDirected<void>
GraphUndirected h(2, uvw);     // GraphUndirected<ll>
GraphDirected uniform(2, uv, 3LL);  // GraphDirected<ll>
```

`GraphDirected<>` や `Graph<true>` は、既定の重み型 `void` を明示的に選ぶ書き方。自動推論には `GraphDirected g(...)` の形を使う。C++17 では従来どおり `GraphDirected<>`・`GraphDirected<ll>` などと型を指定する。

削除可否を固定した別名でも推論できる。

```cpp
template <class Cost = void>
using ErasableUndirected = GraphUndirected<Cost, true>;
ErasableUndirected g(2, uvw);  // GraphUndirected<ll, true>
```

#### size, num_of_edges

```cpp
(1) I size<I = ll>() const
(2) I num_of_edges<I = ll>() const
```

- (1)：頂点数を返す。
- (2)：現在の辺数を返す。

##### 計算量

- $O(1)$

#### out_vertices, out_edges

```cpp
(1) vc<I> out_vertices<I = ll>(int v) const
(2) auto out_edges(int v) const
```

- (1)：頂点 $v$ から出る辺の行き先を配列で返す。
- (2)：頂点 $v$ から出る `Edge<Cost>` を走査する範囲を返す。`size`, `empty`, `operator[]`, `at`, `front`, `back`, `to_v` が使える。

通常のグラフでは、走査中の辺はイテレータ内の一時的な `Edge` に復元される。ループの外で保持する場合は `Edge` としてコピーする。`operator[]`, `at`, `front`, `back` は値を返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- (1)：$O(\deg^+(v))$
- (2)：範囲の取得は $O(1)$、全要素の走査は $O(\deg^+(v))$

#### edges, adj_list

```cpp
(1) vc<Edge<Cost>> edges() const
(2) vvc<Edge<Cost>> adj_list() const
```

- (1)：現在のすべての辺を返す。順序は任意。無向グラフでは `from <= to` の向きで各辺を1回返す。
- (2)：頂点ごとの出辺を配列で返す。無向辺は両方向、自己ループは1回含む。

##### 計算量

- $O(n+m)$

#### adj_matrix_ecnt

```cpp
vvc<I> adj_matrix_ecnt<I = ll>() const
```

各頂点対を結ぶ辺の本数を隣接行列として返す。

##### 計算量

- $O(n^2+m)$

#### indegs, outdegs

```cpp
(1) vc<I> indegs<I = ll>() const
(2) vc<I> outdegs<I = ll>() const
```

各頂点の入次数・出次数を返す。無向グラフでは両者は同じ。自己ループはそれぞれ1として数える。

##### 計算量

- $O(n+m)$

#### erase_edge

```cpp
void erase_edge(int eid)
```

辺番号 `eid` の辺を削除する。残った辺の番号は維持する。隣接辺の順序は変わることがある。

##### 制約

- `is_erasable = true`
- `eid` は現在存在する辺の番号

##### 計算量

- $O(1)$

#### rev_graph

```cpp
GraphDirected<Cost> rev_graph(const GraphDirected<Cost>& g)
```

全辺の向きを反転したグラフを返す。重みを引き継ぎ、辺番号は構築時の順序で振り直す。

##### 計算量

- $O(n+m)$
