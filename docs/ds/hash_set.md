## 概要

open addressing と linear probing によるハッシュセット。[HashMap](hash_map.md) と同じ方式で、キーだけを配列上に格納する。

容量を2冪にし、load factor を最大 $0.7$ に保つ。各スロットに7 bit fingerprint を持ち、fingerprint が一致した場合だけキーを比較する。削除後は backward shift によって衝突列を詰める。

デフォルトのハッシュ関数は `safe_hash`。整数のほか、`pair`, `tuple`, `string` などをキーに使える。範囲 `for` に対応し、キーは `const` 参照で読み取る。

以下では、$n$ を格納している要素数、$C$ を確保済みの容量とする。期待計算量はハッシュ値が十分に分散する場合のもの。ハッシュ・比較・キーの構築・代入の計算量を $O(1)$ として記載する。

## 使用例

```cpp
HashSet<ll> st;
st.reserve(100);
auto [p, inserted] = st.insert(10);
assert(inserted && *p == 10);
assert(st.contains(10));
st.erase(10);

HashSet<pair<ll, ll>> points;
points.insert({2, 3});
for (const auto& [x, y] : points)
  cout << x << ' ' << y << '\n';
```

UnionFind などで集合を併合する場合は、小さい集合から大きい集合へ挿入する。

```cpp
// a, b は別の集合。
if (a.size() < b.size()) a.swap(b);
for (const auto& key : b) a.insert(key);
b.clear();
```

## 詳細なドキュメント

### HashSet

```cpp
HashSet<Key, Hash = safe_hash, Equal = equal_to<Key>>
```

##### 制約

- `Key` はデフォルト構築可能、コピー構築・コピー代入・ムーブ代入可能
- `Hash`, `Equal` はデフォルト構築可能、コピー可能、交換可能
- `Hash(key)` は同じキーに同じハッシュ値を返す
- `Equal` はキーの同値関係を表し、同値なキーのハッシュ値は等しい

#### コンストラクタ

```cpp
HashSet()
```

空の集合を作る。最初の挿入または `reserve` で容量を確保する。
コピーは独立した集合を作る。ムーブ後の元の集合は空になる。

##### 計算量

- 空の集合の構築、ムーブ構築：$O(1)$
- コピー構築：$O(C_{\mathrm{src}})$
- コピー代入：$O(C_{\mathrm{src}} + C_{\mathrm{dst}})$
- ムーブ代入：代入先が元々確保していた容量に比例

#### reserve

```cpp
void reserve(size_t n)
```

`n` 要素を追加の再ハッシュなしで格納できる容量を確保する。確保済みの容量が十分な場合はそのまま使う。

容量が増えると、以前に取得したイテレータ・ポインタ・参照は無効になる。

##### 計算量

- 容量を増やす場合：期待 $O(C_{\mathrm{old}} + C_{\mathrm{new}})$、最悪 $O(C_{\mathrm{old}} + C_{\mathrm{new}} + n^2)$（ここで $n$ は現在の要素数）
- 容量が十分な場合：$O(\log(n + 1))$（ここで $n$ は引数）

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
iterator begin() const
iterator end() const
const_iterator cbegin() const
const_iterator cend() const
```

登録済みのキーだけを走査する前方イテレータを返す。走査順は未規定。
`iterator` と `const_iterator` は同じ型で、`*it` は `const Key&`、`it->` は `const Key*` を返す。
前置・後置 `++`、等値比較を使える。

再ハッシュ、要素の削除、`clear`, `swap`, ムーブはイテレータを無効化する。挿入は再ハッシュを伴う場合にイテレータを無効化する。

##### 計算量

- 全要素の走査：$O(C)$
- `begin` / `cbegin` / `++`：最悪 $O(C)$
- `end` / `cend` / 参照 / 比較：$O(1)$

#### find_ptr / contains

```cpp
const Key* find_ptr(const Key& key) const
bool contains(const Key& key) const
```

`find_ptr` は `key` と同値なキーが存在すれば格納されたキーへのポインタ、存在しなければ `nullptr` を返す。
`contains` は存在する場合に `true` を返す。

再ハッシュ、要素の削除、`clear` で、以前に取得したポインタ・参照は無効になる。`swap` とムーブでは、ポインタ・参照は交換先・移動先の要素を指す。ムーブ代入では、代入先が元々持っていた要素へのポインタ・参照が無効になる。

##### 計算量

- 期待 $O(1)$、最悪 $O(n)$

#### insert

```cpp
pair<const Key*, bool> insert(const Key& key)
```

`key` を挿入する。戻り値は格納されたキーへのポインタと、新しく挿入したかを表す値。同値なキーが既にある場合は、そのキーを保持する。

##### 計算量

- 償却期待 $O(1)$
- 再ハッシュを伴う1回の挿入：期待 $O(n)$、最悪 $O(n^2)$

#### erase

```cpp
bool erase(const Key& key)
```

`key` と同値なキーを削除する。存在していた場合は `true` を返す。

##### 計算量

- 期待 $O(1)$、最悪 $O(n)$

#### clear

```cpp
void clear()
```

全要素を削除する。確保した容量は維持する。

##### 計算量

- $O(C)$

#### swap

```cpp
void swap(HashSet& other)
void swap(HashSet& a, HashSet& b)
```

要素、容量、ハッシュ関数、比較関数を交換する。

##### 計算量

- $O(1)$（`Hash`, `Equal` の交換を $O(1)$ とする）

### 性能比較

`benchmark/hash_set.cpp` は、20万個の頂点に1キーずつ持たせ、成分を2倍ずつ併合する処理を `set`, `unordered_set`, `HashSet` で比較する。ハッシュ集合には同じ `safe_hash` を使い、全キーが異なる場合と1000種類のキーを繰り返す場合を測る。

```sh
g++-15 -std=c++20 -O3 -march=native -I. benchmark/hash_set.cpp
```
