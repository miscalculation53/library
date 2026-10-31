このドキュメントには AI による作成・変更が含まれています。

## 概要

多角形を巡回順に並んだ `vector<Point<T>>` として扱う。`Polygon<T>` は同じ型の別名。終点の重複を省いた点列を基本形とする。数値型・誤差は [共通仕様](geometry.md) を参照。

## 使用例

```cpp
vector<Point<ll>> p{{0, 0}, {3, 0}, {0, 3}, {1, 1}};
auto h = convex_hull(p);
assert(area2(h) == 9);
assert(contains(h, Point<ll>{1, 1}) == Location::Inside);
auto left = convex_cut(h, Line<ll>{{1, 0}, {1, 1}});
PRINT(area(left)); // 5/2
```

## 詳細なドキュメント

#### 始点の正規化

```cpp
void reorder_polygon(vector<Point<T>>& p);
```

`(y, x)` が辞書順で最小の頂点を先頭へ移す。頂点の巡回順と向きを保ち、入力を直接変更する。空の点列も扱える。

凸包の出力形式に合わせる、多角形を巡回シフトして比較する、処理結果の始点をそろえる場合にも使える。同じ座標が複数回ある場合は最初に現れる頂点を選ぶ。向きや頂点の重複の整理は呼び出し側で行う。

##### 計算量

- $O(n)$

#### 面積・周長

```cpp
geometry_wide_t<T> signed_area2(const vector<Point<T>>& p);
geometry_wide_t<T> area2(const vector<Point<T>>& p);
geometry_field_t<T> signed_area(const vector<Point<T>>& p);
geometry_field_t<T> area(const vector<Point<T>>& p);
geometry_real_t<T> perimeter(const vector<Point<T>>& p);
```

`signed_area2` は符号付き面積の二倍で、反時計回りが正。`area2` はその絶対値。`signed_area`、`area` はそれぞれ半分を返す。整数座標の面積は有理数で返す。

周長は最後と最初を結ぶ辺も含める。空・一点の面積と周長は0、二点の周長は二点間距離の二倍。面積の幾何学的な意味は単純多角形を前提とする。

##### 計算量

- $O(n)$

#### 内外判定

```cpp
Location contains(const vector<Point<T>>& p, Point<T> q, geometry_real_t<T> eps = EPS);
```

巻き数を使い `Inside`、`Boundary`、`Outside` を返す。時計回り・反時計回りの双方に対応。空なら `Outside`、一点ならその一点、二点ならその線分を境界として扱う。連続する同一点も扱える。

##### 制約

- 三点以上の内部を幾何学的な内外として使う場合は単純多角形

##### 計算量

- $O(n)$

#### 凸性判定

```cpp
bool is_convex(const vector<Point<T>>& p, geometry_real_t<T> eps = EPS);
```

巡回する三頂点の曲がる方向がそろっているかを返す。同一直線上の頂点を許す。空・一点・二点・全点が同一直線上の場合は `true`。

##### 制約

- 三点以上では、単純多角形の境界順または一直線上の点列。自己交差や境界順そのものの検査は呼び出し側で行う

##### 計算量

- $O(n)$

#### 凸多角形の高速な内外判定

```cpp
Location contains_convex(const vector<Point<T>>& p, Point<T> q,
                         geometry_real_t<T> eps = EPS);
```

反時計回りの凸多角形に対して `Inside`、`Boundary`、`Outside` を返す。最初の頂点からの扇形を二分探索し、問い合わせごとの前処理は行わない。

始点は任意。辺の途中の共線頂点を含められるため、`convex_hull(points, true)` の結果をそのまま使える。空・一点・線分・全点が同一直線上の場合にも対応する。始点と同じ頂点を末尾に一つ付けた形式も扱える。

```cpp
auto hull = convex_hull(points, true);
for (const auto& query : queries) {
  auto location = contains_convex(hull, query);
}
```

##### 制約

- 三点以上の非退化な入力は反時計回りの凸境界順。辺の途中の頂点は辺に沿う順序で並べる
- 隣接する頂点は異なる。末尾の始点の重複は一つまで許す
- 全点が同一直線上の場合は、その座標順または逆順を巡回シフトした点列

##### 計算量

- $O(\log n)$

#### 凸包

```cpp
vector<Point<T>> convex_hull(vector<Point<T>> p, bool keep_collinear = false,
                            geometry_real_t<T> eps = EPS);
```

辞書順最小の点から反時計回りに頂点を返す。末尾に先頭を重複させない。座標が等しい入力点は一つにまとめる。

`keep_collinear == false` なら辺の途中の点を省き、`true` なら境界上の入力点を残す。全点が同一直線上の場合は、前者は両端、後者は重複を除いた辞書順の全点。空・一点はそのまま返す。

##### 計算量

- $O(n\log n)$

#### 凸多角形の切断

```cpp
vector<Point<geometry_field_t<T>>> convex_cut(const vector<Point<T>>& p, Line<T> l,
                                             geometry_real_t<T> eps = EPS);
```

直線 `l.a` から `l.b` に向かって左側の閉半平面との共通部分を返す。頂点の巡回方向を保ち、整数入力の新しい頂点は有理数になる。隣接する同一点と末尾の重複をまとめる。空・一点・線分に退化した入力や結果にも対応する。

##### 制約

- `p` は凸多角形の境界順。退化した点列では線分上を一方向に並べる

##### 計算量

- $O(n)$
