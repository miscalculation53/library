## $\sum_{i} A_i^k$ の $k$ に関する列挙

https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE#toc19

$\displaystyle \sum_{i} (1 + A_i x + A_i^2 x^2 + \cdots) = \sum_{i} \frac{1}{1-A_i x}$ の係数を求めればよい。

有理式の総和を求めてから除算する方法がある。

ほかには、積分すると $-\displaystyle \sum_{i} \log(1 - A_i x) = -\log \prod_i (1 - A_i x)$ になるので、多項式の総積を求めてから $\log$ をとる方法がある。

$i = 0, \dots, n-1$ があって $k = 0, \dots, m-1$ について求めるとするとき、いずれの方法も計算量は $O(n \log^2 n + m \log m)$ 時間。$n \log^2 n$ の定数倍は後者のほうがよく、$m \log m$ の定数倍は前者のほうがよい。$n \simeq m$ の状況では後者の方が定数倍がよい。本ライブラリでは後者で実装。

### ライブラリ

#### pow_sum

```cpp
FormalPowerSeries<mint> pow_sum(vc<mint> as, int m)
```

$\displaystyle \sum_{i=0}^{n-1} a_i^k$ を $k = 0, 1, \dots, m-1$ に対して求め、長さ $m$ の列を返す。

##### 制約

- $m \geq 0$

##### 計算量

- $O(n\log^2 n + m \log m)$
