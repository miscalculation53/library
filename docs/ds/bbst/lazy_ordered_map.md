## 概要

[OrderedMapTree](ordered_map.md) に値への区間作用を加えた map。
順位区間とキー区間の両方で作用・モノイド積を指定できる。
公開型は `LazyOrderedMapTree<Key,AM,Compare>`、分割の返り値もこの型になる。

キー順を保ちながら値を更新する。キーの変更は削除と再挿入で行う。
作用後に挿入した値は、その過去の作用を引き継がない。

`AM` は `S`, `F`, `op`, `e`, `mapping`, `composition`, `id` を持つ作用つきモノイド。
`mapping(f,op(x,y)) = op(mapping(f,x),mapping(f,y))` と恒等作用・合成の法則を満たすこと。
`composition(f,g)` は $g$ の後に $f$ を適用する作用。
作用型は等値比較を省略できる。

## 使用例

```cpp
#include "ds/bbst/lazy_ordered_map.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"

LazyOrderedMapTree<int, ActedMonoidAffineSum<ll>> t;
t.insert(30, 3);
t.insert(10, 1);
t.insert(20, 2);
t.apply_by_order(1, 3, {2, 1});   // 値はキー順に [1,5,7]
assert(t.prod_by_order(1, 3).val == 12);
t.apply_by_key(10, 30, {1, 10});  // [11,15,7]
assert(t.all_prod().val == 33);
```

## 詳細なドキュメント

### LazyOrderedMapTree

コンストラクタと基本操作は [OrderedMapTree](ordered_map.md) を参照。
`M` を `AM`、公開型を `LazyOrderedMapTree` に読み替える。

#### apply_by_order / apply_by_key

```cpp
void apply_by_order(int l, int r, const F& f);
void apply_by_key(const Key& lo, const Key& hi, const F& f);
```

順位 `[l,r)` またはキー区間 `[lo,hi)` にある各値へ `f` を適用する。
キー区間は比較器について `!cmp(key,lo) && cmp(key,hi)` を満たす要素からなる。

##### 制約

- $0\le l\le r\le n$、`!cmp(hi,lo)`。
- `AM` が上記の作用つきモノイドの法則を満たす。

##### 計算量

- 期待 $O(\log(n+1))$。作用・モノイド演算・比較・値のコピーを $O(1)$ とする。

### 対応する問題

- [Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum)：固定キーを用いた区間アフィン変換と区間和。
- キーの増減と区間作用を組み合わせるケースは、独自テストで `std::map` と比較している。
