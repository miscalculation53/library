このドキュメントには AI による作成・変更が含まれています。

## 概要

数学的な背景：[LIS の候補についての解説](https://miscalc.hatenablog.com/entry/2024/07/25/212618)。

## 使用例

```cpp
#include "algo/lis.hpp"

LIS lis(vc<int>{3, 1, 2, 4});
assert(lis.len_of_lis() == 3);
assert(lis.pos_in_lis(0) == -1);
for (int i : lis.candidates(0)) assert(i == 1);
```

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) LIS()
(2) LIS(const vc<T>& a, Compare comp = Compare())
```

- (1)：デフォルトコンストラクタ。
- (2)：列 `a` について、`comp(a[i], a[j])` を増加関係とした最長増加部分列を前計算する。`Compare` のデフォルトは `less<T>`。

##### 制約

- `comp` は `less{}`, `greater{}`, `less_equal{}`, `greater_equal{}` のいずれか

##### 計算量

`a` の長さを $n$ として、

- $O(n \log n)$

#### len_of_lis

```cpp
I len_of_lis<I = ll>()
```

LIS の長さを返す。

##### 計算量

- $O(1)$

#### pos_in_lis

```cpp
I pos_in_lis<I = ll>(int i)
```

`a[i]` がいずれかの LIS に使われ得る場合、その LIS 内での位置を $0$ 始まりで返す。使われ得ない場合は $-1$ を返す。

##### 制約

- $0 \leq i < n$

##### 計算量

- $O(1)$

#### lds_id

```cpp
I lds_id<I = ll>(int i)
```

`a[i]` で終わる最長部分列の長さから1を引いた値を返す。同じ番号の添字を昇順に並べると、隣り合う値の `comp` は偽となる。

##### 制約

- $0 \leq i < n$

##### 計算量

- $O(1)$

#### candidates

```cpp
auto candidates(int j)
```

いずれかの LIS の $j$ 番目として使われ得る添字を、昇順に走査できる範囲として返す。

##### 制約

- $0 \leq j < \mathrm{len\_of\_lis}()$

##### 計算量

- $O(1)$
