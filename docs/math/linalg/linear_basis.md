このドキュメントには AI による作成・変更が含まれています。

## 概要

体上のベクトルを追加し、その線形包を管理する。独立性の判定、消去、元のベクトルによる表現、線形包の和と共通部分を扱う。
係数の追跡は `Track=true` で有効にする。追跡を省くと係数列の保存・演算も省く。

`basis()` は挿入に成功した元のベクトルを挿入順に返す。`coordinates()` の添字もこの順序に対応する。従属だった挿入は添字を消費しない。
係数型と体の演算は [Matrix](matrix.md) と共通。

各ベクトルに tag を付け、tag が threshold 以上のベクトルの線形包も問い合わせられる。`Tag=int` が既定で、`Tag=void` で tag の保存・比較を省く。tag と係数の追跡は併用できる。

## 使用例

```cpp
#include "math/modint/modint.hpp"
#include "math/linalg/linear_basis.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
LinearBasis<F, true> basis(3);
assert(basis.insert({0, 1, 0}));
assert(basis.insert({1, 0, 1}));
assert(!basis.insert({2, 3, 2}));
auto c = basis.coordinates({2, 3, 2});
assert(c == optional<vc<mint>>(vc<mint>{3, 2}));
assert(!basis.coordinates({0, 0, 1}));

LinearBasis<F> other(3, {{1, 0, 1}, {0, 0, 1}});
auto common = basis & other;
assert(common.rank() == 1);
assert((basis | other).rank() == 3);

LinearBasis<F, true> tagged(2);
tagged.insert({1, 0}, 0), tagged.insert({0, 1}, 1);
assert(!tagged.insert({1, 1}, 2));
assert(tagged.rank(2) == 1 && tagged.contains({1, 1}, 2));
assert(tagged.coordinates({1, 1}, 2) == optional<vc<mint>>(vc<mint>{1, 1}));
LinearBasis<F, false, void> plain(2); // tag の保存・比較を省く
plain.insert({1, 0});
```

## 詳細なドキュメント

### LinearBasis

```cpp
template <class F, bool Track = false, class Tag = int> class LinearBasis;
```

`S = F::S` とする。以下、ベクトルの次元を $d$、現在の階数を $r$ とする。
計算量では体の演算と tag の比較を定数時間と数える。
`TagType` は `Tag` と同じ型。

##### 制約

- `F` は可換体で、`S`, `e0`, `e1`, `add`, `minus`, `mul`, `inv` を持つ。
- `S` の等値比較が体の等値関係と一致する。
- 入力するベクトルの長さは $d$。
- `Tag` は `void`、またはコピー・ムーブと `<` による全順序比較ができる型。
- tag を省略する `insert(value)` とベクトル列からの構築では `Tag{}` を使うため、この場合は既定構築も必要。

#### コンストラクタ

```cpp
explicit LinearBasis(int dimension = 0)
LinearBasis(int dimension, const vvc<S>& vectors)
```

空の基底、または `vectors` の各ベクトルを順に挿入した基底を作る。
`dimension >= 0` とする。

##### 計算量

- 空の基底：$O(1)$。
- $m$ 個のベクトルからの構築：最終的な階数を $r$ として $O(md+mdr)$。

#### dimension / rank / basis

```cpp
int dimension() const
int rank() const
const vvc<S>& basis() const
```

ベクトルの次元、現在の階数、挿入に成功した元のベクトル列を返す。

##### 計算量

$O(1)$。

#### insert

```cpp
bool insert(const vc<S>& value)
```

現在の線形包の外にあるときベクトルを追加し、`true` を返す。
現在の線形包に属するときは `false` を返す。
タグの有無によらず、pivot の小さい順に消去できる基底を保持する。

##### 計算量

入力のコピー・走査に $O(d)$、基底による消去に $O(dr)$。

#### reduce / contains

```cpp
vc<S> reduce(const vc<S>& value) const
bool contains(const vc<S>& value) const
```

`reduce` は基底の pivot 成分を消去した残差を返す。元のベクトルと残差の差は現在の線形包に属する。
`contains` は現在の線形包に属するかを返す。所属することと残差が零ベクトルであることは同値。

##### 計算量

入力のコピー・走査に $O(d)$、消去に $O(dr)$。

#### coordinates

```cpp
optional<vc<S>> coordinates(const vc<S>& value) const  // Track=true
```

`basis()[i]` の係数を長さ $r$ の配列で返す。線形包の外では `nullopt` を返す。
空の基底での零ベクトルには、値ありの空配列を返す。

