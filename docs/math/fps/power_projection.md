## Power Projection

https://maspypy.com/fps-%e5%90%88%e6%88%90%e3%83%bb%e9%80%86%e9%96%a2%e6%95%b0%e3%81%ae%e8%a7%a3%e8%aa%ac-1-%e9%80%86%e9%96%a2%e6%95%b0%e3%81%a8-power-projection

FPS $f(x), g(x)$ が与えられるので、$i=0,\dots,m-1$ に対して $[x^{n}]g(x)f(x)^i$ を求める。（$w_j = [x^{n-j}] g_j(x)$ とおけば、これは $\sum_{j=0}^{n-1} w_j \cdot [x^j] f(x)^i$ である。）

$g(x)f(x)^i = [y^i] \dfrac{g(x)}{1-yf(x)}$ であるから、$y$ の多項式 $[x^n] \dfrac{g(x)}{1-yf(x)}$ の $m-1$ 次までを求めればよい。$P(x,y) = g(x), Q(x,y) = 1-yf(x)$ とおく。Bostan–Mori と同様に

$\displaystyle \frac{P(x,y)}{Q(x,y)} = \frac{P(x,y)Q(-x,y)}{Q(x,y)Q(-x,y)} = \frac{U_\mathrm{e}(x^2,y)+xU_\mathrm{o}(x^2,y)}{V(x^2,y)}$

とできるので、

- $n$ が偶数のとき $[x^n]\dfrac{P(x,y)}{Q(x,y)} = [x^{n/2}]\dfrac{U_\mathrm{e}(x,y)}{V(x,y)}$
- $n$ が奇数のとき $[x^n]\dfrac{P(x,y)}{Q(x,y)} = [x^{(n-1)/2}]\dfrac{U_\mathrm{o}(x,y)}{V(x,y)}$

となる。このステップを $O(\log n)$ 回繰り返す。ベースケース $n=0$ は $y$ に関する FPS 除算である。$1$ 回のステップで $y$ の次数は $2$ 倍になるが $x$ の次数は $1/2$ 倍にしてよいから、$1$ 回のステップは $O(n \log n)$ 時間にできて、全体 $O(n\log^2 n + m\log m)$ 時間になる。

逆関数の計算で利用する場合、$n=2^k-1$ の形に限定して実装してしまうのがよい。このとき場合分けは $U_\mathrm{o}$ を使う方のみになる。$P$ の $x$ 方向、$P$ の $y$ 方向、$Q$ の $x$ 方向の長さが $2^k$ の形なので畳み込みしやすい。$Q$ の $y$ 方向の長さが $2^k+1$ の形なのだけ困るが、これは定数項が常に $1$ であることから長さ $2^k$ のところだけ見ればよい（reverse しておくと実装が楽）。

