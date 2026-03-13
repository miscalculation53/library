## LCM/GCD 畳み込み


### 定義

一般には環が載る。

#### LCM 畳み込み

$\displaystyle c(k) = \sum_{\mathrm{lcm}(i, j) = k} a(i) b(j)$

#### GCD 畳み込み

$\displaystyle c(k) = \sum_{\mathrm{gcd}(i, j) = k} a(i) b(j)$


### 計算方法


#### LCM 畳み込み

$\mathrm{lcm}(i, j) \mid k \iff i \mid k \ かつ \ j \mid k$ 

$\begin{aligned}
\zeta c(k) &= \sum_{\ell \mid k} c(\ell) \\\\  
&= \sum_{\ell \mid k} \sum_{\mathrm{lcm}(i, j) = \ell} a(i)b(j) \\\\  
&= \sum_{\mathrm{lcm}(i,j) \mid k} a(i)b(j) \\\\  
&= \sum_{i \mid k} a(i) \sum_{j \mid k} b(j) \\\\  
&= \zeta a(k) \zeta b(k)
\end{aligned}$

#### GCD 畳み込み

$k \mid \mathrm{gcd}(i, j) \iff k \mid i \ かつ \ k \mid j$ 

$\begin{aligned}
\zeta' c(k) &= \sum_{k \mid \ell} c(\ell) \\\\  
&= \sum_{k \mid \ell} \sum_{\mathrm{gcd}(i, j) = \ell} a(i)b(j) \\\\  
&= \sum_{k \mid \mathrm{gcd}(i, j)} a(i)b(j) \\\\  
&= \sum_{k \mid i} a(i) \sum_{k \mid j} b(j) \\\\  
&= \zeta' a(k) \zeta' b(k)
\end{aligned}$


### ライブラリ

#### lcm_convolution

```cpp
vc<R::S> lcm_convolution(vc<R::S> a, vc<R::S> b)
```

1-indexed（`a[0]` や `b[0]` は使わない）。

##### 制約

- `R` は環
- $\lvert a \rvert = \lvert b \rvert$

##### 計算量

$\lvert a \rvert = n$ として

- $O(n \log\log n)$

----

gcd_convolution も同様（省略）