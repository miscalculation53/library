## 概要

畳み込みを転置した積を求める。配列 `a`, `b` に対し

$$c_i=\sum_{j=0}^{m-1}a_{i+j}b_j\quad(0\le i\le n-m)$$

を返す。多点評価と多項式合成で、必要な中間部分だけを計算するために使う。

NTT を使うときは長さ $\operatorname{bit\_ceil}(n)$ の循環積から取り出す。
通常の畳み込みに必要な $\operatorname{bit\_ceil}(n+m-1)$ より短くできる。
通常表現の 32 bit modint では、直接 NTT を使える法以外も 3 素数の CRT で同じ長さを使う。
小さい積や疎な `b` は積和を直接計算する。

## 使用例

```cpp
vc<modint998244353> a{1, 2, 3, 4}, b{5, 6};
auto c = middle_product(a, b); // {17, 28, 39}
```

## 詳細なドキュメント

#### middle_product

```cpp
template <class mint>
vc<mint> middle_product(const vc<mint>& a, const vc<mint>& b)
```

上の式の積を長さ $n-m+1$ で返す。$n=|a|$, $m=|b|$。

##### 制約

- $1\le m\le n$
- 係数型と法の条件は [convolution](convolution.md) と同じ。

##### 計算量

- $O(n\log n)$。直接計算を選ぶ場合は $O((n-m+1)m)$、`b` の非零数が $s$ なら $O(n+m+(n-m+1)s)$。
