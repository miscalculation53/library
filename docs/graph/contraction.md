## グラフの縮約

想定用途：連結成分や強連結成分への分解などと組み合わせて使う。

#### contracted_graph

```cpp
Graph<is_directed, Cost> contracted_graph(const GraphUndirected<is_directed, Cost> g, vc<I> ids)
```

グラフ $g$ を、頂点 $v$ が縮約後に $\mathrm{ids}[v]$ の頂点になるように縮約したグラフを返す。ただし、コストは特に考えず、縮約後のグラフには自己ループや多重辺を含まないようにする。

##### 計算量

- $O(n + m + \lvert\mathrm{ids}\rvert)$

