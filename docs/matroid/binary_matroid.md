このドキュメントには AI による作成・変更が含まれています。

## 概要

ビット列を $\mathbb F_2$ 上のベクトルとみなす binary matroid。選択した列が XOR に関して線形独立な集合を独立とする。零ベクトルはループになる。
符号なし整数（`u128` まで）と [DynamicBitset](../ds/dynamic_bitset.md) に対応する。
線形消去と選択済み要素による表現には [XorBasis](../math/linalg/xor_basis.md) を使う。

線形表現は、このヘッダの `linear_representation<F>(...)` メンバから取得できる。引数・条件は [線形表現](linear_representation.md) を参照。

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
void build(const vc<int>& independent)
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
