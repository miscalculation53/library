## 概要

有向・無向グラフで、重みの和が最小の単純閉路を求める。
グラフ全体、指定頂点、指定辺について重みと辺列を返し、全頂点・全辺について重みの一覧も求められる。

`Cost = void` のグラフでは各辺を1として扱い、辺数が最小の閉路を求める。距離・閉路長の型 `Dist` は `ShortestPath` と共通で、`Cost = void` なら `ll`、それ以外は `Cost`。重みなしで復元する辺の型は `Edge<void>` になる。

無向は非負重み、有向は負閉路のないグラフで最小閉路を求める。有向で計算中に負閉路を検出した場合は、`-infty` で報告する。
自己ループは長さ 1、異なる平行辺による無向閉路は長さ 2 として扱う。
該当する閉路がない場合は `(infty, 空の辺列)` を返す。`infty` の省略時は [型ごとの既定値](../utils/default_infty.md) を使う。

負閉路の検出時は、`solve()`・`solve_vertex(s)`・`solve_edge(eid)` が `(-infty, 空の辺列)`、`vertex_costs()`・`edge_costs()` が全要素 `-infty` の配列を返す。
`-infty` は計算を打ち切ったことを表す状態値。全件版では、各要素に関する負閉路の有無を個別に判定する代わりに、配列全体で検出を報告する。

通常の最短路の手法選択は [ShortestPath](sssp.md)・[AllPairsShortestPath](apsp.md) に任せる。
APSP を使う計算では、先に `negative_cycle()` で判定し、負閉路があればその場で報告する。
全体版と全頂点版は APSP の行単位の処理を利用し、全始点 SSSP が選ばれた場合は空間 $O(n+m)$、Floyd・Johnson では $O(n^2+m)$ を使う。Dial の場合はさらに最大重み $C$ に対して $O(C)$ を使う。
指定頂点・指定辺版の空間は $O(n+m)$（Dial はさらに $O(C)$）。

## 使用例

```cpp
vc<tuple<int, int, ll>> es = {{0, 1, 1}, {1, 2, 2}, {2, 0, 4}, {2, 3, 3}};
GraphUndirected<ll> g(4, es);
MinimumCycle mc(g);
auto [cost, cycle] = mc.solve();  // 全体。cost == 7

auto through_vertex = mc.solve_vertex(1);      // 頂点 1 を通る閉路。辺列は 1 から始まる
auto through_edge = mc.solve_edge(0);   // 辺 0 を通る閉路。辺列の先頭は辺 0
auto vertices = mc.vertex_costs();      // {7, 7, 7, INF}
auto edges = mc.edge_costs();            // {7, 7, 7, INF}

GraphUndirected<> unweighted(4, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 0}, {2, 3}});
MinimumCycle shortest(unweighted);
auto [length, path] = shortest.solve();  // ll の length == 3、vc<Edge<void>> の path
```

## 詳細なドキュメント

### MinimumCycle

```cpp
template <bool is_directed, class Cost, auto infty = nullptr>
struct MinimumCycle;
using Dist = typename ShortestPath<is_directed, Cost, infty>::Dist;
using Result = pair<Dist, vc<Edge<Cost>>>;
```

閉路の辺列は巡回順で、無向でも `from`・`to` は進行方向に合わせる。
辺番号は参照する `Graph` の入力辺番号に対応する。

#### コンストラクタ

```cpp
explicit MinimumCycle(const Graph<is_directed, Cost>& g);
```

`g` への参照を保持する。有向・無向と `Cost` は `MinimumCycle mc(g)` と書けば推論される。

##### 制約

- `g` は計算用オブジェクトより長く生存すること。一時グラフからの構築はコンパイルエラーとなる。

##### 計算量

- $O(1)$

#### solve（全体）

```cpp
Result solve() const;
```

グラフ全体の最小閉路を返す。APSP の `for_each_source` から受け取った距離・親辺を使い、各始点を通る閉路の最小値を求める。
最適な始点の親辺を保存し、最後に閉路を復元する。
グラフのどこかに負閉路がある場合は `(-infty, 空の辺列)` を返す。

##### 計算量

- $O(T_A+n(n+m))$。$T_A$ は APSP の自動選択された手法の時間。
- 単純グラフで Floyd が選ばれた場合は $O(n^3)$。

#### solve_vertex

```cpp
Result solve_vertex(int s) const;
```

