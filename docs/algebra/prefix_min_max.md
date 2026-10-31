このドキュメントには AI による作成・変更が含まれています。

## 概要

列の総和と、先頭からの累積和の最小値・最大値を保持するモノイド。第2テンプレート引数 `include_empty` で、空の累積和 $0$ を含めるかを毎回指定する。

列 $a_0,\ldots,a_{n-1}$ に対して $p_0=0$, $p_k=\sum_{i=0}^{k-1}a_i$ とおく。`sum` は $p_n$ を表す。`include_empty = true` では $0\leq k\leq n$、`false` では $1\leq k\leq n$ における $p_k$ の最小値・最大値を `mn`, `mx` に保持する。`prod(l, r)` の累積和は、区間の左端 `l` を起点とする。

結合では左の列の後ろに右の列をつなぐ。右の列の累積和に左の総和を足すことで、全体の極値を求める。結合順序に意味がある。

`T` は `T{}` を加法単位元とし、加算と大小比較を持つ型とする。加算は結合的で順序を保ち、総和・累積和・中間計算が `T` に収まることを前提とする。

`include_empty = false` では、空列の最小値を $+\infty$、最大値を $-\infty$ として扱う。無限大は [resolved_infty](../utils/resolved_infty.md) で取得し、第3テンプレート引数 `infty` に値または関数を指定できる。`T` には無限大との等価比較も必要で、最大値を持つ型では負の無限大を表す単項マイナスも使う。有限の番兵を使う場合は、最小値を持つ型では実際の累積和より大きい $+\infty$、最大値を持つ型では実際の累積和より小さい $-\infty$ を選ぶ。`include_empty = true` では加算と大小比較だけを使う。

## 使用例

```cpp
#include "algebra/prefix_min_max.hpp"
#include "ds/segtree/segtree.hpp"

using M = MonoidPrefixMinMax<ll, true>;
SegmentTree<M> seg(vc<ll>{3, -5, 4});
auto x = seg.prod(0, 3);  // 累積和は 0, 3, -2, 2
assert(x.sum == 2 && x.mn == -2 && x.mx == 3);

seg.set(1, -1);
assert(seg.prod(0, 3).mn == 0);
assert(seg.prod(1, 3).mx == 3);  // 累積和は 0, -1, 3
```

最小値だけなら `MonoidPrefixMin<T, include_empty>`、最大値だけなら `MonoidPrefixMax<T, include_empty>` を使う。

