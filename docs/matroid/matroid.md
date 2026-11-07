このドキュメントには AI による作成・変更が含まれています。

## 概要

マトロイドは、集合の「独立性」を扱う組合せ構造。森林、グループごとの容量制約、ベクトルの線形独立性などを同じ API で扱う。

台集合を $E$、独立集合族を $\mathcal I$ とする。空集合が独立で、独立集合の部分集合も独立であり、$I,J\in\mathcal I$ かつ $\lvert I\rvert<\lvert J\rvert$ なら、ある $e\in J\setminus I$ について $I\cup\lbrace e\rbrace$ が独立になる。

### 具体的なマトロイド

| 型 | 独立集合 |
| --- | --- |
| [UniformMatroid](uniform_matroid.md) | 要素数が指定の上限以下 |
| [PartitionMatroid](partition_matroid.md) | 各グループの選択数が容量以下 |
| [GraphicMatroid](graphic_matroid.md) | 無向グラフの森林 |
| [BicircularMatroid](bicircular_matroid.md) | 各連結成分の閉路が高々一つの辺集合 |
| [TransversalMatroid](transversal_matroid.md) | 二部グラフで右側へ単射的にマッチできる左頂点集合 |
| [LaminarMatroid](laminar_matroid.md) | 入れ子になったグループごとの選択数が容量以下 |
| [BinaryMatroid](binary_matroid.md) | ビット列を二元体上のベクトルとみなして線形独立 |
| [LinearMatroid](linear_matroid.md) | 同じ体上の列ベクトルが線形独立 |
| [IndependenceMatroid](independence_matroid.md) | 利用者の独立性判定で指定 |

### アルゴリズム

| 関数 | 求めるもの |
| --- | --- |
| [matroid_greedy](matroid_greedy.md) | 最大重み基・最大重み独立集合 |
| [matroid_intersection](matroid_intersection.md) | 二つのマトロイドの最大共通独立集合 |
| [weighted_matroid_intersection](weighted_matroid_intersection.md) | 指定した要素数、または最大要素数での最大重み共通独立集合 |
| [matroid_union](matroid_union.md) | 複数の独立集合に分けられる最大要素集合 |
| [linear_matroid_intersection](linear_matroid_intersection.md) | 同じ体で表現した二つのマトロイドの線形交叉 |
| [linear_matroid_parity](linear_matroid_parity.md) | 指定されたペアを丸ごと選ぶ最大線形独立集合 |

組合せ的な交叉は決定的に解く。線形交叉・パリティの代数的方法は有限体上の乱択法で、実行時間・誤り確率・返す集合の保証は各文書に記載する。

## 使用例

```cpp
#include "matroid/partition_matroid.hpp"
#include "matroid/graphic_matroid.hpp"
#include "matroid/matroid_intersection.hpp"

// 各色から高々1本選んで、最大本数の森林を作る。
vc<pair<int, int>> edges{{0, 1}, {1, 2}, {0, 2}, {2, 3}};
PartitionMatroid color({0, 0, 1, 1}, {1, 1});
GraphicMatroid forest(4, edges);
auto selected = matroid_intersection(color, forest);
// selected は2本の辺番号。
```

## 詳細なドキュメント

### 共通 oracle

台集合の要素番号は `0..size()-1`。各オブジェクトは現在の独立集合 $I$ を保持する。

```cpp
int size() const;
void build(const vc<int>& independent);
optional<vc<int>> circuit(int e) const;
```

`build` は相異なる有効な要素からなる独立集合を渡す。`circuit(e)` は $e\notin I$ に対して問い合わせる。

- $I\cup\lbrace e\rbrace$ が独立なら `nullopt`。
- 従属なら、唯一の fundamental circuit から $e$ を取り除いた要素列を返す。順序は任意。
- 値ありの空配列は、$e$ 単体が従属となるループを表す。

circuit が返った場合、列にある要素 $f$ を取り除くと $I-f+e$ が独立になる。この性質を交叉・合併の交換グラフに使う。

組合せ的なアルゴリズムは oracle を値で受け取る。同じオブジェクトを二つの引数に渡す場合も使える。独立性判定だけを用意できる場合は `IndependenceMatroid` で共通 API に変換できる。

### 共通部品

分割は [CSR](../ds/csr.md)、グラフ的・laminar は [RootedTree](../tree/rooted_tree.md)、bicircular は [UnionFind](../ds/uf/uf.md) と [Graph](../graph/graph.md) を使う。transversal の交互路探索は [BipartiteMatchingState](../flow/bipartite_matching_state.md) に委譲する。

binary・線形の基底管理は [XorBasis](../math/linalg/xor_basis.md)・[LinearBasis](../math/linalg/linear_basis.md) に委譲する。XOR ベクトルは整数に加えて `DynamicBitset` に対応する。

追加したベクトルを tag の threshold や suffix で絞って扱う場合は、[XorBasis](../math/linalg/xor_basis.md)・[LinearBasis](../math/linalg/linear_basis.md) の tag 付き問い合わせを使える。

### 線形表現の選び方

以下の表現は各マトロイドの [`linear_representation` メンバ](linear_representation.md) で作れる。transversal matroid・bicircular matroid の乱択表現も用意している。

対応するマトロイドのヘッダだけで利用できる。保持する全要素から表現を作り、現在の `build` の状態によらず要素番号を保つ。

- グラフ的マトロイドは有向化した接続行列の各辺を列にする。各列は始点に $1$、終点に $-1$、それ以外に $0$ を置く。自己ループは零列。
- 各グループの容量が $1$ の分割マトロイドは、グループごとの単位ベクトルで表現できる。
- 容量が一般の場合は、グループごとに容量と同じ次元を設け、互いに異なる $t_e$ を使う列 $(1,t_e,\ldots,t_e^{c-1})$ を置く。一様マトロイドも同様。係数体には必要な数の異なる値を用意する。
- 一般の線形マトロイドを扱う場合は、入力が持つ係数体を使う。有限体を変更すると独立性も変わり得る。

線形交叉・パリティには、集合の復元版と、階数計算だけで要素数・ペア数を求める `_size` 版がある。サイズだけが必要なら `_size` 版を使うと再計算を省ける。

### 今後の拡張候補

双対・制限・縮約・直和・truncation は、既存のマトロイドを組み合わせるときに役立つ。重み付き線形パリティも用途に応じて追加できる。
