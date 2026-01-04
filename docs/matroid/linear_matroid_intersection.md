## 線形マトロイド交叉（乱択）

https://atcoder.jp/contests/abc399/editorial/12546?lang=ja

$A_1, A_2$ を列数が等しい行列、$E$ を列集合とする。列の部分集合 $I \subseteq E$ であって $A_1$ においても $A_2$ においても線型独立なものの最大サイズを求める問題を考える。

$D = \mathrm{diag}(x_1, \dots, x_{\lvert E \rvert})$ とするとき、この最大サイズは $\mathrm{rank}(A_1 D A_2^\top)$ に等しい。

$A_1, A_2$ が体 $\mathbb{F}_p$ 上の行列であるとき、$D$ の各変数に一様ランダムな $\mathbb{F}_p$ の元を代入した上で rank を計算する。Schwartz–Zippel の補題より、失敗確率はたかだか $\dfrac{\lvert E \rvert}{p}$ である。計算量は $A_1, A_2$ の行数を $n, m$ として $O(nm\min(n, m))$ である。

$\mathbb{F}_p$ 上で線形表現できるマトロイドどうしの交叉はこの手法で解ける。
