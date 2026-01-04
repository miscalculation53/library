## 二項係数テーブル

任意 mod 二項係数 $\displaystyle \binom{n}{k}$ クエリを前計算 $O(\max(n)^2)$ クエリ $O(1)$ で処理する。

static で実装しているので、

```cpp
BinomialTable<mint>::C(n, k)
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

この関数は呼ばなくても自動でテーブルを計算してくれる。しかし、事前に呼んでおくことで定数倍高速化が期待できる。

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n)^2)$


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

- $O(T + \max(n)^2)$


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
