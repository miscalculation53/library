このドキュメントには AI による作成・変更が含まれています。

## 概要

長さ $n$ の列に対して、半開区間の全要素を同じ値に代入し、区間のモノイド積を取得する。積は添字の昇順に結合するので、非可換モノイドも使える。

代入では、区間を覆うノードを下から順に処理し、同じ高さのノード間で新しい値の $2^k$ 乗を共有する。以前の代入によって一様になったノードが端点をまたぐ場合は、元の値が残る部分だけ、その長さに必要な冪を下から順に求める。この処理が必要な一様ノードは端点ごとに高々 $1$ 個である。区間積の取得では、問い合わせと部分的に重なる一様ノード高々 $2$ 個について、重なった長さの累乗を求める。

## 使用例

```cpp
#include "algebra/affine_function.hpp"
#include "ds/segtree/range_assign_segtree.hpp"

using M = MonoidAffineFunction<ll>;
RangeAssignSegmentTree<M> seg(vc<M::S>{{2, 1}, {3, 0}, {1, 4}});
seg.set(1, 3, {1, 2});
auto f = seg.prod(0, 3);
assert(f.b == 2 && f.c == 9);
```

## 詳細なドキュメント

### RangeAssignSegmentTree

```cpp
template <class M>
struct RangeAssignSegmentTree;
```

`M::S` は要素型、`M::op` は結合演算、`M::e` は単位元を返す関数とする。単位元も代入できる。使用するメモリは $O(n)$。

以下の計算量では、`M::op`、`M::e`、要素のコピー・代入を $O(1)$ とする。

#### コンストラクタ

```cpp
RangeAssignSegmentTree();
explicit RangeAssignSegmentTree(int n);
RangeAssignSegmentTree(int n, const S& x);
template <class T>
explicit RangeAssignSegmentTree(const vc<T>& a);
```

引数なしでは空列、`n` のみでは単位元を $n$ 個並べた列、`n, x` では `x` を $n$ 個並べた列を作る。配列 `a` を渡すと、その要素順で列を作る。

##### 制約

- $n \geq 0$
- 配列から構築する場合は、`const T&` から `S` の要素に代入できる

##### 計算量

- $O(n)$

#### set

```cpp
void set(int l, int r, const S& x);
void set(int p, const S& x);
```

区間 $[l,r)$ の各要素を `x` に代入する。`set(p, x)` は一点への代入に相当する。

##### 制約

- $0 \leq l \leq r \leq n$
- 一点代入では $0 \leq p < n$

##### 計算量

- $O(\log n)$

#### prod

```cpp
S prod(int l, int r) const;
S get(int p) const;
S all_prod() const;
```

`prod(l, r)` は区間 $[l,r)$ の積を返す。空区間の積は `M::e()`。`get(p)` は一点の値を返し、`all_prod()` は列全体の積を返す。

##### 制約

- $0 \leq l \leq r \leq n$
- 一点取得では $0 \leq p < n$

##### 計算量

- `prod` と `get` は $O(\log n)$。`prod` が [pow_monoid](../../algebra/monoid_power.md) を通じて `M::pow` を使う場合は、高々 $2$ 回の累乗にかかる時間を加える
- `all_prod` は $O(1)$

#### prod_apply

```cpp
template <class X, class F>
X prod_apply(int l, int r, X x, F&& apply) const;
```

区間 $[l,r)$ の積を `x` に作用させる。[SegmentTree::prod_apply](segtree.md#prod_apply) と同じ作用の条件を満たすものとする。空区間では `apply` を呼ばず、`x` を返す。コールバックは同じオブジェクトを参照で使い、第 $1$ 引数は `X` の左辺値、第 $2$ 引数は呼び出し中に有効な `const S&` とする。

区間に含まれるノードは、格納済みの積を左から順に作用させる。一様なノードと部分的に重なる場合は、`M::pow` があれば必要な長さの累乗を求める。二分累乗を使う場合は、各冪を直接 `x` に作用させる。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- `apply` の呼び出しと結果の代入を合わせて $O(T)$ として $O(T\log n)$。部分的に重なる一様ノードがあれば、高々 $2$ 個について累乗にかかる時間を加える。二分累乗の場合、この追加時間は高々 $O(\log n)$ 回の `M::op` に相当する

#### max_right_ok・min_left_ok

```cpp
template <class G>
int max_right_ok(int l, const G& g) const;
template <class G>
int min_left_ok(int r, const G& g) const;
```

`max_right_ok(l, g)` は `g(prod(l, r))` を満たす最大の `r`、`min_left_ok(r, g)` は `g(prod(l, r))` を満たす最小の `l` を返す。

##### 制約

- $0 \leq l \leq n$ または $0 \leq r \leq n$
- `g(M::e()) == true`
- 区間を伸ばしたとき、`g` は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- `g` を $O(1)$ として $O(\log n)$

#### size・content

```cpp
int size() const;
vc<S> content() const;
```

`size()` は列の長さを返し、`content()` は現在の列を配列として返す。`LOCAL` では `dump(seg)` で列を確認できる。

##### 計算量

- `size` は $O(1)$
- `content` は $O(n)$

## 参考文献

代入値の二冪を複数のノードで共有する考え方の関連解説。noshi91「区間代入/区間積 Θ(logN)/query」（2019 年 10 月 5 日）。[noshi91 のメモ](https://noshi91.hatenablog.com/entry/2019/10/05/203704)
