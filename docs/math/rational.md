## 概要

分子 `num` と分母 `den` を持つ有理数。`T` は分子と分母を格納する型であり、四則演算の結果も `T` に格納する。四則演算の途中では約分せず、整数型の `Rational<T>` を出力または dump するときだけ一時的に約分する。

`Rational<ll>` などの整数型では四則演算、等値比較、大小比較を利用できる。比較時の交差積には `larger_int_t<T>` を使う。ただし、四則演算の結果自体は `T` に格納するため、未約分の分子と分母が `T` の範囲に収まらなければならない。

`Rational<mint>` も四則演算と等値比較に利用できる。有限体には大小関係がないため、大小比較は定義しない。有限体では gcd による約分も行わず、出力時にも内部の `num / den` をそのまま表示する。

## 使用例

```cpp
Rational<ll> x(2, 4), y(1, 3);
assert(x + y == Rational<ll>(5, 6));
cout << x << '\n'; // 1/2

Rational<mint> a(mint(2), mint(3));
Rational<mint> b(mint(4), mint(6));
assert(a == b);
```

## 詳細なドキュメント

### Rational

#### コンストラクタ

```cpp
Rational()
Rational(const T& num)
Rational(const T& num, const T& den)
```

それぞれ $0/1$, $\mathrm{num}/1$, $\mathrm{num}/\mathrm{den}$ を作る。約分はしない。符号つき整数型と浮動小数点数型では、分母が負なら分子と分母の符号を反転する。

##### 制約

- `den != 0`

##### 計算量

- $O(1)$

#### reduced

```cpp
pair<T, T> reduced() const
```

整数型では gcd で約分した `(num, den)` を返す。それ以外の型では内部の `(num, den)` をそのまま返す。自身の内部表現は変更しない。

##### 計算量

- 整数型：$O(\log \min(|\mathrm{num}|,\mathrm{den}))$
- その他：$O(1)$

#### 四則演算

```cpp
Rational operator+() const
Rational operator-() const
Rational& operator+=(const Rational& rhs)
Rational& operator-=(const Rational& rhs)
Rational& operator*=(const Rational& rhs)
Rational& operator/=(const Rational& rhs)
```

通常の有理数として四則演算する。非代入版の `+`, `-`, `*`, `/` も利用できる。演算結果は約分しない。`T` から `Rational<T>` への暗黙変換により、`x + T(1)` のような演算もできる。

##### 制約

- 除算では `rhs.num != 0`
- 演算途中の値が `T` の範囲に収まる

##### 計算量

- $O(1)$

#### 等値比較

```cpp
bool operator==(const Rational& rhs) const
bool operator!=(const Rational& rhs) const
```

両辺を `larger_int_t<T>` に変換してから交差積で値が等しいか判定する。`Rational<mint>` では `larger_int_t<mint> = mint` となるため、そのまま利用できる。

##### 制約

- 交差積が `larger_int_t<T>` で正しく計算できる

##### 計算量

- $O(1)$

#### 大小比較

```cpp
bool operator<(const Rational& rhs) const
bool operator>(const Rational& rhs) const
bool operator<=(const Rational& rhs) const
bool operator>=(const Rational& rhs) const
```

交差積で大小を比較する。`T` が整数型または浮動小数点数型の場合だけ利用できる。

##### 制約

- 交差積が `larger_int_t<T>` の範囲に収まる

##### 計算量

- $O(1)$

#### 出力

```cpp
ostream& operator<<(ostream& os, const Rational& x)
```

`num/den` の形式で出力する。整数型では出力時にだけ約分する。`PRINT` と `dump` も同じ表示用の値を使う。

##### 計算量

- 整数型：$O(\log \min(|\mathrm{num}|,\mathrm{den}))$
- その他：$O(1)$
