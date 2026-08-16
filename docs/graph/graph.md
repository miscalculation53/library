## 概要

グラフを表現するクラス。

### 思想（要約）

- `Edge` 構造体は `from`, `to`, `cost`, `index` を持ち、`int to` としても扱える。
  - dump でデバッグ出力できる（ただし、現時点では `Cost` が `int`, `ll` の場合のみ。そうでないときは `to` が出力される）
  - 標準出力すると `to` が出力される
- `Graph` 構造体のコンストラクタは `n` と `edges`（これは `vc<pair<I, I>>` か `vc<tuple<I, I, Cost>>`）。
  - **`add_edge` は使わない**。これはグラフを作った後 `run` を呼び忘れたりするのを防ぐため。
- パスは辺を並べたもの、つまり `vc<Edge<Cost>>` として管理する。
  - 逆向きにしたパスを `rev_path` 関数で得られるなど
- 通常の `Graph` は CSR の行から `from` を復元できるため、内部では各辺に
  `to`, `cost`, `index` だけを保存する。`out_edges(v)` からは従来どおり
  `from`, `to`, `cost`, `index` を持つ `Edge` が得られる。
- `from` が不要な走査では `out_arcs(v)` を使うと、内部表現を変換せず直接走査できる。
- たとえば `Cost = ll` では、CSR に保存する1要素は `Edge` の24 bytesから
  16 bytesになる。構築時も、全辺を完全な `Edge` として複製した一時配列を作らず、
  CSRへ直接格納する。
- `is_erasable = true` の場合は、辺の削除に `from` が必要なので完全な `Edge` を保存する。

`to`, `index` と `cost` を別々の配列にして、一様な `cost` を1個だけ保存する設計も
考えられる。この場合は1要素を8 bytesまで減らせるが、複数配列の走査による定数倍の
悪化が大きいため、標準の `Graph` では採用していない。

---

以下、`Graph` 構造体について

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) Graph<bool is_directed, class Cost, bool is_erasable = false>(int n, vc<pair<I, I>> es, const Cost dflt_cost = 1)
(2) Graph<bool is_directed, class Cost, bool is_erasable = false>(int n, vc<tuple<I, I, Cost>> es)
```

辺の情報 $(\mathrm{from}, \mathrm{to})$ または $(\mathrm{from}, \mathrm{to}, \mathrm{cost})$ を並べた vector から、グラフを構築する。`is_directed` が `true` なら有向グラフ、`false` なら無向グラフである。辺にはこの順番に番号がつく（無向の場合、$(\mathrm{from}, \mathrm{to})$ と $(\mathrm{to}, \mathrm{from})$ には同じ番号がつく）。

##### 計算量

- $O(n + m)$

#### size, num_of_edges

```cpp
(1) I size<I = ll>()
(2) I num_of_edges<I = ll>()
```

- (1)：頂点数 $n$ を返す。
- (2)：辺数 $m$ を返す。

##### 計算量

- $O(1)$

#### out_vertices, out_edges, out_arcs

```cpp
(1) vc<I> out_vertices(int v)
(2) auto out_edges(int v)
(3) auto out_arcs(int v)
```

- (1)：頂点 $v$ から出る頂点の番号の集合を返す。
- (2)：頂点 $v$ から出る `Edge` の集合を返す。各要素は `from`, `to`, `cost`,
  `index` を持つ。通常の `Graph` では各要素を内部の `GraphArc` から復元するため、
  要素への参照をループ外へ保持してはいけない。保持する場合は `Edge` としてコピーする。
- (3)：頂点 $v$ から出る内部表現の辺の集合を返す。各要素は `to`, `cost`,
  `index` を持つ。通常の `Graph` では `from` を持たない軽量な `GraphArc` であり、
  `is_erasable = true` では `Edge` である。

##### 制約

- $0 \leq v \lt n$

##### 計算量

- (1)：$O(\deg^+(v))$
- (2)：$O(1)$
- (3)：$O(1)$

#### edges

```cpp
vc<Edge<Cost>> edges()
```

$m$ 個すべての辺を返す。辺番号順とは限らない。無向グラフの場合、各辺が
$\mathrm{from} \leq \mathrm{to}$ を満たすように返す。

##### 計算量

- $O(n+m)$

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
