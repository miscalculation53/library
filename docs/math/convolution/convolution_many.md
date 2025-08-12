## 多項式の総積

#### convolution_many

```cpp
F convolution_many(vc<F> fs, int d = -1)
```

`F` は `vc<mint>` または `FormalPowerSeries<mint>` とする。`fs` に格納されている多項式の総積を求める。$d$ を指定した場合、長さ $d$（つまり $d-1$ 次）まで求める。

##### 計算量

`fs` に格納されている多項式の次数の総和を $n$ として

- $O(n \log^2 n)$
