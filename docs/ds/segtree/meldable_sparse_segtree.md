## 概要

長さ $n$、各点の初期値が `M::e()` の列を、併合可能な疎なセグメント木で管理する。`MeldableSparseSegmentTreePool<M>` がノードプールを持ち、`make_tree()` で木 `MeldableSparseSegmentTree<M>` を作る。各木はプールへのポインタと根を持ち、`seg.set(p,x)`、`seg.prod(l,r)`、`seg.merge(other)` で操作する。

一点更新・区間積・境界探索・破壊的な併合に対応する。区間積は添字の昇順に `M::op` でまとめる。同じ点の値の併合は、既定では `M::op`、`merge(other,f)` では `f` を使う。例えば、各点の値を0か1とし、区間集計を加算、点の併合を `max` にすると集合の和と区間内の種類数を扱える。集合専用の [MeldableIntegerSet](../meldable_integer_set.md) は、64種類を1つの葉にまとめている。

内部ノードは固定した区間と区間積を持ち、葉は1点の値を持つ。[SparseSegmentTree](sparse_segtree.md) の1点につき1ノードを持つ構造と比べ、対応する区間同士を直接併合できる。

以下では $H=1+\lceil\log_2\max(1,n)\rceil$、$V$ を対象の木のノード数、$P$ をプール内のノード数とする。1回の更新で増えるノードは高々 $H$ 個。使用メモリは $O(P)$。削除・併合・木の破棄で使い終えたノードもプールに残り、`pool.clear()` でまとめて再利用する。

両方にノードがある区間での併合は、一方のノードを消費する。生成したノード数の合計を $A$、併合回数を $R$ とすると、全併合の時間は $O(A+R)$。併合順は自由に選べる。

計算量は `M::op`、`M::e`、`S` のコピー・代入を $O(1)$ とし、コールバックの実行時間を除く。

## 使用例

```cpp
MeldableSparseSegmentTreePool<MonoidAdd<ll>> pool(1LL << 40);
auto a = pool.make_tree();
a.set(10, 2);
auto b = pool.make_tree(10, 3);
b.set(100, 4);
a.merge(b);
assert(b.empty());
assert(a.get(10) == 5);
assert(a.prod(0, 100) == 5);
assert(a.all_prod() == 9);

auto g = [](ll sum) { return sum <= 5; };
assert(a.max_right_ok(0, g) == 100);

auto copied = a.clone();
copied.erase(10);
assert(a.get(10) == 5);
a.enumerate(0, 101, [&](ll p, ll value)
{
  cout << p << ' ' << value << '\n';
});
```

点の併合を区間集計と変える例：

```cpp
auto x = pool.make_tree(7, 1);
auto y = pool.make_tree(7, 1);
y.set(9, 1);
x.merge(y, [](ll u, ll v) { return max(u, v); });
assert(x.all_prod() == 2);
```

## 詳細なドキュメント

### MeldableSparseSegmentTreePool

```cpp
template <class M> struct MeldableSparseSegmentTreePool
using S = typename M::S;
```

`M` は `S`、結合的な `op`、単位元を返す `e` を持つモノイドとする。`M::op` は非可換でもよい。

#### コンストラクタ

```cpp
explicit MeldableSparseSegmentTreePool(ll n = 0)
```

添字の範囲を $[0,n)$ とする空のノードプールを作る。

##### 制約

- $0\leq n$

##### 計算量

- $O(1)$

#### universe_size / node_count / reserve

```cpp
ll universe_size() const
size_t node_count() const
void reserve(size_t count)
```

列の長さ、プール内のノード数を返す。`node_count` は削除・併合・木の破棄で使い終えたノードも含む。`reserve` は指定したノード数を再確保せず格納できる容量を確保する。

##### 計算量

- `universe_size` / `node_count`：$O(1)$
- `reserve`：$O(P+1)$

#### make_tree

```cpp
Tree make_tree()
Tree make_tree(ll p, const S& value)
```

このプールに結び付いた木を返す。引数なしでは全ての点の値が `M::e()` で、登録済みの点が0個の木を作る。`p,value` を渡すと `p` だけを登録した木を作る。`Tree` は `MeldableSparseSegmentTreePool<M>::Tree` で、`MeldableSparseSegmentTree<M>` はその別名。

##### 制約

- `p` を渡す場合は $0\leq p<n$

##### 計算量

- 空の木：$O(1)$。確保ノード数0個
- 1点を登録した木：償却 $O(H)$。高々 $H$ 個のノードを作る

#### clear

```cpp
void clear()
```

全ノードを削除し、以前の木を全て無効にする。確保済みの容量は維持する。以前の木は破棄するか、`make_tree()` で作った新しい木をムーブ代入して使う。

##### 計算量

- $O(P+1)$

### MeldableSparseSegmentTree

```cpp
template <class M>
using MeldableSparseSegmentTree = typename MeldableSparseSegmentTreePool<M>::Tree;
using S = typename M::S;
```

