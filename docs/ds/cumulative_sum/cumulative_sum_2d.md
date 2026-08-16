## 概要

$2$ 次元累積和

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) CumulativeSum2D<G = GroupAddSub<ll>>()
(2) CumulativeSum2D<G = GroupAddSub<ll>>(const vvc<G::S>& a)
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
