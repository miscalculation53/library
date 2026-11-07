このドキュメントには AI による作成・変更が含まれています。

## 概要

具体的なマトロイドを [`LinearMatroid<F>`](linear_matroid.md) に変換する。要素番号は入力の順序を保つ。係数体 `F` は既存の `FieldAddSubMulDiv<mint>` などを使える。

各マトロイドの `linear_representation` メンバから利用できる。対応するマトロイドのヘッダだけで呼び出せる。`matroid/linear_representation.hpp` は、以下の六つのヘッダをまとめて読み込む入口。既存の構築関数も、それぞれ対応するヘッダに含む。

| マトロイド | ヘッダ |
|---|---|
| 一様 | `matroid/uniform_matroid.hpp` |
| 分割 | `matroid/partition_matroid.hpp` |
| グラフ的 | `matroid/graphic_matroid.hpp` |
| binary | `matroid/binary_matroid.hpp` |
| transversal | `matroid/transversal_matroid.hpp` |
| bicircular | `matroid/bicircular_matroid.hpp` |

一様・分割は Vandermonde 行列、グラフ的は符号付き接続行列、二値は各ビットを座標として表す。これらは各関数の条件の下で元のマトロイドと同じ独立集合を持つ。

transversal matroid・bicircular matroid の表現は乱択で構築する。元の従属集合は常に従属になり、元の独立集合が確率的に従属になる場合がある。線形交叉・線形パリティとの組合せでは、係数体と乱択による失敗確率を考慮する。

## 使用例

```cpp
#include "matroid/uniform_matroid.hpp"
#include "matroid/partition_matroid.hpp"
#include "matroid/graphic_matroid.hpp"
#include "matroid/transversal_matroid.hpp"

using mint = static_modint32<998244353>;
using F = FieldAddSubMulDiv<mint>;
auto uniform = UniformMatroid(4, 2).linear_representation<F>({0, 1, 2, 3});
auto graphic = GraphicMatroid(3, {{0, 1}, {1, 2}, {2, 0}}).linear_representation<F>();
auto partition = PartitionMatroid({0, 0, 1}, {1, 1}).linear_representation<F>({0, 1, 0});


mt19937_64 rng(20261101);
uniform_int_distribution<unsigned> distribution(0, mint::mod() - 1);
auto sample = [&]() { return mint(distribution(rng)); };
GraphUndirected<> g(5, vc<pair<int, int>>{{0, 1}, {2, 1}, {2, 3}, {4, 3}});
vc<bool> color{false, true, false, true, false};
auto transversal = TransversalMatroid(g, color).linear_representation<F>(sample);
```

## 詳細なドキュメント

以下、台集合の要素数を $n$、返す表現の次元を $d$ とする。構築される密行列の記憶量は $O(nd)$。

#### 各マトロイドの linear_representation

```cpp
// UniformMatroid / PartitionMatroid
template<class F>
LinearMatroid<F> linear_representation(const vc<typename F::S>& points) const;
// GraphicMatroid / BinaryMatroid<V>
template<class F>
LinearMatroid<F> linear_representation() const;
// TransversalMatroid / BicircularMatroid
template<class F, class Sample>
LinearMatroid<F> linear_representation(Sample&& sample) const;
```

保持しているパラメータから線形表現を作る。一様・分割では `points.size() == size()` とし、以下に記載した点の相異性の条件も満たす。binary の表現では係数体を標数 $2$ とする。現在の `build` の状態に依存せず、要素番号を保つ。制約・計算量はそれぞれ対応する以下の構築関数と同じ。

#### uniform_linear_representation

```cpp
template<class F>
LinearMatroid<F> uniform_linear_representation(int rank, const vc<typename F::S>& distinct_points);
```

$r=\min(\mathrm{rank},n)$ として、点 $x_e$ に列 $(1,x_e,\ldots,x_e^{r-1})$ を割り当てる。一様マトロイドの線形表現を返す。`rank == 0` では全要素が零次元のループになる。

##### 制約

- `rank >= 0`。
- `distinct_points` の値は体の元で、互いに異なる。

##### 計算量

- $O(nd)$、$d=\min(\mathrm{rank},n)$。

#### partition_linear_representation

```cpp
template<class F>
LinearMatroid<F> partition_linear_representation(
    const vc<int>& group, const vc<int>& capacity, const vc<typename F::S>& points);
```

要素 `e` の所属グループを `group[e]`、グループの容量を `capacity[g]` とした分割マトロイドの線形表現を返す。各グループの Vandermonde 表現を互いに異なる座標に配置する。容量が $0$ のグループの要素は零ベクトルになる。

##### 制約

- `points.size() == group.size()`。
- `0 <= group[e] < capacity.size()`、`capacity[g] >= 0`。
- 正の容量を持つ各グループ内で、`points[e]` は体の元として互いに異なる。異なるグループ間では同じ値を使える。

