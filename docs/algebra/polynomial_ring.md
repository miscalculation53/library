## 概要

係数環 `R` 上の多項式を、係数列として扱う。

## 使用例

```cpp
using P = PolynomialRingVector<RingAddSubMul<ll>>;
const P::S f = {1, 2};
assert(P::add(f, P::minus(f)) == P::e0());
assert((P::mul(f, {1, -2}) == P::S{1, 0, -4}));
```

## 詳細なドキュメント

### PolynomialRingArray

```cpp
PolynomialRingArray<R, n>
```

要素型は `array<R::S, n>`。$x^n$ 以上の項を捨て、次数を $n-1$ で打ち切る。

#### add

```cpp
S add(const S& a, const S& b)
```

多項式 $a+b$ を返す。

##### 制約

- `R` は環
- $n \geq 0$

##### 計算量

- $O(n)$

#### e0

```cpp
S e0()
```

零多項式を返す。

##### 計算量

- $O(n)$

#### minus

```cpp
S minus(const S& a)
```

多項式 $-a$ を返す。

##### 計算量

- $O(n)$

#### mul

```cpp
S mul(const S& a, const S& b)
```

$x^n$ 以上の項を捨てた積 $ab \bmod x^n$ を返す。

##### 計算量

- $O(n^2)$

#### e1

```cpp
S e1()
```

$n \geq 1$ なら定数多項式 $1$、$n=0$ なら空の配列を返す。

##### 計算量

- $O(n)$

### PolynomialRingVector

```cpp
PolynomialRingVector<R>
```

要素型は `vc<R::S>`。`a[i]` を $x^i$ の係数とし、次数を打ち切らない。
`R::S` は零との等値比較ができる必要がある。

演算結果は末尾の零係数を取り除き、零多項式は空の vector に統一する。
入力の末尾に零係数があっても演算できるが、vector の等値比較は長さも比較するので、
直接比較する要素を作る場合は末尾に零係数を残さない。

#### add

```cpp
S add(const S& a, const S& b)
```

多項式 $a+b$ を返す。末尾の零係数を取り除く。

##### 制約

- `R` は環

##### 計算量

- $O(\lvert a\rvert + \lvert b\rvert)$

#### e0

```cpp
S e0()
```

零多項式を空の vector として返す。

##### 計算量

- $O(1)$

#### minus

```cpp
S minus(const S& a)
```

多項式 $-a$ を返す。

##### 計算量

- $O(\lvert a\rvert)$

#### mul

```cpp
S mul(const S& a, const S& b)
```

多項式の積 $ab$ を返す。

`a` または `b` が空、すなわち零多項式なら空の vector を返す。

##### 計算量

- $O(\lvert a\rvert \lvert b\rvert)$

#### e1

```cpp
S e1()
```

定数多項式 $1$ を `{R::e1()}` として返す。係数環で $1=0$ の場合は空の vector を返す。

##### 計算量

- $O(1)$
