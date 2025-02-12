## 二項係数

一番よく使うやつ（素数 mod 二項係数 $\displaystyle \binom{n}{k}$ クエリを前計算 $O(\max(n))$ クエリ $O(1)$ で処理する）。

### コンストラクタ

static で実装しているので、

```cpp
Binomial<mint>::C(n, k)
```

といった感じで使う。

##### 制約

- `mint` は modint 系


### メンバ関数

以下 mod を $p$ と書く。

#### reserve

```cpp
void reserve(int n)
```

サイズ $n$ まで前計算する。

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n))$

#### fac

```cpp
mint fac(int n)
```

$n! \bmod p$ を返す。$n \geq p$ なら $0$ となる。

##### 制約

- $n \geq 0$


##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n))$


#### finv

```cpp
mint finv(int n)
```

$(n!)^{-1}$ を返す。$n \lt 0$ なら $0$ を返す。

##### 制約

- $n \lt p$


##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n))$


#### inv

```cpp
mint inv(mint n)
```

$n^{-1}$ を返す。$n \lt 0$ なら $0$ を返す。

##### 制約

- $n \not\equiv 0 \bmod p$


##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n \bmod p))$


#### P

```cpp
mint P(int n, int k)
```

$n \geq k \geq 0$ のとき、$n(n-1)\cdots (n-(k-1)) = \dfrac{n!}{(n-k)!}$ を返す。

$n \lt k, n \lt 0, k \lt 0$ のいずれかを満たす場合は $0$ を返す。


##### 制約

- $n - k \lt p$

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n \bmod p))$


#### C

```cpp
mint C(int n, int k)
```

$n \geq k \geq 0$ のとき、$\displaystyle \binom{n}{k} = \dfrac{n!}{k!(n-k)!} = [x^k](1 + x)^n$ を返す。

$n \lt k, n \lt 0, k \lt 0$ のいずれかを満たす場合は $0$ を返す。


##### 制約

- $k \lt p$
- $n - k \lt p$

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n \bmod p))$


#### H

```cpp
mint H(int n, int k)
```

$n \geq 0, k \geq 0$ のとき、$[x^k]\dfrac{1}{(1-x)^n} = \begin{cases}
1 & (n = 0, k = 0) \\\\  
\displaystyle \binom{n+k-1}{k} & (\textrm{otherwise})
\end{cases}$ を返す。

$n \lt 0, k \lt 0$ のいずれかを満たす場合は $0$ を返す。


##### 制約

- $k \lt p$
- $n - k \lt p$

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n \bmod p))$



-----

### 実装について

#### 実装方針

$2$ 通りの実装が知られている。

$1$ つ目は、次の漸化式に基づいて `fac`, `finv`, `inv` テーブルを計算するもの。

- $i! = (i-1)! \cdot i$
- $p = qi + r \ (0 \leq r \lt i)$ として $\dfrac{1}{i} \equiv -q \cdot \dfrac{1}{r}$
- $\dfrac{1}{i!} = \dfrac{1}{(i-1)!} \cdot \dfrac{1}{i}$

$2$ つ目は、`fac` テーブルは普通に計算し、`finv` テーブルは末尾だけを $O(\log p)$ 時間で求め、後ろから $\displaystyle \frac{1}{i!} = \frac{1}{(i+1)!} \cdot (i+1)$ で計算するもの。

Library Checker に提出して雑計測した結果をまとめる。

最初に必要なぶんをまとめて確保する場合、微差だが

- 方法 $2$（`fac`, `finv` テーブルだけ）
  - https://judge.yosupo.jp/submission/267167 (260 ms)
- 方法 $1$
  - https://judge.yosupo.jp/submission/267160 (277 ms)
- 方法 $2$（`inv` テーブルも計算）
  - https://judge.yosupo.jp/submission/267165 (292 ms)

の順に速い模様。

必要になったら都度計算するようにする場合は、方法 $1$ のほうは前から計算するだけなのに対し、方法 $2$ はサイズを $2$ 倍にして償却にする例のテクを使う必要があるため若干不利（実装にもよるが、制約が $10^7$ でも $2^{24} = 16777216$ くらいまで計算することになる）。

二項係数が欲しいなら結局 `inv` テーブルも欲しくなるような場面も多いということもあり、本ライブラリでは方法 $1$ を採用。

#### 実装上の注意

- mod を超えて計算される場合
- `set_mod` が複数回呼ばれる場合

などに壊れがち（対策できていると思う）。

#### 負のときの仕様

https://hos-lyric.hatenablog.com/entry/2021/06/10/004947

こんな話があるらしい

本ライブラリは流儀 1 になっている

（多分自分のレートが低いからだと思うが）まだ困ったことがない、困ったら仕様変更を考えるかも