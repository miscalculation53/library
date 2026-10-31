このドキュメントには AI による作成・変更が含まれています。

## 概要

整数・有理数・実数で使える二次元幾何。`geometry/geometry.hpp` から全機能を読み込める。

| ヘッダ | 内容 |
| --- | --- |
| [point.hpp](point.md) | 点兼ベクトル、内積・外積、向き・偏角、回転 |
| [line.hpp](line.md) | 直線・線分、交差、射影・反射、距離 |
| [circle.hpp](circle.md) | 円の位置関係・交点・接線 |
| [polygon.hpp](polygon.md) | 多角形の面積・内外判定・凸包・切断・始点の正規化 |
| [minkowski_sum.hpp](minkowski_sum.md) | 凸多角形の Minkowski 和 |
| [convex_cut.hpp](convex_cut.md) | 凸領域を保持して繰り返し切断 |
| [point_set.hpp](point_set.md) | 最近点対、凸多角形の直径 |
| [triangle.hpp](triangle.md) | 三角形の外心・内心・外接円・内接円 |

点は `Point<T>`、直線は `Line<T>`、線分は `Segment<T>`、円は `Circle<T>`。多角形は `vector<Point<T>>` で表す。座標・端点は公開メンバとして読み書きできる。二つの対象に関する操作は `distance(a, b)`、`intersects(a, b)` などの自由関数を使う。

### 数値型

`T` には符号付き組み込み整数（`i128` を含む）、それらを係数とする `Rational`、`float`・`double`・`long double`・`f128` を使う。各演算の引数の座標型をそろえる。型変換は `p.cast<U>()`、`l.cast<U>()` などで明示する。

| 別名 | 整数 `T` | `Rational<I>` | 実数 `T` |
| --- | --- | --- | --- |
| `geometry_wide_t<T>` | `i128` | `Rational<i128>` | `T` |
| `geometry_field_t<T>` | `Rational<i128>` | `Rational<i128>` | `T` |
| `geometry_real_t<T>` | `ld` | `ld` | `T` |

整数・有理数入力の向き・所属・交差・円の位置関係は厳密に判定する。交点・射影・反射・外心・直線や線分との距離の二乗には既存の [Rational](../math/rational.md) を使う。距離・角度・一般の回転・円の交点は実数で返す。`ld` は既存テンプレートの `decltype(EPS)`。

入力・結果・中間値が使用型に十分な余裕を持って収まることを前提とする。有理数では未約分の分子・分母と比較時の交差積も対象になる。各演算は `i128` までの整数を使う。

### 実数の比較

判定関数の `eps` の既定値は `EPS`。実数では、判定式の値が `[-eps, eps]` にあるときゼロとみなす。外積・内積・距離の二乗など、それぞれの判定式の単位での絶対誤差になる。座標の大きさに応じて `eps` を選ぶ。

`operator==`、辞書順、偏角の比較器、並べ替え時の重複除去は値を直接比較する。偏角の比較には型指定不要の `polar_less` など6種類を使える。同じ半直線上の点は距離によらず同値として扱う。近似的な点の一致には `almost_equal` を使う。浮動小数点座標は有限値を使う。近接した退化ケースの扱いには丸め誤差と `eps` が影響する。

### 入出力・表示

`cin` / `cout`、`READ` / `PRINT`、`dump` に対応する。配列内の図形も同じ操作で扱える。座標は `x y`、直線・線分は `ax ay bx by`、円は `cx cy r` の順。有理数の入力は整数または `num/den`、出力は約分済みの `num/den`。

`dump` では点の `x, y`、直線・線分の `a, b`、円の `c, r`、交差結果の `kind, points` を名前付きで表示する。

## 使用例

```cpp
#include "geometry/geometry.hpp"

using P = Point<ll>;
using S = Segment<ll>;

S s{{0, 0}, {3, 3}}, t{{0, 2}, {3, 0}};
auto hit = intersection(s, t);
if (hit.kind == IntersectionKind::Points) {
  PRINT(hit.points[0]);  // 6/5 6/5
}
dump(s, t, hit);

vector<P> points{{0, 0}, {4, 0}, {4, 3}, {0, 3}, {1, 1}};
auto hull = convex_hull(points);
PRINT(area(hull));      // 12/1
auto farthest = convex_diameter(hull);
PRINT(distance(farthest->first, farthest->second)); // 5
```

## 詳細なドキュメント

各ヘッダの文書を参照。固定幅整数・実数の演算を定数時間として計算量を記す。
