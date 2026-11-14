このドキュメントには AI による作成・変更が含まれています。

## 概要

辺の追加・削除で変化する森を扱う。頂点番号は $0,\ldots,n-1$。成分ごとに根を持ち、パスの積・探索・一括更新はその根を保つ。

| 型 | 値を置く場所 | テンプレート引数 |
| --- | --- | --- |
| `LinkCutTree<M>` | 頂点 | モノイド |
| `EdgeLinkCutTree<M>` | 辺の各通過方向 | モノイド |
| `LazyLinkCutTree<AM>` | 頂点 | 作用つきモノイド |
| `LazyEdgeLinkCutTree<AM>` | 辺の各通過方向 | 作用つきモノイド |

モノイド積は非可換でもよい。値の型を `S`、作用の型を `F` とする。ナビゲーションだけが必要なら `LinkCutTree<>` を使える。

補助木はスプレー木で表す。通常版と遅延更新版の空間計算量は $O(n)$。辺版は各辺を補助木のノードで表し、切った辺の領域を再利用する。以下では値と作用のコピー・演算を定数時間とする。

## 使用例

```cpp
#include "tree/link_cut_tree.hpp"
#include "algebra/acted_monoid/add_sum.hpp"

LinkCutTree<MonoidAdd<ll>> vertices({1, 2, 3});
vertices.link(1, 0);
vertices.link(2, 1);
assert(vertices.prod(0, 2) == 6);
assert(vertices.jump(0, 2, 1) == 1);
assert(vertices.lca(1, 2) == 1);

LazyEdgeLinkCutTree<ActedMonoidAddSum<ll>> edges(3);
edges.link(0, 1, 2, 7);  // 0→1 は 2、1→0 は 7
edges.link(1, 2, 3, 9);
edges.apply(0, 2, 100, 10);
assert(edges.prod(0, 2)->val == 205);
assert(edges.prod(2, 0)->val == 36);
```

## 詳細なドキュメント

### 共通の操作

#### コンストラクタ・size

```cpp
Tree()
explicit Tree(int n)
explicit VertexTree(const vc<T>& values)
VertexTree(initializer_list<S> values)
int size() const
```

`Tree` は各公開型、`VertexTree` は通常版または遅延更新版の頂点版を表す。構築直後は全頂点が孤立し、それぞれが自身の成分の根になる。頂点の初期値を省略した場合は単位元で初期化する。配列の要素は頂点番号順で、各要素から `S` を構築する。デフォルト構築および移動後の移動元は頂点数 $0$。

`ActedMonoidAddSum<ll>` で全頂点の値を $0$ にする場合は `vc<ll>(n, 0)` を渡す。各頂点が長さ $1$ として初期化される。

##### 制約

- $n \geq 0$
- 頂点を指定する以下の関数では、各頂点番号は $0$ 以上 $n$ 未満

##### 計算量

- 構築は $O(n)$。デフォルト構築と `size` は $O(1)$。

#### link・cut・cut_parent

```cpp
bool link(int u, int v)                             // 頂点版
bool link(int u, int v, const S& x)                  // 辺版
bool link(int u, int v, const S& x, const S& y)       // 辺版
bool cut(int u, int v)
bool cut_parent(int v)
```

- `link` は別成分の `u` と `v` をつないで `true` を返す。`u` を `v` の子とし、連結後の根は `v` 側の元の根になる。辺版では `u → v` の値を `x`、`v → u` の値を `y` にする。`y` を省略した場合は両方向とも `x`。
- `cut` は端点の指定順にかかわらず辺を削除する。`cut_parent` は `v` とその親を結ぶ辺を削除する。削除時は `true` を返す。元の根を含む側は根を保ち、子側は取り外した子が根になる。
- 同じ成分に対する `link`、辺が存在しないときの `cut`、根に対する `cut_parent` は `false` を返し、森と根を保つ。

##### 計算量

- 償却 $O(\log n)$

#### same・root・reroot・parent・lca

