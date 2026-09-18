## 概要

アダマール変換・xor 畳み込み

## 詳細なドキュメント

#### アダマール変換

アダマール変換は、クロネッカー冪の作用で行列を $\begin{bmatrix}1 & 1\\\\  1 & -1\end{bmatrix}$ としたもの。一般には可換群が載る。

アダマール変換の逆変換は、クロネッカー冪の作用で行列をこの逆行列 $\dfrac{1}{2} \begin{bmatrix}1 & 1\\\\  1 & -1\end{bmatrix}$ としたもの。順変換をやってから $2^n$ で割ればよい。乗法逆元が必要なので体である必要がある。

アダマール変換は $n$ 次元フーリエ変換の各次元の長さが $2$ の場合と解釈できる。

$\displaystyle b(t) = \sum_{0 \leq s \lt 2^n} a(s) \prod_{0 \leq i \lt n} (-1)^{[i \in s] \cdot [i \in t]}$

#### xor 畳み込み

一般には体が載る。

$\displaystyle c(u) = \sum_{s \oplus t = u} a(s)b(t)$

アダマール変換して、各点積をとって、逆アダマール変換する。


### 通常の 32 bit modint の作業領域

`static_modint32`・`dynamic_modint32` の標準の加法群（`GroupAddSub<mint>`、または標準の環・体から取り出した `GroupOfRingAdd`）では、配列長 $N$ が 1024 以上のとき 64 bit 整数の作業配列を使う。この場合は破壊的なアダマール変換でも約 $8N$ byte、すなわち $O(N)$ の追加領域を使う。順変換・逆変換ともに同じで、返却される各要素は通常の modint の値である。
