このドキュメントには AI による作成・変更が含まれています。

## 概要

無向グラフを二重連結成分（ブロック）に分解し、各成分の頂点と元の辺番号を返す。各非自己ループ辺はちょうど1成分に属し、関節点は複数成分に属する。橋1本も、その両端からなる成分になる。

非連結グラフ、多重辺、重み付きグラフ、辺削除後のグラフに対応する。自己ループは分解から除き、非自己ループ辺を持たない頂点は、辺を持たない1頂点の成分として残す。この規約により、関節点と「複数の成分に属する頂点」が一致する。

グラフから直接構築する場合は、反復 DFS で探索中の頂点と非木辺を管理し、LowLink の分離条件でブロックを確定する。木辺は各頂点の親辺番号として保存し、確定したブロックの頂点・辺を連続した配列へ書き出す。

計算済みの `LowLink` を渡す場合は、DFS 順・親・low の値から各頂点の所属ブロックを求め、元の辺番号を対応するブロックへ格納する。

## 使用例

```cpp
#include "graph/biconnected_components.hpp"

vc<pair<int, int>> es{{0, 1}, {1, 2}, {2, 0}, {2, 3}};
GraphUndirected<> g(5, es);
BiconnectedComponents bcc(g);
// 頂点集合 {0,1,2}, {2,3}, {4} の3成分（順序は任意）。
for (int b = 0; b < bcc.size(); ++b)
{
  auto vs = bcc.vertices(b).to_v();
  auto eids = bcc.edges(b).to_v();
}

LowLink lowlink(g);
BiconnectedComponents reused(g, lowlink);
```

## 詳細なドキュメント

### BiconnectedComponents

#### コンストラクタ

```cpp
BiconnectedComponents();
template <class Cost, bool is_erasable>
explicit BiconnectedComponents(const GraphUndirected<Cost, is_erasable> &g);
template <class Cost, bool is_erasable>
BiconnectedComponents(const GraphUndirected<Cost, is_erasable> &g, const LowLink &lowlink);
```

デフォルト構築は頂点数0・成分数0の結果を作る。グラフのみを渡す形式では、反復 DFS で各ブロックを求める。計算済みの `LowLink` を渡す形式では、その探索結果を使って頂点一覧・辺一覧を構築する。グラフや `LowLink` のコピー・参照は保持しない。

##### 制約

- `lowlink` を渡す場合、同じグラフの同じ辺集合に対して構築したものを渡す。

##### 計算量

- $O(n+m)$。$n$ は頂点数、$m$ は存在する辺数。

#### size / num_of_vertices

```cpp
template <class I = ll> I size() const;
template <class I = ll> I num_of_vertices() const;
```

`size()` は成分数、`num_of_vertices()` は元のグラフの頂点数を返す。成分番号は `[0, size())` で、順序は任意。

##### 計算量

- 各 $O(1)$。

#### vertices / edges

```cpp
auto vertices(int id) const;
auto edges(int id) const;
```

成分 `id` の頂点番号・辺番号を列挙する、読み取り専用の CSR 行を返す。各行の要素は重複せず、並び順は任意。`edges()` の番号は入力グラフの `Edge::index` に対応する。

返り値は range-for、`size()`、`operator[]`、`to_v()` などを使える。

##### 制約

- `0 <= id < size()`。

##### 計算量

- 各 $O(1)$。列挙はその行の要素数に比例する。

#### vertex_groups / edge_groups

```cpp
const CSR<int> &vertex_groups() const;
const CSR<int> &edge_groups() const;
```

すべての成分の頂点一覧・辺一覧を保持する CSR を参照で返す。`to_vv()` で配列の配列に変換できる。

##### 計算量

- 各 $O(1)$。

成分と関節点を結ぶ森は [BlockCutTree](../tree/block_cut_tree.md) で構築できる。
