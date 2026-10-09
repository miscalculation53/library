## 概要

重複する要素を比較器の順に管理し、順位の取得・削除と区間のモノイド積を行う Treap。
既定では要素をそのままモノイド `M` の値として格納する。
必要な場合は射影を指定し、要素から集約用の値を取り出す。比較と順位・端の取得には元の要素を使う。
集約なし、和、最小値、積などを指定できる。単独で使う場合は非可換モノイドにも対応する。

順位は 0-indexed、区間は半開区間。
同値性は `!comp(a,b) && !comp(b,a)` で判定する。
同値な要素は追加順に並び、`erase(x)` はその最初を 1 個削除する。

[PriorityContainer](../priority_container.md) と [TwoMultisets](../two_multisets.md) の基底にも使える。
部分木に集約を持つため、逆元のないモノイドでも削除後の集約を取得できる。

比較・射影・モノイド演算・値のコピーを $O(1)$ とする。
所有権と Treap の計算量の前提は [共通実装](treap_base.md) を参照。

## 使用例

```cpp
#include "ds/bbst/ordered_multiset.hpp"
#include "ds/priority_container/common.hpp"
#include "algebra/algebra_basic_ops.hpp"

OrderedMultisetTree<ll, MonoidAdd<ll>> t;
for (int x : {4, 1, 4, 2}) t.push(x);
assert(t.get_by_order(2) == 4);
assert(t.count(4) == 2);
assert(t.prod_by_order(1, 3) == 6);
t.erase(4);
assert(t.all_prod() == 7);
auto right = t.split_by_order(2);
t.join(std::move(right));

PriorityContainer<OrderedMultisetTree<int, MonoidMin<int>>> q;
q.push(5); q.push(2); q.push(7);
assert(q.all_prod() == 2);
q.erase(2);
assert(q.all_prod() == 5);

using Item = pair<int, ll>;
using Tree = OrderedMultisetTree<Item, MonoidAdd<ll>, less<Item>, decltype(&Item::second)>;
Tree values(less<Item>{}, &Item::second);
values.push({3, 10}); values.push({1, 20}); values.push({2, 7});
assert(values.front() == Item(1, 20) && values.prod_by_order(0, 2) == 27);

// 木に設定したモノイド・射影を引き継ぐ
PriorityContainer<Tree> wrapped(std::move(values));
wrapped.push({4, 5});
assert(wrapped.all_prod() == 42);
```

## 詳細なドキュメント

### OrderedMultisetTree

```cpp
template <class T, class M = GroupTrivial, class Compare = less<T>, class Projection = Identity>
struct OrderedMultisetTree;
```

`value_type = T`, `monoid_type = M`, `projection_type = Projection`, `S = M::S` を公開する。

#### コンストラクタ

```cpp
OrderedMultisetTree();
explicit OrderedMultisetTree(const Compare& comp, Projection projection = Projection());
template <class It>
OrderedMultisetTree(It first, It last, const Compare& comp = Compare(),
                    Projection projection = Projection());
```

空、または入力の全要素で初期化する。コピーは所有権のために削除し、move を提供する。
move 後の元の木は空になり、比較器・射影を保つ。

##### 制約

- `M` はモノイド。
- `Compare` は狭義弱順序を定める。
- 集約を載せる場合、`std::invoke(projection, x)` の戻り値の型（参照・`const` を除いた型）が `M::S` と同じである。
- 射影は `const T&` を受け取り、同じ要素から同じ集約値を返す。既定の [Identity](../../utils/identity.md) では要素をそのまま使う。
- 比較器・射影をコピー構築できる。move 代入ではコピー代入もできる。
- モノイド型で集約の演算と中間計算を表現できる。

##### 計算量

- 空の構築は $O(1)$。範囲の構築は期待 $O(n\log(n+1))$。
- move 構築は $O(1)$。move 代入は元の格納要素数を $m$ として $O(m)$。

#### push / insert / erase / erase_by_order

```cpp
void push(const T& x);
void insert(const T& x);
bool erase(const T& x);
void erase_by_order(int k);
```

`push`, `insert` は同じ操作で、同値な要素の最後へ 1 個追加する。
`erase` は同値な要素の最初を 1 個削除できたときに `true` を返す。
`erase_by_order` は順位で指定した要素を削除する。

##### 制約

- 順位は $0\le k<n$。

##### 計算量

- 期待 $O(\log(n+1))$。

#### front / back / pop_front / pop_back / get_by_order

```cpp
T front() const;
T back() const;
void pop_front();
void pop_back();
T get_by_order(int k) const;
```

比較器順の最初・最後の取得と削除、順位 $k$ の要素の取得を行う。

##### 制約

- 両端操作では要素がある。
- 順位は $0\le k<n$。

##### 計算量

- 期待 $O(\log(n+1))$。

#### order_of_key / upper_order_of_key / count / contains

```cpp
int order_of_key(const T& x) const;
int upper_order_of_key(const T& x) const;
int count(const T& x) const;
bool contains(const T& x) const;
```

`order_of_key` は比較器順で `x` より前の個数、`upper_order_of_key` は同値な要素まで含めた個数を返す。
`count` は同値な要素数、`contains` は存在を返す。

##### 計算量

- 期待 $O(\log(n+1))$。

#### all_prod / prod_by_order / prod_by_key

```cpp
S all_prod() const;
S prod_by_order(int l, int r) const;
S prod_by_key(const T& lo, const T& hi) const;
```

全体、順位 `[l,r)`、キー `[lo,hi)` の集約を比較器順に取る。
キー区間には `lo` と同値な要素を含み、`hi` と同値な要素は境界の外に置く。

##### 制約

- $0\le l\le r\le n$。
- 比較器順で `hi` が `lo` 以降にある。

##### 計算量

- `all_prod` は $O(1)$。
- 区間集約は期待 $O(\log(n+1))$。

#### split_by_order / split_by_key / join

```cpp
OrderedMultisetTree split_by_order(int k);
OrderedMultisetTree split_by_key(const T& x);
void join(OrderedMultisetTree&& other);
```

`split_by_order(k)` は順位 `[k,n)` を新しい木として取り出す。
`split_by_key(x)` は `x` と同値な要素以降を取り出す。
`join` は `other` を末尾へ移し、`other` を空にする。同値な境界同士の結合にも対応する。

##### 制約

- $0\le k\le n$。
- `join` では比較器順で `other` の最初が自身の最後以降にある。
- `other` は自身と別の木で、同じ比較順序・射影を使用する。

##### 計算量

- 期待 $O(\log(n+1))$。

#### size / empty / projection / clear / content

```cpp
int size() const;
bool empty() const;
const Projection& projection() const;
void clear();
vector<T> content() const;
```

個数・空判定・射影の参照の取得、全要素の削除、比較器順の全要素の取得を行う。
`clear` は比較器・射影を維持する。

##### 計算量

- `size`, `empty`, `projection` は $O(1)$。`clear`, `content` は $O(n)$。
