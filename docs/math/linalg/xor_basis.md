このドキュメントには AI による作成・変更が含まれています。

## 概要

$\mathbb F_2$ 上のベクトルを XOR で追加し、線形包を管理する。符号なし整数と [DynamicBitset](../../ds/dynamic_bitset.md) に対応する。
独立性・元ベクトルによる表現・線形包の和と共通部分に加え、数値順での列挙や近傍検索を扱う。

`basis()` は挿入に成功した元のベクトルを挿入順に返す。座標の第 $i$ ビットは `basis()[i]` を使うことを表す。従属だった挿入は添字を消費しない。
`Track=true` で座標を追跡する。追跡を省くと係数の保存・演算も省く。

各ベクトルに tag を付け、tag が threshold 以上のベクトルの線形包も問い合わせられる。`Tag=int` が既定で、`Tag=void` で tag の保存・比較を省く。tag と座標の追跡は併用できる。

## 使用例

```cpp
#include "math/linalg/xor_basis.hpp"

XorBasis<ull, true> basis;
assert(basis.insert(3));
assert(basis.insert(5));
assert(!basis.insert(6));
assert(basis.coordinates(6) == optional<ull>(3));
assert(basis.kth(2) == optional<ull>(5)); // 線形包は 0,3,5,6
assert(basis.order_of_key(5) == 2);
assert(basis.predecessor_strict(5) == optional<ull>(3));
assert(basis.max_xor(1) == 7); // 1 XOR 線形包は 1,2,4,7

XorBasis<> tagged;
tagged.insert(1, 0), tagged.insert(2, 1);
assert(!tagged.insert(3, 2)); // 全体の階数は同じでも tag の情報は更新される
assert(tagged.rank(2) == 1);
assert(tagged.kth(1, 0ULL, 2) == optional<ull>(3));
assert(tagged.predecessor_strict(3, 0ULL, 2) == optional<ull>(0));
XorBasis<ull, false, void> plain; // tag の保存・比較を省く
plain.insert(3);

DynamicBitset x(200), y(200);
x.set(150), y.set(199);
XorBasis<DynamicBitset, true> dynamic(200);
dynamic.insert(x), dynamic.insert(y);
auto c = dynamic.coordinates(x ^ y);
assert(c && c->size() == 2 && c->all());
```

## 詳細なドキュメント

### XorBasis

```cpp
template <class V = ull, bool Track = false, class Tag = int> class XorBasis;
```

`V` は `bool` を除く符号なし整数型（`u128` まで）、または `DynamicBitset`。
`Tag` は `void`、またはコピー・ムーブと `<` による全順序比較ができる型。tag を省略する `insert(value)` とベクトル列からの構築では `Tag{}` を使うため、この場合は既定構築も必要。
`TagType` は `Tag` と同じ型。
`Coefficient` は整数型では `V`、`DynamicBitset` では長さが現在の階数に等しい `DynamicBitset`。

以下、ビット数を $d$、階数を $r$ とする。1ベクトルの XOR・コピー・比較の時間を $B$ とする。
整数型では $B=O(1)$、`DynamicBitset` ではワード幅 $w$ に対して $B=O(\lceil d/w\rceil)$。
数値順は上位ビットを優先する。`DynamicBitset` も同じ規則を使う。

#### コンストラクタ

```cpp
explicit XorBasis(int dimension = default_dimension)
XorBasis(int dimension, const vc<V>& vectors)
explicit XorBasis(const vc<V>& vectors)
```

空の基底、または `vectors` を順に挿入した基底を作る。
整数型の既定次元は型のビット幅、`DynamicBitset` の既定次元は0。
配列だけを渡した場合、`DynamicBitset` の次元は先頭要素の長さから推定し、空配列では0とする。

##### 制約

- `dimension >= 0`。
- 整数型では `dimension` が型のビット幅以下で、入力の `dimension` 以上のビットはすべて0。
- `DynamicBitset` では入力の長さがすべて `dimension`。

##### 計算量

- 空の基底：$O(B)$。
- $m$ 個のベクトルからの構築：最終的な階数を $r$ として $O(mB+mrB)$。

#### dimension / rank / basis

