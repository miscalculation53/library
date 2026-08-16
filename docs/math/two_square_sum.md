## 概要

二平方和

## 詳細なドキュメント

#### two_square_sum_prime

```cpp
pair<bool, pair<T, T>> two_square_sum_prime(T p)
```

素数 `p` について、$p = a^2 + b^2$ となる組を求める。

返り値の `first` は表現が存在するかどうか。存在する場合、`second` は $0 \leq a \leq b$ を満たす一意な組。

##### 制約

- `p` は `is_prime(p) == true` を満たす正の素数
- $p$ と計算途中の積が `T` および内部の modint に収まる

##### 計算量

- $p=2$ または $p \equiv 3 \pmod 4$：素数判定を除いて $O(1)$
- $p \equiv 1 \pmod 4$：最悪 $O(p \log p)$、平方非剰余が早く見つかる場合は $O(\log p)$

#### two_square_sum

```cpp
vc<pair<ll, ll>> two_square_sum(ll n)
```

整数 `n` について、$n = a^2 + b^2$ となる組 `(a, b)` をすべて返す。

返る組は $0 \leq a \leq b$ を満たし、重複を含まない。

##### 制約

- `n` と計算途中の積が `ll` に収まる

##### 計算量

- $n < 0$ または $n=0$：$O(1)$
- $n>0$：`factorize(n)` の計算量に加え、候補の生成と重複除去に出現した候補数に応じた時間

$p \equiv 1 \pmod 4$ である素因数の指数和を $k$ とすると、生成候補数の粗い上界は $2^k$、追加時間は $O(k2^k \log 2^k)$。
