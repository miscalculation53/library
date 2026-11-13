このドキュメントには AI による作成・変更が含まれています。

## 概要

ビット列を $\mathbb F_2$ 上のベクトルとみなす binary matroid。選択した列が XOR に関して線形独立な集合を独立とする。零ベクトルはループになる。
符号なし整数（`u128` まで）と [DynamicBitset](../ds/dynamic_bitset.md) に対応する。
線形消去と選択済み要素による表現には [XorBasis](../math/linalg/xor_basis.md) を使う。

線形表現は、このヘッダの `linear_representation()` メンバから取得できる。既定の係数体は標数 $2$ の `static_modint32<2>`。引数・条件は [線形表現](linear_representation.md) を参照。

`build` の要素番号の配列には、`i128`・`u128` を含む任意の整数型を使える。返す要素番号は `int`。

## 使用例

```cpp
#include "matroid/binary_matroid.hpp"

BinaryMatroid<> m({1, 2, 3, 4, 0});
m.build({0, 1});
assert(m.circuit(2) == optional<vc<int>>(vc<int>{0, 1}));
assert(!m.circuit(3));
assert(m.circuit(4)->empty());

DynamicBitset a(200), b(200);
a.set(129), b.set(199);
BinaryMatroid<DynamicBitset> wide({a, b, a ^ b});
wide.build({1, 0});
assert(wide.circuit(2) == optional<vc<int>>(vc<int>{1, 0}));
```

## 詳細なドキュメント

### BinaryMatroid

#### コンストラクタ

```cpp
template <class V = unsigned long long>
explicit BinaryMatroid(vc<V> vectors);
```

各ビット列をベクトルとして構築する。台集合は `vectors` の添字。初期状態の独立集合は空集合。
`UInt` と `Value` は、どちらも `V` の型別名。

以下、台集合の要素数を $n$、現在の独立集合の大きさを $r$、ビット数を $d$ とする。
1ベクトルの XOR・コピーの時間を $B$ とし、整数型では $B=O(1)$、`DynamicBitset` ではワード幅 $w$ に対して $B=O(\lceil d/w\rceil)$。

##### 制約

- `V` は `bool` を除く符号なし整数型でビット幅は128以下、または `DynamicBitset`。
- `DynamicBitset` の長さはすべて等しい。空配列の場合は次元0とする。

##### 計算量

移動と各列の長さの確認に $O(n+B)$。引数へ配列をコピーする場合は $O(nB)$ も使う。

#### size / dimension / columns

```cpp
int size() const
int dimension() const
const vc<V>& columns() const
```

台集合の要素数、ビット数、元のビット列全体への読み取り用参照を返す。
整数型のビット数は型のビット幅。

##### 計算量

$O(1)$。

#### build

```cpp
template<class I = int>
void build(const vc<I>& independent)
```

現在の独立集合を置き換える。指定する要素の順序は任意で、fundamental circuitもその順序で返す。

##### 制約

- 各要素は台集合に属し、互いに異なる。
- 指定したベクトルは $\mathbb F_2$ 上で線形独立。

##### 計算量

$O(n+r^2B)$。

#### circuit

```cpp
optional<vc<int>> circuit(int e) const
```

現在の独立集合を $I$ とする。$I+e$ が独立なら `nullopt`、従属なら fundamental circuit から `e` を除いた要素列を返す。
返す列のベクトルの XOR は `vectors[e]` に等しい。零ベクトルに対しては値ありの空配列になる。

##### 制約

- `e` は台集合に属し、$I$ の外にある。

##### 計算量

$O(rB)$。

#### sparse_linear_representation

`linear_representation` と同じ引数・係数体で、行を座標、列を要素とする `SparseMatrix<F>` を返す。構築の条件と計算量は [線形表現](linear_representation.md) を参照する。

## 係数体と乱択計算

通常の独立性判定には `XorBasis`、二元体上の行列演算には [MatrixMod2Dynamic](../math/linalg/matrix_mod2_dynamic.md) を使える。線形交叉・parity の多項式評価や BBLA の乱択には、十分大きい標数2の拡大体 $\mathbf F_{2^k}$ を使う。

元の行列の各成分 $0,1$ を拡大体の $0,1$ に移しても、行列の次元と列の独立性は保たれる。`linear_representation<F>()` の `F` に拡大体を指定し、乱択アルゴリズムにはその体の適切な sampler を渡す。入力のビット行列を保存し、その作用を拡大体のベクトルに対して計算する設計も可能。

64ビット nimber の加算 XOR・乗算 nim product は $\mathbf F_{2^{64}}$ の実現方法の一つ。既約多項式で構成する二進拡大体も利用できる。nim arithmetic の数学的な説明は [Ferguson, Game Theory, Part I, Section 5.3](https://www.cs.umb.edu/~eb/320/FergusonGames.pdf#page=34) を参照する。