```cpp
int dimension() const
int rank() const
const vc<V>& basis() const
```

ベクトルのビット数、現在の階数、挿入に成功した元のベクトル列を返す。

##### 計算量

$O(1)$。

#### insert

```cpp
bool insert(const V& value)
```

現在の線形包の外なら追加して `true`、線形包に属するなら `false` を返す。
タグの有無によらず、上位 pivot から順に消去できる基底を保持する。引数なしの問い合わせは全ベクトルの線形包を扱う。

##### 計算量

$O(rB)$。`Track=true` では係数の XOR も行う。

#### reduce / contains / coordinates

```cpp
V reduce(V value) const
bool contains(const V& value) const
optional<Coefficient> coordinates(V value) const  // Track=true
```

`reduce` は pivot 成分を消去した残差を返す。これは `value` と線形包の XOR で得られる集合の最小値でもある。
`contains` は現在の線形包に属するかを返す。
`coordinates` は受理済みの元ベクトルによる表現を返し、線形包の外では `nullopt` を返す。
零ベクトルの座標は値ありで返す。空基底では整数の0、または長さ0の `DynamicBitset` になる。

##### 計算量

$O(rB)$。

#### min_xor / max_xor

```cpp
V min_xor() const
V min_xor(V affine) const
V max_xor() const
V max_xor(V affine) const
```

`affine` と線形包の XOR で得られる集合の最小値・最大値を返す。`affine` を省くと零ベクトルを使う。

##### 計算量

$O(rB)$。

#### span_size

```cpp
template <class Index = ull> Index span_size() const
```

線形包の要素数 $2^r$ を返す。`Index` は `u128` までの符号なし整数型とし、$2^r$ が収まること。

##### 計算量

$O(1)$。

#### kth

```cpp
template <class Index = ull> optional<V> kth(Index k) const
template <class Index = ull> optional<V> kth(Index k, V affine) const
```

`affine` と線形包の XOR で得られる集合を昇順に並べ、0始まりで第 `k` 要素を返す。
範囲外では `nullopt`。`affine` を省くと零ベクトルを使う。
`Index` は符号付き・符号なしの整数型で、`i128`・`u128` まで使える。

階数が `Index` のビット幅以上の場合も、型に収まる添字を渡せる。例えば階数128の基底へ `u128` の最大値を渡せる。

##### 計算量

$O(rB)$。

#### order_of_key

```cpp
template <class Index = ull> Index order_of_key(const V& key) const
template <class Index = ull> Index order_of_key(const V& key, V affine) const
```

`affine` と線形包の XOR で得られる集合のうち、`key` 未満の値の個数を返す。
`Index` は `u128` までの符号なし整数型とし、答えが収まること。`affine` を省くと零ベクトルを使う。

##### 計算量

$O(rB)$。

#### predecessor / predecessor_strict

```cpp
optional<V> predecessor(const V& key) const
optional<V> predecessor(const V& key, V affine) const
optional<V> predecessor_strict(const V& key) const
optional<V> predecessor_strict(const V& key, V affine) const
```

`affine` と線形包の XOR で得られる集合で、`predecessor` は `key` 以下、`predecessor_strict` は `key` 未満の最大値を返す。
該当する値がなければ `nullopt`。`affine` を省くと零ベクトルを使う。

##### 計算量

$O(rB)$。

#### clear

```cpp
void clear()
```

次元を保ち、基底を空にする。

##### 計算量

$O(rB)$。

#### merge / operator\|

```cpp
template <bool OtherTrack> void merge(const XorBasis<V, OtherTrack, Tag>& other)
template <bool OtherTrack> XorBasis operator|(const XorBasis<V, OtherTrack, Tag>& other) const
```

二つの線形包の和を求める。左右の `Tag` 型は等しいこと。
`Tag=void` の `merge` は `other.basis()` を順に追加する。tag 有効時は `other` の保持行と tag を追加し、各 threshold での線形包の和を保つ。追加するベクトルと順序は保持行に対応し、`other.basis()` の元ベクトル列とは異なる場合がある。
`operator|` は左辺のコピーへ追加した結果を返す。左右の次元は等しいこと。

##### 計算量

