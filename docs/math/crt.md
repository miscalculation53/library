## 中国剰余定理 (CRT)

https://qiita.com/drken/items/ae02240cd1f8edfc86fd

連立合同方程式

$\begin{cases}
x \equiv r_1 \pmod {m_1} \\\\  
\vdots \\\\  
x \equiv r_n \pmod {m_n} \\\\  
\end{cases}$

を考える。この解は次のいずれか：

- 解なし
- ある $r, m \: (0 \leq r \lt m)$ が存在して、$x \equiv r \pmod m$ と書ける

後者の場合、$m = \mathrm{lcm}(m_1, \dots, m_n)$ であり、$r$ は連立合同方程式を満たす非負整数 $x$ の最小値となる。

使用場面は大きく次の $2$ つがある：

- $r, m$ が `ll` などに収まる場合に $r, m$ そのものを求める
  - ACL にある
- $r$ に対し、別の mod をとった値を求める
  - 任意 mod 畳み込みとか
  - 前者より計算量が悪い

### ライブラリの使い方

#### crt2

```cpp
tuple<bool, T, T> crt2<T = ll>(T r1, T m1, T r2, T m2)
```

$2$ 元の場合を解く。解が存在しない場合は $(\mathrm{false}, $ 未定義$, $ 未定義$)$、存在する場合は $(\mathrm{true}, r, m)$ を返す。

$n$ 元の場合は $2$ 元の場合を繰り返し解いていることになる。応用例：たとえば $n$ 元の場合に「$r, m$ がある値 $\mathrm{LIM}$ を超えるかどうか（超えないならもとの値）」を求めるには、繰り返しのたびに超えるかどうかを見ればよい。（$\mathrm{LIM}$ が $10^{18}$ 程度のとき、うまく書き換えると `ll` だけでできる（未実装）のだが、`i128` で計算してしまうと思考停止できそう。） https://atcoder.jp/contests/abc150/tasks/abc150_d

##### 制約

- `T` は符号つき整数型
- $m_1, m_2 \geq 1$
- $\mathrm{lcm}(m_1, m_2)$ が `T` に収まる

##### 計算量

- $O(\log \min(m_1, m_2))$

#### crt

```cpp
pair<T, T> crt<T = ll>(V rs, V ms)
```

解が存在しない場合は $(\mathrm{false}, $ 未定義$, $ 未定義$)$、存在する場合は $(\mathrm{true}, r, m)$ を返す。

##### 制約

- `V` は整数を格納した array または vector
- `T` は符号つき整数型
- $m_i \geq 1$
- $\mathrm{lcm}(m_1, \dots, m_n)$ が `T` に収まる

##### 計算量

- $O(n \log \mathrm{lcm}(m_1, \dots, m_n))$


#### crt_mod

```cpp
(1) pair<mint, mint> crt_mod<mint, T = ll>(V rs, V ms)
(2) pair<mint, mint> crt_mod_constexpr<mint, T = ll>(V rs, V ms)
(3) pair<mint, mint> crt_mod_dynamic_32<mint>(V rs, V ms)
```

**$m_i, m_j \: (i \neq j)$ が互いに素であることを前提としている。互いに素でない場合、pre_crt を先に呼ぶこと。**

この制約のもと、解は必ず存在する。$(r, m)$ を `mint` の mod で求めて返す。

(2) は、`V` が array で $m_i$ がコンパイル時定数のときに高速化したもの。

(3) は、$m_i$ が `int` の範囲で動的に決定されるときに剰余演算を高速化したもの。（$m_i$ が `ll` の範囲のときも作ることは可能であるが、使う場面があまりなさそうなこともあり未実装。）

##### 制約

- `V` は整数を格納した array または vector
- `mint` は modint（ull などを $\bmod\ 2^{64}$ の modint として使っても可）
- $m_i \geq 1$
- **$m_i, m_j \: (i \neq j)$ は互いに素**
- (1), (2)：`T` は $m_i$ の $2$ 乗が収まる符号つき整数型
- (2)：
  - `V` は array
  - $0 \leq r_i \lt m_i$

##### 計算量

