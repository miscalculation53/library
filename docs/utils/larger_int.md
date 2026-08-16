## 概要

積などの中間計算に使う拡張型を取得する。

通常の整数型はより広い整数型へ移す。それ以外の型は既定で自身へ移すため、128-bit整数、浮動小数点数型、mint などでは元の型と同じになる。

## 詳細なドキュメント

#### larger_int

```cpp
template <class T>
struct larger_int;
```

`T` に対応する計算型をメンバ型 `type` として持つ。対応する整数の特殊化があればより広い整数型になり、それ以外では `T` 自身になる。

##### 計算量

- コンパイル時 $O(1)$

#### larger_int_t

```cpp
template <class T>
using larger_int_t = typename larger_int<T>::type;
```

`larger_int<T>::type` のエイリアス。主に積を取るときのオーバーフロー回避に使う。

```cpp
larger_int_t<int>       // long long
larger_int_t<long long> // __int128_t
larger_int_t<__int128_t> // __int128_t
larger_int_t<double>     // double
larger_int_t<mint>       // mint
```

##### 計算量

- コンパイル時 $O(1)$
