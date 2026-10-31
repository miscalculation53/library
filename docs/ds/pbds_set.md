このドキュメントには AI による作成・変更が含まれています。

## 概要

`std::set` および `std::multiset` と比較して、$k$ 番目取得ができる。

## 使用例

```cpp
#include "ds/pbds_set.hpp"

pbds_set<int> s;
s.insert(7);
s.insert(2);
s.insert(5);
assert(*s.find_by_order(1) == 5); // 0 始まりで 1 番目
assert(s.lt_cnt(5) == 1);
```

## 詳細なドキュメント

### pbds_set / pbds_multiset

#### コンストラクタ

- `pbds_set<T>`
- `pbds_multiset<T>`

`insert`, `erase`, `find` などはある。

また、`find_by_order(k)` で $k$ 番目の要素のイテレータが取得できる。

新しく定義したもの：

- `contains`
- `lt_cnt`, `leq_cnt`, `gt_cnt`, `geq_cnt`
  - 個数を数える
- `lt_max_it`, `leq_max_it`, `gt_min_it`, `geq_min_it`
  - `optional<const_iterator>` を返す（不在は `nullopt`）
- `lt_max_id`, `leq_max_id`, `gt_min_id`, `geq_min_id`
  - 整数 `I`（既定は `ll`）で順位を返す。左側の `lt_max_id`, `leq_max_id` は `-1`、右側の `gt_min_id`, `geq_min_id` は `size()` を番兵とする。左側の検索には符号付き整数型を使う

**注意**：`pbds_multiset` のほうは、イテレータが指す値が `pair<T, int>` になっている（一応、`second` は追加された順番。これを使うことはあまり想定していない）。
