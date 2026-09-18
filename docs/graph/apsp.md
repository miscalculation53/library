## 概要

`infty` の省略時（`nullptr`）は、[型ごとの既定の無限大](../utils/default_infty.md)を距離の型 `Dist` に対して使う。値や関数を明示した場合は、その指定を使う。

`AllPairsShortestPath<is_directed, Cost, infty = nullptr>` は、`Graph` を参照して全点対最短路を求めるクラス。
同じグラフを SSSP や最小閉路の計算と共有できる。
`solve()` が重み・密度から手法を自動選択する。

距離の型 `Dist` は `ShortestPath` と共通で、`Cost = void` なら `ll`、それ以外は `Cost`。重みなしでは各辺を1として扱い、復元する辺の型は `Edge<void>` になる。

距離は SSSP と同じく最短ウォークの重みとして扱う。到達不能な組は `infty`、負閉路を経由して到達できる組は `-infty`、それ以外は有限の最短距離を返す。
負閉路がある場合に計算を打ち切りたい用途では、先に `negative_cycle()` で有無だけを調べられる。

`solve()` などの計算メソッドは距離行列を保存し、グラフを含めて空間 $O(n^2+m)$ を使う。
始点ごとに結果を消費する用途では `for_each_source` を使える。重みなしの場合は全始点 BFS を行い、空間 $O(n+m)$ で各行を渡す。

## 使用例

```cpp
GraphDirected<ll> g(n, edges);
AllPairsShortestPath ap(g);
if (ap.negative_cycle()) {
  // 負閉路あり。距離行列を計算せずに報告する。
} else {
  const auto &d = ap.solve();
  // d は n x n の距離行列。到達不能は INF。
}

GraphDirected<> unweighted(4, vc<pair<int, int>>{{0, 1}, {1, 2}});
AllPairsShortestPath hops(unweighted);
const auto &dist = hops.solve();  // vvc<ll>、dist[0] == {0, 1, 2, INF}
```

負閉路の影響を受ける組まで求める場合は、そのまま `solve()` を呼ぶ。

```cpp
const auto &walk = ap.solve();
if (walk[s][t] == -INF) {
  // s から t へのウォークの重みを際限なく小さくできる。
} else if (walk[s][t] == INF) {
  // s から t へ到達不能。
} else {
  // walk[s][t] は有限の最短距離。
  auto path = ap.path(s, t);
}
```

## 詳細なドキュメント

### AllPairsShortestPath

`Dist` は距離の型、`Matrix` は `vvc<Dist>`。`infty` を明示する場合は `Dist` に変換できる値、またはその値を返す引数なし関数を指定する。

#### コンストラクタ

```cpp
explicit AllPairsShortestPath(const Graph<is_directed, Cost>& g);
```

`g` への参照を保持する。有向・無向と `Cost` は `AllPairsShortestPath ap(g)` と書けば推論される。
距離・親辺の行列は計算時に確保する。内部で SSSP を反復するときも同じ `g` を参照する。
重み付きの Johnson では、重みを変換した作業用グラフを別に構築する。

##### 制約

- `g` は計算用オブジェクトより長く生存すること。一時グラフからの構築はコンパイルエラーとなる。
- `g` を変更した場合は、距離・復元情報・負閉路判定を使う前に再計算すること。

##### 計算量

- $O(1)$

#### solve

```cpp
const vvc<Dist> &solve();
```

各頂点間の最短ウォークの重みを返す。到達不能は `infty`、負閉路を経由して到達できる組は `-infty`。
対角成分は、負閉路を経由して戻れる場合は `-infty`、それ以外は空のパスにより 0。
戻り値は内部行列への参照であり、再計算・オブジェクトの破棄で内容が変更または無効化される。
`auto d = ap.solve()` と書くとコピーになり、`const auto &d = ap.solve()` ならコピーを避けられる。

`Cost = void` の場合は直接全始点 BFS を選ぶ。重み付きの選択規則は次のとおり。