```cpp
bool same(int u, int v)
int root(int v)
void reroot(int v)
optional<int> parent(int v)
optional<int> lca(int u, int v)
```

`same` は連結判定、`root` は成分の現在の根、`reroot` は根の付け替えを行う。`parent` と `lca` は現在の根を基準にする。根の親と、別成分の二頂点の LCA は `nullopt`。

##### 計算量

- 償却 $O(\log n)$

#### prod・dist・jump

```cpp
optional<S> prod(int u, int v)
optional<int> dist(int u, int v)
template <class I> optional<int> jump(int u, int v, I k)
```

`prod` は `u` から `v` への順に値を積む。頂点版は両端を含む頂点値、辺版は各辺の通過方向の値を使う。`prod(v, v)` は頂点版では `v` の値、辺版では値ありの単位元。別成分では `nullopt`。

`dist` は辺数、`jump` は `u` から `k` 辺進んだ頂点を返す。別成分では両方とも `nullopt`。`jump` は `k` が $0$ からパスの辺数までの範囲外でも `nullopt`。

##### 制約

- `I` は整数型。`i128` と `u128` も使える。

##### 計算量

- 償却 $O(\log n)$

### 頂点版と辺版の操作

#### get・set

```cpp
S get(int v)                                     // 頂点版
void set(int v, const S& x)                       // 頂点版
optional<S> get(int u, int v)                     // 辺版
bool set(int u, int v, const S& x)                // 辺版
bool set(int u, int v, const S& x, const S& y)     // 辺版
```

頂点版は `v` の値を取得・代入する。辺版の `get` と引数3個の `set` は `u → v` の値を対象とする。引数4個の `set` は、さらに `v → u` の値を `y` にする。辺が存在する場合に `set` は `true` を返す。辺が存在しないときの `get` は `nullopt`、`set` は `false`。

##### 計算量

- 償却 $O(\log n)$

#### first_vertex・first_edge

```cpp
template <class Pred> optional<int> first_vertex(int u, int v, Pred&& pred)
template <class Pred> optional<pair<int, int>> first_edge(int u, int v, Pred&& pred)
```

`u` から順に非空の接頭辞を取り、その積で初めて `pred` が真になる最後の頂点または辺を返す。辺の端点は通過順。別成分、空パスの辺検索、条件を満たす非空の接頭辞がない場合は `nullopt`。

##### 制約

- 各接頭辞を短い方から見たとき、判定結果は偽が続いた後に真が続く。偽のみ、真のみでもよい。
- `pred` は `const S&` を受け取る。同じ集約値に対する判定を一定に保ち、探索対象の森をコールバックから操作しない。

##### 計算量

- `pred` を $O(1)$ として償却 $O(\log n)$。呼び出し回数も償却 $O(\log n)$。

### 遅延更新版の操作

#### apply

```cpp
bool apply(int u, int v, const F& f)
bool apply(int u, int v, const F& f, const F& g)   // 辺版
```

頂点版は両端を含む頂点に `f` を作用させる。辺版は、パス上の各辺について通過方向の値に `f`、逆方向の値に `g` を作用させる。`g` を省略した場合は両方向に `f` を使う。

連結な二頂点に対して `true` を返す。辺版で `u == v` の場合は空の操作として成功し、別成分の場合は `false`。

##### 制約

- `AM::mapping` は同じ作用を各要素に適用した積を、元の積から計算できる。空でない二列の連結に対して `op` と両立すること。
- `AM::composition(f, g)` は、先に `g`、次に `f` を適用する作用を表す。
- 区間長を持つ作用つきモノイドで初期値を与えるときは、実際の頂点値・各方向の辺値を長さ $1$ で作る。空の集約の単位元は長さ $0$。

##### 計算量

- 償却 $O(\log n)$

## 参考文献

- Link-Cut Tree のスプレー木による構成と償却計算量。 [Sleator and Tarjan, Self-Adjusting Binary Search Trees, §6 (1985)](https://www.cs.cmu.edu/~sleator/papers/self-adjusting.pdf)
