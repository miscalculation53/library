## 概要

$128$ ビット整数に対応した make_singed と make_unsigned

## 詳細なドキュメント

#### make_unsigned_ext

```cpp
template <class T>
struct make_unsigned_ext;
```

標準の `make_unsigned_t<T>` と同様に符号なし型を得る。`i128` に対しては `u128` を返す。

##### 制約

- `T` は整数型

##### 計算量

- コンパイル時 $O(1)$

#### make_unsigned_ext_t

```cpp
template <class T>
using make_unsigned_ext_t = typename make_unsigned_ext<T>::type;
```

`make_unsigned_ext<T>::type` のエイリアス。

##### 制約

- `make_unsigned_ext<T>::type` が定義されている

##### 計算量

- コンパイル時 $O(1)$

#### make_signed_ext

```cpp
template <class T>
struct make_signed_ext;
```

標準の `make_signed_t<T>` と同様に符号付き型を得る。`u128` に対しては `i128` を返す。

##### 制約

- `T` は整数型

##### 計算量

- コンパイル時 $O(1)$

#### make_signed_ext_t

```cpp
template <class T>
using make_signed_ext_t = typename make_signed_ext<T>::type;
```

`make_signed_ext<T>::type` のエイリアス。

##### 制約

- `make_signed_ext<T>::type` が定義されている

##### 計算量

- コンパイル時 $O(1)$
