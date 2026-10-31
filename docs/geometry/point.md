このドキュメントには AI による作成・変更が含まれています。

## 概要

二次元の点とベクトルを同じ `Point<T>` で扱う。数値型・誤差・入出力の共通仕様は [二次元幾何](geometry.md) を参照。

## 使用例

```cpp
Point<ll> a{1, 2}, b{4, 6};
Point<ll> c = pair{3, 5};
Point d(pair{3, 5LL}); // Point<ll>
auto v = b - a;
assert(norm2(v) == 25);
assert(orientation(a, b, Point<ll>{0, 5}) == 1);
auto half = v / 2LL; // Point<Rational<i128>>{3/2, 2/1}
dump(a, b, v, half);
```

## 詳細なドキュメント

### Point

#### コンストラクタ・型変換

```cpp
template<class T = ld> struct Point;
Point();                         // (0, 0)
Point(const T& x, const T& y);
template<class U, class V> Point(const pair<U, V>& p);
template<class U> explicit Point(const Point<U>& p);
template<class U> Point<U> cast() const;
```

公開メンバ `T x, y` を持つ。コピー・ムーブを使える。整数・実数の構築、加減算・スカラー倍、厳密比較、内積・外積、二乗距離、90度回転は定数式評価にも使える。

`pair` の `first, second` を `x, y` に使う。各要素を `T` へ暗黙変換できる `pair` から、初期化・代入・関数への引数渡しができる。`Point(pair{...})` と型を推論させる場合は `Point<common_type_t<U, V>>` になる。

##### 計算量

- $O(1)$

#### 演算・比較

```cpp
+p; -p;
p + q; p - q; p += q; p -= q;
p * s; s * p; p *= s;
p / s;                           // Point<geometry_field_t<T>>
p /= s;                          // 有理数・実数の座標型
p == q; p != q; p < q; p > q; p <= q; p >= q;
```

`s` は `T`。加減算・スカラー倍は座標型 `T` で計算する。除算は計算型に広げてから行い、整数入力でも分数を保つ。大小比較は `x, y` の辞書順。

##### 制約

- 除算では `s != 0`

##### 計算量

- $O(1)$

#### 内積・外積・向き・二乗距離

```cpp
geometry_wide_t<T> dot(Point<T> p, Point<T> q);
geometry_wide_t<T> cross(Point<T> p, Point<T> q);
geometry_wide_t<T> cross(Point<T> a, Point<T> b, Point<T> c);
geometry_wide_t<T> norm2(Point<T> p);
geometry_wide_t<T> distance2(Point<T> p, Point<T> q);
int orientation(Point<T> a, Point<T> b, Point<T> c, geometry_real_t<T> eps = EPS);
bool almost_equal(Point<T> p, Point<T> q, geometry_real_t<T> eps = EPS);
```

三点の `cross` は $(b-a)\times(c-a)$。差を取る前に計算型へ広げる。`orientation` は反時計回りなら `1`、時計回りなら `-1`、同一直線上なら `0`。同じ点を含む三点にも使える。`almost_equal` は座標ごとに誤差を判定する。

##### 計算量

- $O(1)$

#### 長さ・角度・回転

```cpp
geometry_real_t<T> norm(Point<T> p);
geometry_real_t<T> distance(Point<T> p, Point<T> q);
geometry_real_t<T> argument(Point<T> p);
geometry_real_t<T> angle(Point<T> p, Point<T> q);
Point<T> rot90(Point<T> p);
Point<geometry_real_t<T>> rotate(Point<T> p, Angle theta);
optional<Point<geometry_real_t<T>>> unit(Point<T> p);
```

`argument` は正の x 軸からの偏角。`angle` は `p` から `q` への符号付き角度で、いずれも `atan2` の範囲。角度の単位はラジアン。`rot90` と `rotate` は原点を中心とする反時計回りの回転。`rotate` は元の点を保ち、回転後の点を返す。`unit` は長さ1のベクトルを返し、零ベクトルでは `nullopt`。

##### 制約

- `argument`・`angle` の各ベクトルは零ベクトル以外

##### 計算量

- $O(1)$

#### 偏角の比較

```cpp
polar_less(p, q);          // <
polar_less_equal(p, q);    // <=
polar_greater(p, q);       // >
polar_greater_equal(p, q); // >=
polar_equal_to(p, q);      // ==
polar_not_equal_to(p, q);  // !=

sort(ALL(points), polar_less);
auto ordered = sorted(points, polar_less);
auto first = geq_min(points, p, polar_less);
auto last = gt_min(points, p, polar_less);
// [first, last) は p と同じ偏角の点の区間。
```

各比較器は座標型を推論する関数オブジェクト。正の x 軸から反時計回りに比較し、同じ半直線上の点は距離によらず同値とする。零ベクトルは先頭の独立した同値類として扱う。例えば `(1, 1)` と `(2, 2)` は偏角が等しく、`(-1, -1)` は異なる。

`polar_equal_to` は偏角の一致、`Point` の `operator==` は座標の一致を表す。整数・有理数では三角関数を使わず厳密に比較し、浮動小数点でも `EPS` を使わず値を直接比較する。整数・実数では定数式評価にも使える。

`geq_min` は同じ偏角の区間の先頭、`gt_min` はその直後の添字を返す。両者にはソート時と同じ `polar_less` を渡す。

##### 制約

- 両引数は同じ座標型の `Point<T>`
- 浮動小数点座標は有限値

##### 計算量

- 比較1回 $O(1)$

