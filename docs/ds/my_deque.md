## 概要

`vector` を循環バッファとして使う自作 deque。`std::deque` と異なり、要素を一つの連続した領域に格納する。

容量が不足すると現在の2倍に拡張する。`pop_front`, `pop_back`, `clear` は要素を論理的に削除するだけで、確保したメモリや格納されていたオブジェクトは解放しない。

## 使用例

```cpp
MyDeque<int> deq;
deq.push_back(2);
deq.push_front(1);
deq.push_back(3);
assert(deq.front() == 1 && deq.back() == 3);
```

## 詳細なドキュメント

### MyDeque

```cpp
MyDeque<T>()
```

##### 制約

- `T` はデフォルト構築可能かつ代入可能

#### reserve

```cpp
void reserve(int n)
```

`n` 要素を再確保せず格納できる容量を確保する。現在の容量は減らさない。

##### 制約

- $n \geq 0$

##### 計算量

- 再確保しない場合：$O(1)$
- 再確保する場合：$O(\mathrm{size}())$

#### size / empty

```cpp
(1) I size<I = ll>() const
(2) bool empty() const
```

格納している要素数、または空かを返す。

##### 計算量

- $O(1)$

#### push_front / push_back

```cpp
(1) void push_front(const T& x)
(2) void push_back(const T& x)
```

`x` を先頭または末尾に追加する。

##### 計算量

- 償却 $O(1)$

#### front / back

```cpp
(1) T& front()
(2) const T& front() const
(3) T& back()
(4) const T& back() const
```

先頭要素または末尾要素を返す。

##### 制約

- 空でない

##### 計算量

- $O(1)$

#### pop_front / pop_back

```cpp
(1) void pop_front()
(2) void pop_back()
```

先頭要素または末尾要素を論理的に削除する。

##### 制約

- 空でない

##### 計算量

- $O(1)$

#### clear

```cpp
void clear()
```

すべての要素を論理的に削除する。確保した容量は維持する。

##### 計算量

- $O(1)$

#### operator[] / at

```cpp
(1) T& operator[](int i)
(2) const T& operator[](int i) const
(3) T& at(int i)
(4) const T& at(int i) const
```

先頭を $0$ 番目とした `i` 番目の要素を返す。`at` は範囲を assertion で検査する。

##### 制約

- $0 \leq i < \mathrm{size}()$

##### 計算量

- $O(1)$
