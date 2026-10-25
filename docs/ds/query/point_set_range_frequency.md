このドキュメントには AI による作成・変更が含まれています。

## 概要

一点更新と区間内の値の出現回数を、クエリの先読みによりまとめて処理する。

値ごとの更新候補の添字をCSRにまとめ、1本の01列用Fenwick Treeで管理する。更新先の位置も前計算し、更新時の二分探索と値ごとの配列確保を省く。

## 使用例

```cpp
#include "ds/query/point_set_range_frequency.hpp"

PointSetRangeFrequency<int> queries(vc<int>{1, 2, 1});
queries.range_frequency(0, 3, 1);
queries.point_set(1, 1);
queries.range_frequency(0, 3, 1);
assert(queries.run() == (vc<ll>{2, 3}));
```

## 詳細なドキュメント

### PointSetRangeFrequency

#### コンストラクタ

```cpp
PointSetRangeFrequency(vc<T> a)
```

列を $a$ で初期化する。

##### 制約

- `T` の比較と等値比較が整合する全順序を持つこと。

##### 計算量

- $O(\lvert a \rvert)$

#### point_set

```cpp
void point_set(int p, T v)
```

「添字 $p$ の値を $v$ に更新する」というクエリを追加する。

##### 制約

- $0 \leq p < \lvert a \rvert$

##### 計算量

- $O(1)$

#### range_frequency

```cpp
void range_frequency(int l, int r, T v)
```

「区間 $[l, r)$ の中で値が $v$ である添字の個数を求める」というクエリを追加する。

##### 制約

- $0 \leq l \leq r \leq \lvert a \rvert$

##### 計算量

- $O(1)$

#### run

```cpp
vc<I=ll> run()
```

すべてのクエリにまとめて答え、結果を返す。結果は呼び出し順に格納される。

登録済みのクエリを初期列から処理するため、同じオブジェクトで再実行できる。`I` は各回答を表現できる整数型を指定する。

##### 計算量

$n = \lvert a \rvert$、$q$ をクエリの個数として

- $O((n + q) \log (n + q))$
