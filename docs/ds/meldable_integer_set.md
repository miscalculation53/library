## 概要

$[0,n)$ の整数集合を、併合可能な疎なセグメント木で管理する。葉は64種類分のビットを持ち、内部ノードは部分木の要素数を持つ。

`MeldableIntegerSetPool` がノードプールを持ち、`make_set()` で集合 `MeldableIntegerSet` を作る。集合はプールへのポインタと根を持ち、`st.insert(x)`、`st.size()`、`st.merge(other)` で操作する。挿入・削除、併合、隣接検索、区間の個数、昇順で $k$ 番目の検索に対応する。

以下では $H=1+\lceil\log_2\max(1,\lceil n/64\rceil)\rceil$、$V$ を対象の集合のノード数、$P$ をプール内のノード数とする。1回の挿入で作るノード数は高々 $H$。削除・併合・集合の破棄で使い終えたノードはプールに残り、`pool.clear()` でまとめて再利用する。確保メモリは、予約容量を含むノード配列の容量に比例する。

併合は両方にノードがある区間で一方のノードを消費する。作ったノード数の合計を $A$、併合回数を $R$ とすると、全併合の時間は $O(A+R)$。併合順は自由に選べる。

## 使用例

```cpp
MeldableIntegerSetPool pool(1LL << 40, 3); // 挿入3回分のノード容量を予約する
auto a = pool.make_set();
a.insert(3);
auto b = pool.make_set(70);
b.insert(3);
a.merge(b);
assert(b.empty());
assert(a.size() == 2);
assert(a.count(0, 64) == 1);
assert(a.kth(1) == 70);
assert(a.geq_min(4) == 70);

auto copied = a.clone();
copied.erase(3);
assert(a.contains(3));
for (ll key : a) cout << key << '\n';
```

集合を配列で持つ場合は、生成関数で各集合を作る。

```cpp
auto sets = gen_vec(5, [&](int) { return pool.make_set(); });
sets[0].insert(3);
sets[1].insert(70);
sets[0].merge(sets[1]);
```

UnionFind では頂点データに集合を持たせられる。以下の `pool` はグローバルな共有プールとする。

```cpp
struct VData
{
  MeldableIntegerSet st = pool.make_set();
  VData() = default;
  VData(int i) { st.insert(A[i]); }
};
// 成分を併合する際：
xd.st.merge(yd.st);
```

1要素の集合を $N$ 個作る場合は `MeldableIntegerSetPool(U, N)` で挿入 $N$ 回分を予約できる。判定をやり直すときは、`pool.clear()` の後に `uf.reset(N)` で初期化し直すと、プールとUnionFindの配列容量を再利用できる。

## 詳細なドキュメント

### MeldableIntegerSetPool

#### コンストラクタ

```cpp
explicit MeldableIntegerSetPool(ll n = 0, size_t expected_insertions = 0)
```

値域を $[0,n)$ とする空のノードプールを作る。`expected_insertions * H` ノード分の容量を予約する。

`expected_insertions` は、`pool.clear()` までにプール全体で行う `insert` と `make_set(key)` による登録回数の見積もり。予約容量を超えると自動で拡張する。`clone()` によるノード生成は、この挿入回数の見積もりとは別に発生する。第2引数を省略した場合は、必要に応じて容量を増やす。

##### 制約

- $0\leq n$

##### 計算量

- $O(H)$

#### universe_size / node_count / reserve

```cpp
ll universe_size() const
size_t node_count() const
void reserve(size_t count)
```

値域の大きさ、プール内のノード数を返す。`node_count` は削除・併合・集合の破棄で使い終えたノードも含む。`reserve` は指定したノード数を再確保せず格納できる容量を確保する。

##### 計算量

- `universe_size` / `node_count`：$O(1)$
- `reserve`：$O(P+1)$

#### make_set

```cpp
Set make_set()
Set make_set(ll key)
```

このプールに結び付いた集合を返す。引数なしでは空集合、`key` を渡すと1要素の集合を作る。`Set` は `MeldableIntegerSetPool::Set` で、`MeldableIntegerSet` はその別名。

##### 制約

- `key` を渡す場合は $0\leq key<n$

##### 計算量

- 空集合：$O(1)$。確保ノード数0個
- 1要素の集合：償却 $O(H)$。$H$ 個のノードを作る

#### clear

```cpp
void clear()
```

全ノードを削除し、以前の集合・イテレータを全て無効にする。確保済みの容量は維持する。以前の集合は破棄するか、`make_set()` で作った新しい集合をムーブ代入して使う。

##### 計算量

- $O(P+1)$

### MeldableIntegerSet

```cpp
using MeldableIntegerSet = MeldableIntegerSetPool::Set;
```

`pool.make_set()` または `pool.make_set(key)` で作る。ムーブ構築・ムーブ代入はプールへの結び付きと根を移し、移動元を同じプールの空集合にする。独立した複製は `clone()` で作る。コピー構築・コピー代入は使えない。ムーブの計算量は $O(1)$。

