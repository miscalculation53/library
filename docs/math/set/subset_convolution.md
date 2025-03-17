## subset convolution

### 定義

一般には環が載る。

$\begin{aligned}
\displaystyle c(u) &= \sum_{u = s \coprod t} a(s)b(t) \\\\  
&= \sum_{\substack{u = s \cup t \\\\  \emptyset = s \cap t}} a(s)b(t) \\\\  
&= \sum_{s \subseteq u} a(s)b(u\setminus s)
\end{aligned}$

### 計算方法

$\displaystyle c(u) = \sum_{\substack{u = s \cup t \\\\  \lvert u \rvert = \lvert s \rvert + \lvert t \rvert}} a(s)b(t)$

と解釈して、or 畳み込みに多項式環を載せる。

$f(s) = a(s) x^{\lvert s \rvert}, g(t) = b(t) x^{\lvert t \rvert}$ として $f$ と $g$ の or 畳み込み $h$ を計算すると、$[x^{\lvert u \rvert}] h(u) = c(u)$ となっている。

### ライブラリ

#### subset_convolution

```cpp
vc<R::S> subset_convolution(vc<R::S> a, vc<R::S> b)
```

##### 制約

- `R` は環
- $\lvert a \rvert = \lvert b \rvert$
- $\lvert a \rvert$ は $0$ または $2$ べき

##### 計算量

$\lvert a \rvert = 2^n$ として

- $O(n^2 2^n)$