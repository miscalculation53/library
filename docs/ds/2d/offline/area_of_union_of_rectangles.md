このドキュメントには AI による作成・変更が含まれています。

## 概要

追加した半開長方形の和集合の面積を求める。重なった部分は一度だけ数える。負座標、重複、接触、面積 $0$ の長方形も扱える。

$x$ 座標順に平面走査し、圧縮した $y$ 座標の被覆長を配列上の区間木で管理する。追加メモリは $O(q)$。

## 使用例

```cpp
#include "ds/2d/offline/area_of_union_of_rectangles.hpp"

AreaOfUnionOfRectangles<long long> area;
area.rectangle_add(0, 3, 0, 2);
area.rectangle_add(2, 5, 1, 3);
auto answer = area.run(); // 11
```

## 詳細なドキュメント

### AreaOfUnionOfRectangles

#### コンストラクタ

```cpp
AreaOfUnionOfRectangles<T> area;
```

空のデータ構造を構築する。`T` は座標と面積の型で、既定は `ll`。通常の整数型・浮動小数点型に対応する。

##### 計算量

- $O(1)$

#### rectangle_add

```cpp
void rectangle_add(T lx, T rx, T ly, T ry);
```

半開長方形 $[lx, rx) \times [ly, ry)$ を追加する。幅または高さが $0$ なら面積への寄与は $0$。

##### 制約

- $lx \leq rx$、$ly \leq ry$
- 座標差と答えが `T` に収まる

##### 計算量

- 償却 $O(1)$

#### run

```cpp
T run();
```

追加済みのすべての長方形の和集合面積を返す。空なら `T(0)`。繰り返し呼び出せ、その後に追加した長方形も次回の計算に含む。

##### 計算量

追加した長方形の個数を $q$ として、

- $O(q \log q)$
