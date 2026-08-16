## 概要

下から $k$ 番目のビットを取得

## 詳細なドキュメント

#### kth_bit_pos

```cpp
ull kth_bit_pos(ull x, int k)
```

`x` で立っているビットのうち、下から $k$ 番目のビット位置を返す。

##### 制約

- $0 \leq k < \operatorname{popcount}(x)$

##### 計算量

- $O(1)$
