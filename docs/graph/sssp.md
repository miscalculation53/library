## 概要

`infty` の省略時（`nullptr`）は、[型ごとの既定の無限大](../utils/default_infty.md)を距離の型 `Dist` に対して使う。値や関数を明示した場合は、その指定を使う。

TODO：
- SPFA 等を実装する
- フィボナッチヒープを用いた Dijkstra 法の高速化

`ShortestPath` は `Graph` を参照して単一始点最短路を求める。グラフを複数の計算用オブジェクトで共有できる。

距離の型 `Dist` は、`Cost = void` なら `ll`、それ以外は `Cost`。重みなしグラフでは1辺を距離1として扱い、復元する辺の型は `Edge<void>` になる。

## 使用例

```cpp
GraphDirected<ll> g(4, vc<tuple<int, int, ll>>{{0, 1, 2}, {1, 2, 3}});
ShortestPath sp(g);  // 有向・無向と Cost は g から推論する
auto dist = sp.solve(0);
auto path = sp.path(2);
// dist == {0, 2, 5, INF}

GraphDirected<> unweighted(4, vc<pair<int, int>>{{0, 1}, {1, 2}});
ShortestPath bfs(unweighted);
auto hops = bfs.solve(0);  // vc<ll>{0, 1, 2, INF}
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
- `g` を変更した場合は、`path`・`prev_edges`・`count_paths` を呼ぶ前に再計算すること。

##### 計算量

- $O(1)$

#### solve

```cpp
vc<Dist> solve(int s, int t = -1)
```

$s$ を始点として最短路を求め、各頂点への最短路が格納された vector を返す。$t$ を指定した場合、$t$ までの最短路が判明したらそこで打ち切る（$t$ 以外の結果が正しいとは限らない）。

負辺がある場合、最短のウォークとして求める。`s` から負閉路へ到達でき、その負閉路から `v` へ到達できる場合に `dist[v] == -infty` とする。
到達不能は `infty`、それ以外は有限の最短距離を返す。

`Cost = void` の場合は直接 BFS を実行する。重み付きの場合は、次のアルゴリズムからよいものを自動で選択してくれる。なお、これらのアルゴリズムを自分で指定して呼び出すことも可能（引数はだいたい同じ、ただし `bellman_ford` だけは $t$ を指定できない）。

- `bfs`：辺のコストがすべて $1$ のとき、$O(n + m)$ 時間
- `bfs01`：辺のコストが $0$ または共通の正の値のとき、$O(n + m)$ 時間
- `dial`：辺のコストが $0$ 以上 $C$ 以下の整数のとき、$O(m + nC)$ 時間
- `dijkstra`：辺のコストが非負のとき、$O(m \log n)$ 時間
- `dijkstra_dense`：辺のコストが非負で密グラフのとき、$O(n^2)$ 時間
- `bellman_ford`：辺のコストが負でもよいとき、$O(nm)$ 時間

複数種類の非負整数重みがあり、密グラフ向け Dijkstra 法を選ぶほど辺が多くなく、$nC$ がヒープを使う Dijkstra 法の計算量に対して十分小さい場合は Dial 法を自動で選択する。

`bfs01` は現在の距離と次の距離に対応する2本の `vector<int>` を bucket として使う。重み $0$ の辺では現在の bucket、正の辺では次の bucket に追加する。同じ距離の頂点を処理する順序は最短距離に影響しないため、各 bucket からは末尾要素を取り出す。

#### dial

```cpp
vc<Dist> dial(int s, int max_cost, int t = -1)
```

$s$ を始点として Dial 法で最短路を求める。`max_cost` を $C$ として、$C+1$ 個の bucket を循環させる。各頂点を双方向連結リストによって bucket に高々一つだけ保持し、距離が更新された場合は $O(1)$ 時間で別の bucket に移動する。

`t` を指定した場合、`t` までの最短路が判明した時点で打ち切る。

##### 制約

- `Dist` は整数型
- $0 \leq s < n$
- $0 \leq \mathrm{max\_cost}$
- すべての辺のコスト $w$ について $0 \leq w \leq \mathrm{max\_cost}$
- 最短距離の計算結果が `Dist` の範囲に収まり、`infty` 未満

##### 計算量

- $O(m + nC)$

#### prev_edges

```cpp
vc<Edge<Cost>> prev_edges() const
```

$\mathrm{prv}[v] \coloneqq$ $s$-$v$ 最短路のひとつにおいて $v$ に行くために使った辺（なければ $(\mathrm{from}, \mathrm{to}, \mathrm{index}) = (-1, -1, -1)$）を返す。`solve` の呼び出し後に呼ぶ。
距離が `-infty` の頂点も、親辺は $(-1,-1,-1)$ とする。

`solve` の呼び出し時に $t$ を指定していた場合、$s$-$t$ 最短路のひとつで使った辺は正しく記録されている。

##### 制約

- `solve` が呼び出されている

##### 計算量

- $O(n)$

#### path

```cpp
vc<Edge<Cost>> path(int t) const
```

$s$-$t$ 最短路のひとつを辺の vector として返す。距離が `infty` または `-infty` の場合は空の vector を返す。`solve` の呼び出し後に呼ぶ（`solve` の呼び出し時に $t$ を指定していてもよい）。

##### 制約

- `solve(s)` か `solve(s, t)` が呼び出されている

##### 計算量

- $O(s\text{-}t\ 最短パスの辺の本数)$

#### count_paths

```cpp
vc<mint> count_paths<mint>() const
```

$\mathrm{cnt}[v] \coloneqq$ $s$-$v$ 最短パスの個数 を返す。`solve` の呼び出し後に呼ぶ（このとき **$t$ を指定して打ち切っていてはいけない**）。

##### 制約

- `solve(s)` が呼び出されている（このとき **$t$ を指定して打ち切っていてはいけない**）
- 最短路に使える辺のみからなるグラフが DAG である

##### 計算量

- $O(n + m)$
