## 概要

多倍長整数

## 詳細なドキュメント

### BigInteger

```cpp
BigInteger<base = 10, digit = 6>
```

符号付き多倍長整数。内部では `base^digit` を一つの基数として保持する。

以下では、内部基数での桁数を $N$、長さ $N$ の畳み込み時間を $M(N)$ とする。現在の実装では、大きな整数同士の乗算に畳み込みを使う。

制約：

- $2 \leq \mathrm{base} \leq 36$
- $\mathrm{digit} \geq 1$
- `ipow(base, digit)` が `int` に収まる
- 畳み込み時の係数と桁数が `convolution_4e18` の範囲に収まる

#### コンストラクタ

```cpp
(1) BigInteger()
(2) BigInteger(const string& s)
(3) BigInteger(T x)
```

- (1)：$0$ で初期化する。
- (2)：`base` 進表記の文字列 `s` から構築する。先頭の `'-'` で負数を表せる。
- (3)：整数型 `T` の値から構築する。`i128`, `u128` にも対応する。

##### 制約

- (2)：`s` は `base` 進表記として有効
- (3)：`is_integral_ext<T>` が `true`

##### 計算量

- (1)：$O(1)$
- (2)：`s` の長さに対して $O(\lvert s\rvert)$
- (3)：$O(N)$

#### to_string

```cpp
string to_string()
```

値を `base` 進表記の文字列として返す。負数には先頭に `'-'` が付く。

##### 計算量

- 時間：出力文字列の長さに対して線形

#### 比較演算子

```cpp
operator<, operator>, operator<=, operator>=,
operator==, operator!=
```

二つの `BigInteger` を整数として比較する。

##### 計算量

- $O(\max(N_a,N_b))$

#### 単項マイナス

```cpp
BigInteger operator-()
```

符号を反転した値を返す。

##### 計算量

- $O(N)$

#### abs

```cpp
BigInteger abs()
```

絶対値を返す。

##### 計算量

- $O(N)$

#### BigInteger 同士の加減算

```cpp
(1) BigInteger& operator+=(const BigInteger& b)
(2) BigInteger& operator-=(const BigInteger& b)
(3) BigInteger operator+(const BigInteger& b)
(4) BigInteger operator-(const BigInteger& b)
```

通常の整数の加算・減算を行う。

##### 計算量

- $O(\max(N_a,N_b))$

#### BigInteger 同士の乗算

```cpp
(1) BigInteger& operator*=(const BigInteger& b)
(2) BigInteger operator*(const BigInteger& b)
```

通常の整数の乗算を行う。

##### 計算量

- $O(M(N_a+N_b))$

#### inv

```cpp
BigInteger inv(int d)
```

除算に使う逆数の近似を、内部基数でおよそ `d` 桁の精度まで Newton 法で求める。

##### 制約

- `*this != 0`
- $d \geq 1$

##### 計算量

- $O(M(d)\log d)$

#### BigInteger による除算・剰余

```cpp
(1) BigInteger operator/(const BigInteger& b)
(2) pair<BigInteger, BigInteger> divmod(const BigInteger& b)
(3) BigInteger operator%(const BigInteger& b)
(4) BigInteger& operator/=(const BigInteger& b)
(5) BigInteger& operator%=(const BigInteger& b)
```

- (1), (4)：$0$ 方向へ丸めた商を返す。
- (2)：商と剰余 `(q, r)` を返す。`*this == q * b + r` で、`r` の符号は `*this` と同じ。
- (3), (5)：(2) の剰余を返す。

除数が内部基数で1桁なら線形時間の除算を行う。それ以外では、小〜中サイズには筆算除算、大きいサイズにはNewton法による逆数計算を使用する。

##### 制約

- `b != 0`

##### 計算量

被除数と除数の内部桁数をそれぞれ $N$, $D$、商の最大桁数を $Q=\max(1,N-D+1)$ とする。

- $D=1$：$O(N)$
- 筆算除算を使う場合：$O(DQ)$
- Newton法を使う場合：$O(M(N)\log N)$

#### 整数型との加減算

```cpp
(1) BigInteger& operator+=(T v)
(2) BigInteger& operator-=(T v)
(3) BigInteger operator+(T v)
(4) BigInteger operator-(T v)
(5) BigInteger operator+(T a, const BigInteger& b)
(6) BigInteger operator-(T a, const BigInteger& b)
```

