このドキュメントには AI による作成・変更が含まれています。

## 概要

https://noshi91.github.io/algorithm-encyclopedia/chirp-z-transform

導出は ↑ を参照。

TODO：middle product を使って高速化

## 使用例

```cpp
using mint = modint998244353;
FormalPowerSeries<mint> f{1, 1};
auto ys = multipoint_evaluation_geo(f, 3, mint(1), mint(2));
assert((ys == vc<mint>{2, 3, 5}));
```

## 詳細なドキュメント

#### multipoint_evaluation_geo

```cpp
vc<mint> multipoint_evaluation_geo(fps f, int m, mint a, mint r)
```

多項式 $f$ について、各 $i = 0, \dots, m-1$ に対し $f(ar^i)$ の値を求める。

##### 制約

- $m \geq 0$

##### 計算量

$f$ の次数を $n$ として

- $O((n + m)\log (n + m))$
