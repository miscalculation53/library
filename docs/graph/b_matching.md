このドキュメントには AI による作成・変更が含まれています。

## 概要

一般無向グラフで、頂点容量・辺容量を満たす最大本数の b-matching と、指定次数の部分グラフ（f-factor）を求める。

[GraphUndirected](graph.md) の重み付き・重みなし・辺削除可能な形式に対応する。重みは使わず、多重辺を独立に扱う。入力は変更せず、現在残っている辺の元の番号を返す。返す順序や最適解の選び方は任意。追加メモリは各関数とも $O(n+m)$。

## 使用例

```cpp
#include "graph/b_matching.hpp"

GraphUndirected<> g(3, vc<pair<int, int>>{
    {0, 1}, {1, 2}, {2, 0}});
auto selected = b_matching(g, vc<int>{2, 1, 1});
// 各辺を一度まで使う。{辺番号, 本数} の配列で、2 本を選ぶ。
auto counts = b_matching(g, vc<ll>{10, 10, 10}, nullopt);
// 各辺を任意回使う。総本数は 15。
auto limited = b_matching(g, vc<ll>{10, 10, 10}, vc<ll>{2, 3, 4});
// 辺ごとの上限を指定。総本数は 9。
auto exact = f_factor(g, vc<int>{2, 2, 2});
// 値あり。全 3 辺の番号を返す。
```

## 詳細なドキュメント

以下、$n$ は頂点数、$m$ は現在の辺数、$\alpha$ は逆 Ackermann 関数。

#### b_matching

```cpp
template <class Cost, bool erasable, class Capacity>
vc<pair<int, Capacity>> b_matching(
    const GraphUndirected<Cost, erasable> &g, const vc<Capacity> &capacity);

template <class Cost, bool erasable, class Capacity>
vc<pair<int, Capacity>> b_matching(
    const GraphUndirected<Cost, erasable> &g, const vc<Capacity> &capacity,
    const vc<Capacity> &edge_capacity);

template <class Cost, bool erasable, class Capacity>
vc<pair<int, Capacity>> b_matching(
    const GraphUndirected<Cost, erasable> &g, const vc<Capacity> &capacity, nullopt_t);
```

頂点容量を `capacity[v]` として、選択する辺の総本数を最大化する。辺容量は省略時に $1$、配列を渡すと `edge_capacity[id]`、`nullopt` を渡すと上限なしになる。自己ループ一本は頂点容量を二つ使う。

使用本数が正の辺を `{元の辺番号, 本数}` の配列で返す。総本数は本数の合計で、空配列も有効な答えになる。

辺容量を省略し、有効な頂点容量がすべて $1$ なら [Micali–Vazirani](general_matching.md)、それ以外は Goldberg–Karzanov の最大流＋丸め法を使う。容量の数値に比例した展開は行わない。

##### 制約

- `capacity.size() == g.size()`
- `Capacity` は $128$ ビット以下の整数型（`bool` も可）で、容量は非負
- 辺容量を配列で与える場合、現在残っている各辺の `index` が有効な添字
- 複数回使用する場合の作業型は、`Capacity` が $64$ ビット未満なら `ll`、それ以外は `Capacity`。切り詰めた頂点容量の総和とその定数倍が、この型に十分な余裕を持って収まる

##### 計算量

- 辺容量を省略し、有効な頂点容量がすべて $1$：$O((n+m)\sqrt n\alpha(n))$
- それ以外：$O(n+m+nm\alpha(n)+n^2\sqrt m)$。容量の数値に依存しない算術演算回数

#### f_factor

```cpp
template <class Cost, bool erasable, class Demand>
optional<vc<int>> f_factor(const GraphUndirected<Cost, erasable> &g,
                          const vc<Demand> &demand);
```

各辺を高々一度使い、各頂点の次数が `demand[v]` と一致する辺集合を、元の辺番号の配列で返す。解がなければ `nullopt`、指定次数がすべて $0$ なら値ありの空配列になる。

指定次数を容量とした `b_matching` を求め、選択辺数 $k$ が $2k=\sum_v\mathrm{demand}[v]$ を満たすか調べる。

##### 制約

- `demand.size() == g.size()`
- `Demand` は $128$ ビット以下の整数型（`bool` も可）で、各要素が非負
- グラフは自己ループを持たない

##### 計算量

- `b_matching(g, demand)` に $O(n+m)$ を加えたもの

## 問題への適用

- [ABC482 G](../../verify/atcoder/abc482_g.test.cpp)：$A_i$ を頂点容量として `b_matching(g, a)` を呼び、選択辺数を出力する。
- [ABC263 G](../../verify/atcoder/abc263_g.test.cpp)：種類を頂点、個数を容量とし、和が素数になる種類対に辺を張る。$1+1$ の自己ループも含め、`b_matching(g, b, nullopt)` の本数を合計する。

参考：[Goldberg–Karzanov, Maximum Skew-Symmetric Flows and Matchings, v2](https://arxiv.org/abs/math/0304290v2)、[Path Problems in Skew-Symmetric Graphs（1993, ミラー）](https://ftpmirror.your.org/pub/misc/bitsavers/pdf/stanford/Stanford_CS_TR_Collection_2025-12-12/OCR/CS-TR-93-1489-ocr.pdf)。
