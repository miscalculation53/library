このドキュメントには AI による作成・変更が含まれています。

## 概要

長い一様な列に対し、区間更新をまとめて処理する [imos 法](imos.md)。更新の端点を保存し、更新後の列を、各区間内で値が一定の区間列として返す。列の長さにかかわらず、保持する差分の個数は更新回数に比例する。

値に区間長を掛けると、その区間の全要素の和を求められる。条件を満たす区間の長さを足すと、条件を満たす点の個数を求められる。

## 使用例

```cpp
#include "ds/cumulative_sum/sparse_imos.hpp"

SparseImos<GroupAddSub<ll>> a(1LL << 40);
a.add(10, 20, 1);
a.add(15, 25, 1);
ll covered = 0;
for (auto [l, r, value] : a.content())
  if (value > 0) covered += r - l;
assert(covered == 15);
```

## 詳細なドキュメント

### SparseImos

```cpp
template <class G, class I = ll> struct SparseImos;
using S = typename G::S;
```

添字型 `I` には `i128` や `u128` も使える。

#### コンストラクタ

```cpp
(1) SparseImos()
(2) explicit SparseImos(I n, const S& initial_value = G::e())
```

(1) は長さ $0$、(2) は初期値が各点で `initial_value` の長さ $n$ の列を作る。

##### 制約

- `G` は可換群
- (2) は $n\geq0$

##### 計算量

- $O(1)$

#### add

```cpp
void add(I l, I r, const S& v)
```

$[l,r)$ の各位置 $i$ について、`a[i]` を `G::op(a[i], v)` に更新する。

##### 制約

- $0\leq l\leq r\leq n$

##### 計算量

- 償却 $O(1)$

#### content

```cpp
vc<tuple<I, I, S>> content() const
```

呼び出し時点の列を `(左端, 右端, 一点の値)` の列として左端の昇順に返す。空でない更新区間の端点と $0,n$ で $[0,n)$ を分割する。差分が相殺した端点も境界に含む。$n=0$ なら空の列を返す。呼び出し後も更新を追加できる。

##### 計算量

- 空でない更新区間の個数を $q$ として $O(q\log q)$。`bool` を除く固定幅整数の添字では $O(q)$
