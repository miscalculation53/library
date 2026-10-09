## 概要

要素を `Compare` 順の接頭辞と残りに分け、条件を満たす最大の接頭辞を左側に保つ。
中央値、下位・上位 K 個、和が上限以下の接頭辞を同じ構造で管理できる。

条件は関数オブジェクトとして渡す。左右の個数と集約値から判定し、
`set_condition` で K や上限を変更できる。

集約なしが標準。可換群を指定すると、ヒープや `multiset` でも集約を更新できる。
逆元のない可換モノイドには [OrderedMultisetTree](bbst/ordered_multiset.md) など、
同じ集約を持つコンテナを使う。
既定では要素をそのまま集約する。必要な場合は射影で集約用の値を取り出し、その型を `M::S` と揃える。
比較・端の取得・削除には元の要素を使う。

内部操作は [PriorityContainer](priority_container.md) で統一する。
基底は `multiset` や `priority_queue` のようにテンプレート名を 1 つ指定する。
左右には同じ基底の種類・比較順序を使い、ヒープの取り出す端と集約型を内部で設定する。
操作と集約の更新方法はコンパイル時に切り替える。

## 使用例

```cpp
#include "ds/two_multisets.hpp"

TwoMultisets<ll, multiset> median; // 集約なし。multiset は省略しても同じ
for (ll x : {4, 1, 7, 2}) median.insert(x);
assert(median.left().back() == 2);  // 小さい方の中央値
assert(median.right().front() == 4); // 大きい方の中央値
median.erase(1); // 同じ値を 1 個削除

SumTwoMultisets<ll, multiset, PartitionBySize> smallest(PartitionBySize{2});
for (ll x : {4, 1, 7, 2}) smallest.insert(x);
assert(smallest.left().all_prod() == 3); // 下位 2 個の和。型は ll
smallest.set_condition(PartitionBySize{3});
assert(smallest.left().all_prod() == 7);

// 上位 2 個は右側に残す
auto largest = make_two_multisets<int, multiset, GroupAddSub<int>>(
    [](const TwoMultisetsState<int>& s) { return s.right_size >= min(2, s.size()); });
for (int x : {4, 1, 7, 2}) largest.insert(x);
assert(largest.right().all_prod() == 11);

SumTwoMultisets<ll, multiset, PartitionBySum<ll>> budget(PartitionBySum<ll>{6});
for (int x : {4, 1, 7, 2}) budget.insert(x);
assert(budget.left().size() == 2 && budget.left().all_prod() == 3);
```

比較用のキーと集約用の値が異なる場合は、第 3 引数に射影を渡す。
ラムダやメンバへのポインタを指定できる。

```cpp
using Item = pair<int, ll>; // キー、集約する値。キーが同じなら値順で並ぶ。
auto keyed = make_two_multisets<Item, multiset, GroupAddSub<ll>>(
    PartitionBySize{2}, less<Item>{}, &Item::second);
keyed.insert({3, 10}); keyed.insert({1, 20}); keyed.insert({2, 7});
assert(keyed.left().all_prod() == 27);
assert(keyed.left().back() == Item(2, 7));

auto projected = make_two_multisets<Item, priority_queue, GroupAddSub<ll>>(
    PartitionBySize{2}, less<Item>{}, [](const Item& x) { return x.second; });
projected.insert({1, 20});
assert(projected.left().all_prod() == 20);
```

ヒープもテンプレート名だけで指定する。左には最大側、右には最小側を取り出す型を自動で選ぶ。

```cpp
auto heaps = make_two_multisets<ll, priority_queue, GroupAddSub<ll>>(PartitionBySize{3});
heaps.insert(5);
heaps.insert(2);

TwoMultisets<ll, ErasablePriorityQueue> erasable;
erasable.insert(2);
erasable.erase(2);
```

独自条件はラムダでも指定できる。

```cpp
auto cond = [](const TwoMultisetsState<ll>& s) { return s.left_prod <= 100; };
auto items = make_two_multisets<ll, multiset, GroupAddSub<ll>>(cond);
items.insert(10);
items.insert(30);
```

可換モノイドでの集約は、集約を持つ木に任せる。

```cpp
#include "ds/bbst/ordered_multiset.hpp"

using M = MonoidMin<ll>;
TwoMultisets<ll, OrderedMultisetTree, PartitionBySize, M> q(PartitionBySize{3});
q.insert(5);
q.insert(2);
assert(q.left().all_prod() == 2);
```

`LOCAL` では `dump(q)` で左右の要素と、集約を載せた場合の左右の集約値を表示する。
各側の要素は、どの基底でも `Compare` 順に並ぶ。

## 詳細なドキュメント

### TwoMultisets

