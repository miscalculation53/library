このドキュメントには AI による作成・変更が含まれています。

## 概要

分子 `num` と分母 `den` を持つ有理数。`T` は分子と分母を格納する型であり、四則演算の結果も `T` に格納する。四則演算は gcd を使わず、共通分母や等しい因子、ゼロを簡単に処理できる場合は表現の肥大化を抑える。剰余演算を持つ型では、`reduced()` や出力・dump で gcd による約分を行う。

`Rational<ll>` や `Rational<BigInteger<>>` など、`T` に大小比較がある場合は四則演算、等値比較、大小比較を利用できる。比較時の交差積には `larger_int_t<T>` を使う。ただし、四則演算の結果自体は `T` に格納するため、未約分の分子と分母が `T` の範囲に収まらなければならない。

`larger_int_t<Rational<T>>` は `Rational<larger_int_t<T>>` になる。異なる分子・分母型の `Rational` へは明示的に変換できる。大きな積を計算する場合は、演算前に広い型へ変換する。

大小比較がある型では `1/0`, `-1/0` をそれぞれ正負の無限大として扱う。分母が $0$ の値は分子を $1$ または $-1$ に正規化する。`Rational<T>::infty()` から正の無限大を取得できる。従来の `numeric_limits<Rational<T>>::infinity()` も同じ値を返す。

CHT・最短路などで `infty` を省略した場合も、`Rational` の[既定の無限大](../utils/default_infty.md)として $1/0$ を使う。有限値 `INF/1` を使いたい場合は `INF` を明示する。

`Rational<mint>` も四則演算と等値比較に利用できる。有限体には大小関係がないため、大小比較は定義しない。有限体では gcd による約分も行わず、出力時にも内部の `num / den` をそのまま表示する。

## 使用例

