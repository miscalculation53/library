このドキュメントには AI による作成・変更が含まれています。

## 概要

多項式の総積

## 使用例

```cpp
using mint = modint998244353;
vc<vc<mint>> fs{{1, 1}, {1, 2}, {1, 3}};
auto p = convolution_many(fs);
assert((p == vc<mint>{1, 6, 11, 6}));
```

## 詳細なドキュメント

#### convolution_many

```cpp
F convolution_many(vc<F> fs, int d = -1)
```

`F` は `vc<mint>` または `FormalPowerSeries<mint>` とする。`fs` に格納されている多項式の総積を求める。$d$ を指定した場合、長さ $d$（つまり $d-1$ 次）まで求める。

##### 計算量

`fs` に格納されている多項式の次数の総和を $n$ として

- $O(n \log^2 n)$
