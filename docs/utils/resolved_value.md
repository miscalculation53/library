## 概要

テンプレート引数 `x` が値なら `T(x)` を返し、引数なしで呼び出せる関数なら、その返り値から構築した `T` を返す。関数から作る値は最初の呼び出し時に一度だけ構築され、以後は同じ値への参照を返す。

クラス型の値を非型テンプレート引数にできない場合に、その値を返す関数を代わりに渡すために使用する。

## 使用例

```cpp
static_assert(resolved_value<ll, 123>() == 123);

BigInteger<> bigint_infty()
{
  return BigInteger<>("1000000000000000000000000000000");
}

const auto &infty = resolved_value<BigInteger<>, bigint_infty>();
```

## 詳細なドキュメント

### resolved_value

```cpp
template <class T, auto x>
T または const T& resolved_value()
```

`x` が引数なしで呼び出せない場合は `T(x)` を返す。この場合は定数式としても利用できる。

`x` が引数なしで呼び出せる場合は、`T(x())` を一度だけ構築して `const T&` として返す。

#### 制約

- 値の場合、`T` を `x` から構築できる
- 関数の場合、`T` を `x()` の返り値から構築できる

#### 計算量

- 値の場合：`T(x)` の構築時間
- 関数の場合：初回は `x()` と `T` の構築時間、それ以降は $O(1)$
