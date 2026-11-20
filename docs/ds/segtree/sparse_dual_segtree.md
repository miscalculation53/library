このドキュメントには AI による作成・変更が含まれています。

## 概要

長さ $n$ の一様な列に対し、オンラインで区間作用・点代入・点取得を行う疎双対セグメント木。添字は `ll` 型で扱う。[双対セグメント木](dual_segtree.md) と同じ形式の作用を使う。

一様な区間を保持し、更新境界で分割する。[疎遅延セグメント木](sparse_lazy_segtree.md) と区間の分割・遅延作用を共有する。更新 $1$ 回につき増えるノードは高々 $2$ 個で、$q$ 回の更新後のノード数は高々 $2q+1$ 個。点取得は `const` で、ノードを追加せずに処理する。

## 使用例

```cpp
#include "ds/segtree/dual_segtree.hpp"
#include "ds/segtree/sparse_dual_segtree.hpp"

using A = ActCompoundAssignment<MonoidAdd<ll>>;
constexpr ll n = 1LL << 40;
SparseDualSegmentTree<A> seg(n, 1);
seg.apply(10, n, 3);
seg.set(20, 9);
assert(seg.get(9) == 1);
assert(seg.get(20) == 9);
assert(seg.get(n - 1) == 4);
```

## 詳細なドキュメント

### SparseDualSegmentTree

```cpp
template <class A> struct SparseDualSegmentTree;
using S = typename A::S;
using F = typename A::F;
```

`A` は `S mapping(F f, S x)`、`F composition(F f, F g)`、`F id()` に相当する関数を持つ。
`composition(f,g)` は先に `g`、次に `f` を適用する合成で、`id()` はその単位元。
`mapping(id(),x)` は `x` を返し、`mapping(composition(f,g),x)` は `mapping(f,mapping(g,x))` と等しい。

#### コンストラクタ

```cpp
SparseDualSegmentTree(ll n, const S& initial_value)
```

初期値が各点で `initial_value` の長さ $n$ の列を作る。

##### 制約

- $n\geq0$

##### 計算量

- $O(1)$

#### reserve

```cpp
void reserve(int count)
```

少なくとも `count` ノード分の領域を確保する。

##### 制約

- `count` は非負

##### 計算量

- 現在のノード数を $K$ として $O(K)$

#### set / get

```cpp
void set(ll p, const S& x)
S get(ll p) const
```

`set` は $p$ 番目の値を `x` に変え、`get` は $p$ 番目の値を返す。

##### 制約

- $0\leq p<n$

##### 計算量

- `set` は償却 $O(\log n)$、`get` は $O(\log n)$

#### apply

```cpp
(1) void apply(ll p, const F& f)
(2) void apply(ll l, ll r, const F& f)
```

(1) は $p$ 番目の点、(2) は $[l,r)$ の各点に `f` を作用させる。

##### 制約

- (1) は $0\leq p<n$
- (2) は $0\leq l\leq r\leq n$

##### 計算量

- 償却 $O(\log n)$

#### content

```cpp
vc<tuple<ll, ll, S>> content() const
```

現在の列を `(左端, 右端, 一点の値)` の列として左端の昇順に返す。更新で作られた区間境界を含み、$n=0$ なら空の列を返す。

##### 計算量

- 現在のノード数を $K$ として $O(K)$