##### 計算量

入力のコピー・走査に $O(d)$、消去と係数の計算に $O(dr)$。

#### clear

```cpp
void clear()
```

ベクトルの次元を保ち、基底を空にする。

##### 計算量

$O(dr)$。

#### merge / operator\|

```cpp
template <bool OtherTrack> void merge(const LinearBasis<F, OtherTrack, Tag>& other)
template <bool OtherTrack> LinearBasis operator|(const LinearBasis<F, OtherTrack, Tag>& other) const
```

二つの線形包の和を求める。左右の `Tag` 型は等しいこと。
`Tag=void` の `merge` は `other.basis()` を順に現在の基底へ追加する。tag 有効時は `other` の保持行と tag を追加し、各 threshold での線形包の和を保つ。追加するベクトルと順序は保持行に対応し、`other.basis()` の元ベクトル列とは異なる場合がある。
`operator|` は左辺のコピーへ追加した結果を返す。左右の次元は等しいこと。
元の受理済みベクトルとその添字を保ち、新しく受理したベクトルを後ろへ加える。

##### 計算量

`other` の階数を $s$、和の階数を $R$ として $O(dsR)$。`operator|` はコピーに $O(dr)$ も使う。

#### intersection / operator&

```cpp
template <bool OtherTrack> LinearBasis intersection(const LinearBasis<F, OtherTrack, Tag>& other) const
template <bool OtherTrack> LinearBasis operator&(const LinearBasis<F, OtherTrack, Tag>& other) const
```

二つの線形包の共通部分の基底を返す。左右の次元と `Tag` 型は等しいこと。
返る基底の座標は、共通部分の計算で得た新しい元ベクトル列に対応する。

左の基底に右のベクトルを順に加え、その依存関係のうち左の元ベクトルによる部分を取り出す。
これらが共通部分を生成する。
tag 有効時は左右の保持行を tag の降順に加え、各 threshold での共通部分を生成するベクトルに、その時点の tag を付ける。

##### 計算量

左右の階数を $r,s$ として $O(d(r+s)^2)$。

#### tag 付きの挿入・問い合わせ

```cpp
bool insert(const vc<S>& value, Tag tag)
int rank(const Tag& threshold) const
vc<S> reduce(const vc<S>& value, const Tag& threshold) const
bool contains(const vc<S>& value, const Tag& threshold) const
optional<vc<S>> coordinates(const vc<S>& value, const Tag& threshold) const // Track=true
```

`Tag` 有効時に使える。問い合わせは tag が `threshold` 以上の全挿入ベクトルの線形包を扱う。

`insert(value, tag)` は全体の階数が増えたとき `true` を返す。従属な挿入でも、各 threshold の線形包を反映するために保持行と tag を更新する。同じ tag、任意の挿入順に対応する。

配列の添字を tag として左から挿入すると、右端 `r` まで挿入した状態で threshold に `l` を指定する問い合わせは区間 `[l,r]` の線形包を扱う。各問い合わせを右端順に処理することで、static な区間の問い合わせに使える。

`basis()` は全体で独立だった元ベクトル列を保ち、`coordinates` の係数は常にこの列に対応する。threshold は所属判定に適用され、係数に現れる元ベクトルの tag は threshold より小さい場合もある。例えば `({1,0},0),({0,1},1),({1,1},2)` の挿入後、threshold が2のときの `{1,1}` は、元ベクトル `{1,0}` と `{0,1}` の和で表される。

同じ pivot の二行 $(a,t_a),(b,t_b)$ に対し、$t_a\ge t_b$ なら $b$ から $a$ の適切な定数倍を引き、$t_b$ を保つことで、すべての threshold の線形包を保てる。この消去と正規化により、高い tag の行を優先しながら保持行数を階数以下に抑える。tag に要する追加の記憶量は $O(r)$ 個分。

##### 計算量

- `rank(threshold)`：$O(r)$。
- 挿入・その他の問い合わせ：コピー・走査に $O(d)$、消去と係数計算に $O(dr)$。
- `Track=true` の挿入では、行の入れ替えに備えて元ベクトルによる係数を求める場合がある。計算量は $O(dr)$。

ここで $r$ は条件を満たす行数ではなく、全体の階数。

#### basis(threshold)

```cpp
LinearBasis<F, Track, void> basis(const Tag& threshold) const
```

条件を満たす線形包を表す tag なしの基底を返す。返る基底の `basis()` と係数は、抽出時の保持行に対応する。

##### 計算量

$O(dr^2)$。
