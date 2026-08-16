## 概要

長さ $n$ が大きく、通常の遅延セグメント木を構築できない場合に、更新で必要になったノードだけを作る遅延セグメント木。オンラインの一点変更、区間作用、区間積、セグメント木上の二分探索を処理できる。[Range Affine Range Sum (Large Array)](https://judge.yosupo.jp/problem/range_affine_range_sum_large_array) のように、$n$ が $10^9$ 程度ある問題を対象とする。

列全体の初期値は一様であり、未生成区間の積は `initial_value` を区間長だけ並べた積として復元する。`AM::e()` が1点の初期値として適切とは限らないため、`initial_value` は必ず明示する。

`S` が区間長を持つ acted monoid では注意が必要である。たとえば `ActedMonoidAffineSum<T>` の `AM::e()` は長さ $0$ の空区間なので、これを並べても長さは $0$ のままであり、定数項を加える作用も値を変えない。長さ $n$ の値 $0$ の列を作る場合は、値 $0$ の **1 点** を表す `S(0)` を明示して渡す。

区間作用は未生成の点にも適用される。`SparseSegmentTree` は更新済みの点だけを保持し、未登録点を `M::e()` とみなすため、この点で用途が異なる。

更新 1 回につき高々 $O(\log n)$ 個のノードを生成する。$q$ 回の更新後のノード数は最悪 $O(q \log n)$ となる。`get`, `prod` および二分探索は、祖先からの遅延作用を持って木を降りるため、新しいノードを生成しない。

一様な初期値を区間長だけ並べた積は、コンストラクタで $O(\log n)$ 個の長さからなる小さな DAG にまとめる。各操作ではこの DAG を直接参照するため、ハッシュ表を使わず決定的に動作する。

## 使用例

```cpp
using mint = modint998244353;
using AM = ActedMonoidAffineSum<mint>;

SparseLazySegmentTree<AM> seg(1'000'000'000, AM::S(0));
seg.reserve(1 << 20);
seg.apply(10, 20, AM::F(2, 3));
assert(seg.prod(0, 1'000'000'000).val == 30);
```

## 詳細なドキュメント

### SparseLazySegmentTree

#### コンストラクタ

```cpp
SparseLazySegmentTree(ll n, const S& initial_value)
```

長さ $n$ の列を作る。各点の初期値は `initial_value` とする。

`S` が区間長を持ち、長さ $n$ の通常の列として扱う場合は、`initial_value` の長さを $1$ とする。`S` が表現できる区間長は $n$ 以上でなければならない。

##### 制約

- $0 \leq n$

##### 計算量

- $O(\log n\log\log n)$

#### reserve

```cpp
void reserve(int n)
```

少なくとも `n` ノード分の領域をあらかじめ確保する。必要な場合にコンストラクタの直後で呼ぶ。

##### 制約

- $0 \leq n$

##### 計算量

- 最悪 $O(V)$。$V$ は現在のノード数

#### modify

```cpp
template <class G>
void modify(ll p, const G& g)
```

`p` 番目の値への参照を `g` に渡して変更する。

```cpp
seg.modify(p, [&](S& x)
{
  x.val += value;
});
```

##### 制約

- $0 \leq p < n$

##### 計算量

- $O(\log n)$

#### set

```cpp
void set(ll p, const S& x)
```

`p` 番目の値を `x` にする。

##### 制約

- $0 \leq p < n$
- `x` は 1 点を表す

##### 計算量

- $O(\log n)$

#### get

```cpp
S get(ll p) const
```

`p` 番目の値を返す。

##### 制約

- $0 \leq p < n$

##### 計算量

- $O(\log n)$

#### apply (一点)

```cpp
void apply(ll p, const F& f)
```

`p` 番目の値に作用 `f` を適用する。

##### 制約

- $0 \leq p < n$

##### 計算量

- $O(\log n)$

#### apply (区間)

```cpp
void apply(ll l, ll r, const F& f)
```

区間 $[l, r)$ の各点に作用 `f` を適用する。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- $O(\log n)$

#### prod

```cpp
S prod(ll l, ll r) const
```

区間 $[l, r)$ の積を返す。積の順序は左から右であり、`S` は可換でなくてもよい。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- $O(\log n)$

#### all_prod

```cpp
S all_prod() const
```

列全体の積を返す。

##### 計算量

- $O(1)$

#### max_right_ok

```cpp
template <class G>
ll max_right_ok(ll l, const G& g) const
```

次の $R$ を返す。

$$
R = \max\left\{x \in \mathbb{Z} \mathrel{\middle|} l \leq x \leq n,\ g(\operatorname{prod}(l, x)) = \mathrm{true}\right\}
$$

すなわち、$[l, R)$ は条件を満たし、$R=n$ でなければ $[l, R+1)$ は条件を満たさない。

##### 制約

- $0 \leq l \leq n$
- `g(AM::e()) == true`
- $x$ を $l$ から $n$ へ増やしたとき、`g(prod(l, x))` は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\log n)$

#### min_left_ok

```cpp
template <class G>
ll min_left_ok(ll r, const G& g) const
```

次の $L$ を返す。

$$
L = \min\left\{x \in \mathbb{Z} \mathrel{\middle|} 0 \leq x \leq r,\ g(\operatorname{prod}(x, r)) = \mathrm{true}\right\}
$$

すなわち、$[L, r)$ は条件を満たし、$L=0$ でなければ $[L-1, r)$ は条件を満たさない。

##### 制約

- $0 \leq r \leq n$
- `g(AM::e()) == true`
- $x$ を $r$ から $0$ へ減らしたとき、`g(prod(x, r))` は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\log n)$
