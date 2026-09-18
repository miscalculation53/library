## 概要

積などの中間計算に使う拡張型を取得する。

通常の整数型はより広い整数型へ移す。`Rational<T>` は `Rational<larger_int_t<T>>` へ移す。それ以外の型は既定で自身へ移すため、128-bit整数、浮動小数点数型、mint などでは元の型と同じになる。

例えば `larger_int_t<Rational<ll>>` は `Rational<i128>` になる。`i128` 自体は拡張されないため、`larger_int_t<Rational<i128>>` は `Rational<i128>` のままである。型を取得するだけでは、演算がオーバーフローしないことまでは保証しない。

`larger_int_t<i128>` または `larger_int_t<u128>` を使用すると、それ以上広い整数型にならず中間計算がオーバーフローしうることをコンパイル時に警告する。`Rational<i128>` / `Rational<u128>` の拡張や、他のテンプレートから間接的に使用した場合も対象となる。`larger_int<T>::type` を直接使う場合も同様である。

ヘッダをインクルードするだけの場合や、`ll` → `i128`、`Rational<ll>` → `Rational<i128>` の拡張では、この警告は出ない。警告には `[[deprecated]]`（`-Wdeprecated-declarations`）を使用する。変換結果の型は従来と同じであり、警告をエラーとして扱う設定でなければコンパイルは継続する。

## 詳細なドキュメント

#### larger_int

```cpp
template <class T>
struct larger_int;
```

`T` に対応する計算型をメンバ型 `type` として持つ。対応する整数の特殊化があればより広い整数型になる。`Rational<V>` には、分子・分母の型を拡張した `Rational<larger_int_t<V>>` を返す。それ以外では `T` 自身になる。

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
larger_int_t<__int128_t> // __int128_t（警告）
larger_int_t<__uint128_t> // __uint128_t（警告）
larger_int_t<double>     // double
larger_int_t<mint>       // mint
larger_int_t<Rational<ll>>   // Rational<i128>
larger_int_t<Rational<i128>> // Rational<i128>（警告）
```

##### 計算量

- コンパイル時 $O(1)$
