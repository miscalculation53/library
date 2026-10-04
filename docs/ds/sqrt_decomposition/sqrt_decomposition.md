## 概要

長さ $n$ の列を幅 $B$ のブロックに分け、各ブロックと区間 $[l,r)$ の交わりを左から列挙する。
ブロックごとの値・遅延更新・集計方法は利用者側で管理する。

基本 API は `sqrt_decomposition_blocks`。各ブロックを一度ずつ受け取り、
部分ブロックの展開・更新・再構築や、探索の途中終了を記述できる。
末尾の短いブロックも、その全体が含まれれば完全ブロックとして扱う。

各点と完全ブロックを独立に処理する用途には、簡便版 `sqrt_decomposition` も使える。
問題例と設計の対応は [検討メモ](../../../research/sqrt_decomposition.md) を参照。

## 使用例

```cpp
#include "ds/sqrt_decomposition/sqrt_decomposition.hpp"
#include "math/modint/modint.hpp"

using mint = modint998244353;
constexpr int B = 4;
int n = 10;
vc<mint> a(n), sum((n + B - 1) / B);
vc<mint> mul(sum.size(), 1), add(sum.size());

// [l, r) の各値を x * 値 + y に変換する。
auto apply = [&](int l, int r, mint x, mint y)
{
  for (auto s : sqrt_decomposition_blocks<B>(n, l, r))
  {
    if (s.full())
    {
      sum[s.b] = x * sum[s.b] + y * (s.r - s.l);
      mul[s.b] = x * mul[s.b];
      add[s.b] = x * add[s.b] + y;
    }
    else
    {
      for (int i = s.block_l; i < s.block_r; i++) a[i] = mul[s.b] * a[i] + add[s.b];
      mul[s.b] = 1;
      add[s.b] = 0;
      for (int i = s.l; i < s.r; i++) a[i] = x * a[i] + y;
      sum[s.b] = 0;
      for (int i = s.block_l; i < s.block_r; i++) sum[s.b] += a[i];
    }
  }
};
```

探索では、ブロックの集計で候補を絞ってから各要素を確認する。

```cpp
// a と、各ブロックの最大値 mx を管理しているとする。
int answer = n;
for (auto s : sqrt_decomposition_blocks<B>(n, l, n))
{
  if (s.full() && mx[s.b] < x) continue;
  for (int i = s.l; i < s.r; i++)
    if (a[i] >= x) { answer = i; break; }
  if (answer != n) break;
}
```

## 詳細なドキュメント

#### sqrt_decomposition_blocks

```cpp
template <int B = 512>
auto sqrt_decomposition_blocks(int n, int l, int r)
```

範囲 `for` で `SqrtDecompositionSegment` を取り出せる列挙オブジェクトを返す。
列挙するのは対象区間と交わるブロックで、順序はブロック番号の昇順。
空区間の列挙数は $0$。返されたオブジェクトは引数の値だけを持ち、再度列挙できる。

| メンバ | 意味 |
| --- | --- |
| `s.b` | $0$ 始まりのブロック番号 |
| `s.l`, `s.r` | 対象区間との交わり $[s.l,s.r)$。列全体での添字 |
| `s.block_l`, `s.block_r` | ブロック全体の区間 $[bB,\min((b+1)B,n))$ |
| `s.full()` | 対象区間がこのブロック全体を含むか |

各交差区間は空でなく、互いに重ならず、合わせて $[l,r)$ になる。
部分ブロックは高々 $2$ 個。同じブロック内の区間は一度だけ返す。
ブロック内の要素を読む順序や、全体・部分の処理は利用側で選べる。

配列を確保せず、走査に応じて次の区間を生成する。`break` / `return` でその場で走査を終えられる。

##### 制約

- $B > 0$
- $0 \leq l \leq r \leq n$
- 列挙中は列長とブロックの境界を固定する。ブロック内の値や集計値は変更してよい

##### 計算量

- 列挙オブジェクトの構築・各ブロックの取り出しは $O(1)$
- 全列挙は $O(1 + (r-l)/B)$。利用側の処理時間を加算する
- 部分ブロックの処理が $O(B)$、完全ブロックの処理が $O(1)$ なら、全体で $O(B + (r-l)/B)$

#### sqrt_decomposition

```cpp
template <int B = 512, class Point, class Block>
void sqrt_decomposition(int n, int l, int r, Point&& point, Block&& block)
```

列挙オブジェクトを使った簡便版。完全ブロックには `block(b)`、部分ブロックの各要素には `point(i)` を呼ぶ。
引数はそれぞれブロック番号と列全体の添字。呼び出し順は左から右で、各要素をちょうど一度扱う。
コールバックは参照で受け取り、戻り値は使わない。

各点の値を直接 `a` に、各ブロックの和を `sum` に保持している場合は、次のように使える。

```cpp
sqrt_decomposition<B>(n, l, r,
    [&](int i) { answer += a[i]; },
    [&](int b) { answer += sum[b]; });
```

##### 制約

- $B > 0$
- $0 \leq l \leq r \leq n$
- `point(i)` と `block(b)` が呼び出せる

##### 計算量

- 各処理が $O(1)$ のとき、$O(B + (r-l)/B)$
- 点への呼び出しは高々 $2(B-1)$ 回、ブロックへの呼び出しは高々 $\lceil(r-l)/B\rceil$ 回