1. 全辺が非負で、正の重みが高々 1 種類なら全始点 BFS / 0-1 BFS。
2. それ以外で $n^2<16m$ なら Warshall–Floyd。
3. 疎で負辺があるなら Johnson。
4. 疎で非負なら `ShortestPath::solve(s)` の繰り返し。小さい整数重みには Dial も使われる。

密度の係数 16 は既存 SSSP と合わせた目安であり、全入力で最速となる保証ではない。
各手法を明示的に呼ぶこともできる。

グラフのどこかに負閉路があれば `negative_cycle()` を `true` にし、影響を受ける組だけを `-infty` にする。距離行列は常に $n\times n$。
具体的には、`s` から負閉路へ到達でき、その負閉路から `t` へ到達できる場合に `d[s][t] == -infty` となる。
Johnson が負閉路を検出した場合は Warshall–Floyd へ切り替える。検出で終了する用途では、計算前に `negative_cycle()` を呼ぶ。
無向の負辺は逆向きの往復による負閉路として扱われる。

以下の計算メソッドも `solve()` と同じ戻り値・負閉路判定・復元 API を使う。

#### for_each_source

```cpp
template <class F>
bool for_each_source(const F& f) const;
```

始点順に `f(s, dist, prev)` を呼ぶ。`dist` は `vc<Dist>`、`prev` は `vc<Edge<Cost>>` の参照で、`prev[v]` は頂点 `v` に入る親辺。
有向・無向とも辺の重みと番号は元のグラフに対応する。負閉路の有無によらず全始点について呼び、負閉路がなければ `true`、あれば `false` を返す。
距離が `infty` または `-infty` の頂点の親辺は `index == -1`。

`solve()` と同じ選択規則を使う。全始点 SSSP が選ばれた場合は距離行列を保存せず、各行を計算して渡す。
Floyd・Johnson が選ばれた場合は作業用の APSP を計算してから各行を渡す。
このオブジェクトに保存済みの `solve()` の結果・復元情報・`negative_cycle()` は保持される。

グラフを含む空間は、全始点 SSSP では $O(n+m)$、Floyd・Johnson では $O(n^2+m)$。Dial ではさらに $O(C)$ を使う。

##### 制約

- `f` に渡す距離・親辺への参照は、その呼び出し中に利用すること。保存する場合はコピーする。
- `f` は処理中に参照先のグラフを変更しないこと。

##### 計算量

- `solve()` と同じオーダーに、`f` を $n$ 回呼ぶ時間を加えたもの。

#### repeated_sssp

```cpp
const vvc<Dist>& repeated_sssp();
```

各始点で既存の `ShortestPath::solve` を実行する。**全辺非負**を前提とする。
最短距離と親辺の番号を保存する。

#### floyd_warshall

```cpp
const vvc<Dist>& floyd_warshall();
```

最短距離行列を Warshall–Floyd で更新する。
直接辺が複数ある場合は最小重みのものを初期値にし、親辺も保存する。
距離の更新は狭義の改善時だけ行い、重み 0 の閉路があっても復元可能な親辺を管理する。
更新後、負の対角成分を持つ頂点 `k` について、`s` から `k`、`k` から `t` に到達できる組を `-infty` にする。
負閉路の反復による距離の増幅は `-infty` で打ち切る。

#### johnson

```cpp
const vvc<Dist>& johnson();
```

`Cost = void` の場合は、各辺の重み1に対して全始点 BFS を行う。

重み付きの場合は全頂点へ重み 0 でつながる仮想始点から Bellman–Ford を 1 回だけ実行し、ポテンシャル $h$ を求める。
負閉路がなければ辺を

$$w'(u,v)=w(u,v)+h(u)-h(v)\ge0$$

と変換して、全始点 Dijkstra を行う。最後に

$$d(s,t)=d'(s,t)-h(s)+h(t)$$

で元の重みに戻す。グラフ自体や元の辺番号は変更しない。
Bellman–Ford は更新前の配列から次の配列へ遷移し、1 周で経路の辺数が 1 本増える形にしている。
負閉路があれば Warshall–Floyd へ切り替え、各頂点対の有限値・`infty`・`-infty` を求める。

