## 概要

無限大の非型テンプレート引数を解決する。`nullptr` は「その型の既定値」を表す。数値や引数なし関数を明示した場合は、その指定を使う。

`resolved_value` と分けているため、通常の値の取得で `resolved_value<T*, nullptr>()` を使った場合は従来どおりヌルポインタを返す。

## 使用例

```cpp
static_assert(resolved_infty<i128>() == i128(INF) * INF);
static_assert(resolved_infty<i128, INF>() == i128(INF));

using R = Rational<ll>;
auto a = resolved_infty<R>();           // 1/0
auto b = resolved_infty<R, INF>();      // INF/1
auto c = resolved_infty<R, R::infty>(); // 1/0
```

## 詳細なドキュメント

### resolved_infty

```cpp
template <class T, auto x = nullptr>
T または const T& resolved_infty()
```

- `x` が `nullptr`：`default_infty<T>()` を返す
- 値を明示：`resolved_value<T, x>()` により `T(x)` を返す
- 引数なし関数を明示：`T(x())` を一度だけ構築し、以後は同じ値への参照を返す

明示した `INF` を型の既定値へ置き換えることはない。例えば `i128` に `INF` を明示した場合は、二乗せず `i128(INF)` を返す。

#### 制約

- `nullptr` の場合、その型の既定値が定義されている
- 値または関数の場合、`resolved_value` の制約を満たす

#### 計算量

- `default_infty` または `resolved_value` の計算量に従う
