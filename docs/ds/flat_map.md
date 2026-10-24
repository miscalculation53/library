このドキュメントには AI による作成・変更が含まれています。

## 概要

座圧と同じ仕組み、map と似たインターフェース

おそらく C++ に追加予定の `flat_map` と似た思想？

## 使用例

```cpp
#include "ds/flat_map.hpp"

FlatMap<int, ll> counts(vc<int>{10, 20, 10});
counts[10] += 3;
counts[20] = 5;
assert(counts.size() == 2);
assert(counts.at(10) == 3);
```

## 詳細なドキュメント

### FlatMap

#### コンストラクタ

```cpp
FlatMap<Key, Value>(vc<Key> keys)
```

存在するキーすべてを格納したもの（順番や重複の有無は問わない）を渡して初期化する。

`[]`, `at`, `contains`, `size`, `empty` が使える。

また簡単なイテレータを実装しているので範囲 for が回せる。

#### content

```cpp
vc<pair<Key, Value>> content() const
```

すべてのキーと現在値をキーの昇順で返す。構築時に指定した各キーを含み、値を変更していないキーの値は `Value{}` となる。

##### 計算量

- $O(n)$。$n$ は異なるキーの個数
