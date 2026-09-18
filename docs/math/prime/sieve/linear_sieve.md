## 概要

最小素因数を持つようにした篩。

static で実装しているので、

```cpp
LinearSieve::factorize(n)
```

といった感じで使う。

## 使用例

```cpp
for (auto p : LinearSieve::primes(20)) {  // p は ll
  PRINT(p);  // 2, 3, 5, 7, 11, 13, 17, 19
}

const auto ps = LinearSieve::primes(10);
assert(ps.size() == 4);
assert(ps[2] == 5);
int count = ps.size<int>();

for (auto p : LinearSieve::primes<int>(20)) {
  // p は int
}

LinearSieve::reserve(1000);
// 共有テーブルが拡張されても、ps は引き続き 2, 3, 5, 7 を列挙する。
vc<ll> values(ps.begin(), ps.end());  // 配列としてコピーしたい場合
```

## 詳細なドキュメント

#### メンバ変数

- `int n`：現在 `lpf_` が計算されている最大値。
- `vc<PrimePower<int>> lpf_`：最小素因数 $p$ と、$p$ で割り切れる回数 $e$ について $(p, e, p^e)$ を格納したテーブル（コンテスト中に使うときは、これにアクセスするのではなく `lpf()` 関数を用いた方が安全）。

#### primes

```cpp
template <class P = ll>
static PrimeRange<P> primes(int limit);
```

`limit` 以下の素数を昇順に列挙する読み取り専用の範囲を返す。
列挙する値と `[]` の返り値は `P` 型で、デフォルトは `ll`。
内部の素数テーブルは型によらず共有し、値を取り出すときに `P` に変換する。

返り値には `begin()`, `end()`, `size()`, `empty()`, `[]` が用意されている。

```cpp
template <class I = ll>
I size() const;
```

`size()` の返り値はデフォルトで `ll`。`size<int>()` のように指定することもできる。
素数の型 `P` と個数の型 `I` は独立に指定する。

#### reserve

```cpp
void reserve(int n)
```

テーブルを（少なくとも）$n$ のところまで計算する（実際には、$\max(n, 現在のサイズの\ 2\ 倍)$ まで計算される）。

この関数は呼ばなくても自動でテーブルを計算してくれる。しかし、事前に呼んでおくことで定数倍高速化が期待できる。

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n))$

#### lpf

```cpp
PrimePower<P=int> lpf(int n)
```

$n$ の最小素因数 $p$ と $n$ が $p$ で割り切れる回数 $e$ について $(p, e, p^e)$ を返す。

##### 制約

- $n \geq 1$

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n))$

#### is_prime

```cpp
bool is_prime(int n)
```

$n$ が素数かどうかを返す。（最小素因数が自身に一致するかどうか）

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n))$

#### Omega

```cpp
static int Omega(int n);
```

重複を含めた素因数の個数 $\Omega(n)$ を返す。

##### 制約

- $n \geq 1$

##### 計算量

$T$ 回呼んだとき、

- $O(T+\max(n))$

#### factorize

```cpp
vc<PrimePower<P=int>> factorize(n)
```

$n$ の素因数分解を返す。

##### 制約

- $n \geq 1$

##### 計算量

$T$ 個の値 $n_1, \dots, n_T$ について呼んだとき、

- $\displaystyle O\left(\sum_{i=1}^T \frac{\log n_i}{\log\log n_i} + \max(n) \right)$

この計算量は、$n$ の異なる素因数の個数 $\omega(n)$ が $O\left(\dfrac{\log n}{\log\log n}\right)$ であることから
