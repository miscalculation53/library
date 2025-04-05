## pb_ds の set/multiset

`std::set` および `std::multiset` と比較して、$k$ 番目取得ができる。

### コンストラクタ

- `pbds_set<T>`
- `pbds_multiset<T>`


### メンバ関数

`insert`, `erase`, `find` などはある。

また、`find_by_order(k)` で $k$ 番目の要素のイテレータが取得できる。

新しく定義したもの：

- `contains`
- `lt_cnt`, `leq_cnt`, `gt_cnt`, `geq_cnt`
  - 個数を数える
- `lt_max_it`, `leq_max_it`, `gt_min_it`, `geq_min_it`
  - イテレータを返す（なければ `end()`）
- `lt_max_id`, `leq_max_id`, `gt_max_id`, `geq_max_id`
  - 添字を返す（なければ `lt`, `leq` は $-1$、`gt`, `geq` は $n$）

**注意**：`pbds_multiset` のほうは、イテレータが指す値が `pair<T, int>` になっている（一応、`second` は追加された順番。これを使うことはあまり想定していない）。