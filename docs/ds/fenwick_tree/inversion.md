## 転倒数

#### inversion_number

```cpp
ll inversion_number(V v)
```

$0 \leq i \lt j \lt \lvert v \rvert$ かつ $v_i \gt v_j$ を満たす $(i, j)$ の個数を返す。

##### 制約

- $v$ の要素は比較可能

##### 計算量

- $O(\lvert v \rvert \log \lvert v \rvert)$