整数型 `T` の値との加算・減算を行う。

##### 制約

- `is_integral_ext<T>` が `true`
- `larger_int_t<make_unsigned_ext_t<T>>` が定義されている

##### 計算量

- 最悪 $O(N)$

#### 整数型との乗算

```cpp
(1) BigInteger& operator*=(T v)
(2) BigInteger operator*(T v)
(3) BigInteger operator*(T a, const BigInteger& b)
```

整数型 `T` の値を掛ける。

##### 制約

- `is_integral_ext<T>` が `true`
- `larger_int_t<make_unsigned_ext_t<T>>` が定義されている
- 各内部桁と `v` の積が対応する `larger_int_t` に収まる

##### 計算量

- $O(N)$

#### 整数型による除算・剰余

```cpp
(1) BigInteger& operator/=(T v)
(2) BigInteger& operator%=(T v)
(3) BigInteger operator/(T v)
(4) BigInteger operator%(T v)
```

$0$ 方向へ丸めた商、または被除数と同じ符号の剰余を求める。

##### 制約

- `is_integral_ext<T>` が `true`
- `v != 0`
- `larger_int_t<make_unsigned_ext_t<T>>` が定義されている

##### 計算量

- $O(N)$

#### add_term

```cpp
BigInteger& add_term(long long i, T coef)
```

`coef * base^i` を加える。

##### 制約

- $i \geq 0$
- `is_integral_ext<T>` が `true`
- 計算途中の積が対応する `larger_int_t` に収まる

##### 計算量

- 償却 $O(1)$、桁上がりが連鎖する場合は最悪 $O(N)$

#### sub_term

```cpp
BigInteger& sub_term(long long i, T coef)
```

`coef * base^i` を引く。

##### 制約

- $i \geq 0$
- `is_integral_ext<T>` が `true`
- 計算途中の積が対応する `larger_int_t` に収まる

##### 計算量

- 結果が常に非負なら償却 $O(1)$
- 借りが連鎖する場合は最悪 $O(N)$

### 入出力

#### operator>>, rd1

```cpp
istream& operator>>(istream& is, BigInteger& a)
void rd1(BigInteger& a)
```

`base` 進表記の文字列を読み込む。ビルド設定に応じて通常の stream 版または fast I/O 版が有効になる。

##### 制約

- 入力は有効な `base` 進表記

##### 計算量

- 入力文字列の長さに対して線形

#### operator<<, wt1

```cpp
ostream& operator<<(ostream& os, const BigInteger& a)
void wt1(const BigInteger& a)
```

`base` 進表記で出力する。

##### 計算量

- 出力文字列の長さに対して線形

### 丸めつき除算

以下の関数は、引数の片方または両方に整数型を使う overload も持つ。

#### divfloor

```cpp
BigInteger divfloor(a, b)
```

$\left\lfloor a/b \right\rfloor$ を返す。

##### 制約

- `b != 0`

##### 計算量

- `b` が `BigInteger`：$O(M(N)\log N)$
- `b` が組み込み整数型：$O(N)$

#### divceil

```cpp
BigInteger divceil(a, b)
```

$\left\lceil a/b \right\rceil$ を返す。

##### 制約

- `b != 0`

##### 計算量

- `b` が `BigInteger`：$O(M(N)\log N)$
- `b` が組み込み整数型：$O(N)$

#### divround

```cpp
BigInteger divround(a, b)
```

$a/b$ を最も近い整数へ丸める。同距離の場合は大きい方へ丸める。

##### 制約

- `b > 0`

##### 計算量

- `b` が `BigInteger`：$O(M(N)\log N)$
- `b` が組み込み整数型：$O(N)$

#### safemod

```cpp
BigInteger safemod(a, b)
```

$0 \leq r < b$ を満たす剰余 $r$ を返す。

##### 制約

- `b > 0`

##### 計算量

- `b` が `BigInteger`：$O(M(N)\log N)$
- `b` が組み込み整数型：$O(N)$

#### safedivmod

```cpp
pair<BigInteger, BigInteger> safedivmod(a, b)
```

`q = divfloor(a, b)`, `r = a - qb` を `(q, r)` として返す。$0 \leq r < b$。

##### 制約

- `b > 0`

##### 計算量

- `b` が `BigInteger`：$O(M(N)\log N)$
- `b` が組み込み整数型：$O(N)$
