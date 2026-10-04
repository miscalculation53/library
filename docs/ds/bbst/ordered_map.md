## 概要

キー順に要素を管理し、順位区間・キー区間のモノイド積を取れる map。
`M::S` 型の値をキーごとに1つ格納する。積の順序は `Compare` が定めるキー順。
非可換モノイドに対応する。

順位は 0-indexed、区間は半開区間。キーの同値性は
`!cmp(a,b) && !cmp(b,a)` で判定する。
同じ主キーに複数の値を持たせる場合は `(key,id)` などをキーにする。

基本操作は Treap による期待 $O(\log(n+1))$。比較・モノイド演算・値のコピーを $O(1)$ とする。
所有権と計算量の前提は [共通実装](treap_base.md) を参照。

## 使用例

```cpp
#include "ds/bbst/ordered_map.hpp"

OrderedMapTree<int, MonoidAdd<ll>> t;
t.insert(30, 3);
t.insert(10, 1);
t.insert(20, 2);
assert(t.prod_by_order(1, 3) == 5); // キー 20,30 の値の積
assert(t.prod_by_key(10, 30) == 3); // キー 10,20 の値の積
t.erase(10);
assert(t.get_by_order(0).first == 20);
auto right = t.split_by_key(30);
t.join(std::move(right));
```

## 詳細なドキュメント

### OrderedMapTree

```cpp
template <class Key, class M, class Compare = less<Key>>
using OrderedMapTree = /* 内部の通常版 */;
```

#### コンストラクタ

```cpp
OrderedMapTree<Key,M,Compare>();
OrderedMapTree<Key,M,Compare>(const Compare& cmp);
template <class T>
OrderedMapTree<Key,M,Compare>(const vector<pair<Key,T>>& v, const Compare& cmp = Compare());
template <class Iter>
OrderedMapTree<Key,M,Compare>(Iter first, Iter last, const Compare& cmp = Compare());
```

空の map、または入力の `(key,value)` を順に挿入した map を構築する。
同値なキーが複数ある場合は最初の値を採用する。

##### 制約

- `Compare` が狭義弱順序を定める。
- `Compare` をコピー構築できる。木の move 代入を使う場合は比較器もコピー代入できる。
- 入力の値から `S` を構築できる。

##### 計算量

- 空の場合は $O(1)$、入力が $n$ 個なら期待 $O(n\log(n+1))$。

#### size / empty / clear / content

```cpp
int size() const;
bool empty() const;
void clear();
vector<pair<Key,S>> content() const;
```

要素数、空判定、全要素の削除、キー順の全要素の取得。

##### 計算量

- `size`, `empty` は $O(1)$。`clear`, `content` は $O(n)$。

#### contains / get / get_by_order

```cpp
bool contains(const Key& key) const;
S get(const Key& key) const;
pair<Key,S> get_by_order(int k) const;
```

キーの存在、キーに対応する値、順位 $k$ のキーと値を取得する。
取得する値はコピー。変更には `set` 系の関数を使う。

##### 制約

- `get` のキーが存在する。
- `get_by_order` は $0\le k<n$。

##### 計算量

- 期待 $O(\log(n+1))$。

#### order_of_key / upper_order_of_key

```cpp
int order_of_key(const Key& key) const;
int upper_order_of_key(const Key& key) const;
```

`order_of_key` はキー順で `key` より前の要素数、`upper_order_of_key` は同値な要素まで含めた個数。
`less<Key>` ならそれぞれ `key` 未満・以下の個数になる。

##### 計算量

- 期待 $O(\log(n+1))$。

#### insert / erase / erase_by_order / set / set_by_order

```cpp
bool insert(const Key& key, const S& value);
bool erase(const Key& key);
void erase_by_order(int k);
void set(const Key& key, const S& value);
void set_by_order(int k, const S& value);
```

`insert` は新しいキーを追加できたとき `true`、既存のキーならその値を保ち `false` を返す。
`erase` は実際に要素を削除したとき `true` を返す。
`set` 系は登録済みの値を変更し、キー順を保つ。

##### 制約

- `set` のキーが存在する。
- 順位指定は $0\le k<n$。

##### 計算量

- 期待 $O(\log(n+1))$。

#### prod_by_order / prod_by_key / all_prod

```cpp
S prod_by_order(int l, int r) const;
S prod_by_key(const Key& lo, const Key& hi) const;
S all_prod() const;
```

順位 `[l,r)`、キー区間 `[lo,hi)`、全体の積を返す。空区間の積は `M::e()`。
キー区間は `!cmp(key,lo) && cmp(key,hi)` を満たす要素からなる。

##### 制約

- $0\le l\le r\le n$。
- `!cmp(hi,lo)`。

##### 計算量

- 区間積は期待 $O(\log(n+1))$。`all_prod` は $O(1)$。

#### split_by_order / split_by_key / join

```cpp
OrderedMapTree split_by_order(int k);
OrderedMapTree split_by_key(const Key& key);
void join(OrderedMapTree&& other);
```

`split_by_order` は先頭 $k$ 要素を自身に残し、後半を返す。
`split_by_key` は `key` より前の要素を自身に残し、残りを返す。
返した木は比較器も引き継ぐ。
`join` は自身の後ろに `other` をつなぎ、`other` を空にする。

##### 制約

- $0\le k\le n$。
- `join` の引数は自身と異なる木で、両方の比較器が同じ順序を定める。
- `join` の両方の木が非空なら、自身の最後のキーが相手の最初のキーより前にある。

##### 計算量

- 分割は期待 $O(\log(n+1))$。
- `join` は相手のサイズを $m$ として期待 $O(\log(n+m+1))$。

#### max_right_ok / min_left_ok

```cpp
template <class G> int max_right_ok(int l, const G& g) const;
template <class G> int min_left_ok(int r, const G& g) const;
```

順位区間の積を使い、`g(prod_by_order(l,R))` を満たす最大の $R$、
`g(prod_by_order(L,r))` を満たす最小の $L$ を返す。

##### 制約

- $0\le l,r\le n$、`g(M::e()) == true`。
- 対象区間を伸ばしたとき、判定は `true` から `false` へ高々1回変わる。

##### 計算量

- 期待 $O(\log(n+1))$。`g` の実行を $O(1)$ とする。

### 対応する問題

- [Ordered Set](https://judge.yosupo.jp/problem/ordered_set)：キーの挿入・削除・順位と前後の要素。
- [Point Set Range Composite](https://judge.yosupo.jp/problem/point_set_range_composite)：固定キーを用いた、値の変更と非可換な順位区間積。
