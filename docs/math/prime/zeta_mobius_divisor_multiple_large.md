## 約数・倍数 ゼータ・メビウス変換（大きい $m$ の約数）

$m$ の約数の整除関係に関する約数・倍数 ゼータ・メビウス変換。

### 約数・倍数 ゼータ・メビウス変換の定義

一般には、ゼータ変換には可換モノイドが、メビウス変換には可換群が載る。以下に述べる定義は、演算を $+$ とした場合のもの。

#### 約数ゼータ変換

$\displaystyle \zeta a(n) = \sum_{d\mid n} a(d)$

#### 約数メビウス変換

$\mu$ は $\zeta$ の逆変換、つまり $\displaystyle \mu\zeta a(n) = a(n)$

メビウス関数

$\displaystyle \mu(n) = \begin{cases}
0 & (n \ の素因数に重複がある) \\\\  
1 & (n \ の異なる素因数が偶数個) \\\\  
-1 & (n \ の異なる素因数が奇数個)
\end{cases}$

を用いて

$\displaystyle \mu a(n) = \sum_{d\mid n} \mu\left(\frac{n}{d}\right) a(d)$

#### 倍数ゼータ変換

$\zeta' a(n) = \sum_{n \mid m} a(m)$


#### 倍数メビウス変換

$\mu'$ は $\zeta'$ の逆変換、つまり $\displaystyle \mu'\zeta' a(n) = a(n)$

メビウス関数 $\mu(n)$ を用いて

$\displaystyle \mu' a(n) = \sum_{n\mid m} \mu\left(\frac{m}{n}\right) a(m)$


### 帰着のさせ方

$\gcd = m$ とか $\mathrm{lcm} = m$ という条件に対する和を求めたいときによく登場する。

- $\mathrm{lcm}(a_1, \dots, a_n) \mid m \iff \forall i, \ a_i \mid m$
- $m \mid \gcd(a_1, \dots, a_n) \iff \forall i, \ m \mid a_i$

が成り立つことがポイント。たとえば次のような変形がよくある。

$\begin{aligned}
f(m) &= \sum_{\mathrm{lcm}(a_{i_1}, \dots, a_{i_k}) = m} h(i_1, \dots, i_k)
\end{aligned}$

を求めたいとき、

$\begin{aligned}
g(m) &:= \sum_{\mathrm{lcm}(a_{i_1}, \dots, a_{i_k}) \mid m} h(i_1, \dots, i_k) \\\\  
&= \sum_{\ell \mid m} \sum_{\mathrm{lcm}(a_{i_1}, \dots, a_{i_k}) = \ell} h(i_1, \dots, i_k) \\\\  
&= \sum_{\ell \mid m} f(m) = \zeta f(m)
\end{aligned}$

となる。$g(m)$ が求まるとき、$f(m) = \mu g(m)$ と計算すればよい。


### 使う際の注意

- $m = 1$ がコーナーになりがちなので気をつける（空集合の $\mathrm{lcm}$ は $1$）。
- 万能ではない。普通の（$0/1$ の）ゼータ変換で済むときは約数ゼータ変換が落とされることもある。
  - $\omega(m)$ を $m$ の異なる素因数の個数、$\sigma_0(m)$ を $m$ の約数の個数とするとき、約数ゼータ変換には $O(\omega(m) \sigma_0(m))$ 時間かかる。普通のゼータ変換は $O(\omega(m) 2^{\omega(m)})$ 時間で、$\dfrac{\max_{m \leq M} \sigma_0(m)}{\max_{m \leq M} 2^{\omega(m)}}$ は $M \leq 10^{18}$ の範囲では最大で $5.625$ になる。実装自体の定数倍も普通のゼータ変換のほうが（たぶん）軽い。
  - https://yukicoder.me/problems/no/2578 とか（一応このライブラリは定数倍に気を使って実装したつもりだが、それでもこの問題は結構厳しい）

素因数個数と約数個数の表：

