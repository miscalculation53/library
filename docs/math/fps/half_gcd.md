このドキュメントには AI による作成・変更が含まれています。

## 概要

体上の多項式の Euclid の互除法を、複数ステップまとめて進める。
多項式乗算の計算量を $M(n)$ とすると $O(M(n)\log n)$ 時間。
必要な長さの NTT が使える場合は $O(n\log^2 n)$ 時間、$O(n)$ 空間。
`FormalPowerSeries<mint>` の乗算・逆数と同じ係数型を使う。

## 使用例

```cpp
using mint = modint998244353;
using P = FormalPowerSeries<mint>;
auto [g, s, t] = polynomial_extgcd(P{1, 2, 1}, P{1, 1});
// g == {1,1}, s*a + t*b == g
auto g2 = polynomial_gcd(P{1, 2, 1}, P{1, 1});
auto h = polynomial_inv_mod(P{0, 1}, P{1, 0, 1});
assert(h && *h == (P{0, -1}));
// x * (-x) == 1 mod (1+x^2)
```

## 詳細なドキュメント

係数は定数項から順に並べる。返す多項式は末尾の零を取り除き、零多項式は空配列。
入力の末尾の零は許す。入力は値渡しで、呼び出し元を変更しない。

#### polynomial_extgcd, polynomial_gcd

```cpp
(1) tuple<P, P, P> polynomial_extgcd(P a, P b)
(2) P polynomial_gcd(P a, P b)
```

`P = FormalPowerSeries<mint>` とする。(1) は `(g, s, t)` を返し、`s*a+t*b == g` を満たす。
`g` は最高次係数を $1$ にそろえた最大公約数。(2) は `g` だけを返す。
両方が零なら、(1) は空多項式三つ、(2) は空多項式を返す。

##### 制約

- `mint` は体をなす modint。

##### 計算量

- 最大係数数を $n$、多項式乗算の時間を $M(n)$ として $O(M(n)\log n)$。
- 必要な長さの NTT が使える場合は $O(n\log^2 n)$。

#### polynomial_inv_mod

```cpp
optional<P> polynomial_inv_mod(P f, P mod_poly)
```

`f*h == 1 (mod mod_poly)`、`deg(h)<deg(mod_poly)` を満たす逆元 `h` を返す。
非定数の法では `gcd(f,mod_poly)==1` が存在条件。逆元がなければ `nullopt`。
非零定数の法では、値ありの空多項式を返す。

##### 制約

- `mint` は体をなす modint。
- `mod_poly` は非零多項式。

##### 計算量

- 最大係数数を $n$ として $O(M(n)\log n)$。

## 共通部分

Euclid の 1 ステップは、$q=a\mathbin{\mathrm{div}}b$ として

$$
\begin{pmatrix}b\\a-qb\end{pmatrix}
=\begin{pmatrix}0&1\\1&-q\end{pmatrix}
\begin{pmatrix}a\\b\end{pmatrix}
$$

という多項式行列で表せる。この行列をまとめて計算するのが Half-GCD。
高次側の約半分の係数で再帰し、求めた変換を元の多項式に適用する。
必要なら通常の除算を 1 回行い、残りをもう一度再帰する。
行列の合成と適用を高速な多項式乗算で行うことで、
$T(n)=2T(n/2)+O(M(n))$ から $O(M(n)\log n)$ になる。

`internal::PolynomialHalfGCD<mint>::half(a,b)` は `deg(a)>deg(b)` のもとで、
$m=\lceil\deg(a)/2\rceil$ とし、変換後の組 $(u,v)$ が
$\deg u\ge m>\deg v$ となる最初の地点までの変換行列を返す。
零多項式の次数は $-\infty$ と考える。`full` は剰余が零になるまで進める。

BM は `half` と追加の除算、GCD・逆元は `full` を使う。
除算、$2\times2$ 行列、Half-GCD の再帰はすべて共通。
小さい問題は通常の互除法に切り替え、大きい行列演算では NTT の変換結果を使い回す。

## FPS の逆数との違い

`f.inv(n)` は法が $x^n$ の特殊な場合で、存在条件は `f[0]!=0`。
Newton 法により $O(M(n))$ で求まるため、こちらは引き続き既存の `inv` を使う。
`polynomial_inv_mod(f,g)` は一般の多項式 `g` を法とする逆元。
`f[0]==0` でも存在する場合がある。上の $x^{-1}\bmod(1+x^2)=-x$ がその例。
Half-GCD の高速な多項式除算は、内部でこの既存の FPS 逆数を利用する。

参考：[Li, Moreno Maza, Schost (2007), Appendix A3](https://cs.uwaterloo.ca/~eschost/publications/fp097-li.pdf)、
[Library Checker の問題文](https://judge.yosupo.jp/problem/inv_of_polynomials)。
