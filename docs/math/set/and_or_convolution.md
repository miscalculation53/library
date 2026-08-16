## 概要

and 畳み込みと or 畳み込みを計算する。

### 定義

一般には環が載る。

#### and 畳み込み

$\displaystyle c(u) = \sum_{u = s \cap t} a(s)b(t)$

#### or 畳み込み

$\displaystyle c(u) = \sum_{u = s \cup t} a(s)b(t)$

### 計算方法

#### and 畳み込み

$u \subseteq s \cap t \iff u \subseteq s$ かつ $u \subseteq t$

$\begin{aligned}
\zeta' c(u) &= \sum_{u \subseteq v} c(v) \\\\  
&= \sum_{u \subseteq v} \sum_{v = s\cap t} a(s)b(t) \\\\  
&= \sum_{u \subseteq s \cap t} a(s)b(t) \\\\  
&= \sum_{u \subseteq s} a(s) \sum_{u \subseteq t} b(t) \\\\  
&= \zeta' a(u) \zeta' b(u)
\end{aligned}$

#### or 畳み込み

$s \cup t \subseteq u \iff s \subseteq u$ かつ $t \subseteq u$

$\begin{aligned}
\zeta c(u) &= \sum_{v \subseteq u} c(v) \\\\  
&= \sum_{v\subseteq u} \sum_{v = s\cup t} a(s)b(t) \\\\  
&= \sum_{s\cup t \subseteq u} a(s)b(t) \\\\  
&= \sum_{s\cup u} a(s) \sum_{t\cup u} b(t) \\\\  
&= \zeta a(u) \zeta b(u)
\end{aligned}$

## 詳細なドキュメント

#### and_convolution

```cpp
vc<R::S> and_convolution(vc<R::S> a, vc<R::S> b)
```

##### 制約

- `R` は環
- $\lvert a \rvert = \lvert b \rvert$
- $\lvert a \rvert$ は $0$ または $2$ べき

##### 計算量

$\lvert a \rvert = 2^n$ として

- $O(n 2^n)$

----

or_convolution も同様（省略）
