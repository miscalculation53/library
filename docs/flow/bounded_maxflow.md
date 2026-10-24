このドキュメントには AI による作成・変更が含まれています。

## 概要

各辺の流量に上限・下限があるネットワークで、最大流・最小流・実行可能循環流を求める。負の下限、自己ループ、多重辺に対応する。実行可能な $s$-$t$ 流量が負になる場合も扱う。

下限を引いた残余グラフを既存の [MaxFlow](maxflow.md) で解く。まず補助始点・終点で需給を満たし、補助辺を除いてから最大化または最小化する。作業領域は $O(n+m)$。

`Cap` は明示指定する。容量差や需給の合計が型の幅を越えるときは、同じ幅の符号なし容量の複数の辺・計算へ分ける。総和の相殺による途中のはみ出しも処理する。

## 使用例

```cpp
#include "flow/bounded_maxflow.hpp"

BoundedMaxFlow<ll> g(3);
g.add_edge(0, 1, 2, 5);
g.add_edge(1, 2, 3, 7);
auto range = g.flow_range(0, 2);
assert(range && range->first == 3 && range->second == 5);
// 最大流量 5 の解が残る。
for (auto e : g.edges()) assert(e.lower <= e.flow && e.flow <= e.upper);
```

## 詳細なドキュメント

### BoundedMaxFlow

#### コンストラクタ

```cpp
template <class Cap>
class BoundedMaxFlow;
BoundedMaxFlow();
explicit BoundedMaxFlow(int n);
```

$n$ 頂点・辺なしのネットワークを作る。既定コンストラクタは $n=0$。

##### 制約

- $n\ge0$。
- `Cap` は符号付き整数型。`i128` に対応する。
- 各辺の上下限と、返す最大・最小流量が `Cap` に収まる。

##### 計算量

- $O(1)$。

#### 辺の追加・変更・取得

```cpp
int add_edge(int from, int to, Cap lower, Cap upper);
int add_edge(int from, int to, Cap upper); // lower = 0
void change_edge(int id, Cap lower, Cap upper);
struct edge { int from, to; Cap lower, upper, flow; };
edge get_edge(int id) const;
vector<edge> edges() const;
template <class I = ll> I size() const;
template <class I = ll> I num_of_edges() const;
```

辺番号は追加順。`change_edge` は上下限を更新し、次の求解で解き直す。成功した求解の後、`flow` にその解を格納する。新規辺・変更した辺の初期流量は `lower`。`edges()` は辺番号順。

##### 制約

- $0\le\mathrm{from},\mathrm{to}<n$、`lower <= upper`。
- `id` は追加済みの辺番号。

##### 計算量

- `add_edge` は償却 $O(1)$。`edges` は $O(m)$。その他は $O(1)$。

#### 求解

```cpp
bool feasible();
optional<Cap> flow(int s, int t);
optional<Cap> min_flow(int s, int t);
optional<pair<Cap, Cap>> flow_range(int s, int t);
```

- `feasible()`：全頂点で流量保存則を満たす循環流が存在するか。
- `flow(s,t)`：実行可能な最大流量。解がなければ `nullopt`。
- `min_flow(s,t)`：実行可能な最小流量。解がなければ `nullopt`。
- `flow_range(s,t)`：実行可能な流量の最小値・最大値。存在しない場合は `nullopt`。辺の流量には最大流の解を残す。

流量は「$s$ からの流出量 $-$ $s$ への流入量」。$s,t$ 以外で保存則を満たす。負の流量も有効であり、最小流量は符号付きの最小値となる。求解は毎回、現在の辺の上下限から解き直す。流量 $0$ の実行可能解は値ありの `0` として返す。

##### 制約

- $0\le s,t<n$、$s\ne t$。

##### 計算量

- 補助ネットワークは $n+2$ 頂点、$O(n+m)$ 辺。
- 通常は `feasible` が最大流1回、`flow`・`min_flow` が最大流2回、`flow_range` が最大流3回と $O(n+m)$ の前後処理。
- より一般には $O(n+m+qT(n+2,O(n+m)))$。$T$ は既存 `MaxFlow::flow` の計算量、$q$ は需給充足・増減を同じ幅の符号なし容量へ分けた呼出し回数。$q=O(m)$。

#### min_cut

```cpp
vector<bool> min_cut(int s) const;
```

現在の残余グラフで $s$ から到達可能な頂点を返す。最大流を求めた後は **1 が $s$ 側、0 が $t$ 側**。このカットについて、最大流量は「$s$ 側から $t$ 側へ出る辺の上限の和 $-$ 逆向きに入る辺の下限の和」と一致する。

##### 制約

- 直前の求解が成功している。
- $0\le s<n$。

##### 計算量

- $O(n+m)$。
