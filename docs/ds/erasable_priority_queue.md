## 概要

削除予定の要素を別のヒープに積み、任意要素の削除を遅延して行う priority queue

`value_type`, `compare_type` を公開する。
[PriorityContainer](priority_container.md) で他のコンテナと操作を統一できる。
計算量の $h$ は、遅延削除分を含む内部ヒープの要素数とする。

## 詳細なドキュメント

### ErasablePriorityQueue

`Compare = less<T>` では最小値を先頭にする。

#### コンストラクタ

```cpp
(1) ErasablePriorityQueue<T, Compare = less<T>>()
(2) ErasablePriorityQueue<T, Compare = less<T>>(It first, It last)
(3) ErasablePriorityQueue<T, Compare = less<T>>(const Compare& comp)
(4) ErasablePriorityQueue<T, Compare = less<T>>(It first, It last, const Compare& comp)
```

- (1)：空で初期化する。
- (2)：範囲 `[first, last)` の要素で初期化する。
- (3), (4)：比較器の状態を指定して空、または範囲で初期化する。

##### 制約

- `Compare` は `T` 上の strict weak ordering
- `T` に `==` が定義され、`Compare` と整合している

##### 計算量

範囲の長さを $n$ として、

- (1), (3)：$O(1)$
- (2), (4)：$O(n)$

#### push

```cpp
void push(const T& x)
```

`x` を追加する。

##### 計算量

- 償却 $O(\log(h+1))$

#### pop

```cpp
void pop()
```

先頭の要素を $1$ 個削除する。

##### 制約

- キューが空でない

##### 計算量

- 償却 $O(\log(h+1))$

#### erase

```cpp
void erase(const T& x)
```

`x` を $1$ 個削除する。実際のヒープからの削除は、`x` が先頭に来たときまで遅延される。

##### 制約

- 現在のキューに `x` が存在する
- 同じ値について、追加した個数を超えて `erase` しない

##### 計算量

- 償却 $O(\log(h+1))$

#### top

```cpp
T top()
```

`Compare` で最小の要素を返す。

##### 制約

- キューが空でない

##### 計算量

- $O(1)$

#### empty

```cpp
bool empty()
```

キューが空なら `true` を返す。

##### 計算量

- $O(1)$

#### size

```cpp
template <class I = ll>
I size() const
```

現在の要素数を型 `I` で返す。

##### 計算量

- $O(1)$

#### content

```cpp
vc<T> content()
```

現在の要素を `Compare` の順に並べた vector を返す。

##### 計算量

- $O(h\log(h+1))$

#### clear

```cpp
void clear();
```

全要素と遅延削除の情報を削除し、比較器を維持する。

##### 計算量

- $O(h)$。
