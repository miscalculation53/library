## 概要

https://maspypy.com/fps-%e5%90%88%e6%88%90%e3%83%bb%e9%80%86%e9%96%a2%e6%95%b0%e3%81%ae%e8%a7%a3%e8%aa%ac-1-%e9%80%86%e9%96%a2%e6%95%b0%e3%81%a8-power-projection

$[x^0]f = 0, [x^1]f \neq 0$ を満たす FPS $f$ に対し、$g(f(x)) = f(g(x)) = x$ を満たす FPS $g$ が唯一存在する。$f$ が $n$ 次まで与えられたとき、$g$ を $n$ 次まで求めることを考える。

$c = [x^1]f$ とする。$f(x)/c$ の逆関数が $g(x)$ つまり $g(f(x)/c) = x$ のとき $f(x)$ の逆関数は $g(x/c)$ であるから、$[x^1]f = 1$ の場合のみ考えればよい。

ラグランジュ反転より $n[x^n]f(x)^i = i[x^{n-i}] (g(x) / x)^{-n}$ であるから、$[x^n]f(x)^i$ を $i=1,\dots,n$ に対して求めれば $(g(x)/x)^{-n}$ が $n-1$ 次まで求まるので、その $-1/n$ 乗を求めればよい (\*1) (\*2)。よって Power Projection（の特殊ケース）に帰着できる。

(*1) $(g(x)/x)^{-n}$ の $-1/n$ 乗が定義でき、かつ $g(x)/x$ と一致するには、$[x^1]g = 1$ が必要。$[x^1]f = 1$ よりこれを満たしている。

(*2) $h(x)^{-1/n} = \exp(\log( h(x)^{-1/n})) = \exp\left(-\dfrac{1}{n}\log h(x)\right)$ として計算すればよい。同じことだが `h.pow(-mint(n).inv().val(), n)` として計算するのでもよい。

## 使用例

```cpp
using fps = FormalPowerSeries<modint998244353>;
auto g = compositional_inv(fps{0, 1, -1}, 5); // {0, 1, 1, 2, 5}
```

## 詳細なドキュメント

#### compositional_inv

```cpp
fps compositional_inv(const fps &f, int n)
```

$f(g(x)) = g(f(x)) = x$ を満たす形式的冪級数 $g$ の先頭 $n$ 項を求める。長さ $n$ は必ず指定する。結果は末尾の $0$ も含めてちょうど $n$ 要素で、$n=0$ のときは空配列を返す。入力の保存範囲外の係数は $0$ として扱う。

##### 制約

- $[x^0]f = 0$、$[x^1]f \neq 0$
- $0 \le n \le \bmod$、法は素数

##### 計算量

入力と出力の長さの最大値を $N$ として

- $O(N \log^2 N + \log \bmod)$
