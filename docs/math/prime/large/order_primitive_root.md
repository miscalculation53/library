## 元の位数と原始根

素数 $p$ に対し、乗法群 $(\mathbb{Z}/p\mathbb{Z})^\times$ の元の位数や原始根を求めるライブラリ。

約数ゼータ・メビウス変換が絡んでくることもしばしばある。

### 概要

#### 定義

- 元 $1 \leq x < p$ の**位数**とは、$x^n \equiv 1 \pmod p$ を満たす最小の $n \geq 1$ のこと
- $p$ の**原始根**とは、位数が $p-1$ であるような元のこと

#### 性質

- $a^m \equiv 1 \pmod p$ を満たす $m$ は位数の倍数
  - 特に、位数は $p - 1$ の約数（フェルマーの小定理より $p-1$ は必ず満たす）
- $g^a \equiv g^b \pmod p \iff a \equiv b \pmod{p-1}$
  - 巡回群だから
- $g^a$ の位数は $\dfrac{p-1}{\gcd(p-1, a)}$
  - $d = \gcd(p-1, a)$ とおくと、$g^a$ の累乗として現れるのは $g^0, g^d, g^{2d}, \dots, g^{\left(\frac{p-1}{d}-1\right)d}$
- 位数がちょうど $d$ の元の個数は $\phi(d)$（これは一般に巡回群についてもいえる）
  - 約数ゼータ変換を考えると、位数が $d$ の約数であるような元の個数は $d$

### ライブラリの使い方

元の位数や原始根は、$p-1$ の素因数分解 $O(p^{1/4})$ 時間を除くと $O(\mathrm{polylog}\ p)$ 時間で求まる。特に、（原始根を底とする）**指数は**（離散対数 $O(\sqrt{p})$ 時間等でないと）**求まらないが、位数は求まる**。

`ll` までとれる関数だが、`int` の範囲で計算できるならそうしてくれる（modint パートが int と ll で割と重さが変わる）。

#### order_mod

```cpp
ll order_mod(ll x, ll p, vc<Primepower<ll>> fac)
```

$(\mathbb{Z}/p\mathbb{Z})^\times$ の元 $x$ の位数を求める。`fac` には $p-1$ の素因数分解形が入る。

##### 制約

- $p$ は素数
- $x \not\equiv 0 \pmod p$

##### 計算量

- $O(\log p \log\log p)$

ただし、`fac` の前計算に $O(p^{1/4})$ かかることに注意。


#### primitive_root

```cpp
(1) ll primitive_root(ll p, vc<PrimePower<ll>> fac)
(2) ll primitive_root_min(ll p, vc<PrimePower<ll>> fac)
```

(1) では $p$ の原始根を $1$ つ求める。(2) では $p$ の最小の原始根を求める。`fac` には $p-1$ の素因数分解形が入る。

##### 制約

- $p$ は素数


##### 計算量

- (1) は期待 $O((\log p) (\log\log p)^2)$
- (2) は、原始根の分布に極端な偏りがないと仮定すれば期待 $O((\log p) (\log\log p)^2)$
  - 実際は最小の原始根が大きめな $p$ があるので、何か $1$ つ欲しいだけなら (1) のほうが高速

ただし、`fac` の前計算に $O(p^{1/4})$ かかることに注意。

### 中身

#### ラグランジュ補間風のアルゴリズム

https://twitter.com/noshi91/status/1317025404700405760

```
|       a       |
|5,6,7,8|1,2,3,4| 伝播
|   b   |   c   |
|3,4|1,2|7,8|5,6| 伝播
| d | e | f | g |
|2|1|4|3|6|5|8|7| 伝播
|h|i|j|k|l|m|n|o|
```

これで各 $i$ について $x^{a_1 \cdots a_{i-1} a_{i+1} \cdots a_k}$ が求まる。計算量は、$\log k$ 個ある段それぞれで $a_1, \dots, a_k$ 乗の計算をするので、$O((\log k) \sum_i \log a_i) = O(\log k \log \prod_i a_i)$ 時間。

とりあえず再帰で書いたけど非再帰とどっちが速いかは計測してない（よくわからない）

#### 元の位数

https://atcoder.jp/contests/abc335/editorial/9017
https://atcoder.jp/contests/abc335/editorial/9045