##### 計算量

- $O(nd+k)$。$k$ はグループ数、$d=\sum_g\min(\mathrm{capacity}[g],\lvert\lbrace e\mid\mathrm{group}[e]=g\rbrace\rvert)$。

#### graphic_linear_representation

```cpp
template<class F>
LinearMatroid<F> graphic_linear_representation(int vertices, const vc<pair<int,int>>& edges);
```

辺 $(u,v)$ の列を、行 $u$ に $1$、行 $v$ に $-1$ を加えた接続行列で表す。任意の体でグラフ的マトロイドの線形表現を返す。自己ループは零ベクトルになり、多重辺にも対応する。

##### 制約

- `vertices >= 0`。
- 各辺の両端は `0..vertices-1`。

##### 計算量

- $O(nd)$、$d=\mathrm{vertices}$。

#### binary_linear_representation

```cpp
template<class F, class V>
LinearMatroid<F> binary_linear_representation(const vc<V>& vectors);
```

整数または `DynamicBitset` の第 $b$ ビットを第 $b$ 座標に割り当て、binary matroid の線形表現を返す。係数体は標数 $2$ とする。

##### 制約

- `V` は `bool` を除く符号なし整数型（$128$ ビット以下）、または `DynamicBitset`。後者では全ベクトルの長さを揃える。
- `F::add(F::e1(), F::e1()) == F::e0()`。

##### 計算量

- $O(nd)$。$d$ は整数型のビット幅または `DynamicBitset` の長さ。

#### transversal_linear_representation

```cpp
template<class F, class Cost, bool is_erasable, class Color, class Sample>
LinearMatroid<F> transversal_linear_representation(
    const GraphUndirected<Cost, is_erasable>& graph,
    const vc<Color>& color, Sample&& sample);
template<class F, class Sample>
LinearMatroid<F> transversal_linear_representation(
    int right_vertices, const vvc<int>& adjacency, Sample&& sample);
template<class F, class Sample>
LinearMatroid<F> transversal_linear_representation(
    int right_vertices, const CSR<int>& adjacency, Sample&& sample);
template<class F, class Sample>
LinearMatroid<F> transversal_linear_representation(
    int right_vertices, initializer_list<vc<int>> adjacency, Sample&& sample);
```

グラフと彩色から構築する場合は色 $0$ の頂点を列、色 $1$ の頂点を行とし、それぞれ元の頂点番号の昇順に並べる。辺の重み・番号は使わず、削除可能グラフでは構築時点で有効な辺を使う。隣接列からも構築できる。

右頂点を行、左頂点を列にする。隣接位置ごとに `sample()` で係数を選び、他の位置を零とした乱択線形表現を返す。同じ左頂点の隣接列の重複はまとめ、各位置で一度だけ `sample()` を呼ぶ。

隣接関係に完全な割当があれば対応する小行列式は非零多項式になる。有限体の要素数を $q$ とし、各係数を独立かつ一様に選ぶと、あらかじめ固定した独立集合 $I$ の独立性を失う確率は $\lvert I\rvert/q$ 以下。台集合全体のすべての独立集合を同時に保つ失敗確率には、例えば $n2^n/q$ という和の上界が使える。固定した集合の保証と、全独立集合を同時に保つ保証は区別する。

##### 制約

- グラフからの構築：`color.size() == graph.size()`、各値を整数リテラル `0`・`1` と比較でき、値が $0$ または $1$。辺の両端の色が異なる。
- 隣接列からの構築：`right_vertices >= 0`、隣接先は `0..right_vertices-1`。
- `sample()` は `F::S` に変換できる体の元を返す。
- 上記の確率評価を使う場合、各呼出しは有限体の元を独立かつ一様に返す。

##### 計算量

- $O(nd+n+d+m)$。$d$ は右頂点数、$m$ は重複を含む隣接列の要素数の総和。

#### bicircular_linear_representation

```cpp
template<class F, class Sample>
LinearMatroid<F> bicircular_linear_representation(
    int vertices, const vc<pair<int,int>>& edges, Sample&& sample);
```

各辺を左頂点、その両端を隣接先とする transversal matroid の表現を構築する。自己ループの隣接先は一つになる。辺をそれぞれ異なる端点に割り当てられることと、各連結成分が高々一つの閉路を持つことが同値なので、bicircular matroid の乱択線形表現になる。失敗確率と `sample()` の条件は `transversal_linear_representation` と同じ。

##### 制約

- `vertices >= 0`。
- 各辺の両端は `0..vertices-1`。
- `sample()` は `F::S` に変換できる体の元を返す。

##### 計算量

- $O(nd+d)$、$d=\mathrm{vertices}$。

## 参考文献

transversal matroid の表現の行列式と割当の対応は [Ardila, Transversal and cotransversal matroids via their representations, Section 2](https://fardila.com/Articles/cotransversal.pdf) にある。