`pool.make_tree()` または `pool.make_tree(p,value)` で作る。ムーブ構築・ムーブ代入はプールへの結び付きと根を移し、移動元を同じプールの空の木にする。独立した複製は `clone()` で作る。コピー構築・コピー代入は使えない。ムーブの計算量は $O(1)$。

木の破棄・ムーブ代入で保持を終えたノードも、プール全体の `clear()` まで残る。破棄時にプールへアクセスする処理はない。

木を使っている間は、プールを同じアドレスのまま生存させる。プールへの代入・ムーブも以前の木を無効にする。無効な木は破棄するか、新しい木をムーブ代入して使う。

`set`・`modify` で更新した点は登録済みになる。値が `M::e()` の点も登録状態を維持する。`erase` は登録を削除し、その点の値を `M::e()` に戻す。コールバックの実行中は、ノードプールと木の内容を維持する。

#### universe_size / empty

```cpp
ll universe_size() const
bool empty() const
```

列の長さ、登録済みの点が0個かを返す。

##### 計算量

- $O(1)$

#### set / modify

```cpp
void set(ll p, const S& value)
template <class F> void modify(ll p, const F& f)
```

`set` は `p` の値を `value` に更新する。`modify` は現在の値への参照を `f` にちょうど1回渡す。未登録の点では `M::e()` から始める。

##### 制約

- $0\leq p<n$

##### 計算量

- 償却 $O(H)$。高々 $H$ 個のノードを追加する

#### get / contains

```cpp
S get(ll p) const
bool contains(ll p) const
```

`get` は点の値を返す。未登録なら `M::e()`。`contains` は登録済みの点かを返す。

##### 制約

- $0\leq p<n$

##### 計算量

- $O(H)$

#### erase

```cpp
bool erase(ll p)
```

点の登録を削除する。登録されていた場合に `true` を返す。

##### 制約

- $0\leq p<n$

##### 計算量

- $O(H)$

#### merge

```cpp
void merge(MeldableSparseSegmentTree<M>& other)
template <class F> void merge(MeldableSparseSegmentTree<M>& other, const F& f)
```

`other` の登録済みの点をこの木へ併合し、`other` を空にする。両方で登録されている点の値を `f(get(p),other.get(p))` にする。一方だけで登録されている点は、その値を引き継ぐ。`f` を省略すると `M::op` を使う。

自分自身との併合はそのままになる。

##### 制約

- 2つの木は同じプールに属する
- `f` は `S f(const S&, const S&)` として呼び出せる

`f` と `M::op` の間の分配法則は不要。`f` の可換性も不要で、引数の順序はこの木、`other` になる。

##### 計算量

- $O(t+1)$。$t$ は両方にノードがある区間数
- 生成ノードが合計 $A$ 個、併合が $R$ 回なら、全併合の合計は $O(A+R)$

併合中のノード生成は0個。`f` は両方で登録されている点ごとに1回呼ぶ。

#### clone

```cpp
MeldableSparseSegmentTree<M> clone() const
```

同じプール内に独立した木のコピーを作る。$V$ 個のノードを追加する。

##### 計算量

- 償却 $O(V+1)$

#### prod / all_prod

```cpp
S prod(ll l, ll r) const
S all_prod() const
```

`prod` は $[l,r)$、`all_prod` は列全体の値を、添字の昇順に `M::op` で集計する。空区間の積は `M::e()`。

##### 制約

- $0\leq l\leq r\leq n$

##### 計算量

- `prod`：$O(H)$
- `all_prod`：$O(1)$

#### max_right_ok / min_left_ok

```cpp
template <class G> ll max_right_ok(ll l, const G& g) const
template <class G> ll min_left_ok(ll r, const G& g) const
```

`max_right_ok` は `g(prod(l,R))` が真となる最大の $R\in[l,n]$ を返す。`min_left_ok` は `g(prod(L,r))` が真となる最小の $L\in[0,r]$ を返す。更新点の間に大きな空白があっても、添字の範囲全体での境界を返す。

##### 制約

- $0\leq l\leq n$、$0\leq r\leq n$
- `g(M::e()) == true`
- 区間を広げたとき、判定が `true` から `false` へ高々1回だけ変化する

##### 計算量

- $O(H)$

#### enumerate / content

```cpp
template <class F> void enumerate(ll l, ll r, const F& f) const
map<ll,S> content() const
```

`enumerate` は $[l,r)$ の登録済みの点を昇順に `f(p,value)` へ渡す。`value` は `const S&` で渡す。`content` は全ての登録済みの点と値を `map` で返す。

##### 制約

- $0\leq l\leq r\leq n$

##### 計算量

- $O(V+1)$

#### clear

```cpp
void clear()
```

この木の登録済みの点を全て削除し、ノード領域をプールに残す。

##### 計算量

- $O(1)$

#### swap

```cpp
void swap(MeldableSparseSegmentTree<M>& other) noexcept
void swap(MeldableSparseSegmentTree<M>& a, MeldableSparseSegmentTree<M>& b) noexcept
```

プールへの結び付きと根を交換する。異なるプールに属する木同士でも交換できる。

##### 計算量

- $O(1)$
