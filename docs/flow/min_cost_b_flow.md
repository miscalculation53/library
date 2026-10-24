このドキュメントには AI による作成・変更が含まれています。

## 概要

各辺の流量上下限と費用、各頂点の供給・需要から最小費用 b-flow を求める。供給 $b_v>0$、需要 $b_v<0$ とし、各頂点で「流出量 $-$ 流入量 $=b_v$」を満たす。負の費用・負の上下限・自己ループ・多重辺を扱う。費用・各辺の流量・双対ポテンシャルを取得でき、[Library Checker の Minimum Cost b-flow](https://judge.yosupo.jp/problem/min_cost_b_flow) の入出力に対応する。

供給量を区間 $L_v\leq b_v\leq U_v$ で指定できる。`set_supply` で設定し、`add_supply` で区間を加算する。`get_supply` は最適解で選ばれた供給量を返す。幅のある区間がある場合だけ、内部に補助頂点1個と各区間の費用0の辺を追加する。公開する頂点数・辺番号・辺一覧は元のグラフに対応する。

残余グラフには [CSR](../ds/csr.md) を使う。負費用の辺を上限、それ以外を下限から開始して、残った需給を最短路で修正する。自己ループと上下限が等しい辺は、費用・需給へ反映して探索用グラフから除く。

最短路反復法（SSP）、最短路の森で複数の需要をまとめて処理する primal-dual 法、容量スケーリング法、primal network simplex を備える。自動選択は入力の規模・容量・費用から network simplex の試行を加え、供給・需要点が多い場合に primal-dual、それ以外に SSP を使う。進捗があれば試行量を延長し、探索が続く場合は独立した状態で容量スケーリング・network simplex と交互に進める。試行量を倍増し、先に完了した方式を採用する。状態保存を含む追加メモリは $O(n+m)$。

最短路は二分ヒープ・[Fibonacci heap](../ds/fibonacci_heap.md)・密グラフ向けの走査を選べる。自動では規模・辺密度・需給点の数を使って選ぶ。各方式の実測は入力によって変わる。

## 使用例

```cpp
#include "flow/min_cost_b_flow.hpp"

MinCostBFlow<int, int> g(vc<int>{2, 0, -2});
g.add_edge(0, 1, 0, 3, -2);  // from, to, lower, upper, cost
g.add_edge(1, 2, 0, 3, 5);
auto cost = g.flow();        // optional<int>
assert(cost && *cost == 6);
auto potential = g.potential();
auto edges = g.edges();

// 最適解から b[0] += x, b[2] -= x とする費用差の slope。
auto curve = g.slope(0, 2, 10);
// {{0, 0}, {1, 3}}。追加で1だけ流せる。

// 方式を明示する場合。
using G = MinCostBFlow<ll, ll>;
G h(3);
h.add_supply(0, 2);
h.add_supply(2, -2);
h.add_edge(0, 2, 2, 7);      // 下限0の短縮形
auto result = h.flow(G::Algorithm::capacity_scaling, G::Heap::fibonacci);
auto ns_result = h.flow(G::Algorithm::network_simplex);

// 供給・需要を区間で指定する場合。
G interval(2);
interval.set_supply(0, 1, 3);    // 供給量は [1, 3]
interval.add_supply(1, -3, -1);  // 需要量は [1, 3]
interval.add_edge(0, 1, 5, -2);
auto answer = interval.flow();   // optional<ll>、値は -6
ll selected = interval.get_supply(0);  // 3

// 距離・ポテンシャル・費用合計まで i128 にする場合。
MinCostBFlow<ll, i128> wide(3);
// 費用合計だけ i128 にする場合。Library Checker の入出力例もこの指定。
MinCostBFlow<ll, ll, i128> wide_total(3);
```

## 詳細なドキュメント

### MinCostBFlow

```cpp
template <class Cap, class Cost, class Total = Cost>
class MinCostBFlow;
```

`Cap`・`Cost` は `i128` を含む符号付き整数型。容量・需給は `Cap`、辺費用・ポテンシャル・距離は `Cost` で計算する。公開の型名は `Balance = Cap`・`Potential = Cost`。それぞれ計算中の値を保持できる型を指定する。

費用合計の `Total` は、既定では `Cost` と同じ型。`MinCostBFlow<ll, i128>` は距離と費用合計に `i128` を使い、`MinCostBFlow<ll, ll, i128>` は費用合計だけを `i128` にする。さらに大きな結果には既存の `BigInteger<>` を指定できる。固定長の費用合計は同じ幅の符号なし整数で計算し、相殺後の符号付き結果を復元する。

以下、元の頂点数を $n$、辺数を $m$、幅のある供給区間の数を $k$ とする。内部の頂点数は $N=n+[k>0]$、辺数は $M\leq m+k$。元の辺と供給区間の最大幅を $U$、初期化後の余剰需給の正の部分の合計を $F$ とし、最短路1回の上界を $T$ と書く。

| 最短路の指定 | $T$ |
|---|---|
| `Heap::binary` | $O((N+M)\log(N+M))$ |
| `Heap::fibonacci` | ならし $O(M+N\log N)$ |
| `Heap::dense` | $O(M+N^2)$ |

Fibonacci heap は `decrease_key` によって辺更新の対数を除く。二分ヒープは遅延削除を使うので、平行辺を含む場合の上界を $\log(N+M)$ で表している。

#### コンストラクタ

```cpp
MinCostBFlow();
explicit MinCostBFlow(int n);
explicit MinCostBFlow(const vc<Cap> &b);
```

辺数0のグラフを作る。`n` 指定では供給・需要をすべて0とし、配列指定では `b[v]` を使う。引数を省略すると頂点数0。

##### 制約

- $n\geq0$

##### 計算量

- $O(n)$

#### size / num_of_edges

```cpp
template <class I = ll> I size() const;
template <class I = ll> I num_of_edges() const;
```

元の頂点数・利用者が追加した辺数を返す。

##### 計算量

- $O(1)$

#### add_supply

```cpp
void add_supply(int v, Cap amount);
void add_supply(int v, Cap lower, Cap upper);
```

現在の供給区間 $[L_v,U_v]$ に対して、`amount` 指定では $[L_v+\mathrm{amount},U_v+\mathrm{amount}]$、上下限指定では $[L_v+\mathrm{lower},U_v+\mathrm{upper}]$ とする。頂点数指定の初期区間は $[0,0]$。負の値は需要を表す。解を求めた後にも使え、次の `flow()` で変更後の問題を解き直す。

##### 制約

- $0\leq v<n$
- 上下限指定では $\mathrm{lower}\leq\mathrm{upper}$
- 加算後の上下限が `Cap` に収まる

##### 計算量

- $O(1)$

#### set_supply

```cpp
void set_supply(int v, Cap lower, Cap upper);
```

頂点 `v` の供給区間を $[\mathrm{lower},\mathrm{upper}]$ に設定する。上下限が等しい場合は固定供給。解を求めた後の変更は、次の `flow()` で反映する。

##### 制約

- $0\leq v<n$
- $\mathrm{lower}\leq\mathrm{upper}$

##### 計算量

- $O(1)$

#### get_supply

```cpp
Cap get_supply(int v) const;
```

最後の最適解で選ばれた供給量を返す。元のグラフでの流出量 $-$ 流入量に等しい。

##### 制約

- $0\leq v<n$
- 直前の問題の最適解が求まっている。辺・供給区間の変更後は `flow()` を再実行する

##### 計算量

- $O(1)$

#### add_edge

```cpp
int add_edge(int from, int to, Cap lower, Cap upper, Cost cost);
int add_edge(int from, int to, Cap upper, Cost cost);  // lower = 0
```

流量が $[\mathrm{lower},\mathrm{upper}]$、単位費用 `cost` の有向辺を追加する。追加順の辺番号を返す。解を求めた後の辺追加にも対応し、次の `flow()` で解き直す。

##### 制約

- $0\leq\mathrm{from},\mathrm{to}<n$
- $\mathrm{lower}\leq\mathrm{upper}$

##### 計算量

- ならし $O(1)$

#### flow（b-flow）

```cpp
enum class Algorithm { automatic, ssp, primal_dual, capacity_scaling, network_simplex };
enum class Heap { automatic, binary, fibonacci, dense };
optional<Total> flow(
    Algorithm algorithm = Algorithm::automatic,
    Heap heap = Heap::automatic);
```

現在の辺と供給区間に対して解き直し、最小費用を返す。解がなければ `nullopt`。費用 $0$ の解は値ありの `0` として返す。成功時は `get_edge`・`edges`・`get_supply`・`potential` から最適解を取得できる。繰り返し呼べる。

区間供給は、各頂点の固定供給を下限 $L_v$ とし、補助頂点 $r$ の供給を $-\sum_v L_v$、辺 $r\to v$ の容量を $[0,U_v-L_v]$ として解く。この辺の流量が $b_v-L_v$ になる。固定供給だけの場合は従来のグラフ構成を使う。

方式の違いは次の通り。

- `ssp`：余剰点から最も近い需要点まで探索し、1本の最短路を増加する。
- `primal_dual`：複数の需要点まで探索する。最短路の森で需要を下から集計し、上から容量に合わせて配分する。森上の処理は1回 $O(n)$。
- `capacity_scaling`：大きい容量単位から始め、単位以上の残余容量だけで探索する。負の還元費用の辺を単位の倍数だけ飽和し、primal-dual と同じ森の処理で需給を修正する。最後に単位1で最適性を確定する。
- `network_simplex`：基底木に辺を加えて循環を作り、流量を増減して木の辺を交換する。block search で入る辺を選び、親子関係・ポテンシャルは変更された部分木だけを更新する。人工辺の費用を優先順位と元の費用の組で扱い、巨大な費用定数を避ける。最短路キューの指定は pivot に作用せず、最後の双対解の変換に共有の Fibonacci heap を使う。
- `automatic`：小規模で容量幅が大きい場合や、疎な単位容量で流量が多い場合に network simplex を予算付きで試す。SSP または primal-dual の進捗に応じて予算を延長し、その後は容量スケーリング・network simplex と交互に進める。容量幅が1以下ならスケーリングを省く。

##### 制約

- 辺・供給区間の幅、内部の需給調整中の値・需要の絶対値・余剰需給の合計が `Cap` に収まる
- 逆辺の費用・還元費用・ポテンシャル・距離の計算中の値が `Cost` に収まる。有限の探索距離は `numeric_limits<Cost>::max()` 未満
- 最小費用が `Total` に収まる。`Total` は符号付き整数型、または同等の正確な整数演算を持つ型

##### 計算量

- `ssp`・`primal_dual`：$O(n+m+(F+1)T)$
- `capacity_scaling`：$O(n+m+MT\log U)$
- `network_simplex`：pivot 回数を $P$ として $O(n+m+(P+1)(N+M)+N\log N)$。実際の木の更新は変更された部分木の大きさに比例する。$P$ は入力と辺の選択に依存する
- `automatic`：SSP / primal-dual と容量スケーリングの上界の小さい方を保つ。候補の試行・保存に定数倍の費用が加わる

#### get_edge / edges

```cpp
struct edge {
  int from, to;
  Cap lower, upper;
  Cost cost;
  Cap flow;
};
edge get_edge(int i) const;
vc<edge> edges() const;
```

元の辺番号・端点・上下限・費用と、最後に計算した流量を返す。`edges` は追加順。辺追加直後の `flow` は下限で初期化する。不可能だった探索の後は、途中の流量を保持する。

##### 制約

- `get_edge`：$0\leq i<m$

##### 計算量

- `get_edge`：$O(1)$
- `edges`：$O(m)$

#### potential

```cpp
vc<Potential> potential() const;
```

双対ポテンシャル $p$ を返す。各辺の還元費用を $c_e+p_{s_e}-p_{t_e}$ とすると、流量が上限未満なら非負、下限より大きければ非正となる。

保持している双対解で辺を非負に重み付けし直し、全頂点を始点とする Dijkstra 法で正規化する。$C=\max_e|c_e|$ として、固定供給だけの場合は $-nC\leq p_v\leq0$ に収める。

幅のある供給区間がある場合は、補助頂点のポテンシャルを0に揃え、元の頂点の値だけを返す。このとき $-nC\leq p_v\leq nC$。選んだ供給が上限未満なら $p_v\leq0$、下限より大きければ $p_v\geq0$ となる。結果をキャッシュする。

##### 制約

- 直前の問題の最適解が求まっている。辺・需給を変更した場合は `flow()` を再実行する

##### 計算量

- 初回 $O((N+M)\log(N+M))$、同じ解への2回目以降は戻り値のコピーに $O(n)$

#### slope / flow（s-t の追加流）

```cpp
vc<pair<Cap, Total>> slope(int s, int t,
    Cap limit = numeric_limits<Cap>::max(), Heap heap = Heap::automatic);
pair<Cap, Total> flow(int s, int t,
    Cap limit = numeric_limits<Cap>::max(), Heap heap = Heap::automatic);
```

最適な b-flow を基準に、$b_s$ を $x$ 増やし、$b_t$ を $x$ 減らしたときの最小費用の増分を求める。基準費用を $G(0)$ とすると、返す折れ線は $(x,G(x)-G(0))$。先頭は `(0,0)`、傾きが等しい隣接区間をまとめる。負の傾きも扱い、傾きは単調非減少となる。

区間供給の場合は、`s` の区間を $[L_s+x,U_s+x]$、`t` の区間を $[L_t-x,U_t-x]$ とした問題の最小費用を使う。各頂点の供給量は、移動後の区間内で最適な値を選び直す。

流せる最大の追加量と `limit` の小さい方まで処理し、辺の流量と供給区間をその終点へ更新する。`flow(s,t,...)` は最後の点だけを返す。繰り返しの実行、ヒープを変えた継続、逆方向への取り消しにも対応する。

この2関数は SSP で曲線を構成する。固定の追加量を容量スケーリングで求めたい場合は、その量を需給へ設定して b-flow の `flow()` を呼ぶ。

区間数を $Z$ として、折れ線の保存に $O(Z)$ の領域を使う。

全頂点の供給区間が $[0,0]$、全辺の下限が0、費用が非負なら、`flow()` で基準解を求めてから呼ぶことで、ACL の `flow`・`slope` と同じ流量・費用を返す。

##### 制約

- 基準となる問題の最適解が求まっている。辺・需給の変更後は `flow()` を再実行する
- $0\leq s,t<n$、$s\ne t$
- $0\leq\mathrm{limit}$
- 更新後の供給区間が `Cap` に収まる
- 返す各点の費用差と、更新後の最小費用が `Total` に収まる

##### 計算量

- 増加路を流した回数を $K$ として $O(n+m+(K+1)T)$。$K$ は追加流量以下
