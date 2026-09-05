## 概要

分子 `num` と分母 `den` を持つ有理数。`T` は分子と分母を格納する型であり、四則演算の結果も `T` に格納する。四則演算の途中では約分せず、剰余演算を持つ型の `Rational<T>` を出力または dump するときだけ一時的に約分する。

`Rational<ll>` や `Rational<BigInteger<>>` など、`T` に大小比較がある場合は四則演算、等値比較、大小比較を利用できる。比較時の交差積には `larger_int_t<T>` を使う。ただし、四則演算の結果自体は `T` に格納するため、未約分の分子と分母が `T` の範囲に収まらなければならない。

大小比較がある型では `1/0`, `-1/0` をそれぞれ正負の無限大として扱う。分母が $0$ の値は分子を $1$ または $-1$ に正規化する。`numeric_limits<Rational<T>>::infinity()` から正の無限大を取得できる。

`Rational<mint>` も四則演算と等値比較に利用できる。有限体には大小関係がないため、大小比較は定義しない。有限体では gcd による約分も行わず、出力時にも内部の `num / den` をそのまま表示する。

## 使用例

```cpp
Rational<ll> x(2, 4), y(1, 3);
assert(x + y == Rational<ll>(5, 6));
cout << x << '\n'; // 1/2

Rational<mint> a(mint(2), mint(3));
Rational<mint> b(mint(4), mint(6));
assert(a == b);

using BI = BigInteger<>;
using R = Rational<BI>;
R infty(BI(1), BI(0));
assert(R(100) < infty);
```

## 詳細なドキュメント

### Rational

#### コンストラクタ

```cpp
Rational()
Rational(const T& num)
Rational(const T& num, const T& den)
```

それぞれ $0/1$, $\mathrm{num}/1$, $\mathrm{num}/\mathrm{den}$ を作る。約分はしない。大小比較を持つ型では、分母が負なら分子と分母の符号を反転する。分母が $0$ なら、分子の符号に応じて $1/0$ または $-1/0$ に正規化する。

##### 制約

- `T` が大小比較を持たない場合は `den != 0`
- `den == 0` の場合は `num != 0`

##### 計算量

- 固定幅整数型：$O(1)$
- `BigInteger` など：`T` のコピーと符号比較の計算量

#### reduced

```cpp
pair<T, T> reduced() const
```

剰余演算を持つ型では gcd で約分した `(num, den)` を返す。それ以外の型では内部の `(num, den)` をそのまま返す。自身の内部表現は変更しない。

##### 計算量

- 剰余演算を持つ型：$O(\log \min(|\mathrm{num}|,\mathrm{den}))$ 回の `T` の剰余演算
- その他：$O(1)$

#### is_infinite / is_finite

```cpp
bool is_infinite() const
bool is_finite() const
```

分母が $0$ なら `is_infinite()`、そうでなければ `is_finite()` が `true` を返す。

##### 計算量

- `T` と $0$ の比較の計算量

#### 四則演算

```cpp
Rational operator+() const
Rational operator-() const
Rational& operator+=(const Rational& rhs)
Rational& operator-=(const Rational& rhs)
Rational& operator*=(const Rational& rhs)
Rational& operator/=(const Rational& rhs)
```

有限値は通常の有理数として四則演算する。非代入版の `+`, `-`, `*`, `/` も利用できる。演算結果は約分しない。整数または `T` から `Rational<T>` への暗黙変換により、`x + 1` のような演算もできる。

無限大を含む場合は拡張実数として計算する。符号が打ち消し合う無限大の加減算、$0\times\infty$、$\infty/\infty$ は未定義である。

##### 制約

- 除算では `rhs.num != 0`
- 未定義な無限大演算を行わない
- 演算途中の値が `T` の範囲に収まる

##### 計算量

- 固定幅整数型、`mint`：$O(1)$
- `BigInteger` など：`T` の加減乗除の計算量

#### 等値比較

```cpp
bool operator==(const Rational& rhs) const
bool operator!=(const Rational& rhs) const
```

両辺を `larger_int_t<T>` に変換してから交差積で値が等しいか判定する。`Rational<mint>` では `larger_int_t<mint> = mint` となるため、そのまま利用できる。

##### 制約

- 交差積が `larger_int_t<T>` で正しく計算できる

##### 計算量

- 固定幅整数型、`mint`：$O(1)$
- `BigInteger` など：`larger_int_t<T>` による2回の乗算と比較の計算量

#### 大小比較

```cpp
bool operator<(const Rational& rhs) const
bool operator>(const Rational& rhs) const
bool operator<=(const Rational& rhs) const
bool operator>=(const Rational& rhs) const
```

交差積で大小を比較する。`T` が大小比較を持つ場合だけ利用できる。無限大は $-\infty < x < +\infty$ として比較する。

##### 制約

- 交差積が `larger_int_t<T>` の範囲に収まる

##### 計算量

- 固定幅整数型：$O(1)$
- `BigInteger` など：`larger_int_t<T>` による2回の乗算と比較の計算量

#### 出力

```cpp
ostream& operator<<(ostream& os, const Rational& x)
```

`num/den` の形式で出力する。剰余演算を持つ型では出力時にだけ約分する。無限大は `1/0` または `-1/0` と表示する。`PRINT` と `dump` も同じ表示用の値を使う。

##### 計算量

- `reduced()` と `T` の出力の計算量

#### numeric_limits

```cpp
numeric_limits<Rational<T>>::infinity()
```

`T` が大小比較を持つ場合に $1/0$ を返す。`resolved_value` により、この関数をそのまま `infty` の非型テンプレート引数として渡せる。

```cpp
using R = Rational<BigInteger<>>;
ShortestPath<true, R, numeric_limits<R>::infinity> graph(n, edges);
```

##### 制約

- `T` に大小比較が定義されている

##### 計算量

- `T` から $1$ と $0$ を構築する計算量
