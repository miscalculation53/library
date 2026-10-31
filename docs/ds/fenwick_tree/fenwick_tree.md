このドキュメントには AI による作成・変更が含まれています。

## 概要

可換群に対して一点加算・区間取得ができるデータ構造。（取得が prefix だけでよいなら群でなくモノイドでもよい）

できることはセグ木の下位互換だが、定数倍がよく、実装が軽い。

## 使用例

```cpp
#include "ds/fenwick_tree/fenwick_tree.hpp"

FenwickTree<GroupAddSub<ll>> fw(vc<ll>{2, 5, 3});
fw.add(1, 4);
assert(fw.sum(2) == 11);    // [0, 2)
assert(fw.sum(1, 3) == 12); // [1, 3)
fw.set(0, 7);
assert(fw.get(0) == 7);
```

## 詳細なドキュメント

### FenwickTree

#### コンストラクタ

```cpp
(1) FenwickTree<G>(int n)
(2) FenwickTree<G>(vc<G::S> v)
```

- (1) 長さ $n$、全要素単位元で初期化する。
- (2) vector $v$ で初期化する。

##### 制約

- `G` は**可換**
  - 群であることが多いが、モノイドでも一部機能は OK

##### 計算量

- (1)：$O(n)$
- (2)：$O(n)$

#### size

```cpp
I size<I=ll>()
```

$n$ を返す。

##### 計算量

- $O(1)$

#### sum

```cpp
(1) G::S sum(int r)
(2) G::S sum(int l, int r)
```

- (1)：$[0, r)$ の和を返す。モノイドでよい。
- (2)：$[l, r)$ の和を返す。群であることが必要。

区間和は、左右の prefix に共通する節点までで走査を打ち切る。

##### 制約

- (1)：$0 \leq r \leq n$
- (2)：$0 \leq l \leq r \leq n$

##### 計算量

- $O(\log n)$

#### get

```cpp
G::S get(int i)
```

$i$ 番目の値を返す。群であることが必要。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(\log n)$

#### add

```cpp
void add(int i, G::S x)
```

$i$ 番目の要素に $x$ を加算する。モノイドでよい。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(\log n)$

#### set

```cpp
void set(int i, G::S x)
```

$i$ 番目の要素を $x$ にする。群であることが必要。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(\log n)$

#### leq_max, lt_max, gt_min, geq_min

```cpp
(1) I leq_max<I=ll>(G::S w)
(2) I lt_max<I=ll>(G::S w)
(3) I gt_min<I=ll>(G::S w)
(4) I geq_min<I=ll>(G::S w)
```

`G` が整数の通常の足し算で、要素が非負であるとする。このとき、$[0, r)$ の要素の和に関して二分探索する：

- (1)：$\mathrm{sum}[0, r) \leq w$ となる $r$ の最大値（左番兵 `-1`）を返す。
- (2)：$\mathrm{sum}[0, r) \lt w$ となる $r$ の最大値（左番兵 `-1`）を返す。
- (3)：$\mathrm{sum}[0, r) \gt w$ となる $r$ の最小値（右番兵 `n+1`）を返す。
- (4)：$\mathrm{sum}[0, r) \geq w$ となる $r$ の最小値（右番兵 `n+1`）を返す。

有効な接頭辞長は $0 \leq r \leq n$。左側の検索には `-1` を表せる符号付き整数型 `I` を使う。

##### 計算量

- $O(\log n)$

#### lt_max_id_sum

```cpp
template <class I = ll>
optional<pair<I, S>> lt_max_id_sum(S w) const
```

`sum(r) < w` を満たす最大の接頭辞長と、その累積和を返す。該当する接頭辞がなければ `nullopt`。長さ $0$ の接頭辞も値ありとして返す。

##### 制約

- 要素は非負整数で、`G` は通常の加算を表す。

##### 計算量

- $O(\log n)$

#### content

```cpp
vc<S> content() const
```

全要素を添字順に並べた配列のコピーを返す。線形構築の逆操作で各要素を復元する。群であることが必要。

##### 計算量

- $O(n)$

---

Fenwick Tree は $[0, n)$ の要素からなる多重集合を管理するのにも使える： `G` は整数の通常の足し算にして、要素 $i$ を $c$ 個追加（削除）するときに $i$ 番目の要素に $c$ を加算（減算）すればよい。（クエリ先読み + 座標圧縮と組み合わせることもしばしば）

以下は、この使い方をしたとき用のメソッドである。

（`sum` を使うと「$x$ 以上の値の個数」などがわかるがこれは別のメソッドにはしていない。）

#### size_of_multiset

```cpp
S size_of_multiset()
```

多重集合のサイズを返す。`sum(n)` に一致。

##### 計算量

- $O(\log n)$

#### order_in_multiset

```cpp
pair<I, I> order_in_multiset<I=ll>(int x)
```

$x$ が $[l, r)$ 番目であるとして $l, r$ を返す。$l$ は `sum(x)` に、$r$ は `sum(x + 1)` に一致。

##### 制約

- $0 \leq x \lt n$

##### 計算量

- $O(\log n)$

#### kth_in_multiset

```cpp
optional<I> kth_in_multiset<I=ll>(G::S k)
```

「集合の $k$ 番目に小さい要素」を返す。範囲外の `k` は `nullopt` を返す。

有効な `k` では `leq_max` と一致する。

##### 計算量

- $O(\log n)$

#### lt_max_in_multiset など

```cpp
(1) I lt_max_in_multiset<I=ll>(T x)
(2) I leq_max_in_multiset<I=ll>(T x)
(3) I geq_min_in_multiset<I=ll>(T x)
(4) I gt_min_in_multiset<I=ll>(T x)
```

- (1)：$x$ 未満で最大の要素が**小さい方から何番目か**を返す。左番兵は `-1`。
- (2)：$x$ 以下で最大の要素が**小さい方から何番目か**を返す。左番兵は `-1`。
- (3)：$x$ 以上で最小の要素が**小さい方から何番目か**を返す。右番兵は `size_of_multiset()`。
- (4)：$x$ 超過で最小の要素が**小さい方から何番目か**を返す。右番兵は `size_of_multiset()`。

左側の検索には `-1` を表せる符号付き整数型 `I` を使う。順位は $0$ 始まり。

##### 計算量

- $O(\log n)$
