## 概要

`Graph`・`RootedTree`・親配列から、[GRAPH × GRAPH](https://hello-world-494ec.firebaseapp.com/) の可視化リンクを作る。リンクを開くと、グラフと入力形式の設定が復元される。

有向・無向と重みの有無は `Graph` の型から選ぶ。木と親配列は親から子への有向辺で表示する。森林・孤立頂点・多重辺・自己ループも出力でき、削除可能なグラフでは現在の辺を使う。

`dump_graph` は `LOCAL` が定義されているとき、式の名前とリンクを標準エラー出力に表示する。`LOCAL` が定義されていないときは、引数の評価を省略する。

リンクの長さはグラフの大きさに比例する。大きいグラフは `graph_text` の出力をサイトの入力欄に貼り、`0-indexed` / `1-indexed`・`directed` / `undirected`・`weighted` / `unweighted` を選ぶ。

サイト側は重みを JavaScript の数値として読み込むため、大きい整数などの表示には丸めが入る。`graph_text` には元の整数値を出力し、浮動小数点数は `max_digits10` 桁の精度を使う。

## 使用例

```cpp
#include "graph/visualize.hpp"

GraphUndirected<> g(5, vc<pair<int, int>>{{0, 1}, {0, 2}, {2, 3}, {2, 4}});
dump_graph(g);  // LOCAL 時に表示されるリンクを開く
dump_graph(g, true);  // 頂点を1-indexedで表示する

GraphDirected<ll> weighted(3, vc<tuple<int, int, ll>>{{0, 1, 5}, {1, 2, -2}});
dump_graph(weighted);  // 有向・重み付きを自動選択する

RootedTree tree(5, vc<int>{-1, 0, 0, 2, 2});
dump_graph(tree);  // 親 -> 子の向きで表示する

vc<int> par = {-1, 0, -1, 2, 2};
dump_graph(par);  // 親配列で表す森林にも使える
// MergeTree は dump_graph(merges.parents()) で表示できる。

cerr << graph_url(g) << '\n';  // LOCAL の有無によらずリンクを作る
cerr << graph_text(g);  // サイトに貼る入力テキストを作る
```

## 詳細なドキュメント

#### graph_text

```cpp
(1) template <bool directed, class Cost, bool erasable>
    string graph_text(const Graph<directed, Cost, erasable>& g, bool one_indexed = false)
(2) string graph_text(const RootedTree& tree, bool one_indexed = false)
(3) template <class I>
    string graph_text(const vc<I>& par, bool one_indexed = false)
```

先頭に頂点数と辺数、その後に各辺を `u v`（重み付きなら `u v cost`）の形式で出力する。各行の末尾には改行を付ける。`one_indexed = true` のとき、出力する頂点番号に1を足す。

- (1)：各無向辺を1回ずつ出力する。辺の順序は頂点と隣接辺の走査順。
- (2)：根を除く各頂点について、親からその頂点への辺を作る。
- (3)：`par[v] < 0` または `par[v] == v` を根として扱い、それ以外は `par[v]` から `v` への辺を作る。頂点数は `par.size()`。

##### 制約

- 重み付きの場合、`Cost` を `ostream` に出力でき、出力が空白を含まない1個の数値になる。
- `RootedTree` は構築済み。
- 親配列の `I` は整数型。各要素は負の値または有効な頂点番号で、親子関係は森林を表す。

##### 計算量

- $O(n+m+L)$。$n$ は頂点数、$m$ は辺数、$L$ は出力文字数。重みの出力をその文字数に比例する時間とする。

#### graph_url

```cpp
(1) template <bool directed, class Cost, bool erasable>
    string graph_url(const Graph<directed, Cost, erasable>& g, bool one_indexed = false)
(2) string graph_url(const RootedTree& tree, bool one_indexed = false)
(3) template <class I>
    string graph_url(const vc<I>& par, bool one_indexed = false)
```

`graph_text` の出力と入力形式の設定を含む可視化リンクを返す。リンクにはグラフのデータが入る。ブラウザでリンクを開くと GRAPH × GRAPH にアクセスする。

##### 制約

- `graph_text` と同じ。
- 重みはサイトが読み込める有限の数値として出力する。

##### 計算量

- $O(n+m+L)$。$L$ は `graph_text` の出力文字数。

#### dump_graph

```cpp
dump_graph(g)
dump_graph(g, one_indexed)
```

`LOCAL` が定義されているとき、式の名前と `graph_url` の結果を標準エラー出力に表示する。引数はそれぞれ1回だけ評価する。木・親配列も `g` に渡せる。

##### 制約

- `LOCAL` が定義されている場合、`graph_url` と同じ。

##### 計算量

- `LOCAL` が定義されている場合は `graph_url` と同じ。それ以外は $O(1)$。
