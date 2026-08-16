## 概要

木の直径

## 詳細なドキュメント

#### tree_diameter

```cpp
(1) tuple<I, I, RootedTree<Cost, need_dist>> tree_diameter_by_depth<bool need_dist>(RootedTree<Cost, need_dist> g)
(2) tuple<I, I, RootedTree<Cost, true>> tree_diameter_by_cost(RootedTree<Cost, true> g)
```

いずれも、$(a, b, \mathrm{ga})$ を返す。ここで $a, b$ は直径の両端点、$\mathrm{ga}$ は $a$ を根とする根つき木。

- (1)：深さに基づいた木の直径を求める。
- (2)：コストに基づいた木の直径を求める。

##### 制約

- (2)：各辺のコストは非負

##### 計算量

- $O(n)$

