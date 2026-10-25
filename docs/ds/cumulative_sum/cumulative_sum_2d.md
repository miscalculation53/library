このドキュメントには AI による作成・変更が含まれています。

## 概要

$2$ 次元累積和

更新と取得を混ぜ、取得に使った原点側の矩形を確定できる場合は [動的累積和](cumulative_sum_dynamic_2d.md) を使う。

## 使用例

```cpp
#include "ds/cumulative_sum/cumulative_sum_2d.hpp"

CumulativeSum2D<GroupAddSub<ll>> sums(vvc<ll>{{1, 2, 3}, {4, 5, 6}});
assert(sums.sum(0, 2, 1, 3) == 16); // [0, 2) × [1, 3)
assert(sums.sum(1, 2) == 3);        // [0, 1) × [0, 2)
```

## 詳細なドキュメント

### CumulativeSum2D

#### コンストラクタ

```cpp
(1) CumulativeSum2D<G>()
(2) CumulativeSum2D<G>(const vvc<G::S>& a)
```

- (1)：デフォルトコンストラクタ。
- (2)：$2$ 次元配列 `a` の累積積を構築する。

##### 制約

- `G` は可換群
- `a` の各行の長さは等しい

##### 計算量

`a` の行数を $n$、列数を $m$ として、

- 時間：$O(nm)$

#### sum

```cpp
(1) S sum(int ri, int rj)
(2) S sum(int li, int ri, int lj, int rj)
```

- (1)：$[0, \mathrm{ri}) \times [0, \mathrm{rj})$ の積を返す。
- (2)：$[\mathrm{li}, \mathrm{ri}) \times [\mathrm{lj}, \mathrm{rj})$ の積を返す。

##### 制約

- (1)：$0 \leq \mathrm{ri} \leq n$, $0 \leq \mathrm{rj} \leq m$
- (2)：$0 \leq \mathrm{li} \leq \mathrm{ri} \leq n$, $0 \leq \mathrm{lj} \leq \mathrm{rj} \leq m$

##### 計算量

- $O(1)$

#### content

```cpp
vvc<S> content() const
```

構築元の $n\times m$ 配列を返す。空の行も維持する。デフォルト構築時は空の配列を返す。

##### 計算量

- $O(n+nm)$
