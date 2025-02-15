## 素べき構造体

主に素因数分解を扱うための構造体。

といいつつ素数に関する util 全般を含む。


### メンバ変数

- `P p`：素数
- `int e`：指数
- `P pe`：素べきの値（$p^e$）


### コンストラクタ

```cpp
(1) PrimePower<P>()
(2) PrimePower<P>(P p, int e = 1)
(3) PrimePower<P>(P p, int e, P pe)
```

- (1) `p`, `e`, `pe` を $-1$ で初期化する。
- (2) `p`, `e` を設定し、`pe` は自動で計算する。
- (3) `p`, `e`, `pe` を設定する。

##### 制約

- `P` は `int` か `ll`
- $p$ は素数
- $e \geq 0$
- (3) において、`pe` は $p^e$ に等しい

##### 計算量

- (2) のみ $O(\log e)$、他は $O(1)$


### メンバ関数

#### mul_p

```cpp
void mul_p()
```

素べきに $p$ を掛ける。$p^e$ が `P` に収まらないときは、$\bmod \ 2^{32}$ や $\bmod \ 2^{64}$ で等しい値にする。

#### div_p

```cpp
void div_p()
```

素べきを $p$ で割る。

##### 制約

- $e > 0$（すなわち $p^e > p$）


### 外側の関数

#### ord_pow

```cpp
tuple<int, P, P> ord_pow_div(P n, P m)
```

$n$ が $m$ で割り切れる回数 $e$ について $(e, m^e, n/m^e)$ を返す。

##### 計算量

- $O(\log e)$
  - $e = O(\log_m n)$

#### factors

整数の素因数分解形を受け取り、その相異なる素因数を返す。

```cpp
vc<P> divisors(vc<PrimePower<P>> fac)
```

##### 計算量

- $O(\lvert \mathrm{fac} \rvert)$


#### divisors

整数の素因数分解形を受け取り、そのすべての約数を小さい順に格納した vector を返す。

```cpp
vc<ll> divisors(vc<PrimePower<ll>> fac)
```

##### 制約

- `fac` は素因数分解形として正しい
- `fac` が表す整数は `ll` に収まる

##### 計算量

- $O(\sigma_0(n))$


#### factorized_mul

整数の素因数分解形（`p` についてソート済み）を複数受け取り、その積の素因数分解形（`p` についてソート済み）を返す。`pe` がオーバーフローする場合は $\bmod \ 2^{32}$ や $\bmod \ 2^{64}$ で等しい値を返す。

```cpp
vc<PrimePower<P>> factorized_mul(vc<PrimePower<P>> fac1, vc<PrimePower<P>> fac2)
```

##### 制約

- `fac1`, `fac2` は `p` についてソート済み

##### 計算量

- $O(\lvert \mathrm{fac1} \rvert + \lvert \mathrm{fac2} \rvert)$