## 概要

$n$ 個の等間隔な評価点 $y_i = f(i) \ (0 \leq i \leq n-1)$ が与えられた多項式について、$1$ 個の点で評価することが $O(n)$ 時間ででき（これはこのテク自体が「ラグランジュ補間」と呼ばれているのをよく見る気がする）、連続する何個かの点で評価することが $O((n+m)\log (n+m))$ 時間でできる。多項式を陽に復元してから多点評価すると log ふたつかかるがそれより速い。

$1$ 点のほうは

$\begin{aligned}
f(c) &= \sum_{i=0}^{n-1} y_i \prod_{j=0, j \neq i}^{n-1} \frac{c - j}{i - j} \\\\  
&= \sum_{i=0}^{n-1} y_i \frac{\displaystyle \left(\prod_{j=0}^{i-1} (c-j)\right)\left(\prod_{j=i+1}^{n-1} (c-j)\right)}{\displaystyle \left(\prod_{j=0}^{i-1} (i-j)\right)\left(\prod_{j=i+1}^{n-1} (i-j)\right)} \\\\  
&= \sum_{i=0}^{n-1} y_i \frac{\displaystyle \left(\prod_{j=0}^{i-1} (c-j)\right)\left(\prod_{j=i+1}^{n-1} (c-j)\right)}{i! (n-1-i)! (-1)^{n-1-i}}
\end{aligned}$

なので $c-j$ の左右からの累積積を用いて計算できる。

連続する $m$ 点のほうは、除算を使いそうだがそれだとゼロ除算が面倒で、畳み込みを使う。この方法は大昔に https://mathlog.info/articles/3351 を読んで導出を追ったがすべて忘却してしまった……。

## 詳細なドキュメント

#### shift_of_sampling_points

```cpp
(1) mint shift_of_sampling_points_one(vc<mint> ys, mint c)
(2) vc<mint> shift_of_sampling_points_many(vc<mint> ys, mint c, int m)
```

- (1) $f(i) = y_i \ (0 \leq i \leq n-1)$ で定まる高々 $n-1$ 次の多項式 $f$ に対し、$f(c)$ を求める。

- (2) $f(i) = y_i \ (0 \leq i \leq n-1)$ で定まる高々 $n-1$ 次の多項式 $f$ に対し、$f(c), f(c+1), \dots, f(c+m-1)$ を求める。

##### 制約

- $n > 0$
- $m \geq 0$

##### 計算量

- (1)：$O(n)$
- (2)：$O((n + m) \log (n + m))$