```cpp
template <class T, template <class...> class Base = multiset,
          class Condition = PartitionByMedian, class M = GroupTrivial,
          class Compare = less<T>, class Projection = Identity>
struct TwoMultisets;
```

`M::S` を `S`、`TwoMultisetsState<S>` を `State` とする。
`State` は `left_size`, `right_size`, `left_prod`, `right_prod` と、合計個数を返す `size()` を持つ。
条件判定にはこの `State` を渡す。

`supports_erase`, `supports_contains` はコンパイル時定数。
対応する操作が左右両方にあるときに `true` になる。

`Base` には `multiset`, `priority_queue`, `ErasablePriorityQueue`,
`DoubleEndedPriorityQueue`, `MonotonePriorityDeque`, `OrderedMultisetTree` を指定できる。
`MinPriorityQueue`, `MaxPriorityQueue`, `MinErasablePriorityQueue`, `MaxErasablePriorityQueue` も指定できる。
ヒープの型名によらず、同じ種類のヒープから左右に必要な端の型を選ぶ。
木を使う場合は `ds/bbst/ordered_multiset.hpp` も include する。

独自の基底は `Base<T, Compare>` の形で要素型と基準の比較順序を受け取る。
両端に対応するものを使う。標準ヒープ・遅延削除ヒープのテンプレート別名も自動で端を選べる。
追加の型引数を固定したい場合はテンプレート別名を使える。

##### 制約

- `Compare` が狭義弱順序を定め、左右のコンテナがその順序に沿って要素を管理する。
- 比較器をコピー構築できる。
- 基底の集約を使わずキャッシュで集約する場合、任意削除では同値な要素の射影値も同じである。キーが重なる場合は、値や ID も比較に含める。存在確認のないコンテナで任意削除するときは、同値な要素を同じ値として削除できる。
- `M` は可換モノイド。`M::inv` がある場合は可換群として扱う。
- 集約を載せる場合、`std::invoke(projection, x)` の戻り値の型（参照・`const` を除いた型）が `M::S` と同じである。
- 射影は `const T&` を受け取り、同じ要素から同じ集約値を返す。射影をコピー構築できる。
- `Condition` は `const State&` を受け取り、真偽値を返す純粋な判定である。
- 同じ全要素を `Compare` 順に並べ、左の個数を増やすとき、条件の真偽が `true` から `false` へ単調に変わる。
- 左が空の状態では条件が `true` になる。
- 選んだ集約型で、集約の演算と中間計算を表現できる。

#### コンストラクタ

```cpp
explicit TwoMultisets(Condition cond = Condition(), Compare comp = Compare(),
                     Projection projection = Projection());
```

空で初期化する。左右を同じ比較器・射影で構築する。
ヒープでは、左の `back` と右の `front` を使える型を選ぶ。
木には集約型・射影も渡す。射影を省略すると [Identity](../utils/identity.md) で要素をそのまま使う。

##### 制約

- 選んだ基底を対応する比較器で構築できる。

##### 計算量

- $O(1)$。

#### insert / erase / try_erase / contains

```cpp
void insert(const T& x);
void erase(const T& x);
bool try_erase(const T& x);
bool contains(const T& x) const;
```

`insert` は 1 個追加する。`erase` は存在する要素を 1 個削除する。
`try_erase` は要素があれば 1 個削除して `true`、存在しなければ `false` を返す。
`contains` は存在を判定する。
追加・削除のあとで、条件を満たす最大の接頭辞へ調整する。

`erase` は任意削除、`contains` は存在確認に左右両方が対応するときに使える。
`try_erase` は両方の機能を必要とする。

##### 制約

- `erase` の値が存在する。
- `MonotonePriorityDeque` を使う場合、調整による移動も含めた各 `push` がその両端へ追加できる。

##### 計算量

コンテナの基本操作を $B$、調整で境界を越える要素数を $m$、条件・集約演算を $O(1)$ として、

- 追加・削除は $O((m+1)B)$。逆元のない集約では境界の判定に定数回の追加移動を行う。
- `contains` は左右の存在確認の計算量。
- 個数条件と中央値条件では、1 回の追加・削除について $m=O(1)$。
- 和などの一般条件では、1 回の更新で多数の要素が移動する場合がある。

`multiset` は $B=O(\log(n+1))$、Treap は期待 $O(\log(n+1))$。
遅延削除ヒープは、内部に保持する要素数を $h$ として償却 $B=O(\log(h+1))$。

#### extract_left / extract_right

```cpp
T extract_left();
T extract_right();
```

左の最後・右の最初を 1 個取り出し、分割を調整する。
通常のヒープでも使える。

##### 制約

- 取り出す側に要素がある。

##### 計算量

- $O((m+1)B)$。記号は `insert` と同じ。

