## 概要

転倒数

## 詳細なドキュメント

#### inversion_number

```cpp
ll inversion_number(V v)
```

$0 \leq i \lt j \lt \lvert v \rvert$ かつ $v_i \gt v_j$ を満たす $(i, j)$ の個数を返す。

##### 制約

- $v$ の要素は比較可能

##### 計算量

- $O(\lvert v \rvert \log \lvert v \rvert)$

----

ほかにも、転倒数は区間の伸縮ができる（用途：Mo で区間転倒数など）。これを Mo などに渡せる `InversionSlider` を実装した（`Slider` 系で実装すべきものの説明は Mo を参照）。
