このドキュメントには AI による作成・変更が含まれています。

## 概要

構築時に登録した点の一点更新と、半開矩形内の重みの和を扱う。$2$ 次元目のデータ構造 `D` は `SegmentTree` が既定で、更新がない場合は `CumulativeSum` や (Disjoint) Sparse Table も選べる。

`D<M>` が `CumulativeSum<M>` と同じ型の場合、静的な可換群の矩形和として構築する。$x$ 順の点列を $y$ の順位のビットで安定分割し、各層の位置写像と前置和を保持する。構築は $O(p \log p)$、矩形和と単一 $x$ の和は $O(\log p)$、全点の和は $O(1)$、保持領域は $O(p \log p)$。ここで $p$ は登録した点数とし、群の演算は $O(1)$ とする。

その他の `D` では、内側のデータ構造が $y$ 順に値をまとめる一方、外側の木は $x$ 区間ごとの結果をまとめる。一般の非可換モノイドでは両者の順序を同時に保てないため、`M` は可換である必要がある。

その他の `D` では、Fractional Cascading により $y$ の二分探索を根で一度だけ行う。各ノードの $y$ 座標列から子の列へ移る添字を保持し、外側の木を走査する。

構築時に与えた点の添字を `point_id` として一点更新に使う。$x$ 座標は内部で自動的に座標圧縮し、一点更新では座標の二分探索を行わない。矩形クエリには生の座標を渡す。

その他の `D` では、各ノードの $y$ 座標列を CSR で管理し、子ノードでの添字も同じ平坦な添字に対応する連続配列へ格納する。初期値は内側のデータ構造へ移した後、座標管理側には重複して保持しない。

## 使用例

```cpp
#include "algebra/algebra_basic_ops.hpp"
#include "ds/cumulative_sum/cumulative_sum.hpp"
#include "ds/2d/sparse/segtree.hpp"

SegmentTree2DSparse<GroupAddSub<long long>, int, CumulativeSum> seg(
    {{0, 0, 3}, {0, 1, 5}, {2, 1, 7}});
assert(seg.sum(0, 2, 0, 2) == 8);
assert(seg.sum(0, 0, 2) == 8);
assert(seg.all_sum() == 15);
```

## 詳細なドキュメント

#### コンストラクタ

```cpp
SegmentTree2DSparse<M, I, D>()
SegmentTree2DSparse<M, I, D>(const vc<tuple<I, I, M::S>>& xyws)
```

- `M` は可換モノイド。`D<M>` が `CumulativeSum<M>` と同じ型の場合は可換群で、`M::inv` を持つ
- `I` は座標の型。`CumulativeSum` の経路では `<` が狭義弱順序を与える
- `D<M>` は $2$ 次元目に載せるデータ構造。`D` のデフォルトは `SegmentTree`
- `xyws` は更新されうる点と初期値
- `xyws[i]` の点は `point_id = i` で指定する
- 同じ座標が複数回含まれる場合、初期値は `M::op` でまとめられ、それらの `point_id` は同じ点を指す
- `D<M>` は `vc<M::S>` から構築できる
- `CumulativeSum` の経路では、座標と値をコピー・ムーブ・代入できる
- 集計演算の結果と中間値を表現できる `M::S` を選ぶ

`CumulativeSum` のデフォルト構築は空集合を表す。コピーは値コピーになる。その他の `D` のコピーでは、`D<M>` 自身のコピー動作が適用される。

##### 計算量

$p = \lvert\mathrm{xyws}\rvert$ として、`D<M>` の構築が要素数に対して線形なら

- `CumulativeSum`：$O(p \log p)$
- その他の `D`：$O(p \log^2 p)$

#### set

```cpp
void set(int point_id, M::S val)
```

`xyws[point_id]` の点の値を $\mathrm{val}$ にする。

##### 制約

- $0 \leq \mathrm{point\_id} < \lvert\mathrm{xyws}\rvert$
- `D<M>` は `set` と `get` を持つ

##### 計算量

`D<M>::get`, `D<M>::set` の計算量をそれぞれ $f(p),g(p)$ として

- $O(\log p \cdot (f(p)+g(p)))$

#### get

```cpp
M::S get(int point_id)
```

`xyws[point_id]` の点の値を取得する。

##### 制約

- $0 \leq \mathrm{point\_id} < \lvert\mathrm{xyws}\rvert$
- `D<M>` は `get` を持つ

##### 計算量

`D<M>::get` の計算量を $f(p)$ として

- $O(f(p))$

#### modify

```cpp
void modify(int point_id, const F& f)
```

`xyws[point_id]` の点の値への参照を `f` に渡して変更する。`get` と `set` を続けて呼ぶ場合と異なり、外側の木の走査は一度だけ行う。

##### 制約

- $0 \leq \mathrm{point\_id} < \lvert\mathrm{xyws}\rvert$
- `D<M>` は `get` と `set` を持つ

##### 計算量

`D<M>::get`, `D<M>::set` の計算量をそれぞれ $f(p),g(p)$ として

- $O(\log p \cdot (f(p)+g(p)))$

#### sum

```cpp
(1) M::S sum(I x, I ly, I ry)
(2) M::S sum(I lx, I rx, I ly, I ry)
```

- (1) 領域 $\{\mathrm{x}\} \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの和を求める。
- (2) 矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの和を求める。

##### 制約

- `D<M>` は `sum(l, r)` または `prod(l, r)` を持つ
- (1) では $\mathrm{ly} \leq \mathrm{ry}$、(2) ではそれに加え $\mathrm{lx} \leq \mathrm{rx}$

##### 計算量

$f(p)$ を内側の区間クエリの計算量として

- `CumulativeSum`：(1)、(2) ともに $O(\log p)$
- その他の `D`：(1) は $O(\log p + f(p))$、(2) は $O(\log p \cdot f(p))$

#### all_sum

```cpp
M::S all_sum()
```

すべての点の重みの和を求める。

##### 制約

- `D<M>` は `sum(l, r)` または `prod(l, r)` を持つ

##### 計算量

$f(p)$ を内側の区間クエリの計算量として

- `CumulativeSum`：$O(1)$
- その他の `D`：$O(f(p))$

#### content

```cpp
vc<tuple<I, I, M::S>> content()
```

$(x\ 座標, y\ 座標, 重み)$ の組のうち、重みが `M::e()` でないものを格納した vector を返す。デバッグ用を想定。

##### 制約

- `D<M>` は `get` を持つ

##### 計算量

$u$ を異なる登録点の個数、$f(p)$ を `D<M>::get` の計算量として

- $O(u \cdot f(p))$
