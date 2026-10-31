このドキュメントには AI による作成・変更が含まれています。

## 概要

円の位置関係・交点・接線と、円周までの距離。数値型・誤差・入出力は [共通仕様](geometry.md) を参照。整数入力の位置関係・交差判定は平方根を使わずに行う。

## 使用例

```cpp
Circle<ll> c{{0, 0}, 5};
assert(contains(c, Point<ll>{3, 4}) == Location::Boundary);
auto hit = intersection(c, Line<ll>{{0, 0}, {1, 0}});
assert(hit.points.size() == 2);
auto ts = tangents(c, Point<ll>{10, 0});
assert(ts.lines.size() == 2);
dump(c, hit, ts);
```

## 詳細なドキュメント

### Circle

#### コンストラクタ・型変換

```cpp
template<class T = ld> struct Circle;
Circle();                          // 原点、半径0
Circle(Point<T> c, T r);
template<class U> explicit Circle(const Circle<U>& c);
template<class U> Circle<U> cast() const;
```

公開メンバ `Point<T> c` と `T r` を持つ。半径0は一点の円周として扱う。

##### 制約

- 半径は非負。公開メンバの変更後もこの条件を保つ

##### 計算量

- $O(1)$

#### 内外判定

```cpp
Location contains(Circle<T> c, Point<T> p, geometry_real_t<T> eps = EPS);
```

`Location::Inside`、`Boundary`、`Outside` のいずれかを返す。これらの整数値は順に `1, 0, -1`。

##### 計算量

- $O(1)$

#### 円同士の位置関係

```cpp
CircleRelation circle_relation(Circle<T> c, Circle<T> d, geometry_real_t<T> eps = EPS);
```

| 結果 | 関係 |
| --- | --- |
| `Separate` | 外部に離れている |
| `ExternallyTangent` | 外接 |
| `Intersecting` | 二点で交わる |
| `InternallyTangent` | 内接 |
| `Contained` | 一方が他方の内部に収まり、円周が離れている |
| `Coincident` | 中心・半径が一致 |

半径0の円が他方の円周上にある場合は `ExternallyTangent`、一致する二つの半径0の円は `Coincident`。

##### 計算量

- $O(1)$

#### 交差判定・交点

```cpp
bool intersects(Circle<T> c, A a, geometry_real_t<T> eps = EPS);
Intersection<geometry_real_t<T>> intersection(Circle<T> c, A a, geometry_real_t<T> eps = EPS);
```

`A` は `Line<T>`、`Segment<T>`、`Circle<T>`。引数を逆順にもできる。対象は円周であり、円の内部だけにある線分の交差判定は `false`。

交点が0個なら `Empty`、1個または2個なら `Points`。接点は1個だけ格納する。正の半径の円が一致すれば `Circle`、半径0の円が一致すれば一点を返す。`Intersection` の共通仕様は [直線・線分](line.md) を参照。

交点の順序は計算する図形の向きに依存する。座標順に使いたい場合は `points` を並べ替える。

##### 計算量

- $O(1)$

#### 接線

```cpp
Tangents<geometry_real_t<T>> tangents(Circle<T> c, Point<T> p, geometry_real_t<T> eps = EPS);
Tangents<geometry_real_t<T>> tangents(Point<T> p, Circle<T> c, geometry_real_t<T> eps = EPS);
Tangents<geometry_real_t<T>> common_tangents(Circle<T> c, Circle<T> d, geometry_real_t<T> eps = EPS);

template<class R> struct Tangents {
  bool infinite;
  vector<Line<R>> lines;
};
```

点から引ける接線、または二つの円の共通接線を列挙する。有限個なら `infinite == false` で、解が空の場合も `lines` にそのまま表す。無限個なら `infinite == true`、`lines` は空。

半径0の円への接線は、その中心を通る直線として定義する。中心自身からは無限個、別の点からは一点と中心を通る一本。同じ円同士の共通接線は無限個。異なる二つの半径0の円の共通接線は二点を通る一本。

各直線の二点は直線を定める点で、接点の組とは限らない。

##### 計算量

- $O(1)$

#### 円周までの距離

```cpp
geometry_real_t<T> distance(Circle<T> c, A a);
```

`A` は `Point<T>`、`Line<T>`、`Segment<T>`、`Circle<T>`。逆順にも対応する。円周と相手の図形との最短距離を返す。例えば中心と正の半径の円周との距離は半径に等しい。

##### 計算量

- $O(1)$
