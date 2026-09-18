## 概要

競プロ的には線形漸化的数列の線形漸化式を復元するアルゴリズムとしての使い道の方が多いが、数列の最小多項式を求めているとも解釈できる。

線型漸化的数列が $d$ 個の係数で表される線形漸化式を持つことがわかっているとき、先頭 $2d + 1$ 項がわかっていれば正しい線形漸化式が復元できる。

一般の体で $O(n^2)$ 時間で動作する。NTT が使える場合の
[$O(n\log^2 n)$ の Half-GCD 版](berlekamp_massey_fast.md) もある。
通常版は低次数の列や逐次更新に適しており、[実測比較](../../../benchmark/half_gcd.md) を参照。

## 詳細なドキュメント

#### berlekamp_massey

```cpp
vc<F::S> berlekamp_massey(vc<F::S> a)
```

`F` は体（`FieldAddSubMulDiv<mint>` など）とする。$a_0, \dots, a_{n-1}$ を入力すると、$\displaystyle a_i = \sum_{j = 1}^d c_j a_{i-j}$ を満たす $c_1, \dots, c_d$ を求める。返り値では $c_0 = -1$ とする（これは $\displaystyle \sum_{j=0}^d c_j a_{i-j} = 0$ を満たすようにするため）。多項式 $c_0 x^d + \dots + c_{d-1} x + c_d$ は数列 $a$ の最小多項式となっている。

##### 制約

- `F` は体

##### 計算量

- $O(n^2)$


### 逐次更新

```cpp
BerlekampMassey<F> bm;
bm.append(a_i);                  // 次の 1 項を追加
int d = bm.degree();             // 現在の接頭辞の最短漸化式の次数
auto c = bm.coefficients();      // c[0] == -F::e1()
```

`berlekamp_massey<F>(a)` と同じ処理を逐次実行する。
全体で $O(n^2)$ 時間、保持する数列・係数に $O(n)$ 空間。
`coefficients()` は現在の係数をコピーして返すので $O(d)$ 時間。
途中の接頭辞に対する漸化式が、未知の後続項でも成立するとは限らない。
