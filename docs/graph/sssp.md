このドキュメントには AI による作成・変更が含まれています。

## 概要

`infty` の省略時（`nullptr`）は、[型ごとの既定の無限大](../utils/default_infty.md)を距離の型 `Dist` に対して使う。値や関数を明示した場合は、その指定を使う。

`ShortestPath` は `Graph` を参照して単一・複数始点最短路を求める。グラフを複数の計算用オブジェクトで共有できる。

距離の型 `Dist` は、`Cost = void` なら `ll`、それ以外は `Cost`。重みなしグラフでは1辺を距離1として扱い、復元する辺の型は `Edge<void>` になる。

## 使用例

```cpp
GraphDirected<ll> g(4, vc<tuple<int, int, ll>>{{0, 1, 2}, {1, 2, 3}});
ShortestPath sp(g);  // 有向・無向と Cost は g から推論する
auto dist = sp.solve(0);
auto path = sp.path_edge(2);
assert(path && path->size() == 2);
assert(sp.path_edge(0) && sp.path_edge(0)->empty());
assert(!sp.path_edge(3));
assert(sp.path_vertex(2) == optional<vc<int>>({{0, 1, 2}}));
assert(sp.path_vertex(0) == optional<vc<int>>({{0}}));
// dist == {0, 2, 5, INF}

GraphDirected<> unweighted(4, vc<pair<int, int>>{{0, 1}, {1, 2}});
ShortestPath bfs(unweighted);
auto hops = bfs.solve(0);  // vc<ll>{0, 1, 2, INF}

vector<ll> starts{0, 2};
auto nearest = bfs.solve(starts);  // vc<ll>{0, 1, 0, INF}
auto vertices = bfs.path_vertex(2);  // optional<vc<int>>({{2}})
```

## 詳細なドキュメント

### ShortestPath

```cpp
template <bool is_directed, class Cost, auto infty = nullptr>
struct ShortestPath;
```

`infty` には `Dist` に変換できる値、または `Dist` に変換できる値を返す引数なし関数を指定できる。`BigInteger` などを使う場合は関数を指定する。

```cpp
using R = Rational<BigInteger<>>;
GraphDirected<R> g(n, edges);
ShortestPath<true, R, numeric_limits<R>::infinity> sp(g);
```

#### コンストラクタ

```cpp
explicit ShortestPath(const Graph<is_directed, Cost>& g);
```

`g` への参照を保持する。距離・親辺は計算用オブジェクトごとに管理し、再計算すると更新する。

##### 制約

- `g` は計算用オブジェクトより長く生存すること。一時グラフからの構築はコンパイルエラーとなる。
- `g` を変更した場合は、`path_edge`・`path_vertex`・`prev_edges`・`count_paths` を呼ぶ前に再計算すること。

##### 計算量

- $O(1)$

#### solve

```cpp
vc<Dist> solve(int s, int t = -1)
template <class Starts>
vc<Dist> solve(const Starts& starts, int t = -1)
```

$s$ を始点として最短路を求め、各頂点への最短路が格納された vector を返す。$t$ を指定した場合、$t$ までの最短路が判明したらそこで打ち切る（$t$ 以外の結果が正しいとは限らない）。

`starts` には `vector<int>`・`vector<ll>` など、整数の頂点番号を列挙できる範囲を渡す。すべての始点の初期距離を $0$ とし、各頂点への最小距離を返す。重複した始点は1つとして扱う。空の始点集合では、すべての距離が `infty` になる。各始点は $0 \leq s < n$ を満たすこと。

負辺がある場合、最短のウォークとして求める。いずれかの始点から負閉路へ到達でき、その負閉路から `v` へ到達できる場合に `dist[v] == -infty` とする。始点自身の最短距離も、ほかの始点からの経路によって負になり得る。
到達不能は `infty`、それ以外は有限の最短距離を返す。

`Cost = void` の場合は直接 BFS を実行する。重み付きの場合は、次のアルゴリズムからよいものを自動で選択してくれる。なお、これらのアルゴリズムを自分で指定して呼び出すことも可能（引数はだいたい同じ、`bellman_ford`・`spfa` は始点だけを指定する）。

- `bfs`：辺のコストがすべて $1$ のとき、$O(n + m)$ 時間
- `bfs01`：辺のコストが $0$ または共通の正の値のとき、$O(n + m)$ 時間
- `dial`：辺のコストが $0$ 以上 $C$ 以下の整数のとき、$O(m + nC)$ 時間
- `dijkstra`：辺のコストが非負のとき、$O((n+m)\log(n+m))$ 時間
- `dijkstra_fibonacci`：辺のコストが非負のとき、ならし $O(m+n\log n)$ 時間
- `dijkstra_dense`：辺のコストが非負で密グラフのとき、$O(n^2)$ 時間
- `bellman_ford`：辺のコストが負でもよいとき、$O(nm)$ 時間
- `spfa`：辺のコストが負でもよいとき、最悪 $O(nm)$ 時間。明示的に指定して使う

各アルゴリズムも、第1引数に同じ始点集合を渡せる。始点の指定数を $k$ とすると、重複の処理を含む始点の読み込みに $O(k)$ 時間が追加でかかる。距離・親辺の初期化には $O(n)$ 時間かかる。

`bellman_ford` は距離の更新が止まった時点で終了する。到達可能な負閉路の影響は、従来どおり距離 `-infty` で返す。

