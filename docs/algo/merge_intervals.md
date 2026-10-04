## 概要

半開区間 $[l, r)$ の列を、同じ和集合を表す最小個数の区間列に変換する。
重なる区間と端点が接する区間を併合し、空区間を除いて左端の昇順に返す。

左端でソートし、直前の区間に順に併合する。

## 使用例

```cpp
#include "algo/merge_intervals.hpp"

vc<pair<int, int>> intervals = {{5, 8}, {1, 3}, {2, 4}, {4, 5}, {10, 12}, {9, 9}};
auto merged = merge_intervals(intervals);
// {{1, 8}, {10, 12}}
```

## 詳細なドキュメント

#### merge_intervals

```cpp
template <class T>
vc<pair<T, T>> merge_intervals(vc<pair<T, T>> intervals)
```

各要素 `(l, r)` を半開区間 $[l, r)$ として扱い、和集合を返す。
$l \geq r$ の区間は空区間として扱う。
返り値の各区間は $l < r$ を満たし、隣り合う区間は $r_i < l_{i+1}$ を満たす。
空の列や空区間のみの列には空の列を返す。入力は値渡しで受け取る。

##### 制約

- `T` の `operator<` は端点に全順序を与える

##### 計算量

区間数を $n$ として、

- $O(n \log n)$（端点の比較・コピーを $O(1)$ とする）
