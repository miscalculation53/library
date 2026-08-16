## 概要

尺取り法。

$[0, n)$ に含まれる区間 $[l, r)$ に対し

- `add_r(l, r)`： $r$ を追加して $[l, r+1)$ にする（$0 \leq l \leq r < n$）
- `del_l(l, r)`： $l$ を削除して $[l+1, r)$ にする（$0 \leq l < r \leq n$）
- `judge(l, r)`： 現在の区間 $[l, r)$ が条件を満たすかどうかを判定する

という操作ができるものとする。さらに、

- 空の区間（特に最初は $[0, 0)$）は常に条件を満たす
- 単調性がある。すなわち、ある区間が条件を満たすとき、それに完全に含まれる区間も条件を満たす

と仮定する。このとき、次の $R_l,L_r$ を $O(n)$ 回の操作で求められる。

$$
R_l = \max\left\{x \in \mathbb{Z} \mathrel{\middle|} l \leq x \leq n,\ \operatorname{judge}(l,x) = \mathrm{true}\right\}
$$

$$
L_r = \min\left\{x \in \mathbb{Z} \mathrel{\middle|} 0 \leq x \leq r,\ \operatorname{judge}(x,r) = \mathrm{true}\right\}
$$

## 詳細なドキュメント

#### right_ok

```cpp
vc<I=ll> right_ok(int n, auto add_r, auto del_l, auto judge)
```

各 $l = 0, \dots, n$ に対する $R_l$ を、長さ $n+1$ の vector として返す。

##### 制約

- 「概要」に記載した仮定を満たす

##### 計算量

- `add_r`, `del_l`, `judge` はそれぞれ $O(n)$ 回呼ばれる

#### left_ok

```cpp
vc<I=ll> left_ok(int n, auto add_r, auto del_l, auto judge)
```

各 $r = 0, \dots, n$ に対する $L_r$ を、長さ $n+1$ の vector として返す。

##### 制約

- 「概要」に記載した仮定を満たす

##### 計算量

- `add_r`, `del_l`, `judge` はそれぞれ $O(n)$ 回呼ばれる
