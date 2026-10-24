このドキュメントには AI による作成・変更が含まれています。

## 概要

多項式の総積と同じような分割統治でできる。

さらに、$1$ 回の足し算で NTT を使い回すテクが使える。$\dfrac{p}{q} + \dfrac{r}{s} = \dfrac{ps + qr}{qs}$ で、$q$ と $s$ の NTT を使い回すと $9$ 回から $7$ 回になる。このとき $p$ と $r$ は必要な長さが小さいのが注意が必要なポイント。特に $\dfrac{定数}{1\ 次式}$ の総和の場合は途中で出てくる有理式の分子が $2^{k}-1$ 次、分母が $2^k$ 次なので大きな差になる。

## 使用例

```cpp
using mint = modint998244353;
using P = FormalPowerSeries<mint>;
auto [p, q] = rational_sum<mint>({{P{1}, P{1, -1}}, {P{1}, P{1, 1}}});
auto a = p.div(q, 4);
assert((a == P{2, 0, 2, 0}));
```

## 詳細なドキュメント

#### rational_plus

```cpp
pair<fps, fps> rational_plus(pair<fps, fps> f, pair<fps, fps> g)
```

$2$ つの有理式 $f = \dfrac{p}{q}, g = \dfrac{r}{s}$ の和 $\dfrac{ps + qr}{qs}$ を求める。

##### 計算量

$p, q, r, s$ の次数の最大値を $n$ として

- $O(n \log n)$

#### rational_sum

```cpp
pair<fps, fps> rational_sum(vc<pair<fps, fps>> fs, int d = -1)
```

`fs` に格納された有理式の総和を求める。$d$ を指定した場合、分子・分母の長さを $d$ で打ち切る。

##### 計算量

有理式の分子・分母の次数の総和を $n$ として

- $O(n \log^2 n)$
