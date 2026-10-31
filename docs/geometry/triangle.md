このドキュメントには AI による作成・変更が含まれています。

## 概要

三頂点から外心・内心・外接円・内接円を求める。三点が同一直線上の場合は `nullopt` を返す。数値型・誤差は [共通仕様](geometry.md) を参照。

## 使用例

```cpp
Point<ll> a{0, 0}, b{4, 0}, c{0, 3};
auto o = circumcenter(a, b, c);
PRINT(*o); // 2/1 3/2
auto circle = incircle(a, b, c);
assert(almost_equal(circle->c, Point<ld>{1, 1}));
assert(circle->r == 1);
```

## 詳細なドキュメント

#### 外心

```cpp
optional<Point<geometry_field_t<T>>> circumcenter(Point<T> a, Point<T> b, Point<T> c,
                                                 geometry_real_t<T> eps = EPS);
```

三頂点から等距離の点を返す。整数・有理数入力では有理数の座標を厳密に計算する。

##### 計算量

- $O(1)$

#### 外接円

```cpp
optional<Circle<geometry_real_t<T>>> circumcircle(Point<T> a, Point<T> b, Point<T> c,
                                                 geometry_real_t<T> eps = EPS);
```

三頂点を円周上に持つ円を返す。中心・半径とも実数。

##### 計算量

- $O(1)$

#### 内心・内接円

```cpp
optional<Point<geometry_real_t<T>>> incenter(Point<T> a, Point<T> b, Point<T> c,
                                            geometry_real_t<T> eps = EPS);
optional<Circle<geometry_real_t<T>>> incircle(Point<T> a, Point<T> b, Point<T> c,
                                            geometry_real_t<T> eps = EPS);
```

三辺から等距離で三角形の内部にある点と、その点を中心とする内接円を返す。

##### 計算量

- $O(1)$
