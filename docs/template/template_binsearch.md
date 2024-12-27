## テンプレート（二分探索）

### LB, UB

```cpp
(1) U=ll LB(V v, T val, ...)
(2) U=ll UB(V v, T val, ...)
```

`ranges::lower_bound` や `ranges::upper_bound` で返るイテレータの `begin` からの距離を整数で返す。

$v$ がソート済みで、後ろの引数がデフォルトの場合、

- (1)：$\mathrm{val} \leq v_i$ となる最小の $i$
  - これは $\mathrm{val}$ 未満の値の個数に一致

- (2)：$\mathrm{val} < v_i$ となる最小の $i$
  - これは $\mathrm{val}$ 以下の値の個数に一致

を返す。

後ろには C++17 以前なら `std::lower_bound`, C++20 以降なら `std::ranges::lower_bound` の引数をとれる。

##### 制約

- `V` のイテレータはランダムアクセスイテレータ
  - OK 例：vector など
  - NG 例：set など
- $v$ はソート済みであることを想定（実際はもう少し緩い）

##### 計算量

- $O(\log \lvert v \rvert)$

### lt, leq, gt, geq

```cpp
(1) lt_max(V v, T val, ...)
(2) leq_max(V v, T val, ...)
(3) gt_max(V v, T val, ...)
(4) geq_max(V v, T val, ...)
(5) U=ll lt_cnt(V v, T val, ...)
(6) U=ll leq_cnt(V v, T val, ...)
(7) U=ll gt_cnt(V v, T val, ...)
(8) U=ll geq_cnt(V v, T val, ...)
```

- (1), (2), (3), (4)：次の条件を満たす $v$ の要素を探し、`V` が vector など（ランダムアクセスイテレータ）なら添字を、set など（ランダムアクセス不可で、メンバ関数に `lower_bound` や `upper_bound` を持つ）ならイテレータを返す。前者のときは $v$ がソート済みであることを想定。
  - (1)：$\mathrm{val}$ 未満で最大
  - (2)：$\mathrm{val}$ 以下で最大
  - (3)：$\mathrm{val}$ 超過で最小
  - (4)：$\mathrm{val}$ 以上で最小
  
  そのような要素が存在しない場合は、
  - 添字が返るときは、配列が無限に伸びている（負方向には $-\infty$ で正方向には $\infty$）とみなして計算し、返す（つまり、$-1$ か $n$ が返る）。
  - イテレータが返るときは、前か後ろかにかかわらず `end()` を返す。

- (5), (6), (7), (8)：次の条件を満たす $v$ の要素の個数を返す。ソート済みであることを想定。
  - (5)：$\mathrm{val}$ 未満
  - (6)：$\mathrm{val}$ 以下
  - (7)：$\mathrm{val}$ 超過
  - (8)：$\mathrm{val}$ 以上

##### 制約

- (1), (2), (3), (4)：`V` は次のいずれかを満たす。
  - `V` のイテレータはランダムアクセスイテレータ
  - `V` はメンバ関数に `lower_bound`, `upper_bound` を持つ
- (5), (6), (7), (8)：`V` のイテレータはランダムアクセスイテレータ
- (1), (2), (3), (4) の前者および (5), (6), (7), (8) では、$v$ がソート済みであることを想定（実際はもう少し緩い）


##### 計算量

- $O(\log \lvert v \rvert)$


### binsearch

```cpp
(1) T binsearch(F judge, T ok, T ng)
(2) T binsearch_real(F judge, T ok, T ng, int iteration_count)
```

- (1)：整数の二分探索。整数 `T` から `bool` への関数 $\mathrm{judge}$ が $\mathrm{judge}(\mathrm{ok}) = \mathrm{true}, \mathrm{judge}(\mathrm{ng}) = \mathrm{false}$ を満たすとする。このとき $\mathrm{ok}$ と $\mathrm{ng}$ の間にある整数 $\alpha, \beta$ であって $\mathrm{judge}(\alpha) = \mathrm{true}, \mathrm{judge}(\beta) = \mathrm{false}, \lvert \alpha - \beta \rvert = 1$ を満たすものが存在する（特に $\mathrm{judge}$ が単調な場合は唯一である）。そのような $\alpha$ を一つ返す。
- (2)：実数の二分探索。実数 `T` から `bool` への関数 $\mathrm{judge}$ が $\mathrm{judge}(\mathrm{ok}) = \mathrm{true}, \mathrm{judge}(\mathrm{ng}) = \mathrm{false}$ を満たすとする。$1$ 回の反復で、$\mathrm{judge}(\mathrm{ok}) = \mathrm{true}, \mathrm{judge}(\mathrm{ng}) = \mathrm{false}$ を満たしたまま $\lvert \mathrm{ok} - \mathrm{ng} \rvert$ を半分にできる。この反復を $\mathrm{iteration\_count}$ 回繰り返す。

##### 制約

- $\mathrm{judge}(\mathrm{ok}) = \mathrm{true}$
- $\mathrm{judge}(\mathrm{ng}) = \mathrm{false}$
- (1)：型 `T` は整数
- (1)：$\mathrm{ok}, \mathrm{ng}$ は型 `T` の上限・下限付近ではない
- (2)：型 `T` は実数

##### 計算量

$\mathrm{judge}(x)$ の実行にかかる時間を $T$ として

- (1)：$O(T \log \lvert \mathrm{ok} - \mathrm{ng} \rvert)$
- (2)：$O(T \cdot \mathrm{iteration\_count})$

#### expsearch

`T expsearch(F judge, T first = 0, bool positive = true)`

整数の指数探索。整数 `T` から `bool` への関数 $\mathrm{judge}$ は単調であり、$\mathrm{judge}$ の返り値が切り替わる境界（$\mathrm{judge}(\alpha) = \mathrm{true}, \mathrm{judge}(\beta) = \mathrm{false}, \lvert \alpha - \beta \rvert = 1$ を満たす唯一の整数組 $\alpha, \beta$）が

- $\mathrm{positive} = \mathrm{true}$ なら、$[\mathrm{first}, \infty)$ の範囲にある
- $\mathrm{positive} = \mathrm{false}$ なら、$(-\infty, \mathrm{first}]$ の範囲にある

とする。このとき、その $\alpha$ を返す。

最悪計算量のオーダーは二分探索と変わらない。二分探索と比べると、

- $\mathrm{ok}, \mathrm{ng}$ を詳細に見積もって指定する必要がない
- 答えが小さい場合に高速

である点が優れている。

ただし、判定関数内でオーバーフローしないように注意する必要はある。

##### 制約

- 型 `T` は整数
- $\mathrm{judge}$ は単調
- $\mathrm{positive} = \mathrm{true}$ なら、答えは $[\mathrm{first}, \infty)$ の範囲にある
- $\mathrm{positive} = \mathrm{false}$ なら、答えは $(-\infty, \mathrm{first}]$ の範囲にある
- 答えは `T` の範囲に収まる

##### 計算量

$\mathrm{judge}(x)$ の実行にかかる時間を $T$ として

- $O(T \log \lvert \mathrm{first} - \alpha \rvert)$
