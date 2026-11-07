このドキュメントには AI による作成・変更が含まれています。

## 概要

整数を環の要素へ写す。整数のコンストラクタを持たない要素型でも、環の加法・零元・単位元から計算できる。

## 使用例

```cpp
#include "algebra/from_integer.hpp"

using R = RingAddSubMul<ll>;
assert(from_integer<R>(7) == 7);
assert(from_integer<R>(-3) == -3);
```

## 詳細なドキュメント

#### from_integer

```cpp
template <class R>
constexpr typename R::S from_integer(ll n);
```

環の単位元 `R::e1()` を `n` 倍した要素を返す。`n = 0` なら `R::e0()`、負なら加法逆元を使った結果を返す。

`RingFromGroupMonoid`・`FieldFromGroupGroup` では、元の加法群が持つ `pow` も利用する。各演算が定数式評価に対応していれば、定数式でも利用できる。

##### 制約

- `R` は `S`・`add`・`e0`・`minus`・`e1` を持つ環。
- 入力・結果・中間計算が使用する型に十分な余裕を持って収まる。

##### 計算量

- $O(\log\lvert n\rvert)$ 回の加法。元の加法群の `pow` を使う場合は、その計算量。
