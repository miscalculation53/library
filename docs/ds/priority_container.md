## 概要

基底コンテナを新しい型 `PriorityContainer<Container>` で包み、`push`, `front`, `back`, `pop_front`, `pop_back` の共通インターフェースで扱う。
基底の操作への変換を内部で行い、たとえば `multiset` の `insert` を `push` に対応させる。
片端のヒープは対応する端の操作を公開する。任意削除・存在確認も基底の機能に応じて使える。
`TwoMultisets` や、両側のヒープを使うアルゴリズムの部品になる。

| 基底コンテナ | `front` | `back` | 任意削除 | 存在確認 |
|---|---|---|---|---|
| `multiset<T, Compare>` | ○ | ○ | ○ | ○ |
| `MinPriorityQueue<T, Compare, Sequence>` | ○ | | | |
| `MaxPriorityQueue<T, Compare, Sequence>` | | ○ | | |
| `MinErasablePriorityQueue<T, Compare>` | ○ | | ○ | |
| `MaxErasablePriorityQueue<T, Compare>` | | ○ | ○ | |
| `priority_queue<T, Sequence, Compare>` | | ○ | | |
| `ErasablePriorityQueue<T, Compare>` | ○ | | ○ | |
| `DoubleEndedPriorityQueue<T, Compare>` | ○ | ○ | ○ | |
| `OrderedMultisetTree<T, M, Compare, Projection>` | ○ | ○ | ○ | ○ |
| `MonotonePriorityDeque<T, Compare>` | ○ | ○ | | |

`front` は `Compare` 順の最初、`back` は最後を返す。
最小側・最大側のヒープには、同じ `Compare = less<T>` を渡す。
取り出せる端は型で決まり、比較の反転はヒープの内部で行う。
`with_compare` では、基準の比較器をそのまま渡して構築する。

