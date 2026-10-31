このドキュメントには AI による作成・変更が含まれています。

## 概要

ACL とだいたい同じだが、次の点が異なる：

- コンストラクタにモノイドを構造体として渡す（重要）
- コンストラクタにイテレータを渡すこともできる（必須ではないが便利）
  - たとえばモノイドにするときに列の要素に加えて長さなどを持っておかなければならないという場面がある。このような場合でも、列の要素を `S` のコンストラクタに渡せるように実装すれば、もとの vector をイテレータとしてセグメント木のコンストラクタに渡すことができる。

二分探索の名前は、区間積が条件を満たす境界を返すことを明示するため、ACL の `max_right`, `min_left` に `_ok` を付けている。返り値は要素の添字ではなく半開区間の境界である。

`prod_apply` は、区間を覆うノードの値を左から順に初期値へ作用させる。行列を格納した場合は、行ベクトルへ作用させることで答えの取得を高速化できる。

## 使用例

```cpp
vc<ll> a = {2, 1, 3, 2};
SegmentTree<MonoidAdd<ll>> seg(a);
auto g = [](ll sum) { return sum <= 4; };

assert(seg.max_right_ok(1, g) == 3);
assert(seg.min_left_ok(4, g) == 3);
```

行列を行ベクトルへ作用させる例：

```cpp
#include "math/linalg/matrix_array.hpp"

using Mat = MatrixArray<SemiRingMaxPlus<ll>, 2, 2>;
Mat a, b;
a[0] = {1, 2}; a[1] = {3, 4};
b[0] = {5, 6}; b[1] = {7, 8};
SegmentTree<MonoidMul<Mat>> seg(vc<Mat>{a, b});

array<ll, 2> v{0, -INF};
auto result = seg.prod_apply(0, 2, v,
    [](const auto& x, const auto& mat) { return x * mat; });
assert(result[0] == 9 && result[1] == 10);
```

## 詳細なドキュメント

### SegmentTree

#### prod_apply

```cpp
template <class X, class F>
X prod_apply(int l, int r, X x, F&& apply) const
```

初期値 `x` に区間 $[l,r)$ の積を作用させた `apply(x, prod(l, r))` と等しい値を返す。区間を覆う各ノードの値を左から順に `apply` へ渡す。
空区間では初期値を返す。`apply` の呼び出し回数は $O(\log n)$ で、空区間では $0$ 回。

`apply` の第 $1$ 引数には作業中の `X` の左辺値、第 $2$ 引数には格納された値の `const S&` を渡し、戻り値を作業中の `X` へ代入する。コールバックは参照で受け取り、同じオブジェクトを使って呼び出す。

##### 制約

- $0 \leq l \leq r \leq n$
- `apply(x, M::e()) == x`
- `apply(apply(x, a), b) == apply(x, M::op(a, b))`
- `X` は引数の受け渡し、作用の結果の代入、結果の返却に必要な構築・代入ができる
- コールバック内で、このセグメント木の格納値を変更しない

上の等式は作用の値についての条件である。`apply` は呼び出し回数などを記録してもよい。

##### 計算量

- 作用と結果の代入を合わせて $O(T)$ として $O((T+1)\log n)$。引数の受け渡しと結果の返却にかかる時間を加える。
- $k\times k$ 行列を行ベクトルへ作用させる場合は $O(k^2\log n)$。構築は $O(k^3n)$、点更新は $O(k^3\log n)$。

#### max_right_ok

```cpp
template <class G>
int max_right_ok(int l, const G& g) const
```

次の $R$ を返す。

$$
R = \max\left\lbrace x \in \mathbb{Z} \middle\vert l \leq x \leq n,\ g(\operatorname{prod}(l, x)) = \mathrm{true}\right\rbrace
$$

すなわち、$[l, R)$ は条件を満たし、$R=n$ でなければ $[l, R+1)$ は条件を満たさない。

##### 制約

- $0 \leq l \leq n$
- `g(M::e()) == true`
- $x$ を $l$ から $n$ へ増やしたとき、`g(prod(l, x))` は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\log n)$

#### min_left_ok

```cpp
template <class G>
int min_left_ok(int r, const G& g) const
```

次の $L$ を返す。

$$
L = \min\left\lbrace x \in \mathbb{Z} \middle\vert 0 \leq x \leq r,\ g(\operatorname{prod}(x, r)) = \mathrm{true}\right\rbrace
$$

すなわち、$[L, r)$ は条件を満たし、$L=0$ でなければ $[L-1, r)$ は条件を満たさない。

##### 制約

- $0 \leq r \leq n$
- `g(M::e()) == true`
- $x$ を $r$ から $0$ へ減らしたとき、`g(prod(x, r))` は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\log n)$
