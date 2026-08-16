## 概要

open addressing と linear probing による高速なハッシュマップ。

容量を2冪にし、load factor を最大 $0.7$ に保つ。各スロットにハッシュ値の7 bit fingerprint を持ち、fingerprint が一致した場合だけキーを比較する。削除後は backward shift によって衝突列を詰めるため、tombstone は残らない。

デフォルトのハッシュ関数は `safe_hash` であり、衝突攻撃への耐性を持つ。

イテレータは実装していない。キーと値は配列上に直接確保するため、`Key` と `Value` はデフォルト構築可能である必要がある。

## 使用例

```cpp
HashMap<pair<ll, ll>, int> mp;
mp.reserve(q);
mp[{x, y}]++;

if (int* p = mp.find_ptr({a, b}))
  cout << *p << '\n';
```

## 詳細なドキュメント

### HashMap

```cpp
HashMap<Key, Value, Hash = safe_hash, Equal = equal_to<Key>>
```

##### 制約

- `Key`, `Value` はデフォルト構築可能かつ代入可能
- `Hash(key)` は同じキーに同じ値を返す
- `Equal` はキーの同値関係を表す

#### reserve

```cpp
void reserve(size_t n)
```

`n` 要素を追加の再ハッシュなしで格納できる容量を確保する。現在より容量を減らすことはない。

この呼び出しおよびその後の再ハッシュで、以前に取得した値へのポインタと参照は無効になる。

##### 計算量

- 現在の容量と要素数に比例

#### size / empty

```cpp
size_t size() const
bool empty() const
```

格納している要素数、または空かを返す。

##### 計算量

- $O(1)$

#### find_ptr

```cpp
(1) Value* find_ptr(const Key& key)
(2) const Value* find_ptr(const Key& key) const
```

`key` が存在すれば対応する値へのポインタ、存在しなければ `nullptr` を返す。

##### 計算量

- 期待 $O(1)$、最悪 $O(n)$

#### contains

```cpp
bool contains(const Key& key) const
```

`key` が存在すれば `true` を返す。

##### 計算量

- 期待 $O(1)$、最悪 $O(n)$

#### at

```cpp
(1) Value& at(const Key& key)
(2) const Value& at(const Key& key) const
```

`key` に対応する値を返す。

##### 制約

- `key` が存在する

##### 計算量

- 期待 $O(1)$、最悪 $O(n)$

#### operator[]

```cpp
Value& operator[](const Key& key)
```

`key` に対応する値を返す。存在しなければ `Value{}` を挿入する。

##### 計算量

- 償却期待 $O(1)$、最悪 $O(n)$

#### insert

```cpp
pair<Value*, bool> insert(const Key& key, const Value& value)
```

`key`, `value` を挿入する。戻り値は対応する値へのポインタと、新しく挿入したかを表す値。`key` が既に存在する場合、値を上書きしない。

##### 計算量

- 償却期待 $O(1)$、最悪 $O(n)$

#### insert_or_assign

```cpp
bool insert_or_assign(const Key& key, const Value& value)
```

`key` に `value` を設定する。新しく挿入した場合は `true`、既存の値を更新した場合は `false` を返す。

##### 計算量

- 償却期待 $O(1)$、最悪 $O(n)$

#### erase

```cpp
bool erase(const Key& key)
```

`key` を削除する。存在していた場合は `true` を返す。

##### 計算量

- 期待 $O(1)$、最悪 $O(n)$

#### clear

```cpp
void clear()
```

全要素を削除する。確保した容量は維持する。

##### 計算量

- 容量に比例

### 性能比較

`benchmark/hash_map.cpp` は `unordered_map`、`gp_hash_table`、`HashMap` に同じ `safe_hash` と事前容量確保を使い、整数キーと `pair` キーを比較する。コンパイル例は次の通り。

```sh
g++-15 -std=c++20 -O3 -march=native -I. benchmark/hash_map.cpp
```
