## 概要

整数の符号判定、unsigned の絶対値、unsigned で演算する型の選択、下位 bit を保つ型変換を提供する。`i128` / `u128` にも対応する。

## 使用例

```cpp
#include "utils/integer_arithmetic.hpp"

static_assert(is_negative(-1));
static_assert(!is_negative(1ULL));
static_assert(unsigned_abs(LLONG_MIN) == (1ULL << 63));
static_assert(is_same_v<unsigned_arithmetic_t<short>, unsigned int>);
static_assert(wrapping_cast<ll>(~0ULL) == -1);
static_assert(wrapping_cast<unsigned char>(-1) == 255);
```

## 詳細なドキュメント

#### is_negative

```cpp
template <class T>
constexpr bool is_negative(T x)
```

$x<0$ を返す。unsigned の入力は常に `false`。

##### 制約

- `T` は組み込み整数型。`i128` / `u128` も使える。

##### 計算量

- $O(1)$。

#### unsigned_abs

```cpp
template <class T>
constexpr make_unsigned_ext_t<T> unsigned_abs(T x)
```

$|x|$ を対応する unsigned の型で返す。signed の最小値の絶対値も表せる。

##### 制約

- `T` は `bool` を除く組み込み整数型。`i128` / `u128` も使える。

##### 計算量

- $O(1)$。

#### unsigned_arithmetic / unsigned_arithmetic_t

```cpp
template <class T>
struct unsigned_arithmetic;

template <class T>
using unsigned_arithmetic_t = typename unsigned_arithmetic<T>::type;
```

整数の `T` には `common_type_t<unsigned int, make_unsigned_ext_t<T>>`、それ以外には `T` を返す。整数の加減乗算を unsigned で行い、それ以外は元の型で計算する汎用アルゴリズムに使う。

`unsigned short` なども `unsigned int` 以上にするため、整数昇格によって積が signed で計算されることを防ぐ。最終的な型への変換には `wrapping_cast` を使える。

##### 制約

- `T` は `bool` 以外。

##### 計算量

- コンパイル時 $O(1)$。

#### wrapping_cast

```cpp
template <class R, class T>
constexpr R wrapping_cast(T x)
```

戻り値型 `R` のビット幅を $w$ として、$x$ と $\bmod 2^w$ で合同な値を返す。signed なら $[-2^{w-1},2^{w-1})$、unsigned なら $[0,2^w)$ の代表元になる。幅を広げる場合の負数も、整数として合同な値に変換する。

##### 制約

- `R,T` は `bool` を除く組み込み整数型。`i128` / `u128` も使える。

##### 計算量

- $O(1)$。
