このドキュメントには AI による作成・変更が含まれている可能性があります。

## 概要

長さ $n$ の一様な列に対する、一点変更、区間作用、区間積、セグメント木上の二分探索をオンラインで処理する疎遅延セグメント木。添字と区間長は `ll` 型で扱う。

一様な区間をまとめて保持し、更新の左右端で必要な区間だけを分割する。各区間の左端をキーとして、[疎セグメント木](sparse_segtree.md)と同じ配置で管理するため、木の深さは $O(\log n)$ になる。座標を事前に集める必要はない。

更新 1 回につき高々 $2$ 個のノードを追加する。$q$ 回の更新後のノード数は高々 $2q+1$ 個で、ノードの保存に使うメモリは $O(q)$。`get`, `prod`, `content` および二分探索は `const` であり、ノードを追加しない。

`initial_value` は各点の初期値として明示する。`S` が区間長を持つ場合、値 $0$ の一点を表す `S(0)` を渡すと長さ $n$ の値 $0$ の列になる。`AM::e()` を渡すと、各点が長さ $0$ の空の要素になる。

一様な区間の積は一点の値を区間長だけ `op` で繰り返して求める。`AM::pow(x, k)` があればそれを使い、通常のモノイド累乗より速く計算できる。たとえば `ActedMonoidAffineSum<T, L>` の `pow` は $O(1)$。

## 使用例

```cpp
using mint = modint998244353;
using AM = ActedMonoidAffineSum<mint>;

constexpr ll n = 1'000'000'000'000'000'000LL;
SparseLazySegmentTree<AM> seg(n, AM::S(0));
seg.apply(10, 20, AM::F(2, 3));
assert(seg.prod(0, n).val == 30);
for (auto [l, r, x] : seg.content())
  cout << l << ' ' << r << ' ' << x.val << '\n';
```

## 詳細なドキュメント

### SparseLazySegmentTree

#### コンストラクタ

```cpp
SparseLazySegmentTree(ll n, const S& initial_value)
```

長さ $n$ の列を作る。各点の初期値は `initial_value` とする。

`S` が区間長を持ち、長さ $n$ の通常の列として扱う場合は、`initial_value` の長さを $1$ とする。区間長や集約値、作用の計算結果を保持できる型を選ぶ。`MonoidLenSum` などの長さ型 `L` は、既定値が `ll` であり、各モノイドのテンプレート引数で選べる。

##### 制約

- $0 \leq n$

##### 計算量

- $O(\log n)$。`AM::pow` がある場合はその計算量

#### reserve

```cpp
void reserve(int n)
```

少なくとも `n` ノード分の領域をあらかじめ確保する。更新回数が $q$ 回なら、`reserve(2 * q + 1)` で全更新に必要な領域を確保できる。

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

#### content

```cpp
vc<tuple<ll, ll, S>> content() const
```

保持している各区間を `(l, r, x)` として、左端の昇順で返す。`x` は区間 $[l,r)$ の各点の現在値であり、遅延更新も反映する。`S` が長さを持つ場合、`x` の長さは一点分となる。

区間は $[0,n)$ 全体を覆う。更新で作られた区切りを維持し、隣接する区間の値が同じでも別々に返す。$n=0$ なら空の列を返す。

##### 計算量

- $O(V)$。$V$ は保持している区間数

#### max_right_ok

```cpp
template <class G>
ll max_right_ok(ll l, const G& g) const
```

次の $R$ を返す。

$$
R = \max\left\lbrace x \in \mathbb{Z} \middle\vert l \leq x \leq n,\ g(\operatorname{prod}(l, x)) = \mathrm{true}\right\rbrace
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
L = \min\left\lbrace x \in \mathbb{Z} \middle\vert 0 \leq x \leq r,\ g(\operatorname{prod}(x, r)) = \mathrm{true}\right\rbrace
$$

すなわち、$[L, r)$ は条件を満たし、$L=0$ でなければ $[L-1, r)$ は条件を満たさない。

##### 制約

- $0 \leq r \leq n$
- `g(AM::e()) == true`
- $x$ を $r$ から $0$ へ減らしたとき、`g(prod(x, r))` は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\log n)$
