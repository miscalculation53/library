## 概要

サイクル検出

## 詳細なドキュメント

#### cycle_detection

```cpp
pair<bool, vc<Edge<Cost>>> cycle_detection(Graph<is_directed, Cost> g)
```

無向 or 有向 グラフ $g$ のサイクル検出を行う。サイクルがなければ `(false, 空)` を、サイクルがあれば `(true, サイクルの辺の vector)` を返す。

##### 計算量

- $O(n + m)$
