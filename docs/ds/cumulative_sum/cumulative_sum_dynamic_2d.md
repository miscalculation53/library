## 概要

点更新と矩形取得を混ぜて使う $2$ 次元累積和。もらう DP など、取得に使った原点側の矩形を確定できる場合に使う。

`sum(li, ri, lj, rj)` または `sum(ri, rj)` を呼ぶと、$[0,\mathrm{ri})\times[0,\mathrm{rj})$ が確定する。以後の `set`・`add` は、これまでに確定した矩形の和集合の外側に行う。空矩形の取得と `get` は確定範囲を広げない。

取得位置は任意の順序でよい。例えば `sum(2, 5)` の後に `sum(5, 2)` を取得しても、位置 $(3,3)$ は引き続き更新できる。確定範囲は各行の接頭辞の長さで管理し、新たに必要なマスだけを反復処理で計算する。

$n$ 行 $m$ 列、`content` を除く操作数 $q$ に対し、構築と全操作の時間は $O(nm+n+m+q)$、空間は $O(nm+n+m)$。$n,m\geq1$ ならそれぞれ $O(nm+q)$、$O(nm)$。`G` の演算は $O(1)$ とする。

矩形更新・点取得には [動的 imos 法 (2D)](imos_dynamic_2d.md) を使う。

## 使用例

両方の座標が小さい位置からもらう DP。

```cpp
CumulativeSumDynamic2D<GroupAddSub<ll>> dp(3, 3);
for (int i = 0; i < 3; ++i)
  for (int j = 0; j < 3; ++j)
    dp.set(i, j, i == 0 && j == 0 ? 1 : dp.sum(i, j));
assert(dp.get(2, 2) == 2);
```

## 詳細なドキュメント

### CumulativeSumDynamic2D

#### コンストラクタ

```cpp
(1) CumulativeSumDynamic2D<G>()
(2) CumulativeSumDynamic2D<G>(int n, int m)
(3) CumulativeSumDynamic2D<G>(const vvc<S>& a)
```

`S = G::S`。

- (1)：$0$ 行 $0$ 列で初期化する。
- (2)：単位元を $n\times m$ 個並べた配列で初期化する。
- (3)：配列 `a` で初期化する。各位置は未確定。

##### 制約

- `G` は可換群。
- (2)：$n,m\geq0$。
- (3)：`a` の各行の長さは等しい。

##### 計算量

- (1)：$O(1)$。
- (2), (3)：$O(nm+n+m)$。

#### set / add

```cpp
void set(int i, int j, const S& v)
void add(int i, int j, const S& v)
```

`set` は $a_{i,j}\gets v$、`add` は $a_{i,j}\gets G::op(a_{i,j},v)$ とする。

##### 制約

- $0\leq i<n$、$0\leq j<m$。
- 位置 $(i,j)$ は未確定。

##### 計算量

- $O(1)$。

#### get

```cpp
S get(int i, int j) const
```

$a_{i,j}$ を返す。確定範囲は変わらない。

##### 制約

- $0\leq i<n$、$0\leq j<m$。

##### 計算量

- $O(1)$。

#### sum

```cpp
(1) S sum(int ri, int rj)
(2) S sum(int li, int ri, int lj, int rj)
```

$[0,\mathrm{ri})\times[0,\mathrm{rj})$ を確定する。

- (1)：確定した矩形の積を返す。
- (2)：$[\mathrm{li},\mathrm{ri})\times[\mathrm{lj},\mathrm{rj})$ の積を返す。

取得する矩形が空の場合は単位元を返し、確定範囲を保つ。

##### 制約

- (1)：$0\leq\mathrm{ri}\leq n$、$0\leq\mathrm{rj}\leq m$。
- (2)：$0\leq\mathrm{li}\leq\mathrm{ri}\leq n$、$0\leq\mathrm{lj}\leq\mathrm{rj}\leq m$。

##### 計算量

- 新たに確定するマス数を $k$ として $O(k+1)$。全操作での $k$ の合計は高々 $nm$。

#### content

```cpp
vvc<S> content() const
```

現在の $n\times m$ の配列のコピーを返す。確定範囲を保つため、デバッグ出力の後も同じ位置を更新できる。

##### 計算量

- $O(nm+n)$。
