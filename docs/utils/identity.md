## 概要

引数をそのまま返す関数オブジェクト。射影を省略したときの既定値として使える。
引数の型、参照、値カテゴリを保つ。

## 使用例

```cpp
#include "utils/identity.hpp"

int x = 3;
Identity{}(x) = 5;
assert(x == 5);
static_assert(is_same_v<decltype(Identity{}(declval<const int&>())), const int&>);
```

## 詳細なドキュメント

### Identity

```cpp
struct Identity {
  template <class T>
  constexpr T&& operator()(T&& x) const noexcept;
};
```

`std::forward<T>(x)` を返す。

##### 計算量

- $O(1)$。
