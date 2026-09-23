## 概要

素数 $p$ を法として、階乗と等差数列の積を求める。

幅 $B=2^k$ ごとに階乗を前計算し、各クエリでは残りの高々 $B-1$ 項を掛ける。前計算には多項式の評価点シフトを使う。[手法の解説](https://suisen-kyopro.hatenablog.com/entry/2023/11/22/201600)。

デフォルトの $k$ は NTT friendly な法で $9$、それ以外で $11$。クエリ数が少ないときは $k=13$ 程度が目安になる。小さい法では $B\le\max(1,\lfloor p/2\rfloor)$ となるように $k$ を縮める。

## 使用例

```cpp
using mint = modint998244353;
Factorial<mint> fac;
auto x = fac.factorial(5);                 // 120
auto y = fac.arithmetic_product(4, 2, 3);  // 3*5*7*9 = 945
auto z = fac.arithmetic_product(0, 0, 0);  // 1
```

## 詳細なドキュメント

### Factorial

#### コンストラクタ

```cpp
Factorial<mint> fac(int k = -1)
```

幅 $B=2^k$ のブロックを使って初期化する。`k == -1` では法に応じたデフォルト値を使う。

##### 制約

- `mint` の法 $p$ は素数。
- `k == -1` または $k\ge0$。
- 使用中は `mint` の法を固定する。

##### 計算量

- $O\left(B\log B+(p/B)\log(p/B)\right)$。畳み込みの時間を $O(s\log s)$ とした場合。

#### factorial

```cpp
mint factorial(ll n) const
```

$n!\bmod p$ を返す。$n\ge p$ の場合は $0$。

##### 制約

- $n\ge0$。

##### 計算量

- $O(B)$。

#### arithmetic_product

```cpp
mint arithmetic_product(ll n, mint a, mint b) const
```

$\prod_{i=0}^{n-1}(ai+b)$ を返す。$n=0$ の空積は $1$、$a=0$ の場合は $b^n$。

$a\ne0$ の場合、$c=b/a\in\{0,\ldots,p-1\}$ として

$$
\prod_{i=0}^{n-1}(ai+b)=a^n\frac{(c+n-1)!}{(c-1)!}
$$

を使う。$c=0$ または $n>p-c$ なら積に $0$ が含まれるため $0$ を返す。

実装では $l=c-1$、$r=c+n-1$ とし、$l,r$ の近くに $p-1-L$ と $R$ が $B$ の倍数となる区切り $L,R$ を取る。Wilson の定理による

$$
\frac{R!}{L!}=(-1)^{L+1}R!(p-1-L)!
$$

は、既存の階乗表を 2 回参照して計算できる。この中央部分から左右の端点まで補正する。区切りが内側なら不足した因子を掛け、外側なら余分な因子を分母にまとめて最後に 1 回割る。

$n\le32$ の場合、および補正する因子数 $|L-l|+|R-r|$ が $n$ 以上の場合は、元の等差数列を直接掛ける。追加の逆階乗表は使わない。[実装方針の速度比較](arithmetic_product_benchmark.md)を参照。

##### 制約

- $n\ge0$。

##### 計算量

- $O(B+\log p+\log(n+1))$。
