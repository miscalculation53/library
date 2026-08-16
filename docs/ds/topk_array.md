## 概要

要素の追加・上位 $k$ 個の管理

## 詳細なドキュメント

### TopKArray

要素を追加しながら、`Compare` で小さい順の先頭 $k$ 個だけを保持する固定長配列。デフォルトの `less<T>` では小さい方から $k$ 個を保持する。

#### コンストラクタ

```cpp
(1) TopKArray<T, k, Compare = less<T>>()
(2) TopKArray<T, k, Compare = less<T>>(Iter first, Iter last)
(3) TopKArray<T, k, Compare = less<T>>(initializer_list<T> init)
(4) TopKArray<T, k, Compare = less<T>>(const V& v)
```

- (1)：空で初期化する。
- (2)--(4)：与えられた全要素を順に追加して初期化する。

##### 制約

- $k \geq 1$
- `Compare` は `T` 上の strict weak ordering

##### 計算量

入力要素数を $n$ として、

- (1)：$O(k)$
- (2)--(4)：$O(nk)$

#### add

```cpp
void add(const T& val)
```

`val` を追加し、`Compare` で小さい方から高々 $k$ 個を保持する。

##### 計算量

- $O(k)$

#### size

```cpp
I size<I = ll>()
```

現在保持している要素数を返す。

##### 計算量

- $O(1)$

#### get

```cpp
T get(int i)
```

保持している要素を `Compare` の順に並べたときの `i` 番目を返す。

##### 制約

- $0 \leq i < \mathrm{size}()$

##### 計算量

- $O(1)$

#### merged

```cpp
TopKArray merged(const TopKArray& other) const
```

`*this` と `other` の要素を合わせたものから、`Compare` で小さい方から高々 $k$ 個を持つ新しい `TopKArray` を返す。

##### 計算量

- $O(k)$

#### merge

```cpp
void merge(const TopKArray& other)
```

`*this` を `merged(other)` で置き換える。

##### 計算量

- $O(k)$

#### content

```cpp
vc<T> content()
```

保持している要素を `Compare` の順に格納した vector を返す。

##### 計算量

- 時間：$O(k)$

### TopKArrayMonoid

```cpp
TopKArrayMonoid<T, k, Compare>
```

`TopKArray` の `merged` を積とするモノイド。セグメント木などに載せるために使う。

#### op

```cpp
S op(const S& a, const S& b)
```

`a.merged(b)` を返す。

##### 計算量

- $O(k)$

#### e

```cpp
S e()
```

空の `TopKArray` を返す。

##### 計算量

- $O(k)$