$p-1 = q_1^{e_1} \cdots q_k^{e_k}$ とする。位数 $n$ は $n = q_1^{f_1} \cdots q_k^{f_k} \: (0 \leq f_i \leq e_i)$ と書ける。性質「$x^m \equiv 1 \pmod p$ を満たす $m$ は位数の倍数」より、各 $f_i$ ごとに最小化すればよい。つまり、$i$ ごとに $\left(x^{q_1^{e_1} \cdots q_{i-1}^{e_{i-1}} q_{i+1}^{e_{i+1}} \cdots q_k^{e_k}}\right)^{q_i^{f_i}} \equiv 1 \pmod p$ を満たす $f_i$ の最小値を求めればよい。これは、上述のラグランジュ補間風のアルゴリズムで $x^{q_1^{e_1} \cdots q_{i-1}^{e_{i-1}} q_{i+1}^{e_{i+1}} \cdots q_k^{e_k}}$ を求めた後、それを $q_i$ 乗する回数 $f_i$ を愚直に $1$ ずつ増やしていけばよい。計算量は、$x^{q_1^{e_1} \cdots q_{i-1}^{e_{i-1}} q_{i+1}^{e_{i+1}} \cdots q_k^{e_k}}$ を求めるパートが $O(\log k \log p) = O(\log p \log\log p)$ 時間、$f_i$ を増やしていくパートが $O(\sum_i e_i \log q_i) = O(\log \prod_i q_i^{e_i}) = O(\log p)$ 時間で、全体 $O(\log p\log\log p)$ 時間。

#### 原始根

https://37zigen.com/primitive-root/#i-4
https://37zigen.com/prime-complexity/#i-4

ある元が原始根かどうかの判定は、位数が $p-1$ かどうかなので $O(\log p\log\log p)$ 時間で求まる。より強く $f_i = e_i - 1$（つまり $\dfrac{p-1}{q_i}$）のみを調べればよいこともすぐにわかる。

原始根は $\phi(p-1)$ 個あるので、乱択すれば試行回数の期待値は $\dfrac{p-1}{\phi(p-1)}$ である。https://37zigen.com/prime-complexity/#i-4 より $\dfrac{n}{\phi(n)} = O(\log\log n)$ であるから、全体の期待計算量は $O((\log p) (\log\log p)^2)$ 時間。

---

実装が簡単な（ラグランジュ補間風のアルゴリズムを実装しない）代わりに計算量が劣る方法：

元の位数は、$m = p-1$ から始めて、各 $i$ について $m$ を $q_i$ で割っていくと $O(\log^2 p)$ 時間（https://atcoder.jp/contests/abc335/editorial/9017 ）。各 $i$ について、まず $m$ を $q_i^{e_i}$ で割ってから $m$ を $q_i$ 倍していけば $O(k (\log p + \sum_i e_i \log q_i)) = O(k \log p)$ 時間で、$k = O\left(\dfrac{\log p}{\log\log p}\right)$（https://37zigen.com/prime-complexity/#i-3 ）なので $O\left(\dfrac{\log^2 p}{\log\log p}\right)$ 時間（https://atcoder.jp/contests/abc335/editorial/9042 ）。

原始根判定は、単純に $x^{\frac{p-1}{q_i}}$ を計算することで $O(k \log p) = O\left(\dfrac{\log^2 p}{\log\log p}\right)$ 時間。よって原始根を求めるのは $O(\log^2 p)$ 時間。

### 素数でない場合

#### 元の位数

$n \geq 2$ に対し、乗法群 $(\mathbb{Z}/n\mathbb{Z})^\times$ の元 $x$（つまり $\gcd(x, n) = 1$ を満たす $x$）の位数は、オイラーの定理 $x^{\phi(n)} \equiv 1 \pmod n$ から $\phi(n)$ の約数（「位数の倍数」の性質は巡回群でなくても成り立つ）。`fac` に $\phi(n)$ の素因数分解を渡すことでまったく同様のアルゴリズムで求まる。カーマイケル関数 $\lambda(n)$ を使う改善が考えられるが、最悪ケースでは同じ。

#### 原始根

$n \geq 2$ の原始根が存在する条件は、$n$ が $2, 4, p^e, 2p^e$（$p$ は奇素数、$e \geq 1$）のどれかであることらしい。

また、$(\mathbb{Z}/n\mathbb{Z})^\times$ の元の位数となりうる最大の値は $\lambda(n)$ である。

具体的な元の求め方：
- $p^e$ の場合は、$p$ の原始根を $g$ として $g$ と $g + p$ を試す
- $2p^e$ の場合は、$p^e$ の原始根を $h$ として $h$ と $h + p^k$ を試す

まだちゃんと読んでない：

- https://integers.hatenablog.com/entry/2016/07/24/163831
- https://en.wikipedia.org/wiki/Primitive_root_modulo_n
- https://ocw.mit.edu/courses/18-781-theory-of-numbers-spring-2012/resources/mit18_781s12_lec8/
