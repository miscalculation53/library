このドキュメントには AI による作成・変更が含まれています。

## 概要

無向二部グラフと $0/1$ 彩色から Dulmage–Mendelsohn 分解（DM 分解）を求める。色 $0$ を左、色 $1$ を右とし、領域 $V_0$・中央の balanced な成分・領域 $V_\infty$ へ分ける。分割は選んだ最大マッチングによらず定まる。

各成分の左右の頂点、頂点ごとの成分番号、成分間の DAG、使用した最大マッチングを返す。元の頂点番号を使い、孤立点・多重辺・重み付きグラフ・辺削除可能なグラフに対応する。

最大マッチングには [bipartite_matching](bipartite_matching.md)、分解には [SCC](../graph/scc.md)、成分間の DAG には [グラフの縮約](../graph/contraction.md) を使う。最大マッチングを渡す場合、分解部分は線形時間になる。

## 使用例

```cpp
#include "flow/dm_decomposition.hpp"

GraphUndirected<> g(8, vc<pair<int, int>>{
    {0, 1}, {2, 1}, {4, 3}, {4, 5}, {6, 7}, {6, 1}});
vc<int> color{0, 1, 0, 1, 0, 1, 0, 1};
auto dm = dm_decomposition(g, color);
assert(dm.size() == 3 && dm.matching.size() == 3);
assert(dm.left[0].to_v() == vc<int>({0, 2}));
assert(dm.right[0].to_v() == vc<int>({1}));
assert(dm.left[1].to_v() == vc<int>({6}));
assert(dm.right[1].to_v() == vc<int>({7}));
assert(dm.left[2].to_v() == vc<int>({4}));
assert(dm.right[2].to_v() == vc<int>({3, 5}));
assert(dm.ids[6] != dm.ids[1]); // 辺6--1はどの最大マッチングにも使われない
assert(dm.dag.out_edges(0).front().to == 1);

// 既に求めた最大マッチングを使う場合
const auto supplied = dm_decomposition(g, color, dm.matching);
assert(supplied.ids == dm.ids);
```

## 詳細なドキュメント

### DMDecomposition

```cpp
struct DMDecomposition {
    vc<int> ids;
    CSR<int> left, right;
    GraphDirected<> dag;
    vc<pair<int, int>> matching;
    int size() const;
};
```

成分数を $b$ とする。`size()` は $b$ を返す。結果は $b\ge 2$ で、次の順序を使う。

| 成分番号 | 内容 |
| --- | --- |
| `0` | 左側の頂点数が多い領域 $V_0$ |
| `1..b-2` | balanced な成分。それぞれ左右の頂点数が等しく、完全マッチングを持つ |
| `b-1` | 右側の頂点数が多い領域 $V_\infty$ |

両端の成分は空でも保持する。$V_0$ が非空なら左頂点数の方が多く、$V_\infty$ が非空なら右頂点数の方が多い。$V_0$・$V_\infty$ は複数の連結成分をまとめて含む場合がある。中央の成分はそれぞれ非空で、左右に少なくとも1頂点ずつ持つ。

- `ids[v]`：元の頂点 `v` の成分番号。
- `left[c]`・`right[c]`：成分 `c` に属する左右の元頂点番号。それぞれ昇順。
- `matching`：使用した最大マッチング。各対は `(色0の頂点, 色1の頂点)`。
- `dag`：元の異成分間の辺を、右側の成分から左側の成分へ向けて縮約したグラフ。頂点数は $b$ で、自己ループ・多重辺を含まない。

元の辺を `(u,v)`、`u` を左、`v` を右とすると `ids[v] <= ids[u]` となる。異なる成分を結ぶ辺は `dag` の `ids[v] -> ids[u]` に対応し、DAG の成分番号はトポロジカル順になる。独立な中央成分どうしの並び順は任意。

`dag` は元の辺に由来する関係を表す。文献で定義上追加する「$V_0$ が全体の最小、$V_\infty$ が全体の最大」という関係は、両端の成分の番号で表している。中央成分間の順序は `dag` の到達可能性で分かる。

##### 計算量

- `size()`：$O(1)$。

#### dm_decomposition

```cpp
template<class Cost, bool is_erasable, class Color>
DMDecomposition dm_decomposition(
    const GraphUndirected<Cost, is_erasable>& g,
    const vc<Color>& color);

template<class Cost, bool is_erasable, class Color>
DMDecomposition dm_decomposition(
    const GraphUndirected<Cost, is_erasable>& g,
    const vc<Color>& color,
    vc<pair<int, int>> matching);
```

最大マッチングを求めて分解する。第3引数を渡す場合は、その最大マッチングを再利用する。

頂点数 $0$ のグラフにも対応し、両端の空成分だけを返す。孤立した左頂点は成分 $0$、孤立した右頂点は末尾の成分に属する。重み・辺番号は使わず、削除可能グラフでは現在有効な辺を使う。引数のグラフ・彩色は保持し、結果は元のグラフから独立して所有する。

##### 制約

- `color.size() == g.size()`。
- `Color` の値を整数リテラル `0`・`1` と比較でき、各要素が $0$ または $1$。
- 各辺の両端の色が異なる。
- 第3引数は `g` の最大マッチング。各対が色 $0$ から色 $1$ への実在する辺で、頂点が重複しない。

##### 計算量

$n$ を頂点数、$m$ を辺数、$k$ を孤立点を除いた両側の頂点数の小さい方とする。

- マッチングも求める場合：$O(n+m\sqrt{k})$。
- 最大マッチングを渡す場合：$O(n+m)$。

補助空間は $O(n+m)$。

### 分解の定義と使い方

各辺を左から右へ向け、最大マッチングの各辺には逆辺も加えた交互グラフ $D$ を考える。未対応の左頂点から到達できる領域が $V_0$、未対応の右頂点へ到達できる領域が $V_\infty$ で、残りを SCC で分ける。

元の辺 `(u,v)` がいずれかの最大マッチングで使えることと、`ids[u] == ids[v]` は同値。色 $0$ の頂点をある最大マッチングで未対応にできることは成分 $0$ に属することと、色 $1$ の頂点を未対応にできることは末尾の成分に属することと同値になる。

この交互グラフは [BipartiteMatchingState::alternating_graph](bipartite_matching_state.md) と同じ向きの定義を使う。DM 分解の結果は元の頂点番号を使い、`alternating_graph` は左右を連結した内部番号を使う。

### transversal matroid との関係

独立集合 $I$ と追加候補 $e$ に対して、左頂点を $I\cup\lbrace e\rbrace$ に制限した二部グラフ $H$ を考える。$I$ を覆うマッチングで $e$ からの探索が失敗したら、そのマッチングは $H$ の最大マッチングで、未対応の左頂点は $e$ 一つだけになる。このとき $H$ の $V_0$ の左部分が fundamental circuit である。

[TransversalMatroid](../matroid/transversal_matroid.md) の `circuit(e)` は、その左部分から `e` を除いた要素番号列を返す。グラフ全体の DM 分解と区別し、候補を加えた $H$ に対する領域を使う。

## 参考文献

- Bérczi, Iwata, Kato, Yamaguchi, [Making Bipartite Graphs DM-irreducible, arXiv:1612.08828v3, §2.2](https://arxiv.org/pdf/1612.08828v3#page=7)：DM 分解の定義・性質の解説。最大マッチングと交互グラフによる構成、分割の一意性、matching-covered な成分との関係を述べる。
