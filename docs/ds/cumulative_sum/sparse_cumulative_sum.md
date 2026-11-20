このドキュメントには AI による作成・変更が含まれています。

## 概要

疎な点集合に対し、座標の半開区間に含まれる重みを集約する。座標順の点列に [累積和](cumulative_sum.md) を構築し、問い合わせの端点を二分探索する。座標の範囲にかかわらず、相異なる登録座標数を $K$ として $O(K)$ 空間を使う。

積の順序は座標の昇順で、同じ座標の重みは入力順に結合する。非可換群にも対応する。

## 使用例

```cpp
#include "ds/cumulative_sum/sparse_cumulative_sum.hpp"

SparseCumulativeSum<GroupAddSub<ll>> sums({{12, 7}, {-4, 2}, {12, 3}});
assert(sums.sum(12) == 2);
assert(sums.sum(0, 13) == 10);
assert(sums.sum(13, 18) == 0);
```

## 詳細なドキュメント

### SparseCumulativeSum

```cpp
template <class G, class I = ll> struct SparseCumulativeSum;
using S = typename G::S;
```

`I` は座標型で、`i128` や `u128` も使える。

#### コンストラクタ

```cpp
(1) SparseCumulativeSum()
(2) explicit SparseCumulativeSum(const vc<pair<I, S>>& points)
```

(1) は空の点集合を、(2) は `(座標, 重み)` の列から点集合を構築する。入力の座標順は任意。

##### 制約

- `G` は群
- `I` は `<` が全順序を定める型

##### 計算量

- (1) は $O(1)$
- (2) は入力点数を $N$ として $O(N\log N)$。`bool` を除く固定幅整数の座標では $O(N)$

#### sum

```cpp
(1) S sum(const I& r) const
(2) S sum(const I& l, const I& r) const
```

(1) は座標が $r$ 未満の点、(2) は座標が $[l,r)$ に入る点の重みの積を返す。対象となる点が $0$ 個なら単位元を返す。

##### 制約

- (2) は $l\leq r$

##### 計算量

- $O(\log K)$

#### content

```cpp
vc<pair<I, S>> content() const
```

座標ごとにまとめた `(座標, 重み)` の列を座標順に返す。重みが単位元になった座標も含む。

##### 計算量

- $O(K)$
