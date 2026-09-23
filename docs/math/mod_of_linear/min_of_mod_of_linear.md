## 概要

一次式の剰余 $r_i=(ai+b)\bmod m$ $(0\le i<n)$ の最小値を求める。負の $a,b$ は $[0,m)$ に正規化する。周期を超える長さにも対応する。

列を必要に応じて反転して公差を $m/2$ 以下にし、各折り返し直後の値だけを小さい法で再帰的に調べる。値域に入る項数を求める `count_mod_of_linear` は [floor_sum.hpp](floor_sum.md) にある。

## 使用例

```cpp
#include "math/mod_of_linear/min_of_mod_of_linear.hpp"

// 値は 3,7,2,6。
assert(min_of_mod_of_linear(4, 9, 4, 3) == 2);
```

## 詳細なドキュメント

#### min_of_mod_of_linear

```cpp
ll min_of_mod_of_linear(ll n, ll m, ll a, ll b)
```

$\min_{0\le i<n}r_i$ を返す。入口で $n,m\le2^{32}-1$ なら内部の積を `ull`、それより大きければ `u128` で計算する。正規化後の $a,b$ は $m$ 未満であり、反復中に $n,m$ は増えない。[速度比較](../runtime_integer_width.md)。

##### 制約

- $n\ge1$、$m\ge1$。

##### 計算量

- $O(\log m)$。
