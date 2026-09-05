## 概要

前者は Rolling Hash で一致判定は簡単にできるときなどに使える。後者は Rolling Hash の他に、SA/LCP と一緒に使える。

また、文字列を結合したものの一致判定などができるようなちょっと ad-hoc な場面でも使える。例題： https://atcoder.jp/contests/abc416/tasks/abc416_g

## 詳細なドキュメント

#### calc_lcp

```cpp
I calc_lcp(int i, int j, auto is_equal)
```

ある文字列 $s, t$（陽に与えられない）があって、

- `bool is_equal(int l1, int r1, int l2, int r2)`：$s[l_1, r_1)$ と $t[l_2, r_2)$ の一致判定を行う関数（ただし、**$r_1$ や $r_2$ が範囲外の場合は `false` を返す**）

があるとする。このとき、$s[i, \dots)$ と $t[j, \dots)$ の LCP の長さを返す。

##### 計算量

- `is_equal` の呼び出しを $O(\log n)$ 回

#### compare_substr

```cpp
pair<int, int> compare_substr(int l1, int r1, int l2, int r2, int lcp)
```

ある文字列 $s, t$（陽に与えられない）があって、$s[l_1, \dots)$ と $t[l_2, \dots)$ の LCP の長さ $\mathrm{lcp}$ が計算できているとする。このとき、$s[l_1, r_1)$ と $t[l_2, r_2)$ の辞書順比較の結果は、あるインデックスの組 $(i, j)$ が存在して、$s_i$ と $t_j$ の比較の結果と等しくなる（ただし、存在しないインデックスの文字はどの文字より小さいと考える）。この $(i, j)$ を返す。

##### 計算量

- $O(1)$

----

使用例

```cpp
string S, T;  // 英小文字のみからなる
RollingHash rhS(S), rhT(T);
// S[l1, r1) と S[l2, r2) の比較 (< は -1, = は 0, > は 1)
auto cmp = [&](ll l1, ll r1, ll l2, ll r2) -> ll
{
  return rhS.substr(l1, r1).compare(rhT.substr(l2, r2));
};
```
