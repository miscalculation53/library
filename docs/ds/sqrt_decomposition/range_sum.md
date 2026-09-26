## 概要

平方分割による一点加算・一点更新・区間和。各点の値と各ブロックの和を持ち、
一点加算・一点更新は $O(1)$、区間和は $O(B + n/B)$ 時間で行う。
$B = \Theta(\sqrt n)$ のとき区間和は $O(\sqrt n)$。

`G` は可換モノイドで、`set` を使う場合は可換群とする。
Mo の区間伸縮のように、一点更新の回数が多い用途に向く。
計算量は `G` の各演算を $O(1)$ としている。

## 使用例

```cpp
#include "ds/sqrt_decomposition/range_sum.hpp"

SqrtDecompositionRangeSum<GroupAddSub<ll>, 4> sq(vc<ll>{1, 2, 3, 4, 5});
sq.add(1, 10);
sq.set(4, 7);
assert(sq.get(1) == 12);
assert(sq.sum(1, 5) == 26);
assert(sq.sum(2, 2) == 0);
assert(sq.content() == vc<ll>({1, 12, 3, 4, 7}));
```

## 詳細なドキュメント

### SqrtDecompositionRangeSum

#### コンストラクタ

```cpp
SqrtDecompositionRangeSum<G, B = 512>()
SqrtDecompositionRangeSum<G, B = 512>(int n)
SqrtDecompositionRangeSum<G, B = 512>(const vc<G::S>& vec)
```

それぞれ空列、長さ $n$ の単位元で埋めた列、`vec` で初期化する。
$B$ はブロック幅で、定数除算による高速化のためテンプレート引数で指定する。

区間和で扱う要素数はおおむね $n/B + 2B$ 以下。
最悪時の演算回数を目安にするなら $B \simeq \sqrt{n/2}$、接頭辞クエリなら $B \simeq \sqrt n$。
実際の最適値は、区間長の分布や `G::op` のコストにより変わる。

##### 制約

- $n \geq 0$, $B > 0$
- `G` は `S`, `e()`, `op(a, b)` を持つ可換モノイド
- `G::S` は中間結果を含めて演算の結果を表現できる

##### 計算量

- 空列は $O(1)$、それ以外は $O(n)$

#### get

```cpp
G::S get(int p) const
```

添字 $p$ の値を返す。

##### 制約

- $0 \leq p < n$

##### 計算量

- $O(1)$

#### add

```cpp
void add(int p, const G::S& x)
```

添字 $p$ の値を `G::op(現在の値, x)` にする。

##### 制約

- $0 \leq p < n$

##### 計算量

- $O(1)$

#### set

```cpp
void set(int p, const G::S& x)
```

添字 $p$ の値を $x$ にする。

##### 制約

- $0 \leq p < n$
- `G` は `inv(a)` を持つ可換群

##### 計算量

- $O(1)$

#### sum

```cpp
G::S sum(int l, int r) const
```

区間 $[l, r)$ の和を返す。空区間では `G::e()` を返す。
長さ $32$ 以下は要素を直接足す。それより長い区間では、全体が含まれるブロックの和と両端の要素を足す。
末尾の短いブロックも、全体が含まれればブロックの和を使う。可換性を利用して和を $4$ 本の変数に分けて計算する。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- $O(B + (r-l)/B)$

#### content

```cpp
vc<G::S> content() const
```

現在の全要素を添字順に並べた配列のコピーを返す。空列では空の配列を返す。
`dump(sq)` による内容の表示にも使われる。

##### 計算量

- $O(n)$
