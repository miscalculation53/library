## 概要

セグ木のノード番号と区間の対応を計算する。

深さ $d + 1$ のセグ木は、$1$ から $2^{d+1} - 1$ までの $2^{d+1} - 1$ 個のノードで、$[0, 2^d)$ の区間を扱える。

## 詳細なドキュメント

#### segtree_node_to_segment

```cpp
pair<T, T> segtree_node_to_segment(int d, T i)
```

深さ $d+1$ のセグ木において、ノード $i$ が表す区間 $[a, b)$ を返す。

##### 制約

- $2^d$ は `T` に収まる
- $1 \leq i \leq 2^d - 1$

##### 計算量

- $O(1)$

#### segtree_segment_to_node

```cpp
T segtree_segment_to_node(int d, T a, T b)
```

##### 制約

- $2^d$ は `T` に収まる
- $0 \leq a \leq b \leq 2^d$
- $[a, b)$ は単一の頂点が表す区間である

##### 計算量

- $O(1)$

#### segtree_segments

```cpp
(1) vc<T> segtree_segments_from_left<T = ll>(int d, T l, T r)
(2) vc<T> segtree_segments_from_bottom<T = ll>(int d, T l, T r)
```

深さ $d+1$ のセグ木において、$[l, r)$ を表すノードの集合を返す。順番は：

- (1)：左にある区間から順に
- (2)：下にある区間から順に（同じ段は左から）

