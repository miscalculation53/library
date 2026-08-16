## 概要

$n$ が $10^9$ のように大きい場合に、更新された点に必要なノードだけを作るセグメント木。オンラインの一点更新と区間積を処理できる。オフラインで更新位置が分かる場合は、座標圧縮して通常のセグメント木を使う方が高速である。

各ノードに区間積だけでなく、その区間内の一点 `(p, value)` を持たせることで、一点更新ごとに増えるノード数を高々 $1$ 個にしている。ノードの積は、左の子の積、`p` の値、右の子の積の順にまとめる。

二分探索では、条件を満たす部分木を積ごと飛ばし、初めて条件を満たさなくなる部分木だけを降りる。更新点の間に大きな空白があっても $O(\log n)$ で動作する。

## 使用例

```cpp
SparseSegmentTree<MonoidAdd<ll>> seg(1LL << 40);
seg.reserve(1 << 20);
seg.set(10, 2);
seg.set(100, 3);

auto g = [](ll sum) { return sum <= 2; };
assert(seg.max_right_ok(0, g) == 100);
```

## 詳細なドキュメント

### SparseSegmentTree

#### コンストラクタ

```cpp
SparseSegmentTree(ll n)
```

長さ $n$ の列を作る。未登録の点の値は `M::e()` とする。

##### 制約

- $0 \leq n$

##### 計算量

- $O(1)$

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
template <class F>
void modify(ll p, const F& f)
```

`p` 番目の値への参照を `f` に渡して変更する。`p` がまだ更新されていない場合は、初期値を `M::e()` として新しく登録する。

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

## 参考

- https://lorent-kyopro.hatenablog.com/entry/2021/03/12/025644
- https://kazuma8128.hatenablog.com/entry/2018/11/29/093827

実装にポインタを使う場合との速度差は未検証。
