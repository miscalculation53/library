## Taylor Shift

多項式 $f(x)$ に対して $f(x + c)$ を求める。

$\displaystyle f(x) = \sum_{i = 0}^{n-1} a_i x^i, f(x + c) = \sum_{i=0}^{n-1} b_i x^i$ とすると

$\begin{aligned}
f(x + c) &= \sum_{i = 0}^{n-1} a_i (x + c)^i \\\\
&= \sum_{i = 0}^{n-1} a_i \sum_{j = 0}^{i} \binom{i}{j} x^j c^{i - j} \\\\
&= \sum_{j = 0}^{n - 1} \left( \sum_{i = j}^{n - 1} a_i i! \cdot \frac{c^{i - j}}{(i-j)!} \right) \frac{x^j}{j!}
\end{aligned}$

となっているので、$A_i = a_i i!, C_i = \dfrac{c^i}{i!}$ とすると $\displaystyle b_j = \sum_{i = j}^{n - 1} A_i C_{i - j}$ である。ここで $A$ を反転した列 $\overline{A}$ を考えると（つまり $\overline{A}_i = A_{n - 1 - i}$）、$\displaystyle b_j = \sum_{i = j}^{n - 1} \overline{A}_{n - 1 - i} C_{i - j} = (\overline{A} * C)_{n-1-j}$ となるのでこの畳み込みを計算すればよい。

#### taylor_shift

```cpp
fps taylor_shift(fps f, mint c)
```

多項式 $f$ に対し、$f(x + c)$ を求める。

##### 計算量

- $O(n \log n)$
