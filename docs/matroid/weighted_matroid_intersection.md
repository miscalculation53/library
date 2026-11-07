このドキュメントには AI による作成・変更が含まれています。

## 概要

同じ台集合を持つ二つのマトロイドの共通独立集合について、各要素数で重みの総和を最大化する。最大要素数を優先する場合と、要素数を指定する場合に対応する。

重みを二つのマトロイドへ分ける primal-dual 法を使い、交換グラフの費用を非負に保つ。
最短路には既存の [ShortestPath](../graph/sssp.md) を使い、疎なグラフではヒープ、密なグラフでは配列を使う Dijkstra 法などを自動選択する。同じ費用なら通る要素数が少ない経路を使い、各要素数での最適性を保つ。oracle の仕様は [マトロイド](matroid.md) を参照。

## 使用例

```cpp
#include "matroid/partition_matroid.hpp"
#include "matroid/weighted_matroid_intersection.hpp"

PartitionMatroid left ({0, 0, 1}, {1, 1});
PartitionMatroid right({0, 1, 0}, {1, 1});
vc<ll> weight{10, 4, 3};
auto largest = weighted_matroid_intersection(left, right, weight);
// {7, {1, 2}}。最大本数を優先する。
auto one = weighted_matroid_intersection(left, right, weight, 1);
// {10, {0}}。
auto slope = weighted_matroid_intersection_slope(left, right, weight);
// 重みの最大値は、要素数0,1,2についてそれぞれ0,10,7。
```

## 詳細なドキュメント

#### weighted_matroid_intersection

```cpp
template<class M1, class M2, class W>
optional<pair<W, vc<int>>> weighted_matroid_intersection(
    M1 first, M2 second, const vc<W>& weight, int cardinality = -1);
```

`cardinality == -1` なら最大要素数の中で重みが最大の解、それ以外なら指定した要素数で重みが最大の解を返す。値は `{重みの総和, 昇順の要素番号}`。指定要素数の解が存在するときに値を持ち、要素数 $0$ には `{0, {}}` を返す。

##### 制約

- `first.size() == second.size() == weight.size()`。
- 両方の型が共通 oracle の仕様を満たす。
- `cardinality >= -1`。
- `W` は符号付き整数など、正確な加減算・順序比較ができる型。`ll`・`i128` に対応する。
- 重みの総和、分割した重み、最短距離とその更新中の中間値が `W` に十分な余裕を持って収まる。

##### 計算量

台集合の要素数を $n$、最大共通独立集合の大きさを $r$、二つの `build` の時間の和を $B$、circuit の返却を含む `circuit` の最大時間を $C$ とする。

- 時間 $O(r(B+nC+nr\log n))$。
- 追加メモリ $O(nr)$。oracle の領域は別。

指定要素数に達した時点で探索を終える。1回の交換グラフの辺数は $O(nr)$。その非負最短路は、ヒープによる Dijkstra 法で $O(nr\log n)$、密なグラフでは配列による Dijkstra 法で $O(n^2)$ 時間となる。

#### weighted_matroid_intersection_slope

```cpp
template<class M1, class M2, class W>
vc<pair<W, vc<int>>> weighted_matroid_intersection_slope(
    M1 first, M2 second, const vc<W>& weight);
```

`result[k]` に、要素数 $k$ の最適な `{重みの総和, 要素番号}` を返す。最大共通独立集合の大きさを $r$ とすると、結果の長さは $r+1$。任意の要素数で重みだけを最大化したい場合は、この配列から最大の総和を選べる。

制約・時間計算量は上記と同じ。結果の保存に $O(r^2)$ のメモリを使う。

## アルゴリズム

元の重みを $w=a+b$ と分ける。現在の共通独立集合 $I$ が、第1マトロイドでは重み $a$、第2マトロイドでは重み $b$ に関して、同じ要素数の独立集合の中で最適であることを保つ。初期状態は $I=\varnothing$, $a=w$, $b=0$。

選択済み要素 $x$ と未選択要素 $y$ の交換辺には、第1マトロイドでは $a_x-a_y$、第2マトロイドでは $b_x-b_y$ を費用として与える。個別の最適性から、これらは非負になる。
追加可能な要素の重みの最大値をそれぞれ $\alpha,\beta$ として、始点辺を $\alpha-a_y$、終点辺を $\beta-b_y$ とする。経路費用は $\alpha+\beta$ から元の重みの増分を引いた値に等しい。

最短距離を $d$、終点の距離を $D$ とする。到達不能な頂点の距離を $D$ と扱い、

$$
\delta_v=\min(d_v,D),\qquad
 a_v\gets a_v+\delta_v,\qquad
 b_v\gets b_v-\delta_v
$$

で分割した重みを更新する。この更新は非負性を保ち、最短路上の交換を費用0にする。経路に沿って選択を反転すると、次の要素数でも二つの個別最適性を保つ。

## 参考文献

同じ要素数で最適な集合から、費用最小の増加路のうち辺数最小のものを選ぶと、次の要素数でも最適性を保つ。重みの分割による非負最短路への帰着と合わせて、[Schrijver, Combinatorial Optimization, Sections 41.3–41.3a, Theorems 41.5–41.6, 41.9–41.10](https://page.math.tu-berlin.de/~felsner/Lehre/SemMatS/Literatur/Schrijver-41:MatIntersection.pdf) を参照。