複数種類の非負整数重みがあり、密グラフ向け Dijkstra 法を選ぶほど辺が多くなく、$nC$ がヒープを使う Dijkstra 法の計算量に対して十分小さい場合は Dial 法を自動で選択する。

`bfs01` は現在の距離と次の距離に対応する2本の `vector<int>` を bucket として使う。重み $0$ の辺では現在の bucket、正の辺では次の bucket に追加する。同じ距離の頂点を処理する順序は最短距離に影響しないため、各 bucket からは末尾要素を取り出す。

#### dial

```cpp
vc<Dist> dial(int s, int max_cost, int t = -1)
template <class Starts>
vc<Dist> dial(const Starts& starts, int max_cost, int t = -1)
```

$s$ を始点として Dial 法で最短路を求める。`max_cost` を $C$ として、$C+1$ 個の bucket を循環させる。各頂点を双方向連結リストによって bucket に高々一つだけ保持し、距離が更新された場合は $O(1)$ 時間で別の bucket に移動する。

`t` を指定した場合、`t` までの最短路が判明した時点で打ち切る。

##### 制約

- `Dist` は整数型
- $0 \leq s < n$
- $0 \leq \mathrm{max\\_cost}$
- すべての辺のコスト $w$ について $0 \leq w \leq \mathrm{max\\_cost}$
- 最短距離の計算結果が `Dist` の範囲に収まり、`infty` 未満

##### 計算量

- $O(m + nC)$

#### dijkstra_fibonacci

```cpp
vc<Dist> dijkstra_fibonacci(int s, int t = -1);
template <class Starts>
vc<Dist> dijkstra_fibonacci(const Starts& starts, int t = -1);
```

[FibonacciHeap](../ds/fibonacci_heap.md) の `decrease_key` を使って最短路を求める。戻り値・打切り・親辺の記録は `dijkstra` と同じ。`solve` からは従来の方式を選び、この方式は明示的に指定する。

##### 制約

- $0\leq s<n$
- すべての辺のコストが非負
- 有限の最短距離が `Dist` に収まり、`infty` 未満

##### 計算量

- ならし $O(m+n\log n)$

#### spfa

```cpp
vc<Dist> spfa(int s)
template <class Starts>
vc<Dist> spfa(const Starts& starts)
```

FIFO キューを使う SPFA で、始点または始点集合からの最短ウォークを求める。戻り値、重複・空の始点集合、負閉路の影響、経路復元・経路数の扱いは `bellman_ford` と共通。全頂点の距離を計算する。

距離が改善した頂点を再処理する。同じ頂点をキュー内に高々1つ保持し、負閉路の影響を受ける頂点とその到達先の距離を `-infty` にする。キューと作業配列は $O(n)$ 空間。

`solve` は負辺がある場合に `bellman_ford` を選ぶ。SPFA の速度はグラフと辺の処理順に依存するため、`spfa` で明示的に指定する。

##### 制約

- 各始点は $0 \leq s < n$
- 有限距離と緩和中の加算結果が `Dist` に十分な余裕を持って収まり、有限距離の絶対値が `infty` 未満

##### 計算量

- 最悪 $O(nm)$。初期化に $O(n)$、始点の読み込みに $O(k)$

#### prev_edges

```cpp
vc<Edge<Cost>> prev_edges() const
```

$\mathrm{prv}[v] \mathrel{:=}$ 始点集合から $v$ への最短路のひとつにおいて $v$ に行くために使った辺（経路の先頭・到達不能では $(\mathrm{from}, \mathrm{to}, \mathrm{index}) = (-1, -1, -1)$）を返す。`solve` の呼び出し後に呼ぶ。
距離が `-infty` の頂点も、親辺は $(-1,-1,-1)$ とする。

`solve` の呼び出し時に $t$ を指定していた場合、始点集合から $t$ への最短路のひとつで使った辺は正しく記録されている。

##### 制約

- `solve` が呼び出されている

##### 計算量

- $O(n)$

#### path_edge, path_vertex

```cpp
(1) optional<vc<Edge<Cost>>> path_edge(int t) const
(2) optional<vc<int>> path_vertex(int t) const
```

(1) は最短路の辺列、(2) は両端を含む頂点列を移動順に返す。距離が `infty` または `-infty` の場合は `nullopt`。経路の先頭は指定した始点のいずれかになる。始点 $t$ の最短距離が $0$ なら、辺列は値ありの空配列、頂点列は `{t}`。`solve` の呼び出し後に呼ぶ（`solve` の呼び出し時に $t$ を指定していてもよい）。

##### 制約

- 単一始点または始点集合を指定して `solve` が呼び出されている

##### 計算量

- 返す列の長さに比例

#### count_paths

```cpp
vc<mint> count_paths<mint>() const
```

$\mathrm{cnt}[v] \mathrel{:=}$ 始点集合から $v$ への最短パスの個数を返す。重複した始点は1つとして数える。最短距離が $0$ の各始点から空のパス1本を加え、最短路に使える辺に沿って個数を合計する。始点自身の最短距離が負なら、その始点からの空のパスは最短パスの個数に含めない。空の始点集合では、すべての個数が $0$ になる。`solve` の呼び出し後に呼ぶ（このとき **$t$ を指定して打ち切っていてはいけない**）。

##### 制約

- 単一始点または始点集合を指定して `solve` が呼び出されている（このとき **$t$ を指定して打ち切っていてはいけない**）
- 最短路に使える辺のみからなるグラフが DAG である

##### 計算量

- $O(n + m)$