#### negative_cycle

```cpp
bool negative_cycle() const;
```

グラフ全体の負閉路の有無を返す。
距離の計算前は、Johnson と共通の Bellman–Ford で有無だけを判定する。距離行列を確保せず、グラフを含む空間は $O(n+m)$。
計算後は保存済みの判定結果を返す。

事前判定では距離・ポテンシャルを保存しない。続く `solve()` / `johnson()` は改めて計算するが、負閉路がない場合の Johnson の計算量のオーダーは変わらない。

##### 計算量

- 計算前：$O(n(n+m))$。重みなしは $O(1)$、全辺非負なら $O(n+m)$。
- 計算後：$O(1)$。

#### path / prev_edges

```cpp
vc<Edge<Cost>> path(int s, int t) const;
vc<Edge<Cost>> prev_edges(int s) const;
```

計算後に呼ぶ。グラフに負閉路があっても、有限の最短距離を持つ組について経路を復元できる。

- `path(s,t)`：最短パスの辺を順に返す。距離が `infty`・`-infty`、または $s=t$ なら空。
- `prev_edges(s)`：各頂点へ入る親辺を返す。始点・距離が `infty` または `-infty` の頂点は `index == -1`。
- 辺の `index` は入力辺列の添字。無向の場合、返す辺の向きは復元パスに合わせる。
- Johnson の場合も返す辺の重みは元の重み。

復元は `path` がパス長に比例する時間、`prev_edges` が $O(n)$。
内部では親辺の番号だけを $n^2$ 個保存し、`Edge<Cost>` を全組分保持しない。

#### 計算メソッド共通の制約・計算量

##### 制約

`Cost` は `void`、または加減算・比較が可能な符号付きの数値型を使う。距離の型 `Dist` は `Cost = void` なら `ll`。
`infty` は `Dist` に変換できる値か、それを返す引数なし関数を指定する。
有限の最短距離・ポテンシャル・非負化した距離と、それらの加減算の中間値が `Dist` の範囲に収まり、
絶対値が `infty` 未満であることを前提とする。
例えば最大絶対重み $W$ に対し $(6n+3)W<\mathtt{infty}$ が成り立ち、その範囲が `Dist` に収まれば十分。
負閉路のある入力でも Bellman–Ford の $n$ 辺分の加算が収まる必要がある。

##### 計算量

以下は多重辺も含む二分ヒープ実装の計算量。

| 手法 | 時間 |
| --- | --- |
| 全始点 BFS / 0-1 BFS | $O(n(n+m))$ |
| 全始点 Dial（最大重み $C$） | $O(n(n+m+nC))$ |
| 全始点 Dijkstra | $O(n(n+m\log(m+2)))$ |
| Warshall–Floyd | $O(n^3+n+m)$ |
| Johnson（重み付き・負閉路なし） | $O(n(n+m)+n(n+m\log(m+2)))$ |
| Johnson（負閉路あり、Floyd へ切り替え） | $O(n^3+nm)$ |

構築後の作業空間は距離・親辺・グラフを含め $O(n^2+m)$。
全始点 Dial が選ばれた場合は bucket のためにさらに $O(C)$。
単純グラフで $m=\Omega(n)$ なら、Dijkstra・Johnson は通常の $O(nm\log n)$ と書ける。

## 参考・検証

- [Johnson の説明（NetworkX）](https://networkx.org/documentation/stable/reference/algorithms/generated/networkx.algorithms.shortest_paths.weighted.johnson.html)
- `verify/aoj/apsp.test.cpp`：AOJ GRL_1_C 用。
- `verify/mytest/ai/apsp.test.cpp`：独立した Bellman–Ford との比較、計算前の負閉路判定、有限値・両方の無限大の混在、負閉路があるグラフの経路復元、重みなし・重み 0・多重辺・全手法の一致を確認。