```cpp
Rational<ll> x(2, 4), y(1, 3);
assert(x + y == Rational<ll>(5, 6));
cout << x << '\n'; // 1/2

auto sum = Rational<ll>(1, 1'000'000'000) + Rational<ll>(2, 1'000'000'000);
assert(sum.num == 3 && sum.den == 1'000'000'000);

Rational<ll> z(-7, 3);
assert(abs(z) == Rational<ll>(7, 3));  // z.abs() でもよい
assert(sgn(z) == -1);
assert(inv(z) == Rational<ll>(-3, 7));
assert(floor(z) == -3 && ceil(z) == -2);

using W = larger_int_t<Rational<ll>>;  // Rational<i128>
W wide_x(x);
auto wide_product = W(x) * W(y);

Rational<mint> a(mint(2), mint(3));
Rational<mint> b(mint(4), mint(6));
assert(a == b);

using BI = BigInteger<>;
using R = Rational<BI>;
R infty = R::infty();
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

#### 異なる型からの変換

```cpp
template <class U>
explicit Rational(const Rational<U>& x)
```

分子・分母をそれぞれ `T` に変換し、分母の符号と無限大の表現を正規化する。約分はしない。元の `Rational<U>` は変更しない。

変換は `explicit` なので、`Rational<i128> wide(x)` や `Rational<i128>(x)` と書く。異なる `Rational` 型の間で四則演算や比較を行う場合も、先に型をそろえる。

##### 制約

- `T` を `const U&` から構築できる
- 変換後の分子・分母と符号の正規化に必要な値が `T` に収まる
- 無限大を変換する場合は、変換先も無限大を扱える型である

##### 計算量

- 固定幅整数型：$O(1)$
- その他：分子・分母を `T` に変換し、符号を正規化する計算量

#### infty

```cpp
static Rational infty()
```

正の無限大 $1/0$ を返す。負の無限大は `-Rational<T>::infty()` と書く。

値として使う場合は `R::infty()`、`resolved_value` に対応した非型テンプレート引数には、括弧を外した `R::infty` を渡す。C++17／20の両方で利用できる。

```cpp
using R = Rational<ll>;
using W = Rational<i128>;
auto inf = R::infty();
ConvexHullTrick<R, W, less<>, R::infty, W::infty> cht;
```

##### 制約

- `T` に大小比較が定義されている

##### 計算量

- `T` から $1$ と $0$ を構築する計算量

#### reduced

```cpp
pair<T, T> reduced() const
```

剰余演算を持つ型では gcd で約分した `(num, den)` を返す。それ以外の型では内部の `(num, den)` をそのまま返す。自身の内部表現は変更しない。

組み込み整数型では、分子・分母の絶対値を unsigned で受け取り、[テンプレート側の gcd](../template/template_math.md) を使う。`Rational<i128>` / `Rational<u128>` と、signed の最小値を分子に持つ値の約分にも対応する。

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

有限値は通常の有理数として四則演算する。非代入版の `+`, `-`, `*`, `/` も利用できる。整数または `T` から `Rational<T>` への暗黙変換により、`x + 1` のような演算もできる。

gcd を計算せず、次の場合は掛け算を省く。

- 加減算：分母が等しければ分子だけを加減算する。ゼロとの加減算はもう一方の値を使う。
- 乗算：一方の分子と他方の分母が等しければ、その因子を相殺する。
- 除算：分子どうし、分母どうしが等しければ、その因子を相殺する。
- ゼロの乗算・除算：結果を $0/1$ にする。

たとえば $(a/b)/(c/b)=a/c$、$(a/b)\times(b/c)=a/c$、有限の非零値の `x /= x` は $1/1$ になる。それ以外は交差積で計算する。

固定幅整数の加減算では交差積を符号なし整数で計算し、差し引き後の分子を復元する。各交差積が `T` を超えても、最終的な分子が `T` に収まれば正しく計算できる。`i128` / `u128` も対応する。

整数の除算も、積と符号の正規化を符号なし整数で計算してから復元する。結果の分子が signed の最小値となる場合にも対応する。

無限大を含む場合は拡張実数として計算する。符号が打ち消し合う無限大の加減算、$0\times\infty$、$\infty/\infty$ は未定義である。

##### 制約

- 除算では `rhs.num != 0`
- 未定義な無限大演算を行わない
- 結果として保存する分子・分母が `T` の範囲に収まる

##### 計算量

- 固定幅整数型、`mint`：$O(1)$
- `BigInteger` など：`T` の加減乗除の計算量

#### abs / sgn

```cpp
Rational abs() const
int sgn() const
Rational abs(const Rational& x)
int sgn(const Rational& x)
```

`abs()` は分母を保ったまま絶対値を返す。`sgn()` は負・ゼロ・正に対して $-1,0,1$ を返す。無限大にも対応し、`abs(-infty)` は正の無限大になる。`x.abs()` と `abs(x)`、`x.sgn()` と `sgn(x)` の両形式を使える。

##### 制約

- `T` に大小比較が定義されている
- `abs()` の結果の分子が `T` に収まる

##### 計算量

- 固定幅整数型：$O(1)$
- その他：`T` のコピー、符号比較、符号反転の計算量

#### inv

```cpp
Rational inv() const
Rational inv(const Rational& x)
```

逆数を返す。有限値では分子・分母を交換して符号を正規化し、正負の無限大では $0/1$ を返す。`x.inv()` と `inv(x)` の両形式を使える。`Rational<mint>` にも対応する。

##### 制約

- `num != 0`
- 結果の分子・分母と符号の正規化に必要な値が `T` に収まる

##### 計算量

- 固定幅整数型、`mint`：$O(1)$
- その他：`T` のコピーと符号の正規化の計算量

#### floor / ceil

```cpp
T floor() const
T ceil() const
T floor(const Rational& x)
T ceil(const Rational& x)
```

それぞれ床関数 $\lfloor x\rfloor$ と天井関数 $\lceil x\rceil$ を `T` で返す。整数の商と剰余で計算し、負数も正確に丸める。`x.floor()` と `floor(x)`、`x.ceil()` と `ceil(x)` の両形式を使える。

##### 制約

- `T` が整数の商・剰余と大小比較を持つ。組み込み整数型、`i128` / `u128`、`BigInteger` に対応する。
- 自身が有限値である

##### 計算量

- 固定幅整数型：$O(1)$
- その他：`T` の除算、剰余、符号比較、加減算の計算量

#### 等値比較

```cpp
bool operator==(const Rational& rhs) const
bool operator!=(const Rational& rhs) const
```

分母が等しければ分子を直接比較する。それ以外は両辺を `larger_int_t<T>` に変換してから交差積で値が等しいか判定する。`Rational<mint>` では `larger_int_t<mint> = mint` となるため、そのまま利用できる。

##### 制約

- 分母が異なる場合は、交差積が `larger_int_t<T>` で正しく計算できる

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

分母が等しければ分子を直接比較し、それ以外は交差積で大小を比較する。`T` が大小比較を持つ場合だけ利用できる。無限大は $-\infty < x < +\infty$ として比較する。

##### 制約

- 分母が異なる場合は、交差積が `larger_int_t<T>` の範囲に収まる

##### 計算量

- 固定幅整数型：$O(1)$
- `BigInteger` など：`larger_int_t<T>` による2回の乗算と比較の計算量

#### 出力

```cpp
ostream& operator<<(ostream& os, const Rational& x)
```

`num/den` の形式で出力する。剰余演算を持つ型では、表示する値を一時的に約分する。無限大は `1/0` または `-1/0` と表示する。`PRINT` と `dump` も同じ表示用の値を使う。

`Rational<i128>` も C++17／20 で約分して出力できる。

##### 計算量

- `reduced()` と `T` の出力の計算量

#### numeric_limits

```cpp
numeric_limits<Rational<T>>::infinity()
```

`Rational<T>::infty()` と同じ値を返す。`resolved_value` により、どちらの関数もそのまま `infty` の非型テンプレート引数として渡せる。

```cpp
using R = Rational<BigInteger<>>;
GraphDirected<R> g(n, edges);
ShortestPath<true, R, R::infty> sp(g);
```

##### 制約

- `T` に大小比較が定義されている

##### 計算量

- `T` から $1$ と $0$ を構築する計算量