- $O(n^2 + \sum_{i=1}^n \log m_i)$


#### pre_crt

```cpp
bool pre_crt(V rs, V &ms)
```

**`ms` に対して破壊的変更を行う。**

解が存在しない場合、false を返す（このときの `ms` の内部は未定義）。

解が存在する場合、true を返す。このとき、等価な連立合同方程式である状態を保ったまま、`ms` が $m_i$ たちが互いに素となるように書き換わる。

##### 制約

- `V` は整数を格納した array または vector
- $m_i \geq 1$


##### 計算量

- $O(n^2 \log^2 \max(m_1, \dots, m_n))$

---

### 中身

冒頭の記事の内容を自分用に書き直した。

#### crt2

$g = \gcd(m_1, m_2), $ $k$ を $m_1k \equiv r_2 - r_1 \pmod {m_2}$ の解（解ならば $k \bmod \dfrac{m_1}{g}$ は一意）として

$r = r_1 + m_1 \cdot \left(k \dfrac{r_2 - r_1}{g} \bmod \dfrac{m_1}{g} \right)$
$m = m_1 \cdot \dfrac{m_2}{g}$

なお、$\mathrm{lcm}(m_1, \dots, m_n)$ が `ll` に収まるならばオーバーフローしないことの証明は https://rsk0315.hatenablog.com/entry/2021/01/18/065720#crt にある。

#### crt_mod

$m_i, m_j \: (i \neq j)$ が互いに素であるとする。

$\begin{aligned}
r_1 &\equiv t_1 & \pmod {m_1} \\\\  
r_2 &\equiv t_1 + t_2m_1 &\pmod {m_2} \\\\  
r_3 &\equiv t_1 + t_2m_1 + t_3m_1m_2 &\pmod {m_3} \\\\  
\vdots
\end{aligned}$

を上から順に解いていく形になる（$0 \leq t_i \lt m_i$）。計算途中で、$t_i$ および $m_1\cdots m_i$ の $\bmod \ m_{i+1}, ..., m_n$ および modint の mod での値が必要。


#### pre_crt

各 $i$ について、$m_i$ がどの $m_j \: (j \gt i)$ とも互いに素になるように処理する。

$x \equiv r \pmod m$ という条件は、$m$ の素因数分解を $m = p_1^{e_1} \cdots p_k^{e_k}$ とすると

$\begin{cases}
x \equiv r \pmod {p_1^{e_1}} \\\\  
\vdots \\\\  
x \equiv r \pmod {p_k^{e_k}} \\\\  
\end{cases}$

と分けられる。

$m_i$ と $m_j$ で共通の素因数については、（条件が矛盾しなければ）$e$ が大きいほうの条件となる。共通しない素因数については、そのまま条件となる。

これは陽に素因数分解せずとも、最大公約数を利用して求められる。

$\begin{aligned}
m_i &= p^a q^b s^c, \\\\  
m_j &= p^d q^e u^f
\end{aligned}$

であり、$a \gt d, b \lt e$ であるとする（もっと素因数が多い場合も同様）。このとき（$r_i, r_j$ を変えずに）$m_i$ を $p^as^c$ に、$m_j$ を $q^eu^f$ にすればよい。

$\begin{aligned}
g := \gcd(m_i, m_j) &= p^d q^b, \\\\  
m_i/g &= p^{a-d}s^c, \\\\  
m_j/g &= q^{e-b}u^f, \\\\  
g_i := \gcd(m_i/g, g) &= p^{\min(d,a-d)}, \\\\  
g_j := g/g_i &= p^{\max(0,2d-a)} q^b
\end{aligned}$

となる。あとは $\gcd(g_i, g_j)$ を $g_j$ から $g_i$ に渡していくことを $\max\left(0, \left\lceil\dfrac{2d-a}{a-d}\right\rceil\right)$ 繰り返せば $g_i = p^d, g_j = q^b$ となるので、$(m_i/g) \cdot g_i = p^a s^c, (m_j/g) \cdot g_j = q^e u^f$ となる。

これもうちょっと賢い方法がありそうな予感もするけどどうなんだろう（使う場面があるかもよくわからない）
