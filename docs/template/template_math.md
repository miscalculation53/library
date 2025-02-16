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

```cpp
T divfloor<T = ll>(a, b)
```

$\lfloor a / b \rfloor$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### divceil

```cpp
T divceil<T = ll>(a, b)
```

$\lceil a / b \rceil$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### divround

```cpp
T divround<T = ll>(a, b)
```

$a / b$ を四捨五入した整数を返す（$.5$ は切り上げ）。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### safemod

```cpp
T safemod<T = ll>(a, b)
```

$b > 0$ のとき、$a \bmod b$ を $[0, b)$ の範囲で返す。（基本的には $b > 0$ での使用を想定している。）

$b < 0$ のとき、$-\lbrace (-a) \bmod (-b) \rbrace$ を返す。すなわち、$(b, 0]$ の範囲の値が返る。

いずれの場合も、$a - x$ が $b$ の倍数となるような $x$ のうち $0$ と $b$ の間（$b$ は含まない）にあるものを返す、と解釈できる。

$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

### 累乗・ルート

#### ipow

```cpp
T ipow<T = ll>(a, b)
```

$a^b$ を返す（$0^0$ は $1$ とする）。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $b \geq 0$
- $a^b$ が `T` に収まる

##### 計算量

- $a \in \lbrace -1,0,1 \rbrace$ のとき、$O(1)$
- それ以外のとき、$O(\log b)$

#### iroot

```cpp
T iroot<T = ll>(a, k)
```

$\mathrm{floor}(a^{1/k})$ を返す。$a, k$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $0 \leq a$
- $1 \leq k$

##### 計算量

- $a \in \lbrace 0, 1 \rbrace$ または $k = 1$ のとき、$O(1)$
- それ以外で $k = 2$ かつ $a < 2^{64}$ のとき、標準ライブラリの `sqrtl(a)` の計算量
- それ以外のとき、標準ライブラリの `pow(a, 1.0/k)` の計算量を $T$ として
  - $O(T + \log k)$

### 上限つきの乗算・累乗

（メモ：`__builtin_mul_overflow` というものもある）

#### mul_limited

```cpp
T mul_limited<T = ll>(a, b, m = INF)
```

$\min(a \times b, m)$ を返す。$a, b, m$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $a, b, m \geq 0$

##### 計算量

- $O(1)$

#### pow_limited

```cpp
T pow_limited<T = ll>(a, b, m = INF)
```

$\min(a^b, m)$ を返す。$a, b, m$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $a, b, m \geq 0$

##### 計算量

- $a = 1$ のとき、$O(1)$
- それ以外のとき、$O(\log \min(b, \log_a m))$

### 符号関数（誤差を考慮）

https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html

#### sgn

```cpp
int sgn<D = ld>(a, D eps = EPS)
```

誤差 $\mathrm{eps}$ を考慮した $a$ の符号を、$-1, 0, 1$ で返す。

整数の場合もこれを使ってよい。

### 位取り記数法

#### base_repr, base_repr_str

```cpp
(1) vc<T=ll> base_repr(auto val, auto base)
(2) vc<T=ll> base_repr(auto val, auto base, int n)
(3) string base_repr_str<bool use_upper = true>(auto val, auto base)
(4) string base_repr_str<bool use_upper = true>(auto val, auto base, int n)
```

$\mathrm{val}$ を $\mathrm{base}$ 進法で表現する。

- (1)：leading-zeros のない vector で表す。順番は通常の位取り記数法と同じ（下位桁が後ろ）。$0$ に対しては $(0)$ を返す。
- (2)：必要なら leading-zeros をつけて長さ $n$ の vector で表す。順番は通常の位取り記数法と同じ（下位桁が後ろ）。
- (3)：leading-zeros のない string で表す。$10$ から $35$ は `A` から `Z`（`use_upper` が false の場合は `a` から `z`）で表す。
- (4)：必要なら leading-zeros をつけて長さ $n$ の string で表す。$10$ から $35$ は `A` から `Z`（`use_upper` が false の場合は `a` から `z`）で表す。

##### 制約

- $\mathrm{val} \geq 0$
- $\mathrm{base} \geq 2$
  - (3), (4) では $2 \leq \mathrm{base} \leq 36$
- (2), (4) では $n \geq 0$

##### 計算量

- (1), (3)：$O(\log_\mathrm{base} \mathrm{val})$
- (2), (4)：$O(n + \log_\mathrm{base} \mathrm{val})$

##### 余談

逆変換（$\mathrm{base}$ 進法で整数を表す string を整数型に変換する）は、標準ライブラリの `stoll` 等を用いれば十分である。

```cpp
stoll(str, nullptr, base)  // base 進法表現された str を整数型に
```

（雑談：`stoll` に基数変換あるのに `to_string` にないのはなんでなんだろうね）