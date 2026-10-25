このドキュメントには AI による作成・変更が含まれています。

## 概要

最大値・最小値の集約には、[代数演算の部品](../algebra/algebra_basic_ops.md)を使う。
`MonoidMax<T>` と `MonoidMin<T>` は演算 `op` と単位元 `e` を持ち、`SegmentTree` などに渡せる。

## 使用例

```cpp
#include "algebra/algebra_basic_ops.hpp"
#include "ds/segtree/segtree.hpp"

SegmentTree<MonoidMax<ll>> seg(vc<ll>{3, 1, 4});
assert(seg.prod(0, 3) == 4);
assert(MonoidMax<ll>::op(2, 5) == 5);
```

## 詳細なドキュメント

#### 最大値・最小値の演算

```cpp
MonoidMax<T>::op(a, b)
MonoidMax<T>::e()
MonoidMin<T>::op(a, b)
MonoidMin<T>::e()
```

`op` はそれぞれ最大値・最小値を返す。単位元は `MonoidMax<T>` で負の無限大、`MonoidMin<T>` で正の無限大となる。型ごとの既定値や明示する値の条件は、上記の API 文書に従う。

##### 計算量

- $O(1)$（要素の比較・コピーを $O(1)$ とする）
