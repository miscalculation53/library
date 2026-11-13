このドキュメントには AI による作成・変更が含まれています。

## 概要

modint の法の範囲から一様に乱数を生成する。乱数エンジンには既存の `mt` を使い、`mt.seed(...)` で再現できる。

## 使用例

```cpp
#include "math/modint/random.hpp"

auto x = random_modint();
auto y = random_modint<modint61, true>();
assert(x.val() < modint998244353::mod());
assert(y != 0);
```

## 詳細なドキュメント

#### random_modint

```cpp
template<class mint = modint998244353, bool nonzero = false>
mint random_modint();
```

`nonzero == false` では `0..mint::mod()-1`、`true` では `1..mint::mod()-1` から一様に選び、`mint` に変換して返す。

##### 制約

- `mint::mod()` は正の64ビット整数で、`mint` は符号なし64ビット整数から構築できる。
- `nonzero == true` のとき、法は $2$ 以上。

##### 計算量

- 期待 $O(1)$ 回の乱数生成と、`mint` の構築1回。