空を除く最大値は、累積和が初めて $0$ 以上になる位置の探索にも使える。[ABC292 Ex - Rating Estimator](https://atcoder.jp/contests/abc292/tasks/abc292_h) では、各要素から基準値を引くとこの探索になる。

```cpp
using M = MonoidPrefixMax<ll, false>;
SegmentTree<M> seg(vc<ll>{-3, 1, 4});
assert(seg.prod(0, 2).mx == -2);  // 空を除く累積和は -3, -2
int p = seg.max_right_ok(0, [](const auto& x) { return x.mx < 0; });
assert(p == 2);  // 最初に累積和が 0 以上になる要素の添字
```

## 詳細なドキュメント

### MonoidPrefixMin / MonoidPrefixMax / MonoidPrefixMinMax

```cpp
MonoidPrefixMin<T, include_empty, infty = nullptr>
MonoidPrefixMax<T, include_empty, infty = nullptr>
MonoidPrefixMinMax<T, include_empty, infty = nullptr>
```

要素型 `S` のメンバは次のとおり。

| 型 | メンバ |
| --- | --- |
| `MonoidPrefixMin<T, include_empty, infty>::S` | `T sum, mn` |
| `MonoidPrefixMax<T, include_empty, infty>::S` | `T sum, mx` |
| `MonoidPrefixMinMax<T, include_empty, infty>::S` | `T sum, mn, mx` |

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T x)
(3) S(T sum, T mn_or_mx)  // Min / Max
(4) S(T sum, T mn, T mx)  // MinMax
```

(1) は空列、(2) は値 `x` の一点を構築する。`include_empty = true` の一点では `mn = min(T{}, x)`, `mx = max(T{}, x)`、`false` では `mn = mx = x` となる。(3), (4) は対応する列の集約値を直接指定する。各型は対応するメンバだけを保持する。いずれも `constexpr` である。

##### 計算量

- $O(1)$

#### op

```cpp
static constexpr S op(const S& a, const S& b)
```

`a` の列の後ろに `b` の列を連結した集約値を返す。

$$
\begin{aligned}
\mathrm{sum} &= a.\mathrm{sum}+b.\mathrm{sum}, \\
\mathrm{mn} &= \min(a.\mathrm{mn},a.\mathrm{sum}+b.\mathrm{mn}), \\
\mathrm{mx} &= \max(a.\mathrm{mx},a.\mathrm{sum}+b.\mathrm{mx}).
\end{aligned}
$$

各型は対応するメンバだけを計算する。`include_empty = false` では、空列との結合はもう一方を返し、無限大の番兵を加算から除く。

##### 計算量

- $O(1)$

#### e

```cpp
static constexpr S e()
```

空列の集約値を返す。`sum` は `T{}`。`include_empty = true` では `mn`, `mx` も `T{}`、`false` では `mn` は `resolved_infty<T, infty>()`、`mx` はその符号を反転した値となる。

##### 計算量

- $O(1)$

### 作用付きモノイドへの拡張例

以下は、このモノイドを拡張するときに使える作用の例である。

空列には常に `e()` を対応させる。以下の更新式は非空列に対して使う。

#### 区間の定数倍

区間内の各要素を $c$ 倍すると、すべての累積和も $c$ 倍される。$c\geq 0$ では `sum`, `mn`, `mx` をそれぞれ $c$ 倍する。$c<0$ も扱う場合は `MonoidPrefixMinMax` を使い、更新前の値から次のように求める。

$$
(\mathrm{sum},\mathrm{mn},\mathrm{mx})\gets(c\,\mathrm{sum},c\,\mathrm{mx},c\,\mathrm{mn}).
$$

作用の合成は係数の積、恒等作用は $1$ となる。たとえば括弧を $+1,-1$ と表すと、区間内の括弧の反転は $c=-1$ に対応する。区間が正しい括弧列である条件は `sum == 0 && mn == 0`。

#### 区間代入

区間長 `len` を追加で保持すると、各要素を $c$ に代入する作用を定義できる。$s=c\,\mathrm{len}$ として、`include_empty = true` では集約値を次のように置き換える。

$$
(\mathrm{sum},\mathrm{mn},\mathrm{mx})\gets(s,\min(0,s),\max(0,s)).
$$

`include_empty = false` では次の式になる。

$$
(\mathrm{sum},\mathrm{mn},\mathrm{mx})\gets(s,\min(c,s),\max(c,s)).
$$

最小値だけ・最大値だけの型にも適用できる。作用の合成では後から行う代入を採用する。

#### 区間加算に必要な情報

各要素への $c$ の加算では、長さ $k$ の累積和が $ck$ だけ変わるため、極値を取る位置も変わる。

たとえば $(-2,2,0)$ と $(0,-2,2)$ は、長さ・総和・累積和の最小値・最大値がすべて等しい。各要素に $1$ を加えると、累積和の最小値はそれぞれ $-1$ と $0$ になる。このため、区間加算には極値と区間長以外の情報も必要になる。

### 各型の pow

```cpp
template <class I>
static constexpr S pow(const S& x, I k);
```

3種類に共通する。列を `k` 回連結した集約値を返す。非空列を正の回数だけ反復するときは総和を `k` 倍し、最小値に $\min(0,(k-1)x.\mathrm{sum})$、最大値に $\max(0,(k-1)x.\mathrm{sum})$ を加える。指数 $k$ は非負整数とし、`k = 0` または空列の反復なら `e()` を返す。型と整数倍の条件は [pow の共通仕様](monoid_power.md) を参照。

##### 計算量

- $O(1)$。整数倍を使う場合は、その演算を $O(1)$ とする。
