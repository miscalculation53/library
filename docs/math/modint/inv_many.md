## 概要

$a_0, \dots, a_{n-1}$ の逆元を、逆元計算 1 回と $O(n)$ 回の乗算で求める。
体 `R` を明示して呼び出す。通常の modint には `FieldAddSubMulDiv<mint>` を指定する。

## 使用例

```cpp
#include "math/modint/modint.hpp"
#include "math/modint/inv_many.hpp"
#include "algebra/algebra_basic_ops.hpp"

using mint = modint998244353;
using R = FieldAddSubMulDiv<mint>;
vc<mint> a{2, 3, 5};
auto b = inv_many<R>(a);
```

## 詳細なドキュメント

#### inv_many

```cpp
template <class R>
vc<typename R::S> inv_many(const vc<typename R::S> &a);
```

$a_0^{-1}, \dots, a_{n-1}^{-1}$ を格納した vector を返す。
入力は変更しない。空配列には空配列を返し、その場合は逆元計算を行わない。
この関数自身は結果をキャッシュせず、空でない入力に対して呼び出すたびに逆元を 1 回計算する。

##### 制約

- 入力のすべての要素が可逆な可換環。
- `R::S`, `R::e1()`, `R::mul(a,b)`, `R::inv(a)` と要素の等値比較が利用できる。
  要素型の算術演算子や、整数からの構築は要求しない。

##### 計算量

- 乗算・コピーを $O(1)$、逆元計算 1 回の時間を $I$ として $O(n+I)$。
- 通常の modint では $O(n+\log p)$。
- 空配列なら $O(1)$。

## アルゴリズム

入力の累積積を求め、その全体の積の逆元を 1 回計算する。
後ろから入力の要素を掛けて累積積の逆元を復元し、各要素の逆元を取り出す。
