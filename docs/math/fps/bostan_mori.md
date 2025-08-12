## Bostan–Mori

TODO：定数倍高速化（うまくやると NTT の回数がもう少し減らせるっぽい）

$\displaystyle [x^k] \frac{p(x)}{q(x)}$ を求めるアルゴリズム。ただし $[x^0]q(x) \neq 0$ とする。線形漸化的数列の第 $k$ 項を求めるなどの応用がある。

https://qiita.com/ryuhe1/items/da5acbcce4ac1911f47a

$\dfrac{P(x)}{Q(x)} = \dfrac{P(x) Q(-x)}{Q(x) Q(-x)} = \dfrac{U_\text{e}(x^2) + xU_\text{o}(x^2)}{V(x^2)}$ と書ける（$Q(x)Q(-x)$ は偶多項式なので）。すると

- $k$ が偶数のとき、$[x^k] \dfrac{P(x)}{Q(x)} = [x^k] \dfrac{U_\text{e}(x^2)}{V(x^2)} = [x^{k/2}]\dfrac{U_\text{e}(x)}{V(x)}$
- $k$ が奇数のとき、$[x^k] \dfrac{P(x)}{Q(x)} = [x^k] \dfrac{xU_\text{o}(x^2)}{V(x^2)} = [x^{(k-1)/2}]\dfrac{U_\text{o}(x)}{V(x)}$

となるので、$O(d\log d)$ 時間のステップを $O(\log k)$ 回やればよい。

#### bostan_mori

```cpp
mint bostan_mori(fps p, fps q, ll k)
```

$[x^k] \dfrac{p(x)}{q(x)}$ を求める。

##### 制約

- $[x^0] q(x) \neq 0$

##### 計算量

$q(x)$ の次数を $d$ として

- $O(d \log d \log k)$
