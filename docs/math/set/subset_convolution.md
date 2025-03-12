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

$f_a(s)(x) = a(s) x^{\lvert s \rvert}$（$f_b, f_c$ も同様）とすると $f_c$ は $f_a$ と $f_b$ の or 畳み込みになっている。

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