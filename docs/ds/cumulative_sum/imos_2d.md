このドキュメントには AI による作成・変更が含まれています。

## 概要

$2$ 次元 imos 法

更新と取得を混ぜ、取得した位置まで原点側の差分を順次展開する場合は [動的 imos 法](imos_dynamic_2d.md) を使う。

## 使用例

```cpp
#include "ds/cumulative_sum/imos_2d.hpp"

Imos2D<GroupAddSub<ll>> a(2, 3);
a.add(0, 2, 1, 3, 4); // [0, 2) × [1, 3) に 4 を加算
a.add(1, 2, 0, 2, 1);
assert(a.content() == (vvc<ll>{{0, 4, 4}, {1, 5, 4}}));
```

## 詳細なドキュメント

### Imos2D

#### コンストラクタ

```cpp
(1) Imos2D<G>()
(2) Imos2D<G>(int n, int m)
(3) Imos2D<G>(const vvc<G::S>& a)
```

- (1)：デフォルトコンストラクタ。
- (2)：単位元を $n \times m$ 個並べた配列で初期化する。
- (3)：$2$ 次元配列 `a` で初期化する。

##### 制約

- `G` は可換群
- (2)：$n, m \geq 0$
- (3)：`a` の各行の長さは等しい

##### 計算量

- (2), (3)：$O(nm)$

#### add

```cpp
void add(int li, int ri, int lj, int rj, const S& v)
```

$[\mathrm{li}, \mathrm{ri}) \times [\mathrm{lj}, \mathrm{rj})$ の各要素に $v$ を作用させる。

##### 制約

- $0 \leq \mathrm{li} \leq \mathrm{ri} \leq n$
- $0 \leq \mathrm{lj} \leq \mathrm{rj} \leq m$

##### 計算量

- $O(1)$

#### content

```cpp
vvc<S> content()
```

すべての矩形作用を反映した $n \times m$ の配列を返す。

##### 計算量

- 時間：$O(nm)$
