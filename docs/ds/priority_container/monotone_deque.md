## 概要

追加値を両端のどちらかへ置ける場合に、`deque` で比較器順を保つ優先度付きコンテナ。
両端の取得・削除と追加を $O(1)$ で行う。

[PriorityContainer](../priority_container.md) で包むと、他の基底と操作・集約を揃えられる。

## 使用例

```cpp
#include "ds/priority_container/monotone_deque.hpp"

PriorityContainer<MonotonePriorityDeque<int>> q;
for (int x : {0, 3, -2, 5}) q.push(x);
assert(q.extract_front() == -2);
assert(q.extract_back() == 5);
```

## 詳細なドキュメント

### MonotonePriorityDeque

```cpp
template <class T, class Compare = less<T>>
struct MonotonePriorityDeque;
```

`deque` に `Compare` 順で格納するコンテナ。
`PriorityContainer` で包むと、両端を持つ優先度付きコンテナとして使える。
順序を保てる追加かどうかは、両端との比較で確認する。

#### コンストラクタ

```cpp
MonotonePriorityDeque();
explicit MonotonePriorityDeque(const Compare& comp);
```

空で初期化する。

##### 計算量

- $O(1)$。

#### push / front / back / pop_front / pop_back / size / empty / clear

```cpp
void push(const T& x);
T front() const;
T back() const;
void pop_front();
void pop_back();
int size() const;
bool empty() const;
void clear();
```

順序を保つ端へ追加する。`front`, `back` は最初・最後の値を返す。
`pop_front`, `pop_back` は対応する端を削除する。

##### 制約

- 各追加値が現在の最初以下、または最後以上に置ける。大小は `Compare` 順で判定する。
- 端の取得・削除では要素がある。

##### 計算量

- `clear` は $O(n)$。その他は $O(1)$。
