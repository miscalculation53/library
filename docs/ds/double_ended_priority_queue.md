## 概要

最小値と最大値の両方を取得・削除できる優先度付きキュー。

消せる priority queue を使って実装。

`Compare` 順の最初を `get_min`、最後を `get_max` とする。
`value_type`, `compare_type` を公開する。
[PriorityContainer](priority_container.md) で他のコンテナと操作を統一できる。
計算量の $h$ は、遅延削除分を含む内部ヒープの要素数とする。

## 詳細なドキュメント

### DoubleEndedPriorityQueue

#### コンストラクタ

```cpp
(1) DoubleEndedPriorityQueue<T, Compare = less<T>>()
(2) DoubleEndedPriorityQueue<T, Compare = less<T>>(It first, It last)
(3) DoubleEndedPriorityQueue<T, Compare = less<T>>(const Compare& comp)
(4) DoubleEndedPriorityQueue<T, Compare = less<T>>(It first, It last, const Compare& comp)
```

- (1)：空で初期化する。
- (2)：範囲 `[first, last)` の要素で初期化する。
- (3), (4)：比較器の状態を指定して空、または範囲で初期化する。

##### 制約

- `Compare` が狭義弱順序を定める。
- `T` に `==` が定義され、比較器と整合している。

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

#### extract_min, extract_max

```cpp
(1) T extract_min()
(2) T extract_max()
```

- (1)：最小値を返し、その要素を $1$ 個削除する。
- (2)：最大値を返し、その要素を $1$ 個削除する。

##### 制約

- キューが空でない

##### 計算量

- 償却 $O(\log(h+1))$

#### get_min, get_max

```cpp
(1) T get_min()
(2) T get_max()
```

- (1)：最小値を返す。
- (2)：最大値を返す。

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

現在の要素を昇順に並べた vector を返す。

##### 計算量

- $O(h\log(h+1))$

#### erase / clear

```cpp
void erase(const T& x);
void clear();
```

`erase` は値 `x` を 1 個削除する。
`clear` は全要素と遅延削除の情報を削除し、比較器を維持する。

##### 制約

- `erase` の値が存在する。

##### 計算量

- `erase` は償却 $O(\log(h+1))$。`clear` は $O(h)$。
