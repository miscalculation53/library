## テンプレート（演算）

### 定数

これらは必要に応じて書き換える（そのために `#define` で定義している）。

- `INF` $= 4 \times 10^{18} + 37$
  - $\in [2^{61}, 2^{62})$
  - `INF + INF` はオーバーフローしない
  - 一応素数

- `EPS` $= 10^{-11}$ (double)

### chmin, chmax

更新されたかどうかの bool が返る。

### 除算

負の場合の除算（切り捨て・切り上げ・あまり）にも対応したもの。

#### divfloor

`T divfloor<T = ll>(a, b)`

$\lfloor a / b \rfloor$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### divceil

`T divceil<T = ll>(a, b)`

$\lceil a / b \rceil$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### safemod

`T safemod<T = ll>(a, b)`

$b > 0$ のとき、$a \bmod b$ を $[0, b)$ の範囲で返す。（基本的には $b > 0$ での使用を想定している。）

$b < 0$ のとき、$-\{(-a) \bmod (-b)\}$ を返す。すなわち、$(b, 0]$ の範囲の値が返る。

いずれの場合も、$a - x$ が $b$ の倍数となるような $x$ のうち $0$ と $b$ の間（$b$ は含まない）にあるものを返す、と解釈できる。

$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

### 累乗・ルート

#### ipow

`T ipow<T = ll>(a, b)`

$a^b$ を返す（$0^0$ は $1$ とする）。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $b \geq 0$
- $a^b$ が `T` に収まる

##### 計算量

- $a \in \{-1,0,1\}$ のとき、$O(1)$
- それ以外のとき、$O(b)$

#### iroot

`T iroot<T = ll>(a, k)`

$\mathrm{floor}(a^{1/k})$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $0 \leq a$
- $1 \leq k$

##### 計算量

- $a,k \geq 2$ のとき、$O(\log a)$
- それ以外のとき、$O(1)$

### 上限つきの乗算・累乗

（メモ：`__builtin_mul_overflow` というものもある）

#### mul_limited

`T mul_limited<T = ll>(a, b, m = INF)`

$\min(a \times b, m)$ を返す。$a, b, m$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $a, b, m \geq 0$

#### pow_limited

`T pow_limited<T = ll>(a, b, m = INF)`

$\min(a^b, m)$ を返す。$a, b, m$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $a, b, m \geq 0$

### 符号関数（誤差を考慮）

https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html

#### sgn

`int sgn<D = ld>(a, D eps = EPS)`

誤差 $\mathrm{eps}$ を考慮した $a$ の符号を、$-1, 0, 1$ で返す。

整数の場合もこれを使ってよい。