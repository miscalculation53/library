このドキュメントには AI による作成・変更が含まれています。

## 概要

連立合同方程式

$\begin{cases}
x \equiv r_1 \pmod {m_1} \\\\
\vdots \\\\
x \equiv r_n \pmod {m_n} \\\\
\end{cases}$

を考える。この解は次のいずれか：

- 解なし
- ある $r, m \ (0 \leq r \lt m)$ が存在して、$x \equiv r \pmod m$ と書ける

後者の場合、$m = \mathrm{lcm}(m_1, \dots, m_n)$ であり、$r$ は連立合同方程式を満たす非負整数 $x$ の最小値となる。

使用場面は大きく次の $2$ つがある：

- $r, m$ が `ll` などに収まる場合に $r, m$ そのものを求める
  - ACL にある
- $r$ に対し、別の mod をとった値を求める
  - 任意 mod 畳み込みとか
  - 前者より計算量が悪い

### 解法

`crt2` は2本の合同式を最大公約数と逆元で併合し、`crt` はこれを順に適用する。併合後の法は元の法の最小公倍数になる。

`crt_mod` は互いに素な法に対する混合基数表示を順に決め、必要な法での値だけを保持する。積全体を明示せずに、別の法での解と周期を求められる。

`pre_crt` は各素数について必要な最大指数の条件を残し、法を互いに素にする前処理である。最大指数が同じなら、元の添字が最も後ろの要素にその素数冪を残す。最大公約数で共有部分を移すため、素因数分解は不要。

数学的な背景：[中国剰余定理の解説](https://qiita.com/drken/items/ae02240cd1f8edfc86fd)。`crt2` の AtCoder Library 由来部分はヘッダの出典表示を参照。

## 使用例

```cpp
auto answer = crt(vc<ll>{2, 3}, vc<ll>{3, 5});
assert(answer);
assert(((*answer) == pair<ll, ll>{8, 15}));
```

## 詳細なドキュメント

#### crt2

```cpp
template <class T = ll, class R0, class R1, class M0, class M1>
constexpr optional<pair<T, T>> crt2(R0 r0, R1 r1, M0 m0, M1 m1)
```

$2$ 元の場合を解く。引数は剰余 `r0, r1`、法 `m0, m1` の順。解が存在する場合は `(r, m)`、存在しない場合は `nullopt` を返す。

##### 制約

- `T` は符号つき整数型
- $m_1, m_2 \geq 1$
- $\mathrm{lcm}(m_1, m_2)$ が `T` に収まる

##### 計算量

- $O(\log \min(m_1, m_2))$

#### crt

```cpp
template <class T = ll, class V1, class V2>
constexpr optional<pair<T, T>> crt(const V1& rs, const V2& ms)
```

解が存在する場合は `(r, m)`、存在しない場合は `nullopt` を返す。条件が0個の場合は値ありの `(0, 1)`。

##### 制約

- `V` は整数を格納した array または vector
- `T` は符号つき整数型
- $m_i \geq 1$
- $\mathrm{lcm}(m_1, \dots, m_n)$ が `T` に収まる

##### 計算量

- $O(n \log \mathrm{lcm}(m_1, \dots, m_n))$

#### crt_mod

```cpp
(1) pair<mint, mint> crt_mod<mint>(V rs, V ms)
(2) pair<mint, mint> crt_mod_constexpr<mint>(V rs, V ms)
```

**$m_i, m_j \ (i \neq j)$ が互いに素であることを前提としている。互いに素でない場合、pre_crt を先に呼ぶこと。**

この制約のもと、解は必ず存在する。$(r, m)$ を `mint` の mod で求めて返す。

(1) は法が $2^{31}-1$ 以下の32 bit整数なら Barrett reduction を使う。法を `ll` に格納した場合も、式が4本以上で法がすべてこの範囲なら計算幅を切り替える。4〜15本では `ull` の積と剰余、16本以上では Barrett reduction を使う。大きい符号なし整数や128 bit整数の法は、各法の範囲内で安全に剰余を計算する経路で扱う。

(2) は `array` 用で、C++17以降では定数式としても評価できる。実行時の符号付き32・64 bit整数では、法がすべて $2^{31}-1$ 以下なら、法・剰余を `int`、途中の計算を `ll` に切り替える。

##### 制約

- `rs` と `ms` は同じ長さの組込み整数列。符号付き・符号なし、GNUの128 bit整数を扱える。
- `mint` は整数からの構築・加算・乗算が目的の環の演算になる型。modint、`ull`、`BigInteger` などを使える。
- $m_i \geq 1$
- **$m_i, m_j \ (i \neq j)$ は互いに素**
- (2)：
  - `V` は array
  - $0 \leq r_i \lt m_i$

##### 計算量

- $O(n^2 + \sum_{i=1}^n \log m_i)$

#### pre_crt

```cpp
template <class V1, class V2>
bool pre_crt(const V1 &rs, V2 &ms);
```

解が存在すれば `true` を返し、`ms` を変換する。解が存在しない場合は `false` を返し、そのときの `ms` の内容は未規定とする。空入力は `true` を返す。

`rs` と `ms` の要素型は異なっていてもよい。同じオブジェクトを両引数へ渡した場合も、変更前の値から合同式を計算し、そのオブジェクトを変換後の法で更新する。

##### 制約

- `rs` と `ms` は、`vector` や `array` などの同じ長さの整数列。
- 要素型は組込み整数型、または整数の加減乗除・剰余・比較ができる多倍長整数型。GNUの128 bit整数も扱える。
- 剰余側は `bool` も使える。法の要素型は `bool` を除く。
- 負数の除算は0方向、または負の無限大方向に丸める。
- 各 `ms[i]` は正。

##### 計算量

要素数を $n$、元の法の最大値を $M$ とする。

- $O(n^2 \log^2 M)$ 回の整数演算
- 異なる整数型間で直接正規化できない場合は、剰余の絶対値を $R_i$ として追加で $O(\sum_i \log R_i)$ 回の整数演算
