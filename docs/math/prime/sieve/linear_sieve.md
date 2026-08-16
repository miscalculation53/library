## 概要

最小素因数を持つようにした篩。

static で実装しているので、

```cpp
LinearSieve::factorize(n)
```

といった感じで使う。

## 詳細なドキュメント

#### メンバ変数

- `int n`：現在 `lpf_` が計算されている最大値。
- `vc<PrimePower<int>> lpf_`：最小素因数 $p$ と、$p$ で割り切れる回数 $e$ について $(p, e, p^e)$ を格納したテーブル（コンテスト中に使うときは、これにアクセスするのではなく `lpf()` 関数を用いた方が安全）。
- `vc<int> primes`：現在計算されている素数を小さい順に格納したもの。 

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

計算量解析は、$n$ の異なる素因数の個数 $\omega(n)$ が $\omega(n) = O\left(\dfrac{\log n}{\log\log n}\right)$ であることから。
