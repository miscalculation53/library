このドキュメントには AI による作成・変更が含まれています。

## 概要

凸領域を保持し、向き付き直線の左半平面との共通部分へ繰り返し更新する。整数・有理数入力では境界の交点を有理数で厳密に保持する。

各辺を定める元の直線を保持し、新しい交点を元の境界直線と切断直線から直接計算する。有理数座標は保存時に約分する。整数型・中間値・実数の誤差の条件は [共通仕様](geometry.md) を参照。

## 使用例

```cpp
#include "geometry/convex_cut.hpp"

ConvexCut<ll> region(10); // [-10,10] × [-10,10]
region.cut(Point<ll>{0, -1}, Point<ll>{0, 1}); // x <= 0
region.cut(Line<ll>{{-1, 0}, {1, 0}});        // y >= 0
auto polygon = region.nw_poly();
assert(area(polygon) == Rational<i128>(100));
dump(region, polygon);
```

## 詳細なドキュメント

### ConvexCut

#### コンストラクタ

```cpp
template<class T = ld> class ConvexCut;
explicit ConvexCut(const T& sp = T(100000), geometry_real_t<T> eps = EPS);
explicit ConvexCut(const vector<Point<T>>& p, geometry_real_t<T> eps = EPS);
```

前者は正方形 `[-sp,sp] × [-sp,sp]`、後者は与えた凸領域を初期状態にする。内部座標型 `value_type` は `geometry_field_t<T>`。整数・有理数の入力では `Rational<i128>`、実数では入力と同じ型になる。

点列による初期化では、向きを反時計回りにそろえ、連続する同一点・末尾の重複・共線の中間頂点を整理する。空・一点・線分も扱える。コピー・ムーブ後の領域は独立して更新できる。

##### 制約

- `sp >= 0`
- `p` は凸多角形の境界順、または同一直線上の点列

##### 計算量

- 正方形からの構築：$O(1)$
- 点列からの構築：$O(n)$

#### cut

```cpp
void cut(const Line<T>& line, geometry_real_t<T> eps = EPS);
void cut(const Point<T>& a, const Point<T>& b, geometry_real_t<T> eps = EPS);
```

`line.a` から `line.b`、または `a` から `b` へ向かう直線の左側を残す。直線上の点を含める。問い合わせ直前の領域を更新し、空になった領域もそのまま扱える。

##### 制約

- 切断直線を定める二点は異なる

##### 計算量

- 現在の頂点数を $n$ として $O(n)$

#### nw_poly / content

```cpp
vector<Point<value_type>> nw_poly() const;
vector<Point<value_type>> content() const;
```

現在の境界を反時計回りの点列としてコピーして返す。始点は `(y, x)` 最小。空・一点・線分への退化結果は、それぞれ空配列・一点・両端で表す。

`content()` は同じ結果を返す表示用の操作。`dump(region)` でも現在の点列を名前付きで確認できる。

##### 計算量

- $O(n)$

#### empty / size

```cpp
bool empty() const;
size_t size() const;
```

領域が空かどうかと、現在保持する頂点数を返す。

##### 計算量

- $O(1)$
