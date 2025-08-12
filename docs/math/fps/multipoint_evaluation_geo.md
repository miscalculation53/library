## 多項式の多点評価（評価点が等比数列）

https://noshi91.github.io/algorithm-encyclopedia/chirp-z-transform

導出は ↑ を参照。

TODO：middle product を使って高速化

#### multipoint_evaluation_geo

```cpp
vc<mint> multipoint_evaluation_geo(fps f, int m, mint a, mint r)
```

多項式 $f$ について、各 $i = 0, \dots, m-1$ に対し $f(ar^i)$ の値を求める。

##### 制約

- $m \geq 0$

##### 計算量

$f$ の次数を $n$ として

- $O((n + m)\log (n + m))$