## 概要

挿入・削除・反転・区間移動・分割・連結とモノイド積を扱う列。
要素の順序は列中の位置で決まり、区間は 0-indexed の半開区間で指定する。
非可換モノイドに対応し、`prod(l, r)` は左から右へ積を取る。

`M` は `S`, `op`, `e` を持つモノイドとする。
各要素は `M::S` 型で、順位は格納した要素の個数で数える。
各基本操作は Treap による期待 $O(\log(n+1))$。演算・値のコピーを $O(1)$ とする。
所有権と計算量の前提は [共通実装](treap_base.md) を参照。

## 使用例

```cpp
#include "ds/bbst/sequence.hpp"

SequenceTree<MonoidAdd<ll>> t({1, 2, 3, 4});
t.reverse(1, 4);                 // [1, 4, 3, 2]
t.insert(2, 10);                 // [1, 4, 10, 3, 2]
assert(t.prod(1, 4) == 17);
auto right = t.split(3);         // t=[1,4,10], right=[3,2]
t.concat(std::move(right));
t.move(1, 3, 3);                 // [1,3,2,4,10]
assert(t.content() == vector<ll>({1, 3, 2, 4, 10}));
```

## 詳細なドキュメント

### SequenceTree

```cpp
template <class M>
using SequenceTree = /* 内部の通常版 */;
```

#### コンストラクタ

```cpp
SequenceTree<M>();
SequenceTree<M>(initializer_list<S> v);
template <class T> SequenceTree<M>(const vector<T>& v);
template <class Iter> SequenceTree<M>(Iter first, Iter last);
```

空の列、または与えた順序の列を構築する。入力の各要素から `S` を構築できること。

##### 計算量

- 空の列は $O(1)$、$n$ 要素の列は $O(n)$。

#### size / empty / clear / content

```cpp
int size() const;
bool empty() const;
void clear();
vector<S> content() const;
```

要素数、空判定、全要素の削除、現在の列の取得。

##### 計算量

- `size`, `empty` は $O(1)$。
- `clear`, `content` は $O(n)$。

#### get / set / insert / erase

```cpp
S get(int p) const;
void set(int p, const S& x);
void insert(int p, const S& x);
void erase(int p);
void erase(int l, int r);
```

位置 `p` の値を取得・変更・削除する。`insert` は位置 `p` の直前に挿入し、`p == size()` なら末尾へ追加する。
区間版 `erase` は `[l,r)` のノードを解放する。

##### 制約

- `get`, `set`, 単一要素の `erase` は $0\le p<n$。
- `insert` は $0\le p\le n$。
- 区間版 `erase` は $0\le l\le r\le n$。

##### 計算量

- 単一要素の操作は期待 $O(\log(n+1))$。
- 区間版 `erase` は期待 $O(\log(n+1)+r-l)$。

#### prod / all_prod

```cpp
S prod(int l, int r) const;
S all_prod() const;
```

`[l,r)` または列全体の積を返す。空区間の積は `M::e()`。

##### 制約

- $0\le l\le r\le n$。

##### 計算量

- `prod` は期待 $O(\log(n+1))$、`all_prod` は $O(1)$。

#### split / concat / extract

```cpp
SequenceTree<M> split(int k);
void concat(SequenceTree<M>&& other);
SequenceTree<M> extract(int l, int r);
```

`split(k)` は先頭 $k$ 要素を自身に残し、後半を返す。
`concat` は `other` の列を自身の末尾につなぎ、`other` を空にする。
`extract` は `[l,r)` を切り出して返し、自身には残りの列を順序を保って残す。

##### 制約

- $0\le k\le n$、$0\le l\le r\le n$。
- `concat` の引数は自身と異なる木。

##### 計算量

- `split`, `extract` は期待 $O(\log(n+1))$。
- `concat` は相手の長さを $m$ として期待 $O(\log(n+m+1))$。

#### reverse / move / rotate

```cpp
void reverse(int l, int r);
void move(int l, int r, int k);
void rotate(int l, int m, int r);
```

`reverse` は `[l,r)` の要素の順序を反転する。
`move` は `[l,r)` を取り除いた列の位置 `k` の直前に、その区間を挿入する。
`rotate` は `[l,m)` と `[m,r)` の順を入れ替える。

##### 制約

- $0\le l\le r\le n$。
- `move` は $0\le k\le n-(r-l)$。
- `rotate` は $l\le m\le r$。

##### 計算量

- 期待 $O(\log(n+1))$。

#### max_right_ok / min_left_ok

```cpp
template <class G> int max_right_ok(int l, const G& g) const;
template <class G> int min_left_ok(int r, const G& g) const;
```

`max_right_ok` は `g(prod(l,R))` を満たす最大の $R$、
`min_left_ok` は `g(prod(L,r))` を満たす最小の $L$ を返す。

##### 制約

- $0\le l,r\le n$、`g(M::e()) == true`。
- 対象区間を伸ばしたとき、判定は `true` から `false` へ高々1回変わる。

##### 計算量

- 期待 $O(\log(n+1))$。`g` の実行を $O(1)$ とする。