頂点 `s` を通る最小閉路を返す。辺列は `s` から始まる。
`ShortestPath::solve(s)` の距離・親辺を使い、追加 $O(n+m)$ で閉路を選ぶ。
`s` から到達可能な負閉路があれば `(-infty, 空の辺列)` を返す。判定範囲は `s` へ戻れるかによらず、探索で到達する全頂点とする。

##### 制約

- $0\le s<n$

##### 計算量

- $O(T_S+n+m)$。$T_S$ は SSSP の自動選択された手法の時間。

#### solve_edge

```cpp
Result solve_edge(int eid) const;
```

辺 `eid` を通る最小閉路を返す。辺列の先頭が指定辺となる。
指定辺を除いたグラフで両端間の最短路を求め、その辺を足す。復元時に辺番号を元のグラフの番号へ戻す。
自己ループなら、非負の場合はその 1 辺、負の場合は `(-infty, 空の辺列)` を返す。
帰路の探索始点から到達可能な負閉路を検出した場合、または指定辺を足して負の閉路が得られた場合も `(-infty, 空の辺列)` を返す。

##### 制約

- $0\le eid<m$

##### 計算量

- $O(T_S+n+m)$。$T_S$ は指定辺を除いたグラフに対する SSSP の時間。

#### vertex_costs

```cpp
vc<Dist> vertex_costs() const;
```

各頂点を通る最小閉路の重みを頂点番号順に返す。該当する閉路がない要素は `infty`。
**有向・無向とも APSP の結果を利用する。** 指定頂点版と同じ閉路判定を、距離・親辺の各行に適用する。
有向でグラフのどこかに負閉路がある場合は、長さ $n$ の全要素 `-infty` の配列を返す。

##### 計算量

- $O(T_A+n(n+m))$。単純グラフで Floyd が選ばれた場合は $O(n^3)$。

#### edge_costs

```cpp
vc<Dist> edge_costs() const;
```

各辺を通る最小閉路の重みを辺番号順に返す。該当する閉路がない要素は `infty`。
有向は APSP の距離から求める。無向は最初に通る辺が異なる経路を 2 種類保持する専用の Dijkstra を全始点で行う。
有向でグラフのどこかに負閉路がある場合は、長さ $m$ の全要素 `-infty` の配列を返す。
**無向の各辺版を APSP の結果から求める処理は未実装。** 無向の各辺版の空間は $O(n+m)$。

##### 計算量

- 有向：$O(T_A+n+m)$
- 無向：$O(n(n+m\log(m+2)))$

#### 共通の制約

- 無向は全辺非負。有向で最小閉路を求める場合は負閉路がないこと。
- `Cost` は `void`、または加減算・比較ができる符号付きの数値型。
- `infty` を明示する場合は `Dist` に変換できる正の値、またはその値を返す引数なし関数を指定する。
- 有限の答え・必要な最短距離は `infty` 未満であること。閉路候補の非負の加算は `infty` で打ち切る。
- SSSP・APSP 内の計算は、それぞれのライブラリの係数型・中間値の条件を満たすこと。

有向の負閉路を含む入力については、各 API の検出範囲に従って `-infty` を返す。

## 何がどの計算量でできるか

この節の表は、自己ループ・多重辺を除いた単純グラフで記す。
二分ヒープによる 1 始点最短路の時間を $D=O(n+m\log(n+1))$ とする。
孤立点などの初期化を明示しているため、よく使う $O(m\log n)$ より $n$ の項が多い。