#### condition / set_condition

```cpp
const Condition& condition() const;
void set_condition(Condition cond);
```

現在の条件の参照を取得する。または、条件を置き換えて分割を調整する。
`PartitionBySize{k}` や `PartitionBySum<S>{limit}` を渡すと、K や上限を変更できる。

##### 制約

- `set_condition` は `Condition` を move 構築できる。C++17 のキャプチャ付きラムダにも対応する。
- 新しい条件も単調性と空の接頭辞に関する条件を満たす。

##### 計算量

- 取得は $O(1)$。変更は $O((m+1)B)$。

#### size / empty

```cpp
int size() const;
bool empty() const;
```

全体の要素数・空判定を取得する。

##### 計算量

- $O(1)$。

#### all_prod / left / right

```cpp
S all_prod() const;
const LeftContainer& left() const;
const RightContainer& right() const;
```

全体の集約と、左右の共通インターフェースの読み取り用参照を取得する。
各側の要素数・空判定・集約は `left().size()`・`left().empty()`・`left().all_prod()` などで取得する。
`left().container()` などを通じて、木の順位取得や区間集約も利用できる。

境界は `left().back()` と `right().front()` で取得する。
各側の `front`・`back` は、全体と同じ `Compare` 順の最初・最後を返す。
両端に対応する基底では `left().front()`・`right().back()` も使える。

| 比較器 | `left().back()` | `right().front()` |
|---|---|---|
| `less<T>` | 左側の最大値 | 右側の最小値 |
| `greater<T>` | 左側の最小値 | 右側の最大値 |

一般の比較器では、左の接頭辞の最後と、右の残りの最初を返す。
中央値条件では、奇数個なら `left().back()` が中央値、偶数個なら左右の境界が中央の 2 値になる。
境界の取得には、その側に要素があることが必要。

##### 計算量

- 参照取得は $O(1)$。集約取得・集約演算が $O(1)$ なら、全体の集約の取得も $O(1)$。
- 境界の取得は基底コンテナの端の取得の計算量。

#### clear

```cpp
void clear();
```

全要素を削除する。比較器・射影・条件を維持する。

##### 制約

- 空の全体に対しても条件が `true` になる。

##### 計算量

- 左右の `clear` の計算量。

#### dump

```cpp
dump(q);
dump(q.left(), q.right());
```

`LOCAL` では左右の内容を表示する。`dump(q)` は集約を載せた場合に `left_prod`, `right_prod` も表示する。
空の側も表示できる。内容の取得は読み取りだけを行う。

##### 制約

- 内容の取得に関する条件は [PriorityContainer](priority_container.md) の `content` と同じ。
- 要素型・集約型が `dump` の表示に対応している。

##### 計算量

- 左右の `content` と、取得した値の表示の計算量。

### 標準の条件と集約

#### PartitionByMedian / PartitionBySize / PartitionBySum

```cpp
struct PartitionByMedian;
struct PartitionBySize { int k = 0; };
template <class S> struct PartitionBySum { S limit; };
```

- `PartitionByMedian`：左を全体の半数、奇数個なら切り上げた個数にする。
- `PartitionBySize{k}`：左を先頭の `min(k, size())` 個にする。
- `PartitionBySum<S>{limit}`：左の集約値が `limit` 以下になる最大の接頭辞を選ぶ。

`k`, `limit` は実行時の値として渡せる。入力値で構築でき、`set_condition` で後から変更できる。

##### 制約

- `k >= 0`。
- 和による条件では、和を集約として指定し、各要素の射影値と `limit` が非負である。

##### 計算量

- 判定は $O(1)$。

#### SumTwoMultisets / make_two_multisets

```cpp
template <class T, template <class...> class Base = multiset,
          class Condition = PartitionByMedian, class Compare = less<T>, class Projection = Identity>
using SumTwoMultisets = TwoMultisets<T, Base, Condition,
    GroupAddSub<decay_t<invoke_result_t<const Projection&, const T&>>>, Compare, Projection>;

template <class T, template <class...> class Base = multiset, class M = GroupTrivial,
          class Condition = PartitionByMedian, class Compare = less<T>, class Projection = Identity>
auto make_two_multisets(Condition cond = Condition(), Compare comp = Compare(),
                       Projection projection = Projection());
```

`SumTwoMultisets` は射影の戻り値の型で和を管理する別名。既定の射影では要素と同じ型 `T` になる。
和と演算の中間値がその型に収まることを利用者が保証する。
`make_two_multisets` は条件・比較器・射影の型を引数から推論する。
基底を省略すると `multiset` を使う。たとえば `make_two_multisets<int>(PartitionBySize{3})` で
下位 3 個を管理する構造を作れる。

##### 計算量

- $O(1)$。
