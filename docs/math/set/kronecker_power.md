## クロネッカー冪の作用

a.k.a. ゼータ変換の一般化

https://q.c.titech.ac.jp/docs/progs/kronecker.html

### 概要

クロネッカー積 $\otimes$ は

$A \otimes B = \begin{bmatrix}
a_{11}B & \dots & a_{1n}B \\\\  
\vdots & \ddots & \vdots \\\\  
a_{n1}B & \dots & a_{nm}B
\end{bmatrix}$

で定義される。またクロネッカー冪は $A^{\otimes n} =  \underbrace{A \otimes \dots \otimes A}_{n}$ で定義される。

$k \times k$ 行列 $A$ と $k^n$ 次元ベクトル $v$ が与えられたとき、ベクトル $A^{\otimes n}v$ を高速に計算する方法がある。ゼータ変換、メビウス変換、アダマール変換などがその例。

また、クロネッカー積の混合積性質

$(A \otimes B)(C \otimes D) = AC \otimes BD$（ただし $AC, BD$ が定義できる）

において、$C = A^{-1}, D = B^{-1}$ とすると

$(A \otimes B)^{-1} = A^{-1} \otimes B^{-1}$

より

$(A^{\otimes n})^{-1} = (A^{-1})^{\otimes n}$

がわかる。つまり、逆変換が必要になったら、単に $A$ の逆行列について同じことをやればよい。

### 例

- ゼータ変換、メビウス変換、アダマール変換
- https://atcoder.jp/contests/arc136/tasks/arc136_d （ゼータ変換の $k=10$ バージョン）
- https://atcoder.jp/contests/abc288/tasks/abc288_g （$k=3$）
- https://atcoder.jp/contests/utpc2023/tasks/utpc2023_c （subset convolution のノリで多項式の行列 $\begin{bmatrix}1 & x \\\\  x & 1 \end{bmatrix}$ を考えることで「何個異なる」の情報を持たせられる）
- （FFT はクロネッカー冪ではないので仕組みが異なるが、行列で書いて分割統治するという点では似ている）
  - クロネッカー冪ではないが行列で書いて分割統治で解ける例： https://codeforces.com/contest/1967/problem/C

### ライブラリ

使い方のコツ：渡す行列や線型写像は $n=1$ の場合の振る舞いを記述すると考えるとよい。

#### kronecker_power

一般には半環が載る。

```cpp
(1) vc<SR::S> kronecker_power_array_destructive(array<array<SR::S, k>, k> mat, vc<SR::S> v)
(2) vc<SR::S> kronecker_power_array(array<array<SR::S, k>, k> mat, vc<SR::S> v)
```

- (1)：$v$ を $\mathrm{mat}^{\otimes k} v$ で置き換える。
- (2)：$\mathrm{mat}^{\otimes k} v$ を返す。

##### 制約

- $\lvert v \rvert$ は $0$ か $k$ べき

##### 計算量

$\lvert v \rvert = k^n$ として

- $O(n k^{n+1})$

#### tensor_power

行列を陽に渡すのではなく、線型写像として渡す。次のような場合に特に有効：

- 行列が疎であるなどの理由で、行列の定義を使うよりも高速に計算できる場合
- ゼータ変換・メビウス変換のように行列の要素が $1$ や $-1$ であり、本来加減算だけでよいのに、行列の定義を使うと乗算を書かされる場合（特に加算にあたる部分が通常の乗算であると、その上の乗算って何？ となって苦しい）

```cpp
(1) auto tensor_power_array_destructive<k>(auto linear_map, vc<T> v)
(2) vc<T> tensor_power_array<k>(auto linear_map, vc<T> v)
```

`linear_map` は `array<T, k>` から `array<T, k>` への関数を渡す。この関数の内部では破壊的変更を行ってもよい。

- (1)：$v$ を $\mathrm{linear\_map}^{\otimes k} v$ で置き換える。
- (2)：$\mathrm{linear\_map}^{\otimes k} v$ を返す。

##### 制約

- $\lvert v \rvert$ は $0$ か $k$ べき

##### 計算量

$\lvert v \rvert = k^n$ として

- $O(n k^{n-1})$ 回の `linear_map` 呼び出し


#### zeta, mobius

```cpp
(1) vc<M::S> zeta_subset_general(vc<M::S> a)
(2) vc<M::S> zeta_supset_general(vc<M::S> a)
(3) vc<G::S> mobius_subset_general(vc<G::S> a)
(4) vc<G::S> mobius_supset_general(vc<G::S> a)
```

$k$ 進法での各桁の大小によって半順序関係を定め、これに関するゼータ・メビウス変換を行った結果を返す。（省略しているが destructive 版もある。）

##### 制約

- $\lvert v \rvert$ は $0$ か $k$ べき
- モノイドや群は**可換**

##### 計算量

$\lvert v \rvert = k^n$ として

- $O(n k^n)$

---

### 中身

$I_n$ を $k^n \times k^n$ 単位行列とする。

混合積性質において $B = I_{n-1}, C = I_{1}, D = A^{\otimes n-1}$ とすると

$(A \otimes I_{n-1})(I_1 \otimes A^{\otimes n-1}) = A \otimes A^{\otimes n-1}$

となる。

$I_1 \otimes A^{\otimes n-1} = \begin{bmatrix}
A^{\otimes n-1} & & \\\\  
& \ddots & \\\\  
& & A^{\otimes n-1}
\end{bmatrix}$

の作用は再帰でできる（分割統治）。

$A \otimes I_{n-1} = \begin{bmatrix}
a_{11}I_{n-1} & \dots & a_{1k}I_{n-1} \\\\  
\vdots & \ddots & \vdots \\\\  
a_{k1}I_{n-1} & \dots & a_{kk}I_{n-1}
\end{bmatrix}$

は $k^2 \cdot k^{n-1} = k^{n+1}$ 要素。

よって、計算量 $T_k(n)$ は

$T_k(n) = k T_k(n-1) + O(k^{n+1})$

を満たすので、$T_k(n) = O(n k^{n+1})$ となる。

#### 非再帰にする

例として $A = \begin{bmatrix}a & b \\\\  c & d\end{bmatrix}$ に対して $A^{\otimes 3}$ を（最後の深さまで）展開してみると、次のようになる。

```
a...b...  a.b.....  ab......
.a...b..  .a.b....  cd......
..a...b.  c.d.....  ..ab....
...a...b  .c.d....  ..cd....
c...d...  ....a.b.  ....ab..
.c...d..  .....a.b  ....cd..
..c...d.  ....c.d.  ......ab
...c...d  .....c.d  ......cd
```

右から順に

- $(i, i), (i, i+1), (i+1, i), (i+1, i+1) \quad (i = 0,2,4,6)$
- $(i, i), (i, i+2), (i+2, i), (i+2, i+2) \quad (i = 0,1,5,6)$
- $(i, i), (i, i+4), (i+4, i), (i+4, i+4) \quad (i = 0,1,2,3)$

となっている。つまり、$d$ を $2$ べきとして、$i$ は $d$ のビット以外を立てたり立てなかったりしたもの。このような $i$ は $0 \leq i_\ell \lt d, 0 \leq i_u \cdot 2d \lt 2^n$ を使って $i = i_u \cdot 2d + i_\ell$ と書けるもの。

また、これは特にビット演算を使わずにできるので $k=2$ に限らず使える。

参考：Nyaan さんの提出 https://atcoder.jp/contests/abc288/submissions/38620359
