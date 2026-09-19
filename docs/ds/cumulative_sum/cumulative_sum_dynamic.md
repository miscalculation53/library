## 概要

点更新と区間取得を混ぜて使う $1$ 次元累積和。もらう DP など、取得した接頭辞を確定できる場合に使う。

`sum(l, r)` または `sum(r)` を呼ぶと、$[0,r)$ が確定する。以後の `set`・`add` は確定範囲より後ろの位置に行う。取得の右端は任意の順序でよく、確定済みの区間は何度でも取得できる。空区間の取得は確定範囲を広げない。`get` は点の現在値を読むだけで、確定範囲を広げない。

各要素の累積積を初めて必要になったときに一度だけ計算する。長さ $n$、`content` を除く操作数 $q$ に対し、構築と全操作の時間は $O(n+q)$、空間は $O(n)$。`G` の演算は $O(1)$ とする。

区間更新・点取得には [動的 imos 法](imos_dynamic.md)、$2$ 次元には [動的累積和 (2D)](cumulative_sum_dynamic_2d.md) を使う。

## 使用例

直前の $2$ 項からもらう DP。

```cpp
CumulativeSumDynamic<GroupAddSub<ll>> dp(6);
dp.set(0, 1);
for (int i = 1; i < 6; ++i)
  dp.set(i, dp.sum(max(0, i - 2), i));
assert(dp.get(5) == 8);
```

## 詳細なドキュメント

### CumulativeSumDynamic

#### コンストラクタ

```cpp
(1) CumulativeSumDynamic<G>()
(2) CumulativeSumDynamic<G>(int n)
(3) CumulativeSumDynamic<G>(const vc<S>& a)
```

`S = G::S`。

- (1)：長さ $0$ で初期化する。
- (2)：単位元を $n$ 個並べた列で初期化する。
- (3)：列 `a` で初期化する。各位置は未確定。

##### 制約

- `G` は群。
- (2)：$n \geq 0$。

##### 計算量

- (1)：$O(1)$。
- (2), (3)：$O(n)$。

#### set / add

```cpp
void set(int i, const S& v)
void add(int i, const S& v)
```

`set` は $a_i \gets v$、`add` は $a_i \gets G::op(a_i,v)$ とする。

##### 制約

- $0 \leq i < n$。
- 位置 $i$ は未確定。

##### 計算量

- $O(1)$。

#### get

```cpp
S get(int i) const
```

$a_i$ を返す。確定範囲は変わらない。

##### 制約

- $0 \leq i < n$。

##### 計算量

- $O(1)$。

#### sum

```cpp
(1) S sum(int r)
(2) S sum(int l, int r)
```

- (1)：$[0,r)$ を確定し、その累積積を返す。
- (2)：$[0,r)$ を確定し、$[l,r)$ の積を返す。$l=r$ の場合は単位元を返し、確定範囲を保つ。

積の順序は添字の昇順。

##### 制約

- (1)：$0 \leq r \leq n$。
- (2)：$0 \leq l \leq r \leq n$。

##### 計算量

- 新たに確定する要素数を $k$ として $O(k+1)$。全操作での $k$ の合計は高々 $n$。

#### content

```cpp
vc<S> content() const
```

現在の長さ $n$ の列のコピーを返す。確定範囲を保つため、デバッグ出力の後も同じ位置を更新できる。

##### 計算量

- $O(n)$。