| 求めるもの・条件 | 方法 | 時間 |
| --- | --- | --- |
| 閉路をどれか 1 個見つける | DFS（既存の `cycle_detection`） | $O(n+m)$ |
| 全体の最小閉路、重みなし、有向・無向 | 全始点 BFS | $O(n(n+m))$ |
| 全体の最小閉路、0/1 重み、有向・無向 | 全始点 0-1 BFS | $O(n(n+m))$ |
| 全体の最小閉路、非負重み、有向・無向 | 全始点 Dijkstra＋辺の走査 | $O(nD)$ |
| 全体の最小閉路、整数重み $0\le w\le C$ | 全始点 Dial | $O(n(n+m+nC))$ |
| 全体の最小閉路、非負重み、密グラフ | 全始点の密 Dijkstra または Floyd–Warshall の変形 | $O(n^3)$ |
| 指定有向辺 $u\to v$ を通る最小閉路、非負重み | $w(u,v)+d(v,u)$ | $O(D)$ |
| 指定無向辺 $e=\{u,v\}$ を通る最小閉路、非負重み | $w(e)+d_{G-e}(u,v)$ | $O(D)$ |
| 全体の最小閉路、無向・非負重みの素朴解 | 全辺について上の処理 | $O(mD)$ |
| 指定頂点 $s$ を通る最小閉路、有向・非負重み | $\min_{v\to s}(d(s,v)+w(v,s))$ | $O(D)$ |
| 指定頂点 $s$ を通る最小閉路、無向・非負重み | $s$ を辺ごとに分割し、多始点 Dijkstra | $O(D)$ |
| 各辺を通る最小閉路をすべて、有向・非負重み | 全始点最短路＋各辺の帰路 | $O(nD)$ |
| 各辺を通る最小閉路をすべて、無向・非負重み | 最初の辺のラベルを 2 種類保持する全始点 Dijkstra | $O(nD)$ |
| 各頂点を通る最小閉路をすべて、非負重み | 各辺の答えを接続頂点へ配る | 各辺の計算後 $O(n+m)$ |
| 有向、負辺あり・負閉路なし | Johnson 法で非負化して全始点 Dijkstra | $O(nm+nD)$ |
| 有向、負辺あり・負閉路なし、密グラフ | Floyd–Warshall＋各辺の帰路 | $O(n^3)$ |

重みがすべて同じ正数の場合は BFS、0 と共通の正数の場合は 0-1 BFS でよい。
指定頂点は `solve_vertex(s)`、指定辺は `solve_edge(eid)`、全件の重みは `vertex_costs()` / `edge_costs()` で求める。
全体の最小閉路には、既存の `ShortestPath` と `AllPairsShortestPath` を使い分ける。

## 閉路の選び方

有向では、始点 $s$ への流入辺 $u\to s$ について $d(s,u)+w(u,s)$ を調べる。
各辺 $u\to v$ の値は $w(u,v)+d(v,u)$ となる。

無向の指定頂点版は、根 $s$ の最短路木で「最初に通る辺の番号」を各頂点のラベルとする。
異なるラベルの頂点を結ぶ非木辺 $\{u,v\}$ について

$$d(s,u)+w(u,v)+d(s,v)$$

を調べる。2 経路は $s$ だけで交わるため、候補は $s$ を通る単純閉路となる。
ラベルを辺番号にすることで平行辺も区別できる。$s$ の自己ループは別に候補へ加える。
この処理は距離・親辺だけを使うため、Floyd で得た最短路木にも適用できる。

無向の各辺版では、根 $s$ の各接続辺 $e=(s,v)$ を初期経路とし、$s$ を再び通らずに探索する。
各頂点に最初の辺が異なる最短経路を 2 種類まで保持する。
辺 $e=(s,v)$ の答えは、$v$ に届いた経路のうち最初の辺が $e$ と異なるものに $w(e)$ を足した値となる。
この探索は `MinimumCycle` の非公開処理で、候補管理に [TopKArray](../ds/topk_array.md) を使う。

## ABC308 Ex への適用

[Make Q](https://atcoder.jp/contests/abc308/tasks/abc308_h) では接続点 $s$ を固定し、`solve_vertex(s)` で閉路を復元する。
閉路が $s$ に接続する 2 辺以外から尻尾を選ぶ場合と、その 2 辺をそれぞれ除いて尻尾にする場合を調べる。
各頂点につき高々 3 回の指定頂点版で、密グラフでは全体 $O(n^3)$ となる。

- `verify/atcoder/abc308_h.test.cpp`：この問題での使用例。
- [公式解説](https://atcoder.jp/contests/abc308/editorial/6718)：候補の調べ方と、尻尾の先が閉路上にある場合の変形。

## 検証

- `verify/yukicoder/minimum_cycle.test.cpp`：[yukicoder No.1320](https://yukicoder.me/problems/no/1320) の全体版。
- `verify/mytest/ai/minimum_cycle.test.cpp`：重み付き・重みなしを単純閉路の全列挙と比較し、全体・指定頂点・指定辺・各頂点・各辺の値と復元を検証する。

## 参考

- [Fine-Grained Complexity and Conditional Hardness for Sparse Graphs](https://www.cs.utexas.edu/~vlr/papers/hardness16.pdf), Section 10。
- [Space Efficient Edge-Fault Tolerant Routing](https://drops.dagstuhl.de/opus/volltexte/2012/3872/pdf/33.pdf), Section 3：正重みの無向グラフで、APSP の復元情報と追加 $O(nm)$ から各辺の最小閉路を求める手法。こちらの各辺版は未実装。
