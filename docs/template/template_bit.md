## 概要

テンプレート（ビット演算）

## 詳細なドキュメント

#### pow2, MASK

```cpp
pow2(k) == (1ULL << k)
MASK(k) == (1ULL << k) - 1
```

### C++20 にある機能

C++17 でも使えるようにしてある。また、C++20 では引数が unsigned でないと使えないが、signed でも使えるようにしてある。（あまりないと思うが、型の上限付近では注意。）

#### bit_width

```cpp
ull bit_width(ull x)
```

$x = 0$ ならば $0$、そうでなければ $1 + \lfloor \log_2 x \rfloor$ を返す。

|0|1|2|3|4|5|6|7|8|9|
|-|-|-|-|-|-|-|-|-|-|
|0|1|2|2|3|3|3|3|4|4|

#### bit_floor

```cpp
ull bit_floor(ull x)
```

$x = 0$ ならば $0$、そうでなければ $x$ 以下の最大の $2$ べきを返す。

|0|1|2|3|4|5|6|7|8|9|
|-|-|-|-|-|-|-|-|-|-|
|0|1|2|2|4|4|4|4|8|8|

#### bit_ceil

```cpp
ull bit_ceil(ull x)
```

$x$ 以上の最小の $2$ べきを返す。

|0|1|2|3|4|5|6|7|8|9|
|-|-|-|-|-|-|-|-|-|-|
|1|1|2|4|4|8|8|8|8|16|

#### countr_zero

```cpp
ull countr_zero(ull x)
```

下の桁に $0$ が続く個数を返す。

$x \neq 0$ の場合、$x$ が $2$ で割り切れる回数や `lsb_pos(x)` と一致。

$x = 0$ の場合に使うことは想定していない。使った場合以下の挙動を示す：
- C++17 以前・C++20 の signed で使えるようにこちらで実装したものを使った場合、エラー
- C++20 にある関数を使った場合（引数が unsigned の場合が該当する）、符号なし整数のビット数が返る

#### popcount

```cpp
ull popcount(ull x)
```

立っているビットの個数を返す。

#### has_single_bit

```cpp
bool has_single_bit(ull x)
```

$x$ が $2$ べきかどうかを返す。

#### lsb, msb

```cpp
(1) ull lsb_pos(ull x)
(2) ull msb_pos(ull x)
(3) ull lsb_mask(ull x)
(4) ull msb_mask(ull x)
```

- (1)：LSB（立っている最小のビット）の位置
- (2)：MSB（立っている最大のビット）の位置
- (3)：LSB だけを立てた値
- (4)：MSB だけを立てた値

##### 制約

- $x \neq 0$

### ビット操作

#### btest

```cpp
bool btest(ull x, uint k)
```

$x$ の下から $k$ ビット目が立っているかどうかを返す。

#### bset

```cpp
void bset(ull x, uint k, bool b = 1)
```

$x$ の下から $k$ ビット目を $b$ にする。

#### bflip

```cpp
void bflip(ull x, uint k)
```

$x$ の下から $k$ ビット目を反転させる。

#### bsubset

```cpp
bool bsubset(ull x, ull y)
```

$x \subseteq y$ かどうかを返す。

#### bsupset

```cpp
bool bsupset(ull x, ull y)
```

$x \supseteq y$ かどうかを返す。

#### bsetminus

```cpp
ull bsetminus(ull x, ull y)
```

$x \setminus y$ を返す。
