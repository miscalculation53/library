このドキュメントには AI による作成・変更が含まれています。

## 概要

同じ台集合上の複数のマトロイドについて、互いに素な独立集合に分けられる最大個数の要素を選ぶ。同じマトロイドを複数渡すと、その独立集合への分割を扱える。

未選択の要素から、fundamental circuit 上の交換を幅優先探索し、割当先を入れ替えて採用数を増やす。共通仕様は [マトロイド](matroid.md) を参照。

## 使用例

```cpp
#include "matroid/graphic_matroid.hpp"
#include "matroid/matroid_union.hpp"

GraphicMatroid forest(3, {{0, 1}, {1, 2}, {2, 0}});
auto parts = matroid_union(vc<GraphicMatroid>{forest, forest});
// 三角形の3辺すべてを、2つの森林へ分ける。
```

## 詳細なドキュメント

#### matroid_union

```cpp
template<class M>
vvc<int> matroid_union(vc<M> matroids);
```

`result[i]` は `matroids[i]` で独立な要素番号の昇順列。各要素は全体で高々一回現れ、採用した要素の総数が最大になる。空のマトロイド列には空配列を返す。

各 oracle を値で受け取る。同じ型のマトロイドで、パラメータを変えたものも組み合わせられる。

##### 制約

- 各 oracle の台集合の大きさが同じ。
- `M` が共通 oracle の仕様を満たす。

##### 計算量

台集合の要素数を $n$、マトロイド数を $k$、採用できる最大要素数を $R$ とする。各 oracle の `build` の最大時間を $B$、circuit の返却を含む `circuit` の最大時間を $C$ とする。

- 時間 $O(R(kB+knC))$。
- 追加メモリ $O(n+k)$。oracle の領域は別。