|$m \leq$|$\max 2^{\omega(m)}$|$\max \sigma_0(m)$|$\dfrac{\max \sigma_0(m)}{\max 2^{\omega(m)}}$|
|---|---|---|---|
|$10^{3}$|$2^{4} = 16$|$32$|$2$|
|$10^{4}$|$2^{5} = 32$|$64$|$2$|
|$10^{5}$|$2^{6} = 64$|$128$|$2$|
|$10^{6}$|$2^{7} = 128$|$240$|$1.875$|
|$10^{7}$|$2^{8} = 256$|$448$|$1.75$|
|$10^{8}$|$2^{8} = 256$|$768$|$3$|
|$10^{9}$|$2^{9} = 512$|$1344$|$2.625$|
|$10^{10}$|$2^{10} = 1024$|$2304$|$2.25$|
|$10^{11}$|$2^{10} = 1024$|$4032$|$3.9375$|
|$10^{12}$|$2^{11} = 2048$|$6720$|$3.28125$|
|$10^{13}$|$2^{12} = 4096$|$10752$|$2.625$|
|$10^{14}$|$2^{12} = 4096$|$17280$|$4.21875$|
|$10^{15}$|$2^{13} = 8192$|$26880$|$3.28125$|
|$10^{16}$|$2^{13} = 8192$|$41472$|$5.0625$|
|$10^{17}$|$2^{14} = 16384$|$64512$|$3.9375$|
|$10^{18}$|$2^{15} = 32768$|$103680$|$3.1640625$|

----

以下、本ライブラリの話。

### メンバ変数

- `ll m`
- `vc<PrimePower<ll>> fac`：$m$ の素因数分解
- `ll pnum`：$m$ の異なる素因数の個数
- `ll dnum`：$m$ の約数の個数
- `vc<ll> ds`：$m$ の約数を格納したもの。格納順は指数のベクトルの辞書順（$d$ の昇順ではない）。

### コンストラクタ

```cpp
ZetaMobiusDivisorMultipleLarge(ll m)
```

##### 計算量

- $O(m^{1/4})$


### メンバ関数

#### divisor_map

```cpp
(1) DivisorMap<T> divisor_map()
(2) DivisorMap<T> divisor_map(function<T(ll)> func)
```

変換の対象となる列は `DivisorMap` という構造体で管理する。

- (1)：全要素が `T{}` の `DivisorMap` を返す。
- (2)：約数 $d$ に紐づく値を $\mathrm{func}(d)$ で初期化した `DivisorMap` を返す。

`DivisorMap` のメンバ関数：

- `int dtoi(ll d)`：約数 $d$ に紐づく値がデータ構造上で保持されている添字 $i$ を返す。$O(\log\log d)$ 時間。
- `T get_by_d(ll d)`：約数 $d$ に紐づく値の取得・書き換えができる。$O(\log\log d)$ 時間。
- `T get_by_i(int i)`：約数 $d$ に対し、$i = \mathrm{dtoi}(d)$ から値の取得・書き換えができる。$O(1)$ 時間。
  - `get_by_d(A[j])` を何回もやるのは無駄。そういう場合にこちらを使うことで高速化できる。

- `map<ll, T> to_map()`：map にしたものを返す。デバッグ用の想定。

#### zeta_divisor

```cpp
DivisorMap<Monoid::S> zeta_divisor(DivisorMap<Monoid::S> a)
```

可換モノイド `Monoid` 上での $a$ の約数ゼータ変換を返す。

##### 制約

- `Monoid::op` は可換

##### 計算量

- $O(\omega(m) \sigma_0(m))$


#### mobius_divisor

```cpp
DivisorMap<Group::G> mobius_divisor(DivisorMap<Group::G> a)
```

可換群 `Group` 上での $a$ の約数メビウス変換を返す。

##### 制約

- `Group::op` は可換

##### 計算量

- $O(\omega(m) \sigma_0(m))$


#### mobius_divisor_point

```cpp
Group::G mobius_divisor_point(DivisorMap<Group::G> a, ll n)
```

可換群 `Group` 上での $a$ の約数メビウス変換の $n$ での値 $\mu a(n)$ を返す。

##### 制約

- `Group::op` は可換

##### 計算量

- $O(\omega(m) 2^{\omega(m)})$

##### 仕組み

約数メビウス変換の定義式を考えると、$n/d$ が重複する素因数を持たないような $2^{\omega(m)}$ 通りを見ればよいので。

----

`divisor` を `multiple` に変えたメンバ関数もある（同様なので省略）