`other` の階数を $s$、和の階数を $R$ として $O(sRB)$。`operator|` はコピーに $O(rB)$ も使う。

#### intersection / operator&

```cpp
template <bool OtherTrack> XorBasis intersection(const XorBasis<V, OtherTrack, Tag>& other) const
template <bool OtherTrack> XorBasis operator&(const XorBasis<V, OtherTrack, Tag>& other) const
```

二つの線形包の共通部分の基底を返す。左右の次元と `Tag` 型は等しいこと。
`Tag=void` では、左の基底に右のベクトルを順に加え、依存関係に現れる左の元ベクトルの XOR を取り出す。
tag 有効時は左右の保持行を tag の降順に加え、各 threshold での共通部分を生成するベクトルに、その時点の tag を付ける。
返る基底の座標は、この計算で得た新しい元ベクトル列に対応する。

##### 計算量

左右の階数を $r,s$ として $O((r+s)^2B)$。

#### tag 付きの挿入・問い合わせ

```cpp
bool insert(const V& value, Tag tag)
int rank(const Tag& threshold) const
V reduce(V value, const Tag& threshold) const
bool contains(const V& value, const Tag& threshold) const
optional<Coefficient> coordinates(V value, const Tag& threshold) const // Track=true
V min_xor(V affine, const Tag& threshold) const
V max_xor(V affine, const Tag& threshold) const
template <class Index = ull> Index span_size(const Tag& threshold) const
template <class Index = ull> optional<V> kth(Index k, V affine, const Tag& threshold) const
template <class Index = ull> Index order_of_key(const V& key, V affine, const Tag& threshold) const
optional<V> predecessor(const V& key, V affine, const Tag& threshold) const
optional<V> predecessor_strict(const V& key, V affine, const Tag& threshold) const
```

`Tag` 有効時に使える。問い合わせは tag が `threshold` 以上の全挿入ベクトルの線形包を扱う。数値順の問い合わせでは `affine` も指定する。零ベクトルを渡すと線形包自体の値を調べられる。

`insert(value, tag)` は全体の階数が増えたとき `true` を返す。従属な挿入でも、各 threshold の線形包を反映するために保持行と tag を更新する。同じ tag、任意の挿入順に対応する。

配列の添字を tag として左から挿入すると、右端 `r` まで挿入した状態で threshold に `l` を指定する問い合わせは区間 `[l,r]` の線形包を扱う。各問い合わせを右端順に処理することで、static な区間の問い合わせに使える。

`basis()` は全体で独立だった元ベクトル列を保ち、`coordinates` の係数は常にこの列に対応する。threshold は所属判定に適用され、係数に現れる元ベクトルの tag は threshold より小さい場合もある。例えば `(1,0),(2,1),(3,2)` の挿入後、threshold が2のときの3は、元ベクトル1と2の XOR で表される。

同じ pivot の二行 $(a,t_a),(b,t_b)$ に対し、$t_a\ge t_b$ なら $(a,t_a),(a\mathbin{\mathrm{xor}}b,t_b)$ への置換はすべての threshold の線形包を保つ。この性質に従って高い tag の行を優先し、保持行数を階数以下に抑える。tag に要する追加の記憶量は $O(r)$ 個分。

`kth` は条件を満たす行の pivot を上位から処理し、結果の pivot ビットを添字の対応するビットに合わせる。同じ上位ビットを持つ候補は次の pivot ビットで等分されるため、通常の基底と同じ計算量で昇順の第 `k` 要素を求められる。

##### 計算量

- `rank(threshold)`・`span_size(threshold)`：$O(r)$。
- 挿入・その他の問い合わせ：$O(rB)$。ベクトルのコピー・生成には $O(B)$ も使う。
- `Track=true` の挿入では、行の入れ替えに備えて元ベクトルによる係数を求める場合がある。計算量は $O(rB)$。

ここで $r$ は条件を満たす行数ではなく、全体の階数。tag の比較を定数時間と数える。

#### basis(threshold)

```cpp
XorBasis<V, Track, void> basis(const Tag& threshold) const
```

条件を満たす線形包を表す tag なしの基底を返す。返る基底の `basis()` と係数は、抽出時の保持行に対応する。

##### 計算量

$O(r^2B)$。
