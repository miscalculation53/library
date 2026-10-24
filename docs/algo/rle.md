このドキュメントには AI による作成・変更が含まれています。

## 概要

ランレングス圧縮

## 使用例

```cpp
#include "algo/rle.hpp"

auto runs = rle(string("aaabbc"));
assert(runs[0].val == 'a' && runs[0].len == 3);
assert(runs[1].left == 3 && runs[1].right == 5);
```

## 詳細なドキュメント

#### rle

```cpp
(1) template <class T, class I = ll, class Equal = equal_to<>>
    vc<rle_info<T, I>> rle(const vc<T>& a, Equal equal = {})
(2) template <class Equal = equal_to<>>
    vc<rle_info<char>> rle(const string& s, Equal equal = {})
```

ランレングス圧縮し、連ごとに

- 値 $\mathrm{val}$
- 長さ $\mathrm{len}$
- もとの列での区間 $[\mathrm{left}, \mathrm{right})$

を持った構造体を格納した vector を返す。

隣の要素を現在の連と同じとみなすかは `equal` で指定する。`equal(res.back().val, a[i])` が `true` なら現在の連に追加する。デフォルトでは `operator==` を使う。

##### 制約

- `equal` は要素に対する同値関係を表す

##### 計算量

vector や文字列の長さを $n$ として、

- `equal` の呼び出しを $O(n)$ 回
