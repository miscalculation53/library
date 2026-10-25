このドキュメントには AI による作成・変更が含まれています。

## 概要

標準整数型に加えて `i128`, `u128` を扱うための型特性。

## 使用例

```cpp
#include "template/template_all_but_modint.hpp"
#include "utils/is_integral_ext.hpp"

static_assert(is_integral_ext<i128>);
static_assert(is_signed_ext<i128>);
static_assert(is_unsigned_ext<u128>);
```

## 詳細なドキュメント

#### is_integral_ext

```cpp
template <class T>
constexpr bool is_integral_ext
```

`T` が標準整数型、`i128`, `u128` のいずれかなら `true`。

##### 計算量

- コンパイル時 $O(1)$

#### is_signed_ext

```cpp
template <class T>
constexpr bool is_signed_ext
```

`T` が標準の符号付き整数型または `i128` なら `true`。

##### 計算量

- コンパイル時 $O(1)$

#### is_unsigned_ext

```cpp
template <class T>
constexpr bool is_unsigned_ext
```

`T` が標準の符号なし整数型または `u128` なら `true`。

##### 計算量

- コンパイル時 $O(1)$
