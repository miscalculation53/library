## 多項式補間（評価点が等比数列）

https://noshi91.github.io/algorithm-encyclopedia/polynomial-interpolation-geometric#noredirect

導出は ↑ を参照（結構手間）。

実装上は $r^n \equiv 1$ となる場合にちょっと注意。

#### interpolation_geo

```cpp
FormalPowerSeries<mint> interpolation_geo(mint a, mint r, vc<mint> ys)
```

$f(a r^i) = y_i \ (0 \leq i \leq n-1)$ を満たす、高々 $n-1$ 次の多項式 $f(x)$ を求める。

##### 制約

- $r^i \not\equiv 1 \ (1 \leq i \leq n-1)$

##### 計算量

- $O(n \log n)$
