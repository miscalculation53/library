このドキュメントには AI による作成・変更が含まれています。

## 概要

整数の分割を全探索

## 使用例

```cpp
#include "itertools/partition.hpp"

auto values = partitions<int>(4);
assert(values.size() == 5);
for (const auto& parts : values) assert(SUM(parts) == 4);
```

## 詳細なドキュメント

#### partitions

```cpp
vvc<I> partitions(I n)
```

整数 $n$ の分割すべてを格納した vector を返す。

##### 制約

- $n \geq 0$
