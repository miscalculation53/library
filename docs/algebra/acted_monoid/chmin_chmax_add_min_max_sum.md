## 概要

区間 `chmin`・`chmax`・加算と、区間最小値・最大値・和の取得を Segment Tree Beats で処理する作用つきモノイド。

各区間について、最小値・第二最小値・最小値の個数、最大値・第二最大値・最大値の個数、要素数、総和を持つ。clamp によって第二最小値以上または第二最大値以下の要素まで変化し、保持情報だけでは更新できない場合は `fail` を立てる。`SegmentTreeBeats` はその作用を子へ伝播し、親を再構築する。

## 使用例

```cpp
using AM = ActedMonoidChminChmaxAddMinMaxSum<ll>;
using F = AM::F;
SegmentTreeBeats<AM> seg(a);

seg.apply(l, r, F::chmin(x));
seg.apply(l, r, F::chmax(x));
seg.apply(l, r, F::add(x));
auto res = seg.prod(l, r);
cout << res.mn << ' ' << res.mx << ' ' << res.sum << '\n';
```

## 詳細なドキュメント

### ActedMonoidChminChmaxAddMinMaxSum

```cpp
ActedMonoidChminChmaxAddMinMaxSum<T, infty = INF>
```

作用型 `F` は `MonoidClampAdd<T, infty>::S`。`F::chmin`, `F::chmax`, `F::add` で各更新を構築する。

#### S のコンストラクタ

```cpp
S(T x, int len = 1)
```

値 `x` が `len` 個並ぶ区間情報を構築する。一点の初期値は `S(x)` で構築できる。

`S` は公開メンバ `mn`, `mx`, `mn2`, `mx2`, `sum`, `len`, `mn_cnt`, `mx_cnt`, `fail` を持つ。

##### 制約

- `len >= 0`
- `x * len` が `T` に収まる

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& a, const S& b)
```

`a`, `b` を連結した区間情報を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

長さ $0$ の単位元を返す。

##### 制約

- `-infty` は取り得るすべての値以下、`infty` は取り得るすべての値以上

##### 計算量

- $O(1)$

#### mapping

```cpp
S mapping(const F& f, S x)
```

区間 `x` に `f` を作用させる。保持情報だけで正しい結果を求められる場合は更新後の情報を返し、求められない場合は `fail = true` として返す。

##### 制約

- 葉への作用は失敗しない
- 値、総和および中間計算が `T` に収まる

##### 計算量

- $O(1)$

#### composition

```cpp
F composition(const F& f, const F& g)
```

先に `g`、次に `f` を作用させる合成を返す。

##### 計算量

- $O(1)$

#### id

```cpp
F id()
```

作用の恒等関数を返す。

##### 計算量

- $O(1)$

### SegmentTreeBeats と組み合わせた場合

`SegmentTreeBeats<ActedMonoidChminChmaxAddMinMaxSum<T, infty>>` として利用する。

#### apply / prod

```cpp
void apply(int l, int r, const F& f)
S prod(int l, int r)
```

`apply` は $[l,r)$ に `chmin`・`chmax`・加算を適用する。`prod` は $[l,r)$ の最小値・最大値・和を返す。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- 各操作あたり償却 $O(\log^2 n)$
