## 概要

デフォルト値を指定できる map。Python の defaultdict のように、未登録のキーへのアクセス時に指定した値を使う。

`ds/default_map.hpp` を読み込むと、次の3種類を利用できる。

| 型 | 内部で使うコンテナ |
| --- | --- |
| `DefaultMap` | `std::map` |
| `DefaultUnorderedMap` | `std::unordered_map`（ハッシュ関数は `safe_hash`） |
| `DefaultHashMap` | 自作の `HashMap` |

## 使用例

```cpp
DefaultMap<ll, ll, INF> mp;
DefaultUnorderedMap<ll, ll, -1> ump;
DefaultHashMap<ll, ll, INF> hmp;

chmin(mp[10], 100);
hmp.reserve(1000);
chmin(hmp[20], 200);

// ラムダ式も渡せる（C++20 以降）
DefaultMap<ll, ll, []() { return INF; }> mp2;
DefaultHashMap<ll, vl, []() { return vl{100, 200}; }> lists;
lists[0].push_back(300);
```

## 詳細なドキュメント

#### コンストラクタ

```cpp
DefaultMap<Key, Value, default_value>()
DefaultUnorderedMap<Key, Value, default_value>()
DefaultHashMap<Key, Value, default_value, Hash = safe_hash, Equal = equal_to<Key>>()
```

第3テンプレート引数 `default_value` には、次のいずれかを指定する。

- 値：非型テンプレート引数として渡せる定数。`Value(default_value)` をデフォルト値とする。
- 関数：引数なしで呼び出せて、返り値を `Value` に暗黙変換できる関数。キャプチャなしラムダ式は C++20 以降で渡せる。

関数は未登録のキーへのアクセスごとに呼び出す。登録済みのキーへのアクセスでは呼び出さない。
`vector` など、値を直接テンプレート引数にできない型には関数を使う。

#### operator[] / at

```cpp
Value& operator[](const Key& key)
Value operator[](const Key& key) const
Value& at(const Key& key)
Value at(const Key& key) const
```

登録済みのキーなら、その値を返す。未登録の場合の動作は次の通り。

- 非 `const`：デフォルト値を挿入し、その値への参照を返す。
- `const`：デフォルト値を値として返す。キーは挿入しない。

`at` も `operator[]` と同じ動作をする。

##### 計算量

- `DefaultMap`：$O(\log(n + 1))$
- `DefaultUnorderedMap` / `DefaultHashMap`：償却期待 $O(1)$、最悪 $O(n)$

デフォルト値の生成や値のコピーにかかる時間は別途必要。

#### その他の操作

内部のコンテナの操作をそのまま利用できる。
`DefaultHashMap` では `reserve`, `find_ptr`, `contains`, `insert`, `insert_or_assign`, `erase`, `clear`, `size`, `empty`, `begin`, `end`, `cbegin`, `cend` が使える。

`for (auto& [key, val] : hmp)` で登録済みの要素を走査できる。走査によるデフォルト値の生成やキーの挿入は行わない。

`DefaultHashMap` の型の制約、イテレータ・ポインタ・参照の無効化、各操作の詳細は [HashMap](hash_map.md) を参照。
