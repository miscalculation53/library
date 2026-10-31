このドキュメントには AI による作成・変更が含まれています。

## 概要

直線と線分を別の型で扱い、距離や交差を同じ関数名で呼べる。数値型・誤差・入出力は [共通仕様](geometry.md) を参照。交差・所属の判定は端点を含む。

## 使用例

```cpp
using P = Point<ll>;
Line<ll> l{P{0, 0}, P{2, 1}};
auto p = projection(l, P{1, 0});
PRINT(p);                          // 4/5 2/5
PRINT(distance2(l, P{1, 0}));       // 1/5
Segment<ll> s{P{0, 0}, P{4, 0}}, t{P{2, 0}, P{6, 0}};
auto hit = intersection(s, t);
assert(hit.kind == IntersectionKind::Segment);
dump(hit);                         // 重なりの両端は (2,0), (4,0)
```

## 詳細なドキュメント

### Line / Segment

#### コンストラクタ・型変換・方向

```cpp
template<class T = ld> struct Line;
template<class T = ld> struct Segment;
Line();                            // (0,0), (1,0) を通る直線
Segment();                         // 原点だけの線分
Line(Point<T> a, Point<T> b);
Segment(Point<T> a, Point<T> b);
template<class U> explicit Line(const Line<U>& l);
template<class U> explicit Segment(const Segment<U>& s);
template<class U> Line<U> Line::cast() const;
template<class U> Segment<U> Segment::cast() const;
Point<geometry_wide_t<T>> vec() const;
```

公開メンバ `Point<T> a, b` を持つ。`vec()` は `b-a`。直線の向きは `a` から `b`。線分は `a == b` も有効。

##### 制約

- 直線は常に `a != b`。公開メンバの変更後と型変換後も同じ条件を保つ

##### 計算量

- $O(1)$

#### 所属・平行・垂直・交差判定

```cpp
bool contains(Line<T> l, Point<T> p, geometry_real_t<T> eps = EPS);
bool contains(Segment<T> s, Point<T> p, geometry_real_t<T> eps = EPS);
bool parallel(A a, B b, geometry_real_t<T> eps = EPS);
bool perpendicular(A a, B b, geometry_real_t<T> eps = EPS);
bool intersects(A a, B b, geometry_real_t<T> eps = EPS);
```

`A, B` はそれぞれ `Line<T>` または `Segment<T>`。直線の一致・線分の重なりも交差に含む。整数・有理数では各判定を厳密に行う。

##### 制約

- `parallel`・`perpendicular` に渡す線分は異なる二端点を持つ

##### 計算量

- $O(1)$

#### 交差部分

```cpp
Intersection<geometry_field_t<T>> intersection(A a, B b, geometry_real_t<T> eps = EPS);

template<class F> struct Intersection {
  IntersectionKind kind;
  vector<Point<F>> points;
  bool empty() const;
  bool infinite() const;
};
```

`A, B` は `Line<T>` または `Segment<T>`。結果は次の形式。

| `kind` | `points` |
| --- | --- |
| `Empty` | 空 |
| `Points` | 唯一の交点1個 |
| `Segment` | 重なる線分の異なる両端 |
| `Line` | 一致する直線を定める二点 |

`empty()` は `Empty`、`infinite()` は連続した図形の交差部分で `true`。`IntersectionKind` には円同士の一致を表す `Circle` もある。線分同士の重なりの両端は辞書順、直線と線分の重なりは元の線分の端点順。整数入力の座標は有理数として返す。

##### 計算量

- $O(1)$

#### 射影・反射・最近点

```cpp
Point<geometry_field_t<T>> projection(Line<T> l, Point<T> p);
Point<geometry_field_t<T>> reflection(Line<T> l, Point<T> p);
Point<geometry_field_t<T>> closest_point(Segment<T> s, Point<T> p);
```

それぞれ直線への垂線の足、直線に関する対称点、線分上の最近点を返す。一点だけの線分の最近点はその一点。

##### 計算量

- $O(1)$

#### 距離

```cpp
geometry_field_t<T> distance2(A a, B b);
geometry_real_t<T> distance(A a, B b);
```

`A, B` は点・直線・線分の全組合せに対応する。点同士の `distance2` は `geometry_wide_t<T>`、それ以外は `geometry_field_t<T>`。図形間の最短距離を返す。

実数入力では、図形同士の交差による距離ゼロの判定に既定の `EPS` を使う。

##### 計算量

- $O(1)$
