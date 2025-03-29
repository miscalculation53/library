## 拡張ユークリッドの互除法 (extgcd)

```cpp
tuple<T, T, T> extgcd(T a, T b)
```

次の条件を満たす $(g, x, y)$ の組を $1$ 組返す。

- $g = \gcd(a, b)$
  - $g \geq 0$ となるように定める
  - $\gcd(c, 0) = \gcd(0, c) = c \quad (c \geq 0)$
  - $\gcd(0, 0) = 0$
- $ax + by = g$
- 上記を満たす中で $\lVert(x,y)\rVert_p \: (1 \leq p \leq \infty)$ が最小？
- $\lVert(x,y)\rVert_p \leq \lVert(a,b)\rVert_p \: (1 \leq p \leq \infty)$ ？

##### 制約

- `T` は符号つき整数型


##### 計算量

- $O(1 + \log \min(\lvert a \rvert, \lvert b \rvert))$


##### 関連事実

- $(a, b) \neq (0, 0)$ とする。$(x, y)$ についての一次不定方程式 $ax + by = c$ が整数解を持つ必要十分条件は、$c$ が $\gcd(a, b)$ で割り切れることである。
- $(a, b) \neq (0, 0)$ とする。$(x, y)$ についての一次不定方程式 $ax + by = c$ が整数解を持つとき、$g = \gcd(a, b)$、整数解の $1$ つを $(x_0, y_0)$ とすると、一般解は

  $\displaystyle x = x_0 + k\frac{b}{g}, y = y_0 - k\frac{a}{g} \quad (k \in \mathbb{Z})$

  と書ける。$g$ で割り忘れないよう注意。
- $m \geq 1$ とする。$x$ についての合同方程式 $ax \equiv c \pmod m$ の解集合は、$(x, y)$ についての一次不定方程式 $ax + my = c$ の解集合の $x$ だけとってきた集合と一致。つまり、
  - $ax \equiv c \pmod m$ の解が存在する必要十分条件は $c$ が $\gcd(a, m)$ で割り切れることである（特に、$\bmod \ m$ で $a$ の逆元が存在する必要十分条件は $\gcd(a, m) = 1$ であることである）。
  - $ax \equiv c \pmod m$ の解が存在するとき、$g = \gcd(a, m)$、解の $1$ つを $x_0$ とすると、一般解は

    $\displaystyle x \equiv x_0 + k\frac{m}{g} \pmod m \quad (k = 0, \dots, g-1)$
    
    と書ける。