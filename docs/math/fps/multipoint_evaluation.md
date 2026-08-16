## 概要

https://37zigen.com/multipoint-evaluation/

多項式の各点 $x_0, \dots, x_{m-1}$ での値 $f(x_0), \dots, f(x_{m-1})$ を求める。

方法のひとつとして、subproduct tree, subremainder tree を用いるものがある。これはセグ木状の木で、$[l, r)$ を表すノードに、subproduct tree では $(x - x_l) \cdots (x - x_{r-1})$ を、subremainder tree では $f(x) \bmod (x - x_l) \cdots (x - x_{r-1})$ を書く。subproduct tree の構築はボトムアップに行い（これは簡単）、subremainder tree の更新はトップダウンに行う（これは $(f \bmod g_0g_1) \bmod g_0 = f \bmod g_0$ を利用する）。剰余の定理より $f(x_i) = f(x) \bmod (x - x_i)$ なのでこれでできる。なお、実装上は subremainder tree の構築を途中で止めて、多項式の次数が十分小さくなった段階で評価してやるほうが定数倍がよい。$O(m \log^2 m + n \log n)$ 時間だが、剰余演算が必要なため定数倍が重い。現在の実装はこれ。

もうひとつの方法として、転置原理を用いるものがある（らしい）。時間計算量のオーダーは変わらず $O(m \log^2 m + n \log n)$ だが定数倍がよい（らしい）。これを勉強してこれに書き換えたいと思っている。

## 詳細なドキュメント

#### multipoint_evaluation

```cpp
vc<mint> multipoint_evaluation(fps f, vc<mint> xs)
```

多項式 $f$ の各点 $x_0, \dots, x_{m-1}$ での値 $f(x_0), \dots, f(x_{m-1})$ を求める。

##### 計算量

$f$ の次数を $n$ として

- $O(m \log^2 m + n \log n)$
