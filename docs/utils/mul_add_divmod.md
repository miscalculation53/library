## 概要

非負整数の $ab+c$ を $m$ で割った商と剰余を求める。$a,c$ が正規化済み、すなわち $a,c<m$ の場合を扱う。このとき商は $b$ 以下になり、積が入力型を超えても商と剰余は同じ型で返せる。

64 bit 以下の入力は積を広い型で計算する。128 bit の入力で積があふれる場合は、商・剰余を保ちながら $b$ の各 bit を読み、256 bit の整数型を使わずに計算する。

## 使用例

```cpp
#include "utils/mul_add_divmod.hpp"

constexpr auto qr = mul_add_divmod(7ULL, 100ULL, 3ULL, 11ULL);
static_assert(qr.first == 63 && qr.second == 10);

const u128 m = ~u128(0);
auto [q, r] = mul_add_divmod(m - 1, m, m - 1, m);
assert(q == m - 1 && r == m - 1);
```

## 詳細なドキュメント

#### mul_add_divmod

```cpp
template <class U>
constexpr pair<U, U> mul_add_divmod(U a, U b, U c, U m)
```

$(\lfloor(ab+c)/m\rfloor,(ab+c)\bmod m)$ を返す。引数は同じ unsigned の型で指定する。

##### 制約

- `U` は `bool` を除く組み込み unsigned 整数型。`u128` も使える。
- $m\ge1$、$a<m$、$c<m$。

##### 計算量

- 64 bit 以下では $O(1)$。
- 128 bit の積が収まる場合は $O(1)$、あふれる場合はビット幅を $w=128$ として $O(w)$。
