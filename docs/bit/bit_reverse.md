このドキュメントには AI による作成・変更が含まれています。

## 概要

ビットリバース

## 使用例

```cpp
#include "bit/bit_reverse.hpp"

assert(bitrev(8, 3) == 6);  // 011 → 110
assert(bit_reverse32(1) == (uint32_t(1) << 31));
```

## 詳細なドキュメント

#### bit_reverse32

```cpp
uint32_t bit_reverse32(uint32_t x)
```

`x` の $32$ bit を左右反転した値を返す。

##### 計算量

- $O(1)$

#### bitrev

```cpp
int bitrev(int pw2, int i)
```

`pw2` が $2$ 冪のとき、$[0, \mathrm{pw2})$ の範囲で `i` をビットリバースした値を返す。

具体的には、`pw2` のビット長を $k+1$ としたとき、`i` の下位 $k$ bit の順序を反転する。

##### 制約

- `pw2` は正の $2$ 冪
- $0 \leq i < \mathrm{pw2}$

##### 計算量

- $O(1)$
