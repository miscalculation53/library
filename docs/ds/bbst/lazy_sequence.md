## 概要

[SequenceTree](sequence.md) に区間作用を加えた列。
挿入・削除・反転・移動・分割・連結・区間積・境界探索も同じ形式で使える。
公開型は `LazySequenceTree<AM>`、分割や切り出しの返り値もこの型になる。

`AM` は既存の `LazySegmentTree` と同じく `S`, `F`, `op`, `e`, `mapping`, `composition`, `id` を持つ。
作用は各要素に一様に適用し、`composition(f,g)` は $g$ の後に $f$ を適用する作用とする。
`mapping(f,op(x,y)) = op(mapping(f,x),mapping(f,y))` と恒等作用・合成の法則を満たすこと。
作用の有効フラグを内部に持ち、作用型は等値比較を省略できる。

反転時は順方向・逆方向の積を交換し、作用時は両方の積を更新する。
長さを用いる和などは、`ActedMonoidAffineSum` のように `S` に長さを含める。
各要素に保持した座標へ依存する作用は、その座標を要素と一緒に移動させる意味になる。
現在の列中の位置に応じた作用には、位置の移動に合わせた専用の管理が必要。

## 使用例

```cpp
#include "ds/bbst/lazy_sequence.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"

LazySequenceTree<ActedMonoidAffineSum<ll>> t(vector<ll>{1, 2, 3});
t.apply(0, 2, {2, 1});           // [3, 5, 3]
t.reverse(0, 3);                 // [3, 5, 3]
t.insert(1, 10);                 // [3, 10, 5, 3]
assert(t.prod(1, 3).val == 15);
```

## 詳細なドキュメント

### LazySequenceTree

コンストラクタと基本操作は [SequenceTree](sequence.md) を参照。
`M` を `AM`、`SequenceTree<M>` を `LazySequenceTree<AM>` に読み替える。

#### apply

```cpp
void apply(int p, const F& f);
void apply(int l, int r, const F& f);
```

単一要素または `[l,r)` の各要素に `f` を適用する。
作用後に挿入した要素には、その過去の作用を引き継がない。

##### 制約

- 単一要素版は $0\le p<n$、区間版は $0\le l\le r\le n$。
- `AM` が上記の作用つきモノイドの法則を満たす。

##### 計算量

- 期待 $O(\log(n+1))$。作用・モノイド演算・値のコピーを $O(1)$ とする。

### 対応する問題

- [Dynamic Sequence Range Affine Range Sum](https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum)：挿入・削除・反転・区間アフィン変換・区間和。
