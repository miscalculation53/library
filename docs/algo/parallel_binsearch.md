このドキュメントには AI による作成・変更が含まれています。

## 概要

$q$ 個の関数 $\mathrm{judge}_i\colon$ `T` $\to$ `bool` があり、いずれも $\mathrm{judge}_i(\textrm{init\\_ok}) = \mathrm{true}$, $\mathrm{judge}_i(\textrm{init\\_ng}) = \mathrm{false}$ を満たしているとする。このとき各 $i$ について $\mathrm{judge}_i(\mathrm{ok}_i) = \mathrm{true}$, $\mathrm{judge}_i(\mathrm{ng}_i) = \mathrm{false}$, $\lvert \mathrm{ok}_i - \mathrm{ng}_i \rvert = 1$ を満たす $\mathrm{ok}_i, \mathrm{ng}_i$ を求めたい。

これは、$q$ 個の $\mathrm{judge}_i(x_i)$ たちをまとめて求めることが高速にできればよい。（$1$ 個ずつ求めることが可能な場合はそもそも普通の二分探索でよいので、このパートの解法は自然と クエリ先読み + イベントソート のようになる。）

引数に渡す `judge` は、$x_i$ たちを受け取り、 $\mathrm{judge}_i(x_i)$ たちをまとめて求めて返す関数。

返り値は、first が $\mathrm{ok}_i$ を並べたもの、second が $\mathrm{ng}_i$ を並べたもの。

使う際の注意：**`judge` の中で長い vector を宣言しない（外で宣言する）**。典型定数倍高速化テクではあるのだが、思ったより変わるので注意。

## 使用例

```cpp
#include "algo/parallel_binsearch.hpp"

vc<ll> threshold{2, 5, 8};
auto judge = [&](const vc<ll>& xs) {
  vc<bool> answers(xs.size());
  for (int i = 0; i < int(xs.size()); ++i)
    answers[i] = xs[i] >= threshold[i];
  return answers;
};
auto [ok, ng] = parallel_binsearch(3, judge, 10, -1);
assert(ok == threshold);
assert(ng == vc<ll>({1, 4, 7}));
```

## 詳細なドキュメント

#### parallel_binsearch

```cpp
pair<vc<T>, vc<T>> parallel_binsearch(int q, auto judge, T init_ok, T init_ng)
```

##### 計算量

`judge` の計算量が $O(q + T)$ であるとき

- $O((q + T)\log \lvert \textrm{init\\_ok} - \textrm{init\\_ng} \rvert)$

#### parallel_binsearch_real

```cpp
vc<T> parallel_binsearch_real<T = ld>(
    int q, auto judge, T init_ok, T init_ng,
    int iteration_count = 100, bool check_ok = true, bool check_ng = true)
```

各クエリの単調な実数判定を `iteration_count` 回まとめて二分探索し、条件を満たす側の端点を返す。`judge` は長さ $q$ の中点列から、同じ長さの真偽列を返す。

##### 制約

- 各判定は区間内で単調であり、`init_ok` は真、`init_ng` は偽となる
- $0 \leq \mathrm{iteration\\_count}$

##### 計算量

- `judge` 1 回の時間を $J$、反復回数を $k$ として $O((q+J)k)$
