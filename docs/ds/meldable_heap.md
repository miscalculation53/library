## 概要

複数の優先度付きキューを、併合可能な左偏木で管理する。`MeldableHeapPool<T, Compare>` がノード配列と比較関数を持ち、`make_heap()` でヒープを作る。既定の `less<T>` では最小値を取り出す。

ヒープはプールへのポインタと根、要素数を持つ。ムーブで所有権を移し、独立した複製には `clone()` を使う。`a.merge(b)` は要素を `a` に集め、`b` を空にする。

以下では対象ヒープの要素数を $s$、併合する2ヒープの要素数を $a,b$、プール内のノード数を $P$ とする。`push` / `emplace` は1ノード、`clone` は $s$ ノードを作る。使い終えたノードと値は `pool.clear()` までプールに残る。メモリは予約容量を含むノード配列の容量に比例する。

比較と値の構築・コピー・ムーブ・破棄を $O(1)$ として計算量を書く。

## 使用例

```cpp
MeldableHeapPool<int> pool(100);
auto a = pool.make_heap();
auto b = pool.make_heap(3);
a.push(7);
a.emplace(1);
a.merge(b);
assert(b.empty());
assert(a.size() == 3 && a.top() == 1);
a.pop();
assert(a.top() == 3);

auto copied = a.clone();
copied.pop();
assert(a.top() == 3 && copied.top() == 7);
assert(a.content() == vc<int>({3, 7}));
dump(a);  // LOCAL 時に取り出す順の内容を表示

MeldableHeapPool<int, greater<int>> max_pool;
auto largest = max_pool.make_heap();
largest.push(2);
largest.push(9);
assert(largest.top() == 9);
```

## 詳細なドキュメント

### MeldableHeapPool

#### コンストラクタ

```cpp
template <class T, class Compare = less<T>>
explicit MeldableHeapPool(size_t expected_pushes = 0, Compare comp = Compare{})
```

空のノードプールを作り、`expected_pushes` ノード分の容量を予約する。予約容量を超えると自動で拡張する。比較関数は `comp(x, y)` が `true` なら `x` を先に取り出すものとする。

##### 制約

- `T` はコピー構築またはムーブ構築が可能
- `Compare` は `T` に対する狭義弱順序を定め、格納中の要素の順序が変化しない

##### 計算量

- $O(1)$

#### node_count / reserve / clear

```cpp
size_t node_count() const
void reserve(size_t count)
void clear()
```

ノード数を返す、指定したノード容量を予約する、全ノードを削除する。`node_count` は取り出し・ヒープの破棄で使い終えたノードも含む。

`pool.clear()` は全ヒープを無効にし、確保済み容量を維持する。以前のヒープは破棄するか、新しいヒープをムーブ代入して使う。

##### 計算量

- `node_count`：$O(1)$
- `reserve` / `clear`：$O(P+1)$

#### make_heap

```cpp
Heap make_heap()
Heap make_heap(const T& value)
Heap make_heap(T&& value)
```

このプールに結び付いた空ヒープ、または1要素のヒープを作る。

##### 制約

- `const T&` を渡す場合、`T` はコピー構築可能

##### 計算量

- 空ヒープ：$O(1)$
- 1要素のヒープ：償却 $O(1)$

### MeldableHeap

```cpp
template <class T, class Compare = less<T>>
using MeldableHeap = typename MeldableHeapPool<T, Compare>::Heap;
```

`pool.make_heap()` で作る。ムーブ構築・代入は $O(1)$ で、移動元を同じプールの空ヒープにする。コピーには `clone()` を使う。ヒープを使っている間は、プールを同じアドレスで生存させる。プールへの代入・ムーブも以前のヒープを無効にする。

ヒープの破棄・ムーブ代入は、以前のノードやプールにアクセスしない。使い終えたノードはプール全体の `clear()` まで残る。

#### size / empty / top

```cpp
int size() const
bool empty() const
const T& top() const
```

要素数、空かどうか、先頭の値を返す。`top` の参照はプールの再確保や `pool.clear()` で無効になる。

##### 制約

- `top`：ヒープが空でない

##### 計算量

- $O(1)$

#### push / emplace / pop

```cpp
void push(const T& value)
void push(T&& value)
template <class... Args> void emplace(Args&&... args)
void pop()
```

値の追加、引数から構築した値の追加、先頭の削除を行う。重複する値も個別に保持する。

##### 制約

- `push(const T&)`：`T` はコピー構築可能
- `emplace`：渡す引数から `T` を構築可能
- `pop`：ヒープが空でない

##### 計算量

- `push` / `emplace`：償却 $O(\log(s+2))$
- `pop`：$O(\log(s+1))$

#### content / dump

```cpp
vc<T> content() const
dump(heap);  // LOCAL 時
```

取り出す順に値をコピーした配列を返す。比較関数で同順位となる要素の順序は任意。`dump(heap)` もこの配列を表示する。ヒープの内容とプールのノード数を維持する。

##### 制約

- `T` はコピー構築可能
- `dump`：`T` を `cpp-dump` で表示可能

##### 計算量

- $O(s\log(s+1)+1)$

#### merge

```cpp
void merge(MeldableHeap& other)
```

`other` の要素をこのヒープに移し、`other` を空にする。自分自身との併合はそのままになる。併合中のノード生成は0個。

##### 制約

- 2ヒープは同じプールに属する

##### 計算量

- $O(\log(a+1)+\log(b+1)+1)$

#### clone / clear / swap

```cpp
MeldableHeap clone() const
void clear()
void swap(MeldableHeap& other) noexcept
void swap(MeldableHeap& a, MeldableHeap& b) noexcept
```

独立した複製を作る、ヒープを空にする、プールへの結び付きと内容を交換する。`swap` は異なるプール間でも使える。

##### 制約

- `clone`：`T` はコピー構築可能

##### 計算量

- `clone`：償却 $O(s+1)$
- `clear` / `swap`：$O(1)$
