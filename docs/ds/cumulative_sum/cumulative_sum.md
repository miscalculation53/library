このドキュメントには AI による作成・変更が含まれています。

## 概要

$1$ 次元累積和

更新と取得を混ぜ、取得した接頭辞を確定できる場合は [動的累積和](cumulative_sum_dynamic.md) を使う。

## 使用例

```cpp
#include "ds/cumulative_sum/cumulative_sum.hpp"

CumulativeSum<GroupAddSub<ll>> sums(vc<ll>{2, 5, 3});
assert(sums.sum(2) == 7);    // [0, 2)
assert(sums.sum(1, 3) == 8); // [1, 3)
```

## 詳細なドキュメント

### CumulativeSum

#### コンストラクタ

```cpp
(1) CumulativeSum<G>()
(2) CumulativeSum<G>(const vc<G::S>& a)
```

- (1)：デフォルトコンストラクタ。
- (2)：列 `a` の累積積を構築する。

##### 制約

- `G` は群

##### 計算量

- (2)：$O(n)$

#### sum

```cpp
(1) S sum(int r)
(2) S sum(int l, int r)
```

- (1)：$a_0 \bullet \cdots \bullet a_{r-1}$ を返す。
- (2)：$a_l \bullet \cdots \bullet a_{r-1}$ を返す。

##### 制約

- (1)：$0 \leq r \leq n$
- (2)：$0 \leq l \leq r \leq n$

##### 計算量

- $O(1)$

#### content

```cpp
vc<S> content() const
```

構築元の列を添字順に返す。デフォルト構築時は空の列を返す。

##### 計算量

- $O(n)$
