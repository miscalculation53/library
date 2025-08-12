## グラフクラス

### 思想（要約）

- `Edge` 構造体は `from`, `to`, `cost`, `index` を持ち、`int to` としても扱える。
  - dump でデバッグ出力できる（ただし、現時点では `Cost` が `int`, `ll` の場合のみ。そうでないときは `to` が出力される）
  - 標準出力すると `to` が出力される
- `Graph` 構造体のコンストラクタは `n` と `edges`（これは `vc<pair<I, I>>` か `vc<tuple<I, I, Cost>>`）。
  - **`add_edge` は使わない**。これはグラフを作った後 `run` を呼び忘れたりするのを防ぐため。
- パスは辺を並べたもの、つまり `vc<Edge<Cost>>` として管理する。
  - 逆向きにしたパスを `rev_path` 関数で得られるなど



---

以下、`Graph` 構造体について

### コンストラクタ

```cpp
(1) Graph<bool is_directed, class Cost>(int n, vc<pair<I, I>> es, const Cost dflt_cost = 1)
(2) Graph<bool is_directed, class Cost>(int n, vc<tuple<I, I, Cost>> es, Cost dflt_cost = 1)
```

辺の情報 $(\mathrm{from}, \mathrm{to})$ または $(\mathrm{from}, \mathrm{to}, \mathrm{cost})$ を並べた vector から、グラフを構築する。`is_directed` が `true` なら有向グラフ、`false` なら無向グラフである。辺にはこの順番に番号がつく（無向の場合、$(\mathrm{from}, \mathrm{to})$ と $(\mathrm{to}, \mathrm{from})$ には同じ番号がつく）。

##### 計算量

- $O(n + m)$


### メンバ関数

#### size, num_of_edges

```cpp
(1) I size<I = ll>()
(2) I num_of_edges<I = ll>()
```

- (1)：頂点数 $n$ を返す。
- (2)：辺数 $m$ を返す。

##### 計算量

- $O(1)$

#### out_vertices, out_edges

```cpp
(1) vc<I> out_vertices(int v)
(2) CSR<Edge<Cost>>::Row out_edges(int v)
```

- (1)：頂点 $v$ から出る頂点の番号の集合を返す。
- (2)：頂点 $v$ から出る辺の集合（を表す `CSR` の行）を返す。

##### 制約

- $0 \leq v \lt n$

##### 計算量

- $O(1)$


#### get_edge, edges

```cpp
(1) Edge<Cost> get_edge(int eid)
(2) vc<Edge<Cost>> edges()
```

- (1)：辺番号が $\mathrm{eid}$ であるような辺を取得する。無向グラフの場合、$\mathrm{from} \leq \mathrm{to}$ を満たすように返す。

- (2)：$m$ 個すべての辺を、辺番号順に返す。無向グラフの場合、各辺が $\mathrm{from} \leq \mathrm{to}$ を満たすように返す。

##### 制約

- (1)：$0 \leq \mathrm{eid} \lt m$

##### 計算量

- (1)：$O(1)$
- (2)：$O(n+m)$


#### adj_list

```cpp
vvc<Edge<Cost>> adj_list()
```

隣接リストを `vvc` として返す。（無向グラフの場合、合計 $2m$ 個の要素が格納されていることになる。）

##### 計算量

- $O(n+m)$

#### adj_matrix

```cpp
vvc<I> adj_matrix_ecnt()
```

隣接行列を返す。多重辺がある場合、辺の本数が格納される。

##### 計算量

- $O(n^2 + m)$

#### indegs, outdegs

```cpp
(1) vc<I> indegs()
(2) vc<I> outdegs()
```

- (1)：各頂点の入次数を格納した vector を返す。
- (2)：各頂点の出次数を格納した vector を返す。

##### 計算量

- $O(n+m)$

