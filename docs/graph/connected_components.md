## 連結成分への分解

#### connected_component_ids

```cpp
vc<I> connected_component_ids<I = ll>(const GraphUndirected<Cost> &g)
```

無向グラフを受け取り、各頂点の連結成分の番号を返す。

##### 計算量

- $O(n + m)$

