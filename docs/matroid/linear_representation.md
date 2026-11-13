このドキュメントには AI による作成・変更が含まれています。

## 概要

具体的なマトロイドを [`LinearMatroid<F>`](linear_matroid.md) に変換する。要素番号は入力の順序を保つ。係数体 `F` は既存の `FieldAddSubMulDiv<mint>` などを使える。

各マトロイドの `linear_representation` メンバから利用できる。以下の対応するヘッダを読み込めば呼び出せる。構築関数も、それぞれ対応するヘッダに含む。

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

後続の交叉・パリティの `trials` は、生成済みの線形表現を固定して反復する。表現生成側の失敗確率も減らす場合は、線形表現の再生成を含めて反復する。

`F` の既定値は `FieldAddSubMulDiv<modint998244353>`。binary matroid は標数を保つため `FieldAddSubMulDiv<static_modint32<2>>` を既定にする。

一様・分割は `points` を省略でき、既存の [from_integer](../algebra/from_integer.md) で整数の像を作る。一様は要素番号、分割は各グループ内の出現順 `0,1,2,...` を点とする。この構築には乱数を使わない。

transversal・bicircular の `sample` を省略すると、[random_modint](../math/modint/random.md) で零を含む体の元から一様に選ぶ。`F::S` が modint の場合に使える。

グループ番号・容量・辺の両端・隣接先の整数型は、それぞれ独立に選べる。`i128`・`u128` にも対応する。

疎な表現は各 struct の `sparse_linear_representation<F>(...)` で取得する。引数と既定の係数体は `linear_representation` と同じで、戻り値は [SparseMatrix<F>](../math/linalg/sparse_matrix.md)。行が座標、列が台集合の要素に対応する。`LinearMatroid::matrix()` は同じ向きの密行列を返す。

疎な表現は生成時から非零成分を蓄積する。graphic は各列高々2成分、partition は各列が所属グループの座標だけを持つ。transversal・bicircular は対応する隣接位置だけを持つ。一様・binary も疎な形式で取得できる。

構築時の非零候補数を $t$ とすると、疎な出力の保存領域は $O(d+t)$、構築には候補の列挙と $O(d+t)$ の整理を要する。各行には要素番号順に追加されるため、ソートは不要。候補の列挙時間は一様・binary が $O(nd)$、分割が $O(n+k+\sum_e\min(\mathrm{capacity}[\mathrm{group}[e]],\mathrm{count}[\mathrm{group}[e]]))$、graphic・bicircular が $O(n+d)$、transversal が $O(n+d+m)$。点の自動生成の費用は密な表現と同じ。

## 使用例

```cpp
#include "matroid/uniform_matroid.hpp"
#include "matroid/partition_matroid.hpp"
#include "matroid/graphic_matroid.hpp"
#include "matroid/transversal_matroid.hpp"

auto uniform = UniformMatroid(4, 2).linear_representation();
auto graphic = GraphicMatroid(3, {{0, 1}, {1, 2}, {2, 0}}).linear_representation();
auto partition = PartitionMatroid({0, 0, 1}, {1, 1}).linear_representation();
auto sparse = GraphicMatroid(3, {{0, 1}, {1, 2}}).sparse_linear_representation();
GraphUndirected<> g(5, vc<pair<int, int>>{{0, 1}, {2, 1}, {2, 3}, {4, 3}});
vc<bool> color{false, true, false, true, false};
auto transversal = TransversalMatroid(g, color).linear_representation();
```

## 詳細なドキュメント

以下、台集合の要素数を $n$、返す表現の次元を $d$ とする。構築される密行列の記憶量は $O(nd)$。

#### 各マトロイドの linear_representation

