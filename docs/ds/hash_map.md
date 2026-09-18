## 概要

open addressing と linear probing による高速なハッシュマップ。

容量を2冪にし、load factor を最大 $0.7$ に保つ。各スロットにハッシュ値の7 bit fingerprint を持ち、fingerprint が一致した場合だけキーを比較する。削除後は backward shift によって衝突列を詰めるため、tombstone は残らない。

デフォルトのハッシュ関数は `safe_hash` であり、衝突攻撃への耐性を持つ。

範囲 `for` による走査に対応する。キーと値は配列上に直接確保するため、`Key` と `Value` はデフォルト構築可能である必要がある。

## 使用例

```cpp
HashMap<pair<ll, ll>, int> mp;
mp.reserve(q);
mp[{x, y}]++;

if (int* p = mp.find_ptr({a, b}))
  cout << *p << '\n';

for (const auto& [key, val] : mp)
  cout << key.first << ' ' << key.second << ' ' << val << '\n';

for (auto& [key, val] : mp)
  val++;  // 値を更新できる。key は変更しない。
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

容量が増えた場合、およびその後の再ハッシュで、以前に取得したイテレータ・値へのポインタ・参照は無効になる。

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

#### begin / end / cbegin / cend

```cpp
iterator begin()
iterator end()
const_iterator begin() const
const_iterator end() const
const_iterator cbegin() const
const_iterator cend() const
```

登録済みの要素だけを走査する前方イテレータを返す。走査順は未規定。
`*it` は非 `const` では `pair<Key, Value>&`、`const` では `const pair<Key, Value>&` を返す。
`it->first`, `it->second`、前置・後置 `++`、等値比較を使える。`iterator` から `const_iterator` に変換できる。

範囲 `for` では次の書き方を使える。

- `for (auto [key, val] : mp)`：要素をコピーして使う。
- `for (auto& [key, val] : mp)`：格納された値を更新できる。
- `for (const auto& [key, val] : mp)`：要素をコピーせず読み取る。`const` なマップでも使える。

イテレータ経由で格納されたキーを変更してはいけない。型としては書き換え可能だが、変更するとハッシュ値と格納位置の対応が崩れる。
要素の値だけの更新はイテレータを無効化しない。
再ハッシュ、要素の削除、`clear` はイテレータを無効化するため、走査中には行わない。

##### 計算量

- 全要素の走査：確保済みの容量に比例（空きスロットを含む）
- `begin` / `cbegin` / `++`：最悪で容量に比例
- `end` / `cend` / 参照 / 比較：$O(1)$

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
