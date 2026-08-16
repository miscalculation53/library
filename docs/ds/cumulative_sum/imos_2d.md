## 概要

$2$ 次元 imos 法

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) Imos2D<G = GroupAddSub<ll>>()
(2) Imos2D<G = GroupAddSub<ll>>(int n, int m)
(3) Imos2D<G = GroupAddSub<ll>>(const vvc<G::S>& a)
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
