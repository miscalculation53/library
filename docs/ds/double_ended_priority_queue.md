## 概要

最小値と最大値の両方を取得・削除できる優先度付きキュー。

消せる priority queue を使って実装。

## 詳細なドキュメント

### DoubleEndedPriorityQueue

#### コンストラクタ

```cpp
(1) DoubleEndedPriorityQueue<T>()
(2) DoubleEndedPriorityQueue<T>(It first, It last)
```

- (1)：空で初期化する。
- (2)：範囲 `[first, last)` の要素で初期化する。

##### 制約

- `T` に `<` と `==` が定義されている

##### 計算量

範囲の長さを $n$ として、

- (1)：$O(1)$
- (2)：$O(n)$

#### push

```cpp
void push(const T& x)
```

`x` を追加する。

##### 計算量

- $O(\log n)$

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

- 償却 $O(\log n)$

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

#### content

```cpp
vc<T> content()
```

現在の要素を昇順に並べた vector を返す。

##### 計算量

- 時間：$O(n \log n)$
