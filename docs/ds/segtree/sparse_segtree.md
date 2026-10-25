このドキュメントには AI による作成・変更が含まれています。

## 概要

$n$ が $10^9$ のように大きい場合に、更新された点に必要なノードだけを作るセグメント木。各点の初期値を明示し、オンラインの一点更新、区間積、セグメント木上の二分探索を処理できる。添字と区間長は `ll` 型で扱う。

初期値が単位元と確認できる場合は、更新済みの座標を AVL 木で保持する。相異なる登録点数を $K$ とすると、一点更新・点取得・区間積・二分探索は $O(\log K)$、全体の積は $O(1)$。部分木全体の積と、根の値を含む左右それぞれの積を保持し、添字の順に結合する。登録点が単位元になった場合も登録状態を保持する。

単位元の確認には、任意の `M::is_identity(x)` または利用可能な値の比較演算を使う。`M::is_identity(x)` は、`x` が単位元と等しいときに `true` を返すものとする。`GroupAffineFunction`・`MonoidAffineFunction` とその `OppositeMonoid`・`OppositeGroup` では、係数型の比較演算から確認する。確認できる操作がある場合だけ、この経路を構築時に選ぶ。

一般の初期値では、各ノードに区間 $[l,r)$、左端 $l$ の値、$[l+1,r)$ の初期値の積を保持する。更新位置で区間を分割するため、一点更新ごとに増えるノード数は高々 $1$ 個になる。$q$ 回の更新後のノード数は高々 $q+1$ 個で、ノードの保存に使うメモリは $O(q)$。`get`, `prod`, `all_prod`, `content` および二分探索は `const` であり、ノードを追加せずに処理する。

`initial_value` は各点の初期値として明示する。`S` が区間長を持つ場合、値 $0$ の一点を表す `S(0)` を渡すと長さ $n$ の値 $0$ の列になる。`M::e()` を渡すと、各点が長さ $0$ の空の要素になる。

一般の初期値による一様な区間の積は、初期値を区間長だけ `op` で繰り返して求める。`M::pow(x, k)` は任意で、定義されていればそれを使い、それ以外は二乗法で計算する。累乗を求めるのは区間の分割時や問い合わせの端の部分区間に対して定数回で、祖先の更新では保持済みの積を結合する。以下の計算量は `op` が $O(1)$ で、累乗を二乗法かそれ以上に高速な `M::pow` で求める場合を示す。更新・区間積・二分探索は $O(\log n)$。

二分探索では、条件を満たす部分木を積ごと飛ばし、初めて条件を満たさなくなる部分木だけを降りる。未更新部分で条件が変わる場合は、初期値の累乗を使ってその中の境界を求める。

オフラインで更新位置が分かる場合は、座標圧縮して通常のセグメント木を使う方法もある。初期値が単位元以外の場合は、圧縮でまとめた未更新部分の寄与を区間長に応じて扱う。

複数の木を共有ノードプールで管理し、破壊的に併合する用途には [MeldableSparseSegmentTreePool](meldable_sparse_segtree.md) を使える。こちらは1回の更新で高々 $O(\log n)$ 個のノードを作り、対応する区間同士を直接併合する。

## 使用例

```cpp
SparseSegmentTree<MonoidAdd<ll>> seg(1LL << 40, 1);
seg.reserve(1 << 20);
seg.set(10, 2);
seg.set(100, 3);

assert(seg.get(20) == 1);
assert(seg.prod(0, 20) == 21);
auto g = [](ll sum) { return sum <= 15; };
assert(seg.max_right_ok(0, g) == 14);
```

## 詳細なドキュメント

### SparseSegmentTree

#### コンストラクタ

```cpp
SparseSegmentTree(ll n, const S& initial_value)
```

長さ $n$ の列を作る。各点の初期値は `initial_value` とする。

`S` が区間長を持ち、長さ $n$ の通常の列として扱う場合は、`initial_value` の長さを $1$ とする。区間長や集約値の計算結果を保持できる型を選ぶ。

##### 制約

- $0 \leq n$

##### 計算量

- 単位元を確認できる初期値は $O(1)$
- 一般の初期値は $O(\log n)$。`M::pow` がある場合はその計算量

#### reserve

```cpp
void reserve(int n)
```

少なくとも `n` ノード分の領域をあらかじめ確保する。更新回数が $q$ 回なら、`reserve(q + 1)` で全更新に必要な領域を確保できる。

##### 制約

- $0 \leq n$

##### 計算量

- 最悪 $O(V)$。$V$ は現在のノード数

#### modify

```cpp
template <class F>
void modify(ll p, const F& f)
```

`p` 番目の値への参照を `f` に渡して変更する。`p` がまだ更新されていない場合は、`initial_value` から始める。`f` は現在の値に対してちょうど $1$ 回呼ばれる。

```cpp
seg.modify(p, [&](S& x)
{
  x = M::op(x, value);
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

##### 計算量

- $O(\log n)$

#### get

```cpp
S get(ll p) const
```

`p` 番目の値を返す。未更新の点では `initial_value` を返す。

##### 制約

- $0 \leq p < n$

##### 計算量

- $O(\log n)$

#### prod

```cpp
S prod(ll l, ll r) const
```

$[l, r)$ の積を添字順に求める。未更新の点も含めて集約し、$l=r$ の場合は `M::e()` を返す。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- $O(\log n)$

#### all_prod

```cpp
S all_prod() const
```

全体の積を返す。$n=0$ の場合は `M::e()` を返す。

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

すなわち、$[l, R)$ は条件を満たし、$R=n$ でなければ $[l, R+1)$ は条件を満たさない。返す境界は更新済みの点とは限らない。

##### 制約

- $0 \leq l \leq n$
- `g(M::e()) == true`
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

すなわち、$[L, r)$ は条件を満たし、$L=0$ でなければ $[L-1, r)$ は条件を満たさない。返す境界は更新済みの点とは限らない。

##### 制約

- $0 \leq r \leq n$
- `g(M::e()) == true`
- $x$ を $r$ から $0$ へ減らしたとき、`g(prod(x, r))` は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\log n)$

#### content

```cpp
map<ll, S> content() const
```

更新で登録された添字と現在の値を返す。デバッグ用。初期値と同じ値に更新した点も含む。

##### 計算量

- 単位元を確認できる初期値は $O(K)$。$K$ は登録された点の数
- 一般の初期値は $O(K\log K)$

## 参考

- https://lorent-kyopro.hatenablog.com/entry/2021/03/12/025644
- https://kazuma8128.hatenablog.com/entry/2018/11/29/093827

実装にポインタを使う場合との速度差は未検証。
