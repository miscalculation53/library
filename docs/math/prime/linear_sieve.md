## 線形篩

最小素因数を持つようにした篩。

static で実装しているので、

```cpp
LinearSieve::factorize(n)
```

といった感じで使う。


### メンバ変数

- `int n`：現在 `lpf_` が計算されている最大値。
- `vc<PrimePower<int>> lpf_`：最小素因数 $p$ と、$p$ で割り切れる回数 $e$ について $(p, e, p^e)$ を格納したテーブル（コンテスト中に使うときは、これにアクセスするのではなく `lpf()` 関数を用いた方が安全）。
- `vc<int> primes`：現在計算されている素数を小さい順に格納したもの。 


### メンバ関数


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



##### 計算量

#### is_prime

#### factorize

```cpp
vc<PrimePower<P=int>> factorize(n)
```

$n$ の素因数分解を返す。

##### 制約

##### 計算量

$T$ 個の値 $n_1, \dots, n_T$ について呼んだとき、

- $\displaystyle O\left(\sum_{i=1}^T \frac{\log n_i}{\log\log n_i} + \max(n) \right)$

計算量解析は、$n$ の異なる素因数の個数 $\omega(n)$ が $\omega(n) = O\left(\dfrac{\log n}{\log\log n}\right)$ であることから。
