## 概要

体上の多項式の Euclid の互除法を、複数ステップまとめて進める。
多項式乗算の計算量を $M(n)$ とすると $O(M(n)\log n)$ 時間。
必要な長さの NTT が使える場合は $O(n\log^2 n)$ 時間、$O(n)$ 空間。
`FormalPowerSeries<mint>` の乗算・逆数と同じ係数型を使う。
計測・使い分けは [ベンチマーク](../../../benchmark/half_gcd.md) を参照。

```cpp
using mint = modint998244353;
using P = FormalPowerSeries<mint>;
auto [g, s, t] = polynomial_extgcd(P{1, 2, 1}, P{1, 1});
// g == {1,1}, s*a + t*b == g
auto g2 = polynomial_gcd(P{1, 2, 1}, P{1, 1});
auto [ok, h] = polynomial_inv_mod(P{0, 1}, P{1, 0, 1});
// x * (-x) == 1 mod (1+x^2), h == {0,-1}
```

## API

係数は定数項から順に並べる。返す多項式は末尾の零を取り除き、零多項式は空配列。
入力の末尾の零は許す。入力は値渡しで、呼び出し元を変更しない。

- `polynomial_extgcd(a,b)`：`(g,s,t)` を返し、`s*a+t*b == g`。`g` は monic な最大公約数。
  両方零のときは `({},{},{})`。
- `polynomial_gcd(a,b)`：monic な最大公約数。現実装は拡張 GCD の結果から取り出すため、
  Bézout 係数を省く専用実装に比べて定数倍を詰める余地がある。
- `polynomial_inv_mod(f,g)`：`(存在するか,h)`。`f*h == 1 (mod g)`、`deg(h)<deg(g)`。
  `g` は非零。非定数の `g` では `gcd(f,g)==1` が存在条件。
  非零定数の `g` では `(true,{})`。存在しなければ `(false,{})`。

`mint` は体の演算を満たすこと（例えば素数を法とする modint）。
上記の時間計算量では $n$ を入力多項式の最大係数数とする。
直接 NTT が使えない場合も既存の convolution の代替経路を使用するが、今回の速度測定は
`modint998244353` の直接 NTT のみ。

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
