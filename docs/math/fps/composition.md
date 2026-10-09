## 概要

形式的冪級数 $f(x), g(x)$ に対して、$f(g(x)) \bmod x^n$ を求める。$g$ の定数項は $0$ とする。

既存の [Power Projection](power_projection.md) に転置原理を適用している。$g$ を固定し、$A_{i,j}=[x^j]g(x)^i$ とおくと、Power Projection は $w\mapsto Aw$、合成は $f\mapsto A^{\mathsf T}f$ である。

実装では長さを $N=2^{\lceil\log_2 n\rceil}$ にそろえ、`power_projection` と同じ分母の更新を順方向に行う。その後、分子に対する線形演算を逆順に転置する。

- 係数の取り出しは、対応する位置への $0$ 埋めにする。
- 固定した $r$ との畳み込みは、$a_i=\sum_j r_j b_{i+j}$ を求める演算にする。
- 終端では $g(0)=0$ より分母が $1$ なので、係数列を反転する。

NTT が使える場合は、既存実装の NTT による高速化も転置する。`ntt` はビット反転順の出力、`intt` は正規化前の逆変換なので、先頭以外の反転を $R$ とすると `ntt` の転置は $R\circ\mathrm{intt}$、`intt` の転置は $\mathrm{ntt}\circ R$ になる。ほかの法では [中間積](../convolution/middle_product.md) を使う。通常表現の 32 bit modint の CRT 経路では、転置した積の変換長を $8N$ から $4N$ に縮める。各段階の分母を保存するため、空間計算量は $O(n\log n)$。

参考：[FPS 合成・逆関数の解説（2）転置原理による合成アルゴリズムの導出](https://maspypy.com/fps-%E5%90%88%E6%88%90%E3%83%BB%E9%80%86%E9%96%A2%E6%95%B0%E3%81%AE%E8%A7%A3%E8%AA%AC%EF%BC%882%EF%BC%89%E8%BB%A2%E7%BD%AE%E5%8E%9F%E7%90%86%E3%81%AB%E3%82%88%E3%82%8B%E5%90%88%E6%88%90%E3%82%A2)

## 使用例

```cpp
using fps = FormalPowerSeries<modint998244353>;
fps f{1, 2, 3}, g{0, 1, 1};
auto h = composition(f, g, 5); // {1, 2, 5, 6, 3}
```

## 詳細なドキュメント

#### composition

```cpp
fps composition(const fps &f, const fps &g, int n)
```

$f(g(x))$ の先頭 $n$ 項を返す。結果は末尾の $0$ も含めてちょうど $n$ 要素で、$n=0$ のときは空配列を返す。入力の保存範囲外の係数は $0$ として扱う。$f, g$ は任意の長さでよく、$n$ 次以上の係数は結果に影響しない。

##### 制約

- $n\geq 0$
- $[x^0]g=0$
- 係数型は既存の `convolution` が対応する modint 型。畳み込みの長さと法は、その利用条件を満たすこと。

##### 計算量

- $O(n\log^2 n)$（$n\geq 2$）
