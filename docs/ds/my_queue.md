## 概要

`vector` と先頭位置で管理する自作 queue。

デフォルトの `std::queue` は、`std::deque` を使っているが、`std::deque` はメモリ使用量が多い。

## 詳細なドキュメント

### MyQueue

#### コンストラクタ

```cpp
MyQueue<T>()
```

空の queue を構築する。

##### 計算量

- $O(1)$

#### reserve

```cpp
void reserve(int n)
```

内部の vector に少なくとも `n` 要素分の容量を確保する。

##### 制約

- $n \geq 0$

##### 計算量

- 再確保がない場合：$O(1)$
- 再確保がある場合：$O(\mathrm{size}())$

#### size, empty

```cpp
(1) I size<I = ll>()
(2) bool empty()
```

- (1)：現在の要素数を返す。
- (2)：queue が空なら `true` を返す。

##### 計算量

- $O(1)$

#### push

```cpp
void push(const T& x)
```

末尾に `x` を追加する。

##### 計算量

- 償却 $O(1)$

#### front

```cpp
(1) T front() const
(2) T& front()
```

先頭要素を返す。

##### 制約

- queue が空でない

##### 計算量

- $O(1)$

#### pop

```cpp
void pop()
```

先頭要素を論理的に削除する。内部の vector からは削除しない。

##### 制約

- queue が空でない

##### 計算量

- $O(1)$

#### clear

```cpp
void clear()
```

すべての要素と内部に残っている削除済み要素を消去する。

##### 計算量

- $O(n)$

#### operator[]

```cpp
(1) T operator[](int i) const
(2) T& operator[](int i)
```

先頭を $0$ 番目とした `i` 番目の要素を返す。範囲検査は行わない。

##### 制約

- $0 \leq i < \mathrm{size}()$

##### 計算量

- $O(1)$

#### at

```cpp
(1) T at(int i) const
(2) T& at(int i)
```

先頭を $0$ 番目とした `i` 番目の要素を返す。範囲外なら assertion に失敗する。

##### 制約

- $0 \leq i < \mathrm{size}()$

##### 計算量

- $O(1)$

#### content

```cpp
vc<T> content()
```

現在の要素を先頭から順に格納した vector を返す。

##### 計算量

- 時間：$O(\mathrm{size}())$
