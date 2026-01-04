## 黄金分割探索

https://nyaannyaan.github.io/library/dp/golden-section-search.hpp

#### golden_section_search

```cpp
pair<ll, T> golden_section_search(auto f, ll l, ll r, auto comp = less{})
```

$f$ は `ll` から `T` への関数。

- `comp` が `less{}` のとき、$f$ は $[l, r]$ の範囲で下に凸であるとして、$[l, r]$ での (最小値をとる点, 最小値) の組を返す。
- `comp` が `greater{}` のとき、$f$ は $[l, r]$ の範囲で上に凸であるとして、$[l, r]$ での (最大値をとる点, 最大値) の組を返す。

#### 計算量

- $O(\log(r - l))$
