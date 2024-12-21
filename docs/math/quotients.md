## 商列挙

```cpp
quotients(ll n, int d = 1)
```

次の条件を満たす $(y, l, r)$ を $y$ の昇順（$l, r$ の降順）に列挙する。

- $\displaystyle y \in \left\lbrace \left\lfloor \frac{n}{x^d} \right\rfloor \ \middle\vert \ x \in \mathbb{Z}, 1 \leq x \leq n^{1/d} \right\rbrace$
- $x \in \mathbb{Z}, 1 \leq x \leq n^{1/d}$ に対し、$\displaystyle y = \left\lfloor \frac{n}{x^d} \right\rfloor \iff x \in (l, r]$

使い方としては

```cpp
fec([y, l, r] : quotients(n))
```

という感じ。

##### 制約

- $n \geq 1$
- $d \geq 1$

##### 計算量

- $1$ 回のイテレーションは `ipow`, `iroot` がボトルネック
- 条件を満たすものの個数は $O(n^{1/(d+1)})$

##### 関連事実

- 中身は以下の通り。

  $\begin{aligned}
    y = \left\lfloor \frac{n}{x^d} \right\rfloor
    &\iff \frac{n}{x^d} - 1 \lt y \leq \frac{n}{x^d} \\
    &\iff \frac{n}{y+1} \lt x^d \leq \frac{n}{y} \\
    &\iff \left\lfloor \left\lfloor\frac{n}{y+1}\right\rfloor^{1/d} \right\rfloor \lt x \leq \left\lfloor \left\lfloor\frac{n}{y}\right\rfloor^{1/d} \right\rfloor
  \end{aligned}$

  $(y, l, r)$ から $(y', l', r')$ を得るには、$r' = l$ とし、$y'$ を計算し、$l'$ を上式から求める。
- 個数の評価について
  - $x \leq m$ なものの個数は、$x$ の個数（$\leq m$）で抑えられる。
  - $x \geq m$ なものの個数は、$y$ の個数（$\leq n/m^d$）で抑えられる。
  - $m = n/m^d$ とすると $m = n^{1/(d+1)}$ で、結局全体では $2n^{1/(d+1)}$ で抑えられる。
- $d$ が一般の場合の応用先
  - $d = 2$ の場合は無平方数の数え上げに利用できる。