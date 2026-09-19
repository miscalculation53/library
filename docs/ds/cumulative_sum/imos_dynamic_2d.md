## 概要

矩形更新・点代入・点取得を扱う $2$ 次元 imos 法。配る DP などで使う。

`get(i, j)` と `set(i, j, v)` は $[0,i]\times[0,j]$ の差分を値に展開する。以後の `add` は、これまでに展開した矩形の和集合の外側に行う。空矩形への更新はいつでも行える。`set` による点代入は取得済みの位置にも使える。取得位置は任意の順序でよく、展開済みの値は何度でも取得できる。

例えば `get(1, 4)` の後に `get(4, 1)` を取得しても、位置 $(3,3)$ を含む未展開の矩形には引き続き `add` できる。展開範囲は各行の接頭辞の長さで管理する。各マスの差分を必要になったときに一度だけ累積し、その場で値に置き換える。

$n$ 行 $m$ 列、`content` を除く操作数 $q$ に対し、構築と全操作の時間は $O(nm+n+m+q)$、空間は $O(nm+n+m)$。$n,m\geq1$ ならそれぞれ $O(nm+q)$、$O(nm)$。`G` の演算は $O(1)$ とする。

点更新・矩形取得には [動的累積和 (2D)](cumulative_sum_dynamic_2d.md) を使う。

## 使用例

両方の座標が大きい位置へ配る DP。

```cpp
ImosDynamic2D<GroupAddSub<ll>> dp(3, 3);
dp.set(0, 0, 1);
for (int i = 0; i < 3; ++i)
  for (int j = 0; j < 3; ++j)
  {
    ll v = dp.get(i, j);
    dp.add(i + 1, 3, j + 1, 3, v);
  }
assert(dp.get(2, 2) == 2);
```

## 詳細なドキュメント

### ImosDynamic2D

#### コンストラクタ

```cpp
(1) ImosDynamic2D<G>()
(2) ImosDynamic2D<G>(int n, int m)
(3) ImosDynamic2D<G>(const vvc<S>& a)
```

`S = G::S`。

- (1)：$0$ 行 $0$ 列で初期化する。
- (2)：単位元を $n\times m$ 個並べた配列で初期化する。
- (3)：配列 `a` で初期化する。各位置は未展開。

##### 制約

- `G` は可換群。
- (2)：$n,m\geq0$。
- (3)：`a` の各行の長さは等しい。

##### 計算量

- (1)：$O(1)$。
- (2), (3)：$O(nm+n+m)$。

#### add

```cpp
void add(int li, int ri, int lj, int rj, const S& v)
```

$[\mathrm{li},\mathrm{ri})\times[\mathrm{lj},\mathrm{rj})$ の各要素を $a_{i,j}\gets G::op(a_{i,j},v)$ とする。

##### 制約

- $0\leq\mathrm{li}\leq\mathrm{ri}\leq n$、$0\leq\mathrm{lj}\leq\mathrm{rj}\leq m$。
- 面積が正の場合、矩形内の全位置が未展開。これは位置 $(\mathrm{li},\mathrm{lj})$ が未展開であることと同値。

##### 計算量

- $O(1)$。

#### set

```cpp
void set(int i, int j, const S& v)
```

位置 $(i,j)$ の現在値を $v$ に置き換える。取得済みの位置にも使え、他の位置の値は保つ。内部で `get(i, j)` を行うため、$[0,i]\times[0,j]$ の差分が展開される。

##### 制約

- $0\leq i<n$、$0\leq j<m$。

##### 計算量

- 新たに展開するマス数を $k$ として $O(k+1)$。展開済みの位置への代入は $O(1)$。

#### get

```cpp
S get(int i, int j)
```

$[0,i]\times[0,j]$ の差分を値に展開し、$a_{i,j}$ を返す。

##### 制約

- $0\leq i<n$、$0\leq j<m$。

##### 計算量

- 新たに展開するマス数を $k$ として $O(k+1)$。全操作での $k$ の合計は高々 $nm$。

#### content

```cpp
vvc<S> content() const
```

すべての更新を反映した $n\times m$ の配列のコピーを返す。元の展開範囲を保つため、デバッグ出力の後も同じ範囲に `add` できる。

##### 計算量

- $O(nm+n)$。