集合の破棄・ムーブ代入で保持を終えたノードも、プール全体の `clear()` まで残る。破棄時にプールへアクセスする処理はない。

集合を使っている間は、プールを同じアドレスのまま生存させる。プールへの代入・ムーブも以前の集合を無効にする。無効な集合は破棄するか、新しい集合をムーブ代入して使う。

#### universe_size / size / empty

```cpp
ll universe_size() const
ll size() const
bool empty() const
```

値域の大きさ、集合の要素数、空かを返す。

##### 計算量

- $O(1)$

#### contains

```cpp
bool contains(ll key) const
```

`key` が登録されていれば `true` を返す。

##### 制約

- $0\leq key<n$

##### 計算量

- $O(H)$

#### insert / erase

```cpp
bool insert(ll key)
bool erase(ll key)
```

キーを登録・削除する。集合の内容が変わった場合に `true` を返す。

##### 制約

- $0\leq key<n$

##### 計算量

- `insert`：償却 $O(H)$
- `erase`：$O(H)$

#### merge

```cpp
void merge(MeldableIntegerSet& other)
```

`other` の要素をこの集合へ移し、`other` を空にする。重複するキーは1個として扱う。自分自身との併合はそのままになる。

##### 制約

- 2つの集合は同じプールに属する

##### 計算量

- $O(t+1)$。$t$ は両方にノードがある区間数
- ノード生成が合計 $A$ 個、併合が $R$ 回なら、全併合の合計は $O(A+R)$

併合中のノード生成は0個。

#### clone

```cpp
MeldableIntegerSet clone() const
```

同じプール内に独立した集合のコピーを作る。$V$ 個のノードを追加する。

##### 計算量

- 償却 $O(V+1)$

#### lt_cnt / leq_cnt / geq_cnt / gt_cnt

```cpp
ll lt_cnt(ll key) const
ll leq_cnt(ll key) const
ll geq_cnt(ll key) const
ll gt_cnt(ll key) const
```

それぞれ `key` 未満、以下、以上、より大きいキーの個数を返す。`key` は値域の外でもよい。

##### 計算量

- $O(H)$

#### count

```cpp
ll count(ll l, ll r) const
```

$[l,r)$ にあるキーの個数を返す。

##### 制約

- $0\leq l\leq r\leq n$

##### 計算量

- $O(H)$

#### kth

```cpp
ll kth(ll k) const
```

昇順で `k` 番目のキーを返す。`k` は0始まり。

##### 制約

- $0\leq k<size()$

##### 計算量

- $O(H)$

#### 隣接検索

```cpp
ll geq_min(ll key) const
ll gt_min(ll key) const
ll leq_max(ll key) const
ll lt_max(ll key) const
ll min_element() const
ll max_element() const
```

| 関数 | 返すキー | 該当するキーがない場合 |
|---|---|---|
| `geq_min(key)` | `key` 以上で最小 | `n` |
| `gt_min(key)` | `key` より大きいキーの最小 | `n` |
| `leq_max(key)` | `key` 以下で最大 | `-1` |
| `lt_max(key)` | `key` より小さいキーの最大 | `-1` |
| `min_element()` | 全体の最小 | `n` |
| `max_element()` | 全体の最大 | `-1` |

`key` は値域の外でもよい。

##### 計算量

- $O(H)$

#### enumerate / content

```cpp
template <class F> void enumerate(ll l, ll r, const F& f) const
vc<ll> content() const
```

`enumerate` は $[l,r)$ のキーを昇順に `f(key)` へ渡す。`content` は全キーを昇順の配列で返す。コールバックの実行中はノードプールと集合の内容を維持する。

##### 制約

- $0\leq l\leq r\leq n$

##### 計算量

- $O(V+s+1)$。$s$ は列挙するキーの個数（`f` の実行時間を除く）

#### begin / end / cbegin / cend

```cpp
iterator begin() const
iterator end() const
const_iterator cbegin() const
const_iterator cend() const
```

昇順に走査する入力イテレータを返す。`iterator` と `const_iterator` は同じ型で、参照するとキーの `ll` の値を返す。前置・後置 `++` と等値比較に対応し、範囲 `for` に使える。

集合への更新・併合・`clear`・ムーブ・`swap`・破棄は、その集合のイテレータを無効化する。プール全体の `clear`・代入・ムーブも全てのイテレータを無効にする。

##### 計算量

- `end` / `cend` / 参照 / 比較：$O(1)$
- `begin` / `cbegin` / `++`：$O(H)$
- 全要素の走査：$O((s+1)H)$

全体を高速に列挙する用途では `enumerate` を使える。

#### clear

```cpp
void clear()
```

この集合だけを空にし、ノード領域をプールに残す。

##### 計算量

- $O(1)$

#### swap

```cpp
void swap(MeldableIntegerSet& other) noexcept
void swap(MeldableIntegerSet& a, MeldableIntegerSet& b) noexcept
```

プールへの結び付きと根を交換する。異なるプールに属する集合同士でも交換できる。

##### 計算量

- $O(1)$