```cpp
// UniformMatroid / PartitionMatroid
template<class F = FieldAddSubMulDiv<modint998244353>>
LinearMatroid<F> linear_representation() const;
template<class F = FieldAddSubMulDiv<modint998244353>>
LinearMatroid<F> linear_representation(const vc<typename F::S>& points) const;
// GraphicMatroid
template<class F = FieldAddSubMulDiv<modint998244353>>
LinearMatroid<F> linear_representation() const;
// BinaryMatroid<V>
template<class F = FieldAddSubMulDiv<static_modint32<2>>>
LinearMatroid<F> linear_representation() const;
// TransversalMatroid / BicircularMatroid
template<class F = FieldAddSubMulDiv<modint998244353>,
         class Sample = decltype(random_modint<typename F::S>) &>
LinearMatroid<F> linear_representation(Sample&& sample = random_modint<typename F::S>) const;
```

保持しているパラメータから線形表現を作る。`points` を指定する場合、一様・分割では `points.size() == size()` とし、以下に記載した点の相異性の条件も満たす。binary の表現では係数体を標数 $2$ とする。現在の `build` の状態に依存せず、要素番号を保つ。制約・計算量はそれぞれ対応する以下の構築関数と同じ。

点を省略する場合、階数・容量が $2$ 以上の一様マトロイド・グループでは整数の像が相異なることが必要。正標数 $p$ の体では、それぞれ要素数・グループの要素数を $p$ 以下とする。拡大体でも整数の像は 素体内にある。階数・容量が $0,1$ の場合、点の重複は結果に影響しない。

点の自動生成1回の整数埋め込みの最大費用を $T$ とすると、追加費用は一様で $O(nT)$、分割で $O(nT+k)$。$k$ はグループ数。既定の modint では $T=O(1)$、一般の体では $O(\log n)$ 回の加法となる。

#### uniform_linear_representation

```cpp
template<class F = FieldAddSubMulDiv<modint998244353>>
LinearMatroid<F> uniform_linear_representation(int rank, const vc<typename F::S>& distinct_points);
```

$r=\min(\mathrm{rank},n)$ として、点 $x_e$ に列 $(1,x_e,\ldots,x_e^{r-1})$ を割り当てる。一様マトロイドの線形表現を返す。`rank == 0` では全要素が零次元のループになる。

##### 制約

- `rank >= 0`。
- `distinct_points` の値は体の元で、$\min(\mathrm{rank},n)\geq 2$ のとき互いに異なる。

##### 計算量

- $O(nd)$、$d=\min(\mathrm{rank},n)$。

#### partition_linear_representation

```cpp
template<class F = FieldAddSubMulDiv<modint998244353>, class G = int, class C = int>
LinearMatroid<F> partition_linear_representation(
    const vc<G>& group, const vc<C>& capacity, const vc<typename F::S>& points);
template<class F = FieldAddSubMulDiv<modint998244353>, class G = int, class C = int>
LinearMatroid<F> partition_linear_representation(const vc<G>& group, const vc<C>& capacity);
```

要素 `e` の所属グループを `group[e]`、グループの容量を `capacity[g]` とした分割マトロイドの線形表現を返す。各グループの Vandermonde 表現を互いに異なる座標に配置する。容量が $0$ のグループの要素は零ベクトルになる。

##### 制約

- `points` を指定する場合は `points.size() == group.size()`。
- `0 <= group[e] < capacity.size()`、`capacity[g] >= 0`。
- 容量が $2$ 以上の各グループ内で、`points[e]` は体の元として互いに異なる。異なるグループ間では同じ値を使える。

##### 計算量

- $O(nd+k)$。$k$ はグループ数、$d=\sum_g\min(\mathrm{capacity}[g],\lvert\lbrace e\mid\mathrm{group}[e]=g\rbrace\rvert)$。

#### graphic_linear_representation

```cpp
template<class F = FieldAddSubMulDiv<modint998244353>, class I = int, class J = int>
LinearMatroid<F> graphic_linear_representation(int vertices, const vc<pair<I, J>>& edges);
```

辺 $(u,v)$ の列を、行 $u$ に $1$、行 $v$ に $-1$ を加えた接続行列で表す。任意の体でグラフ的マトロイドの線形表現を返す。自己ループは零ベクトルになり、多重辺にも対応する。

