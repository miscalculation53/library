このドキュメントには AI による作成・変更が含まれています。

## 概要

重みの大きい要素から、独立性を保てるものを順に採用する。最大重み基、または最大重み独立集合を求める。oracle の仕様は [マトロイド](matroid.md) を参照。

## 使用例

```cpp
#include "matroid/partition_matroid.hpp"
#include "matroid/matroid_greedy.hpp"

PartitionMatroid m({0, 0, 1}, {1, 1});
auto basis = matroid_greedy(m, vc<ll>{5, 3, -2}); // {0, 2}
auto best = matroid_greedy(m, vc<ll>{5, 3, -2}, true); // {0}
```

## 詳細なドキュメント

#### matroid_greedy

```cpp
template<class M, class W>
vc<int> matroid_greedy(M matroid, const vc<W>& weight,
                      bool nonnegative_only = false);
```

`nonnegative_only == false` なら最大重み基を返す。`true` なら非負の重みの要素を対象とし、最大重み独立集合を返す。要素番号は昇順。同じ重みの要素は番号の小さいものから試す。空配列も有効な結果。

##### 制約

- `matroid.size() == weight.size()`。
- `M` が共通 oracle の仕様を満たす。
- `W` が順序比較と `W{}` による零の構築に対応する。

##### 計算量

台集合の要素数を $n$、ランクを $r$、`build` の時間を $B$、`circuit` の最大時間を $C$ とする。

- 時間 $O(n\log n+rB+nC)$。
- 追加メモリ $O(n)$。oracle の領域は別。
