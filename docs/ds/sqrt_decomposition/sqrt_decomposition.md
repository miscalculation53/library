## 概要

長さ $n$ の列を幅 $B$ のブロックに分け、区間 $[l, r)$ に対して処理を行う。
区間に全体が含まれるブロックには `block(b)`、端の要素には `point(i)` を呼び出す。
データやブロックごとの集計値は利用者側で管理する。

呼び出しは左から右の順で、各要素をちょうど一度扱う。末尾の短いブロックも、全体が含まれれば `block` に渡す。
区間加算や区間集計に使える。ブロック幅は定数除算を使えるようにテンプレート引数で指定する。

## 使用例

```cpp
#include "ds/sqrt_decomposition/sqrt_decomposition.hpp"

constexpr int B = 4;
int n = 10;
vc<ll> value(n), lazy((n + B - 1) / B);

// [2, 10) に 5 を加える。
sqrt_decomposition<B>(n, 2, 10,
    [&](int i) { value[i] += 5; },
    [&](int b) { lazy[b] += 5; });
// value[i] + lazy[i / B] が各点の値。
```

## 詳細なドキュメント

#### sqrt_decomposition

```cpp
template <int B = 512, class Point, class Block>
void sqrt_decomposition(int n, int l, int r, Point&& point, Block&& block)
```

ブロック $b$ は $[bB, \min((b+1)B, n))$ を表す。ブロック番号は $0$ 始まり。
`point(i)` の引数は列の添字、`block(b)` の引数はブロック番号。各処理の戻り値は使わない。
空区間では呼び出し回数は $0$。状態を持つ関数オブジェクトも参照で受け取る。

##### 制約

- $B > 0$
- $0 \leq l \leq r \leq n$
- `point(i)` と `block(b)` が呼び出せる

##### 計算量

- 各処理が $O(1)$ のとき、$O(B + (r-l)/B)$
- 点への呼び出しは高々 $2(B-1)$ 回、ブロックへの呼び出しは高々 $\lceil (r-l)/B \rceil$ 回
