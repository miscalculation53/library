## 概要

$1$ 次元累積和

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) CumulativeSum<G = GroupAddSub<ll>>()
(2) CumulativeSum<G = GroupAddSub<ll>>(const vc<G::S>& a)
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