##### 制約

- `vertices >= 0`。
- 各辺の両端は `0..vertices-1`。

##### 計算量

- $O(nd)$、$d=\mathrm{vertices}$。

#### binary_linear_representation

```cpp
template<class F = FieldAddSubMulDiv<static_modint32<2>>, class V>
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
template<class F = FieldAddSubMulDiv<modint998244353>, class Cost, bool is_erasable, class Color,
         class Sample = decltype(random_modint<typename F::S>) &>
LinearMatroid<F> transversal_linear_representation(
    const GraphUndirected<Cost, is_erasable>& graph,
    const vc<Color>& color, Sample&& sample = random_modint<typename F::S>);
template<class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &, class I = int>
LinearMatroid<F> transversal_linear_representation(
    int right_vertices, const vvc<I>& adjacency, Sample&& sample = random_modint<typename F::S>);
template<class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &,
         class I = int, bool erasable = false>
LinearMatroid<F> transversal_linear_representation(
    int right_vertices, const CSR<I, erasable>& adjacency, Sample&& sample = random_modint<typename F::S>);
template<class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &, class I = int>
LinearMatroid<F> transversal_linear_representation(
    int right_vertices, initializer_list<vc<I>> adjacency, Sample&& sample = random_modint<typename F::S>);
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
template<class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &,
         class I = int, class J = int>
LinearMatroid<F> bicircular_linear_representation(
    int vertices, const vc<pair<I, J>>& edges, Sample&& sample = random_modint<typename F::S>);
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

## 疎な表現を使う例

[ABC399 G - Colorful Spanning Tree](https://atcoder.jp/contests/abc399/tasks/abc399_g) は、色ごとの容量制約を分割マトロイド、閉路を持たない制約を graphic matroid で表せる。両表現を $P,G$、乱数の対角行列を $D$ とすると $PDG^{\mathsf T}$ を作り、色区間に対応する行の rank で判定できる。

```cpp
auto g = GraphicMatroid(vertices, edges).sparse_linear_representation();
auto p = PartitionMatroid(color, capacity).sparse_linear_representation();
p.scale_columns(gen_vec(edges.size(), [](int) { return random_modint<modint998244353>(); }));
auto small = p.multiply_dense(g.transpose());
```

辺数が大きくても、密な結果の次元は容量の総和 × 頂点数になる。グループ $c$ に $m_c$ 要素、実効容量 $k_c=\min(A_c,m_c)$ がある場合、分割表現の非零候補数は $\sum_c m_c k_c$。例の問題では $m_c\leq N(N-1)/2$ と $\sum_c A_c\leq300$ により、この保存量も扱える。

色順に行を挿入し、[LinearBasis](../math/linalg/linear_basis.md) のタグと `rank(left)` を使うと区間の rank を得られる。各左端の最小の右端は単調なので、区間をまとめて数えられる。実装例は `verify/atcoder/abc399_g.test.cpp`。

数学的な適用例の参考：[ABC399 G 公式解説](https://atcoder.jp/contests/abc399/editorial/12546?lang=ja)。上の色区間の行範囲は、各色の実効容量の累積和を使う。


例の処理は $K=\sum_c k_c$、$S=\sum_c m_c k_c$ として、1試行 $O(M+S+KN^2+CN)$ 時間、$O(M+S+KN)$ 領域。区間ごとに交叉を最初から解く処理を省ける。

独立な一様乱数を $q$ 元体から選ぶと、固定した存在する区間の rank を過小評価する確率は $(N-1)/q$ 以下。各左端について最初に存在する右端を考え、rank の単調性を使うと、全区間の個数を誤る確率は $C(N-1)/q$ 以下と評価できる。検証例は独立に2試行し、個数の大きい方を採用するので、失敗確率はこの上界の二乗以下となる。これは rank の多項式評価の保証と単調性からの推論。