集約は可換モノイド `M` で指定する。既定では要素をそのまま集約し、
必要な場合は `Projection` で集約用の値を取り出す。比較と端の取得は元の要素に対して行う。
射影の戻り値の型（参照・`const` を除いた型）は `M::S` と揃える。
基底コンテナが同じモノイド・射影型で集約を持つ場合は、その集約と射影を利用する。
それ以外は `M::inv` を使って追加・削除の寄与を更新する。
この選択はコンパイル時に行う。逆元の有無は [HasInverse](../algebra/algebra_base.md#hasinverse) で判定する。
既定の [GroupTrivial](../algebra/trivial.md) は集約の計算を省く。

独自コンテナは `value_type`, `push`, `size`, `empty` と、
`front`・`pop_front` または `back`・`pop_back` を持たせると利用できる。
各端の操作、`erase`, `contains`, `clear` はあれば検出する。
`erase(x)` の意味は、同じ値を 1 個削除する操作に揃える。

使用する基底に応じて、次のヘッダを読み込む。

| `ds/priority_container/` 以下のヘッダ | 内容 |
|---|---|
| `common.hpp` | ラッパー、共通操作の検出と呼び出し |
| `aggregate.hpp` | 集約方式の検出とキャッシュ |
| `multiset.hpp` | `multiset` の操作名・1 個削除への対応 |
| `priority_queue.hpp` | 最小・最大ヒープの型と `priority_queue` のアダプタ |
| `erasable_priority_queue.hpp` | 任意削除できる最小・最大ヒープの型とアダプタ |
| `double_ended_priority_queue.hpp` | 両端ヒープの操作名への対応 |
| `monotone_deque.hpp` | [MonotonePriorityDeque](priority_container/monotone_deque.md) の実装 |
| `all.hpp` | 上記をまとめて読み込み |

まとめて読み込む場合は `ds/priority_container/all.hpp` を include する。
共通操作を備える `OrderedMultisetTree` や独自コンテナは、そのヘッダと `common.hpp` を読み込めば使える。

`aggregate.hpp` は、次の内部補助型を定義する。

| 補助型 | 役割 |
|---|---|
| `NativeMonoid<C>` | `M` を省略したときの既定型。`C::monoid_type` があればそれを使い、それ以外は `GroupTrivial` にする。 |
| `NativeProjection<C>` | `C::projection_type` と `projection()` があればその射影を引き継ぎ、それ以外は [Identity](../utils/identity.md) を使う。 |
| `HasNativeAggregate<C, M, Projection>` | 基底のモノイド・射影型が指定と一致し、`all_prod()` を呼べるかを判定する。射影型の省略時は `NativeProjection<C>::type` を使う。 |
| `AggregateCache<M, Native>` | 基底に集約を任せる場合と `GroupTrivial` では空の型。それ以外では、`M::e()` で初期化した集約値 `product` を保持する。 |

たとえば `multiset<ll>` に和を載せる場合は、キャッシュを持ち、追加で `sum += x`、削除で `sum -= x` に相当する更新を行う。
集約を持つ木では、その木の `all_prod()` を使う。集約なしでは `GroupTrivial` を使う。
追加・削除に伴うキャッシュの更新は `common.hpp` で行う。

## 使用例

```cpp
#include "ds/priority_container/multiset.hpp"
#include "ds/priority_container/priority_queue.hpp"
#include "ds/priority_container/double_ended_priority_queue.hpp"
#include "algebra/algebra_basic_ops.hpp"

PriorityContainer<multiset<ll>, GroupAddSub<ll>> q;
q.push(3); q.push(1); q.push(3);
assert(q.front() == 1 && q.back() == 3 && q.all_prod() == 7);
q.erase(3); // 1 個削除
assert(q.all_prod() == 4);
assert(!q.try_erase(100));

PriorityContainer<MinPriorityQueue<int>> heap;
heap.push(4); heap.push(2);
assert(heap.extract_front() == 2);

PriorityContainer<MaxPriorityQueue<int>> max_heap;
max_heap.push(4); max_heap.push(2);
assert(max_heap.extract_back() == 4);

PriorityContainer<DoubleEndedPriorityQueue<int>> ends;
ends.push(2); ends.push(7); ends.push(4);
assert(ends.extract_front() == 2);
assert(ends.extract_back() == 7);

// キー順に並べ、値側を集約する。メンバへのポインタも指定できる。
using Item = pair<int, ll>;
using Project = decltype(&Item::second);
using Values = PriorityContainer<multiset<Item>, GroupAddSub<ll>, Project>;
auto values = Values::with_compare(less<Item>{}, &Item::second);
values.push({3, 10}); values.push({1, 20});
assert(values.front() == Item(1, 20) && values.all_prod() == 30);
```

## 詳細なドキュメント

### PriorityContainer

```cpp
template <class Container,
          class M = /* Container::monoid_type があればそれ、他は GroupTrivial */,
          class Projection = /* 基底の射影があればその型、他は Identity */>
struct PriorityContainer;
```

`value_type` は基底コンテナの要素型、`S` は `M::S`、`projection_type` は `Projection`。
`supports_erase`, `supports_contains`, `supports_front`, `supports_back` は機能の有無を表すコンパイル時定数。
対応する操作だけが公開される。汎用コードでは `if constexpr (Q::supports_erase)` のように分岐できる。
操作の式が成立するかを `void_t` で調べ、構築方法は `is_constructible` で調べる。
検出した情報で次も選択する。

- 基底が集約を持つ場合はそこへ委譲し、それ以外は逆元でキャッシュを更新する。
- `clear` があれば使い、それ以外は優先する端から全要素を削除する。
- 比較器を受け取れる構築方法を使う。

`TwoMultisets` でも、存在確認を使う削除と境界値で振り分ける削除、逆元を使う再分割と実際に移動して判定する再分割を切り替える。

##### 制約

- 集約は可換モノイドで、`M::inv` を使う場合は可換群である。
- 基底コンテナの集約を使う場合は、モノイド・射影型が指定と一致する。
- 集約を載せる場合、`std::invoke(projection, x)` の戻り値の型（参照・`const` を除いた型）が `M::S` と同じである。
- 射影は `const value_type&` を受け取り、同じ要素から同じ集約値を返す。射影をコピー構築できる。
- キャッシュで集約するコンテナで `erase` を使う場合、比較器が同値とする要素の射影値も同じである。キーが重なる場合は、値や ID も比較に含める。
- 集約型で演算と中間計算を表現できる。
- 各端の操作が、基準の比較順における最初・最後に対応する。

#### コンストラクタ / with_compare

```cpp
PriorityContainer();
explicit PriorityContainer(Container data);
PriorityContainer(Container data, Projection projection); // キャッシュで集約する場合
template <class Compare>
static PriorityContainer with_compare(const Compare& comp, Projection projection = Projection());
```

空、既存のコンテナ、または大小を定める順序を指定して構築する。
既存コンテナに集約があればそれを使い、可換群のキャッシュが必要な場合は要素を走査する。
比較器を設定済みのコンテナも渡せる。
既存の木を包むと、その木の射影も引き継ぐ。キャッシュで集約する場合は第 2 引数に射影を指定できる。
`with_compare` では比較器・射影を指定して空のコンテナを作る。木への射影の設定もここで行う。

##### 制約

- キャッシュを初期構築する場合、基底コンテナをコピーできる。
- `with_compare` では基底コンテナを比較器で構築できる。
- `with_compare` に渡す比較器の型が、基底の基準の比較器型と一致する。
- 比較器を受け取る構築方法がない独自コンテナでは、空の比較器を指定するか、設定済みのコンテナを直接渡す。
- 射影を持つ独自の集約コンテナを `with_compare` で作る場合、そのコンテナが `(comp, projection)` で構築できる。
- move 代入では射影をコピー代入できる。

##### 計算量

- 空の構築は $O(1)$。
- 既存コンテナは、その move と、必要な集約の初期走査の計算量。
- キャッシュの初期走査は、`multiset` で $O(n)$、ヒープで $O(h\log(h+1))$。

#### push

```cpp
void push(const value_type& x);
```

要素を 1 個追加し、集約を更新する。

##### 計算量

- 基底コンテナの操作と、定数回の集約演算の計算量。

#### front / back / pop_front / pop_back / extract_front / extract_back

```cpp
value_type front() const;
value_type back() const;
void pop_front();
void pop_back();
value_type extract_front();
value_type extract_back();
```

比較順の最初・最後の値を取得する。`pop_*` は 1 個削除し、`extract_*` は値を返して 1 個削除する。
削除と同時に集約を更新する。
最初側の操作は `supports_front`、最後側の操作は `supports_back` が `true` のときに使える。

##### 制約

- コンテナに要素がある。

##### 計算量

- 基底コンテナの操作と、定数回の集約演算の計算量。

#### erase / contains / try_erase

```cpp
void erase(const value_type& x);
bool contains(const value_type& x) const;
bool try_erase(const value_type& x);
```

`erase` は存在する値を 1 個削除する。`multiset::erase(x)` も 1 個削除に揃える。
`contains` は存在を確認する。`try_erase` は削除できたときに `true` を返す。
`try_erase` には任意削除と存在確認の両方が必要。
遅延削除ヒープの `erase` は、存在を利用者が保証して使う。

##### 制約

- `erase` の値が存在する。

##### 計算量

- 基底コンテナの操作と、定数回の集約演算の計算量。

#### size / empty / all_prod / container / projection / clear

```cpp
int size() const;
bool empty() const;
S all_prod() const;
const Container& container() const;
const Projection& projection() const;
void clear();
```

個数・空判定・全体の集約・基底コンテナと射影の参照を取得する。
`clear` は全要素を削除し、比較器・射影を維持する。

##### 計算量

- 個数・空判定・参照取得は $O(1)$。
- 集約の取得は、その基底コンテナの計算量。
- `clear` は基底コンテナの計算量。専用の `clear` がない場合は優先する端から全要素を削除する。

#### content

```cpp
vector<value_type> content() const;
```

全要素を基準の比較器順で返す。取り出せる端によらず、同じ順序に揃える。
基底の `content()` があればそれを使い、
それ以外ではコピーから全要素を取り出す。元の要素と集約は維持する。
`LOCAL` では `dump(q)` でもこの内容を表示する。

##### 制約

- 基底に `content()` がある場合、その結果が `vector<value_type>` で、基準の比較器順に並んでいる。
- 基底に `content()` がない場合は、基底をコピーできる。

##### 計算量

- 基底の `content` の計算量、またはコピーと全要素の端の取得・削除の計算量。
- `multiset`、Treap、`MonotonePriorityDeque` は $O(n)$。
- ヒープは、内部に保持する要素数を $h$ として $O(h\log(h+1))$。

### MinPriorityQueue / MaxPriorityQueue / MinErasablePriorityQueue / MaxErasablePriorityQueue

```cpp
template <class T, class Compare = less<T>, class Sequence = vector<T>>
using MinPriorityQueue = /* front 側のヒープ */;
template <class T, class Compare = less<T>, class Sequence = vector<T>>
using MaxPriorityQueue = /* back 側のヒープ */;
template <class T, class Compare = less<T>>
using MinErasablePriorityQueue = /* front 側の遅延削除ヒープ */;
template <class T, class Compare = less<T>>
using MaxErasablePriorityQueue = /* back 側の遅延削除ヒープ */;
```

いずれも基準の比較器を受け取り、最小側と最大側を別の型として扱う。
共通する操作は `push`, `size`, `empty`, `clear`, `content`。
`Min*` は `front`, `pop_front`, `extract_front`、`Max*` は `back`, `pop_back`, `extract_back` を持つ。
`*Erasable*` は存在する値を 1 個削除する `erase` も持つ。
集約を載せる場合は `PriorityContainer<MinPriorityQueue<T>, M>` などで包む。

#### コンストラクタ

```cpp
Q();
explicit Q(const Compare& comp);
template <class It>
Q(It first, It last, const Compare& comp = Compare());
```

`Q` は上記のいずれかの型。空、またはイテレータ範囲の要素で初期化する。

##### 計算量

- 空の構築は $O(1)$。範囲からの構築は $O(n)$。

#### 操作

型が持つ端の取得・削除を行う。`content` は基準の比較器順で全要素を返す。

##### 制約

- 比較器が狭義弱順序を定める。
- 端の取得・削除では要素がある。`erase` の値が存在する。

##### 計算量

- `size`, `empty`, 端の取得は $O(1)$。
- 通常のヒープの追加・端の削除は $O(\log(n+1))$。
- 遅延削除ヒープの追加・端の削除・任意削除は、内部要素数を $h$ として償却 $O(\log(h+1))$。
- `content` は $O(h\log(h+1))$。`clear` は基底ヒープの計算量。
