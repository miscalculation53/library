このドキュメントには AI による作成・変更が含まれています。

## 概要

多項式の各点 $x_0,\dots,x_{m-1}$ での値 $f(x_0),\dots,f(x_{m-1})$ を求める。

転置原理に基づく多点評価を使う。$Q_v(x)=\prod_{i\in v}(1-x_i x)$ の積木を作り、
根で $Q_1^{-1}$ と入力の中間積を求め、子の $Q_v$ との中間積で値を下へ伝える。
積木の構築で得た NTT 結果を保持し、次の段では偶数番の根での評価を再利用する。
残る半分だけを係数への根の冪の乗算と短い NTT で求める。
子 2 個への更新は順 NTT 1 回・逆 NTT 2 回で行う。
短い入力は各点で Horner 法を使う。

## 使用例

```cpp
using mint = modint998244353;
FormalPowerSeries<mint> f{1, 2, 3};
auto ys = multipoint_evaluation(f, vc<mint>{0, 1, 2}); // {1, 6, 17}
```

## 詳細なドキュメント

#### multipoint_evaluation

```cpp
template <class mint>
vc<mint> multipoint_evaluation(const FormalPowerSeries<mint>& f, const vc<mint>& xs)
```

各評価点での値を `xs` と同じ順序で返す。重複点と $0$ も扱える。

##### 計算量

- $n=|f|$, $m=|\mathrm{xs}|$ として $O(m\log^2 m+(n+m)\log(n+m))$。
- $n\le64$ または $m\le1$ の場合は $O(nm+m)$。
