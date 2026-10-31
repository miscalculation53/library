このドキュメントには AI による作成・変更が含まれています。

## 概要

要素が 1 つだけの自明群。`GroupTrivial::S` は `monostate` で、演算結果は常にその唯一の値になる。
可換群・可換モノイドとして使える。データ構造に集約を載せるとき、集約を省く場合にも利用できる。

## 使用例

```cpp
#include "algebra/trivial.hpp"

using G = GroupTrivial;
constexpr auto e = G::e();
static_assert(G::op(e, e) == e);
static_assert(G::inv(e) == e);
```

## 詳細なドキュメント

### GroupTrivial

#### e / op / inv

```cpp
using S = monostate;
static constexpr S e();
static constexpr S op(S a, S b);
static constexpr S inv(S a);
```

単位元、2 要素の積、逆元を返す。いずれも唯一の値 `monostate{}` になる。

##### 計算量

- $O(1)$。

#### pow

```cpp
template <class I>
static constexpr S pow(S x, I k);
```

単位元を返す。指数 $k$ は非負整数とし、`k = 0` なら `e()` を返す。型と整数倍の条件は [pow の共通仕様](monoid_power.md) を参照。

##### 計算量

- $O(1)$。整数倍を使う場合は、その演算を $O(1)$ とする。
