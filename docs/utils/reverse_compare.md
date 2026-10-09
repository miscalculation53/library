## 概要

比較器の順序を反転する。比較器が持つ状態も引き継ぐ。
`less<T>` と `greater<T>` は互いの型に、`ReverseCompare<Compare>` は元の型に変換する。

## 使用例

```cpp
#include "utils/reverse_compare.hpp"

static_assert(is_same_v<reverse_compare_t<less<int>>, greater<int>>);
auto comp = [](int a, int b) { return a % 7 < b % 7; };
auto reversed = reverse_compare(comp);
assert(reversed(5, 2));
```

## 詳細なドキュメント

### ReverseCompare / reverse_compare_t / reverse_compare

```cpp
template <class Compare> struct ReverseCompare { Compare comp; };
template <class Compare> using reverse_compare_t = /* 反転した比較器の型 */;
template <class Compare>
reverse_compare_t<Compare> reverse_compare(const Compare& comp);
```

`ReverseCompare<Compare>{comp}` は `comp(b,a)` で `a,b` を比較する。
`reverse_compare` は反転した型の比較器を返す。2 回反転すると元の型と状態に戻る。

##### 制約

- 比較器をコピー構築できる。

##### 計算量

- 比較器のコピーが $O(1)$ なら $O(1)$。
