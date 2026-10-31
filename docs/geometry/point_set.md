このドキュメントには AI による作成・変更が含まれています。

## 概要

最近点対と凸多角形の直径を求める。結果は元の点のペアで返す。距離の比較には二乗距離を使い、整数・有理数では厳密に比較する。数値型の条件は [共通仕様](geometry.md) を参照。

## 使用例

```cpp
vector<Point<ll>> p{{0, 0}, {3, 0}, {0, 4}};
auto nearest = closest_pair(p);
assert(distance2(nearest->first, nearest->second) == 9);
auto farthest = convex_diameter(convex_hull(p));
assert(distance2(farthest->first, farthest->second) == 25);
```

## 詳細なドキュメント

#### 最近点対

```cpp
optional<pair<Point<T>, Point<T>>> closest_pair(vector<Point<T>> p);
```

距離が最小の異なる二要素を返す。同じ座標の入力要素が複数あれば距離0のペアが有効な結果になる。要素数が2未満なら `nullopt`。複数の最適解がある場合はいずれかを返す。

分割統治し、y 座標順に結合した帯状領域で候補を調べる。入力は値で受け取り、呼び出し元の配列を保つ。

##### 計算量

- $O(n\log n)$

#### 凸多角形の直径

```cpp
optional<pair<Point<T>, Point<T>>> convex_diameter(const vector<Point<T>>& p);
```

距離が最大の二点を返す。空なら `nullopt`、一点なら同じ点を二つ持つペア。全点が同一直線上の場合は辞書順の両端。時計回り・反時計回り、辺の途中の点、連続する重複点、末尾の先頭点の重複を扱う。

一直線上の途中の点を整理し、回転キャリパー法で求める。複数の最適解がある場合はいずれかを返す。

##### 制約

- `p` は凸多角形の境界順、または同一直線上の点列

##### 計算量

- $O(n)$
