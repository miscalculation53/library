このドキュメントには AI による作成・変更が含まれています。

## 概要

二つの凸多角形の Minkowski 和を求める。集合としては $P+Q=\lbrace p+q\mid p\in P,\ q\in Q\rbrace$。入力頂点を全て組み合わせて足した点集合の凸包と一致する。

数値型・誤差・入出力は [共通仕様](geometry.md) を参照。図形の平行移動や、物体の大きさを加味した接触・衝突判定などに使える。

## 使用例

```cpp
#include "geometry/minkowski_sum.hpp"

using P = Point<ll>;
vector<P> square{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
vector<P> segment{{0, 0}, {3, 0}};
auto sum = minkowski_sum(square, segment);
assert((sum == vector<P>{{0, 0}, {5, 0}, {5, 2}, {0, 2}}));
```

## 詳細なドキュメント

#### minkowski_sum

```cpp
vector<Point<T>> minkowski_sum(const vector<Point<T>>& p,
                               const vector<Point<T>>& q,
                               geometry_real_t<T> eps = EPS);
```

両方の入力を保ち、共線の中間頂点を省いた反時計回りの凸境界を返す。始点は `(y, x)` 最小の頂点。[reorder_polygon](polygon.md) と同じ始点規則になる。

入力は時計回り・反時計回りの双方に対応する。連続する同一点、末尾の始点の重複、辺の途中の共線頂点を整理してから、辺の向きの列を併合する。二つの引数に同じ点列を渡すこともできる。

片方が空なら空、片方が一点なら平行移動した図形を返す。線分同士の和も扱える。退化した結果は一点または異なる両端で表す。

##### 制約

- 各入力は凸多角形の境界順、または同一直線上の点列
- 座標和は座標型 `T` に、中間計算は [共通仕様](geometry.md) の計算型に十分な余裕を持って収まる

##### 計算量

- $O(n+m)$
