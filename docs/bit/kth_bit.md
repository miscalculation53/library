このドキュメントには AI による作成・変更が含まれています。

## 概要

下から $k$ 番目のビットを取得

## 使用例

```cpp
#include "bit/kth_bit.hpp"

assert(kth_bit_pos(0b101100ULL, 0) == 2);
assert(kth_bit_pos(0b101100ULL, 2) == 5);
```

## 詳細なドキュメント

#### kth_bit_pos

```cpp
ull kth_bit_pos(ull x, int k)
```

`x` で立っているビットのうち、下から $k$ 番目のビット位置を返す。

##### 制約

- $0 \leq k < \operatorname{popcount}(x)$

##### 計算量

- $O(1)$
