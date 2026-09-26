## 概要

線形写像の転置を計算する部品と、内積による検査を提供する。
`tools/transpose.py` が出力する C++ から利用する。
[生成器の使い方と問題例](../../../tools/transpose_examples/README.md)を参照。

固定パラメータのもとで $F(x)=Ax$ と書けるとき、転置は $y\mapsto A^{\mathsf T}y$。
係数には modint などの可換環を使う。除算には逆元が必要で、整数型を使う場合は演算結果がその型に収まること。

## 使用例

```cpp
using T = modint998244353;
vc<T> b{2, 3}, w{5, 7, 11};
auto t = linear_transpose::convolution_transpose(w, b, 2);
// {2*5 + 3*7, 2*7 + 3*11} = {31, 47}
assert(linear_transpose::check<T>(2, 3,
  [&](const vc<T>& x) { return convolution(x, b); },
  [&](const vc<T>& y) {
    return linear_transpose::convolution_transpose(y, b, 2);
  }));
```

## 詳細なドキュメント

以下はすべて `linear_transpose` 名前空間にある。$M(s)$ は長さ $O(s)$ の畳み込みの計算量とする。

#### check

```cpp
bool check<T>(int n, int m, const Forward& forward,
              const Transpose& transpose, int trials = 8)
```

固定シードで $\{-3,\ldots,3\}$ から入力を生成し、入出力の長さと
$\langle F(x),y\rangle=\langle x,F^{\mathsf T}(y)\rangle$ を検査する。
正しさを調べるための乱択テストで、少数の入力に対する照合になる。

##### 制約

- $n,m,\mathrm{trials}\geq0$
- `forward` は長さ $n$ から $m$、`transpose` は長さ $m$ から $n$ への写像。
- `T` は加算・乗算・等値比較を持つ正確な係数型。

##### 計算量

- $O(\mathrm{trials}\cdot(T_F+T_{F^{\mathsf T}}+n+m))$

#### convolution_transpose

```cpp
vc<T> convolution_transpose(const vc<T>& w, const vc<T>& b, int n)
```

長さ $n$ の入力に対する `a -> convolution(a, b)` の転置を返す。
結果の第 $i$ 成分は $\sum_{j=0}^{|b|-1}b_jw_{i+j}$。
`w` の保存範囲外は $0$ として扱い、$n+|b|-1$ 項目以降は使わない。
`b` が空の場合は長さ $n$ の零ベクトルを返す。

##### 制約

- $n\geq0$
- `T` は既存の `convolution` が対応する modint 型。法と長さはその利用条件を満たすこと。

##### 計算量

- $O(M(n+|b|))$

#### polynomial_mod / polynomial_mod_transpose

```cpp
vc<T> polynomial_mod(const vc<T>& a, const vc<T>& g)
vc<T> polynomial_mod_transpose(const vc<T>& w, const vc<T>& g, int n)
```

`polynomial_mod` は $a\bmod g$ をちょうど $d=\deg g$ 項で返す。
`polynomial_mod_transpose` は、入力長を $n$ としたこの写像の転置を返す。

##### 制約

- $n\geq0$, $d\geq1$, `g.back() == T(1)`。
- `w.size() == d`。
- `T` は既存の `convolution` と `FormalPowerSeries::inv` が対応する modint 型。

##### 計算量

- $O(M(n+d))$（順方向では $n=|a|$）

#### ntt_forward / ntt_transpose / intt_forward / intt_transpose

```cpp
vc<T> ntt_forward(vc<T> a)
vc<T> ntt_transpose(vc<T> a)
vc<T> intt_forward(vc<T> a)
vc<T> intt_transpose(vc<T> a)
```

ライブラリの `ntt`, `intt` と、それぞれの転置。`intt` は正規化前の逆変換。
先頭以外の要素を反転する演算を $R$ とすると、`ntt_transpose` は $R\circ\mathrm{intt}$、
`intt_transpose` は $\mathrm{ntt}\circ R$ を計算する。空配列はそのまま返す。

##### 制約

- 空配列、または `ntt_ok<T>(a.size())` が真。

##### 計算量

- $O(n\log n)$、$n=|a|$。

#### add_to / slice / resized / reversed

```cpp
void add_to(vc<T>& a, const vc<T>& b)
vc<T> slice(const vc<T>& a, int l, int r)
vc<T> resized(vc<T> a, int n)
vc<T> reversed(vc<T> a)
```

順に、要素ごとの加算、半開区間 $[l,r)$ のコピー、長さの変更、配列の反転。
`resized` で追加する要素は $0$。

##### 制約

- `add_to`: `a.size() == b.size()`。
- `slice`: $0\leq l\leq r\leq |a|$。
- `resized`: $n\geq0$。

##### 計算量

- `add_to`, `reversed`: $O(|a|)$。
- `slice`: $O(r-l)$。
- `resized`: $O(|a|+n)$。
