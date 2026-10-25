このドキュメントには AI による作成・変更が含まれています。

## 概要

添字付きの Fibonacci heap。各要素に $[0,n)$ の固定の添字を割り当て、`decrease_key` をならし $O(1)$ で行う。既定では最小値を取り出す。Dijkstra 法のキューに使うと、最短路探索を $O(m+n\log n)$ にできる。

ノードは配列に保持し、親・子・兄弟を添字で参照する。追加メモリは $O(n)$。一般のヒープよりノードの情報量が多く、実測の速さは更新の回数に依存する。

## 使用例

```cpp
FibonacciHeap<ll> que(5);
que.push(2, 10);             // 添字2、キー10
que.push(4, 7);
que.decrease_key(2, 3);
auto [key, id] = que.top();  // {3, 2}
que.pop();
que.push_or_decrease(4, 5);
```

## 詳細なドキュメント

### FibonacciHeap

```cpp
template <class Key, class Compare = less<Key>>
class FibonacciHeap;
```

`Compare(a,b)` が真となるキーを優先する。`less<Key>` なら最小ヒープ、`greater<Key>` なら最大ヒープ。`decrease_key` はこの比較における優先度を上げる操作となる。`Key` はデフォルト構築・コピー・代入ができ、`Compare` は狭義弱順序を定めるものとする。

#### コンストラクタ

```cpp
explicit FibonacciHeap(int n = 0, Compare comp = Compare{});
```

添字 $0,\ldots,n-1$ を使える空のヒープを作る。

##### 制約

- $n\geq0$

##### 計算量

- $O(n)$

#### size / empty / contains / key / top

```cpp
int size() const;
bool empty() const;
bool contains(int id) const;
const Key &key(int id) const;
pair<Key, int> top() const;
```

要素数、空かどうか、添字の存在、添字に対応するキー、最優先の `(キー, 添字)` を返す。同じキーを持つ要素の順序は任意。

##### 制約

- `contains`・`key`：$0\leq\mathrm{id}<n$
- `key`：指定した添字が存在する
- `top`：ヒープが空でない

##### 計算量

- $O(1)$

#### push / decrease_key / push_or_decrease

```cpp
void push(int id, const Key &key);
void decrease_key(int id, const Key &key);
void push_or_decrease(int id, const Key &key);
```

`push` は要素を追加し、`decrease_key` は既存のキーの優先度を上げる。`push_or_decrease` は存在する場合に更新し、それ以外では追加する。取り出した添字は再び使える。

##### 制約

- $0\leq\mathrm{id}<n$
- `push`：指定した添字が存在しない
- `decrease_key`：指定した添字が存在する
- 更新時は `Compare(古いキー, 新しいキー) == false`

##### 計算量

- ならし $O(1)$

#### pop

```cpp
void pop();
```

最優先の要素を取り出す。

##### 制約

- ヒープが空でない

##### 計算量

- ならし $O(\log n)$

#### clear

```cpp
void clear();
```

全要素を除き、同じ添字の範囲で再利用できる状態にする。

##### 計算量

- $O(n)$
