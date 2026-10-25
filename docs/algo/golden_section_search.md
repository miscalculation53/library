このドキュメントには AI による作成・変更が含まれています。

## 概要

三分探索と使用可能場面はだいたい同じで、探索回数が少なくなる。

https://nyaannyaan.github.io/library/dp/golden-section-search.hpp

## 使用例

```cpp
#include "algo/golden_section_search.hpp"

auto [x, value] = golden_section_search(
    [](ll x) { return (x - 7) * (x - 7); }, 0, 20);
assert(x == 7 && value == 0);
```

## 詳細なドキュメント

#### golden_section_search

```cpp
pair<ll, T> golden_section_search(auto f, ll l, ll r, auto comp = less{})
```

$f$ は `ll` から `T` への関数。

- `comp` が `less{}` のとき、$f$ は $[l, r]$ の範囲で下に凸であるとして、$[l, r]$ での (最小値をとる点, 最小値) の組を返す。
- `comp` が `greater{}` のとき、$f$ は $[l, r]$ の範囲で上に凸であるとして、$[l, r]$ での (最大値をとる点, 最大値) の組を返す。

#### 計算量

- $O(\log(r - l))$
