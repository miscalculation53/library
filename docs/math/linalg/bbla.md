## 概要

Black Box Linear Algebra とは、線形代数のアルゴリズムのうち、行列を陽に入力することを必要とせず、線型写像だけを受け取って計算を行うアルゴリズム。線型写像の作用の時間に依存するので、特に疎行列に対して高速となる。基本的に作用させるベクトルを乱択することになり、決定的解法ではない。

たとえば
- 線型方程式を解く
- 行列式を求める

などができる。（他のことがどれくらいできるか全然知らない……）

記事： https://yukicoder.me/wiki/black_box_linear_algebra

論文： http://www.enseignement.polytechnique.fr/informatique/profs/Francois.Morain/Master1/Crypto/projects/Wiedemann86.pdf （ちゃんと読んでなし）

---

### 中身の簡単な解説

確率評価はよくわからなかったので一旦さぼります（どうせ Schwartz–Zippel を使いそう感はあるけど）

#### 最小多項式

正方行列 $A$ の最小多項式 $m_A$ とは、$m(A) = O$ を満たす多項式 $m(x) = \sum_{j=0}^d m_j x^j$ のうち次数 $d$ が最小のもの。ランダムベクトル $\boldsymbol{u}, \boldsymbol{v}$ をとって $a_i = \boldsymbol{u}^\top A^i \boldsymbol{v}$ で定まる列 $a$ に対し、$a$ の最小多項式、つまり $\sum_{j=0}^d m_j a_{i+j} = 0 \ (i \geq 0)$ を満たすような最小次数の $m$ をとる（これは Berlekamp–Massey でできる。$d \leq n$ なので $a$ は $a_{2n}$ までとればよい）。すると、この $m$ は高確率で $m$ の最小多項式になっている（らしい）。

#### 線型方程式

最小多項式が求まれば、（少なくとも $A$ が正則な正方行列の場合）線型方程式 $A\boldsymbol{x} = \boldsymbol{b}$ が解ける。

$m_0 I + m_1 A + m_2 A^2 + \cdots + m_d A^d = 0$

より

$\displaystyle A^{-1} \boldsymbol{b} = -\frac{1}{m_0}\left(m_1 \boldsymbol{b} + m_2 A \boldsymbol{b} + \cdots + m_d A^{d-1} \boldsymbol{b} \right)$

であるため。

正則じゃない・正方行列じゃない場合も論文に書いてありそうだが読んでいない。

#### 行列式

$\det(A) = 0 \iff$ 固有値 $0$ をもつ $\iff$ $m_A(0) = m_0 = 0$　であるから、$\det(A) = 0$ は簡単に判定できる。以下 $A$ は正則とする。

$c_A$ を $A$ の特性多項式とすると、$\det(A) = (-1)^n c_A(0)$ である。ここで対角行列 $D$ をランダムにとると高確率で $c_{AD} = m_{AD}$ である（らしい）。$c_{AD}$ が求まれば $\det(A) = \det(AD)/\det(D)$ より求まる。また $c_{AD} = m_{AD}$ かどうかは $n$ 次かどうかで簡単に判定できるので、この部分の成功確率は誤答確率に影響しないようにできる。

---

定数倍が割と死んでます（実装が悪いかもしれない）

## 使用例

- https://judge.yosupo.jp/problem/sparse_matrix_det ： 疎行列の行列式そのもの。
- https://yukicoder.me/problems/no/310 ： BEST 定理を疎グラフに対して用いることになるので、疎行列の行列式が登場する。
- https://codeforces.com/contest/963/problem/E ： マルコフ連鎖（というかただの期待値 DP）で、立式すると線型方程式を解く問題になる（遷移が少ないので疎行列）。想定解は掃き出しを工夫する方針だが BBLA でも通る。

## 詳細なドキュメント

#### minimal_polynomial

```cpp
bbla::minimal_polynomial<F>(int n, auto linear_map, auto random_sample)
```

$S^n$ から $S^n$ への線型写像に対し、最小多項式を求める。

使い方：

- `linear_map` は `vc<F::S>` を参照で受け取って破壊的に変更する関数として定義する。
- `random_sample` は `F::S` の要素をランダムに選ぶ関数（`F::S` が modint のときはデフォルトでよい）。

##### 制約

- `F` は体
- `linear_map` は長さ $n$ の vector に作用する
- `random_sample` のプールは十分大きい

##### 計算量

- $t$ を `linear_map` の計算量として、$O(n^2 + nt)$

#### solve

```cpp
bbla::solve<F>(vc<F::S> y, auto linear_map, auto random_sample)
```

$S^n$ から $S^n$ への正則な線型写像 $\mathrm{linear\_map}$ に対し、線型方程式 $\mathrm{linear\_map}(x) = y$ の（唯一の）解を求める。

使い方：

- `linear_map` は `vc<F::S>` を参照で受け取って破壊的に変更する関数として定義する。
- `random_sample` は `F::S` の要素をランダムに選ぶ関数（`F::S` が modint のときはデフォルトでよい）。

##### 制約

- `F` は体
- $y$ の次元を $n$ として、`linear_map` は長さ $n$ の vector に作用する
- `random_sample` のプールは十分大きい

##### 計算量

- $t$ を `linear_map` の計算量として、$O(n^2 + nt)$

#### det

```cpp
bbla::det<F>(int n, auto linear_map, auto random_sample)
```

$S^n$ から $S^n$ への線型写像に対し、その表現行列の行列式を求める。

使い方：

- `linear_map` は `vc<F::S>` を参照で受け取って破壊的に変更する関数として定義する。
- `random_sample` は `F::S` の要素をランダムに選ぶ関数（`F::S` が modint のときはデフォルトでよい）。

##### 制約

- `F` は体
- `linear_map` は長さ $n$ の vector に作用する
- `random_sample` のプールは十分大きい

##### 計算量

- $t$ を `linear_map` の計算量として、$O(n^2 + nt)$


## 追加 API・現行実装の補足

- `minimal_polynomial` の返り値は昇冪順で、最高次係数は $-1$。$n=0$ には `{-F::e1()}` を返す。
- `solve` の現行の引数順は `bbla::solve<F>(linear_map, y, random_sample)`。
  零の右辺には零ベクトルを返す。途中で低次数の漸化式を得た場合も、
  `linear_map(x) == y` を検算してから返し、射影に失敗した場合は再試行する。
- `det` は $n=0$ に `F::e1()` を返す。
- 乱択 API は十分大きいサンプル集合を仮定する。特に $\mathbf F_2$ の既定のサンプラーは常に 1 を返すため、
  一般には使えない。`solve` と `det` は不適切なサンプラーでは停止しない場合がある。

#### krylov_sequence

```cpp
vc<F::S> bbla::krylov_sequence<F>(linear_map, const vc<F::S>& u,
                                 vc<F::S> v, int count)
```

$u^\top A^i v$（$0\le i<\mathrm{count}$）を返す。乱択しない。
`u` と `v` は同じ長さ、`count >= 0`。
時間 $O(\mathrm{count}(n+t))$、空間 $O(n+\mathrm{count})$。
行列作用は $\max(0,\mathrm{count}-1)$ 回。

#### minimal_polynomial_vector

```cpp
vc<F::S> bbla::minimal_polynomial_vector<F>(linear_map, vc<F::S> v,
                                           random_sample = ...)
```

$m(A)v=0$ の最小多項式を高確率で返す。昇冪順、最高次係数は $-1$。
零ベクトルには `{-F::e1()}` を返す。
時間 $O(n^2+nt)$、空間 $O(n)$。射影に失敗すると真の最小多項式の約数を返す。
`apply_polynomial(linear_map, v, m)` が零かどうかで、その $v$ に作用させてよいことを検査できる。

#### apply_polynomial

```cpp
vc<F::S> bbla::apply_polynomial<F>(linear_map, const vc<F::S>& v,
                                  const vc<F::S>& p)
```

昇冪順の多項式 $p$ に対し $p(A)v$ を返す。空・零多項式には零ベクトルを返す。
末尾の零を除く次数を $d$ とすると、時間 $O((d+1)n+dt)$、空間 $O(n)$。
行列作用は $d$ 回。この操作は環でも使える。

#### rank

```cpp
#include "math/linalg/bbla_rank_solve.hpp"

int bbla::rank<F>(int rows, int cols, apply, transpose,
                  int trials = 2, random_sample = ...)
```

$\mathrm{rows}\times\mathrm{cols}$ 行列 $A$ の rank を高確率で返す。
`apply` は長さ `cols` のベクトルを $Av$（長さ `rows`）に、
`transpose` は長さ `rows` のベクトルを $A^\top v$（長さ `cols`）に変更する関数。
どちらも `vc<F::S>&` を受け取り、長方形の場合は長さも変更する。

Monte Carlo 法で、失敗時には過小評価する。過大評価はしない。
`trials > 0` 回の独立な試行の最大値を返す。零次元には 0 を返す。
nullity は `cols - rank` で求まるが、rank の失敗時には過大評価する。

$d=\min(\mathrm{rows},\mathrm{cols})$、$T$ を作用と転置作用の計算量の和とすると、
時間 $O(\mathrm{trials}\cdot d(\mathrm{rows}+\mathrm{cols}+T))$、作業領域 $O(\mathrm{rows}+\mathrm{cols})$。
乱数は十分大きい集合から独立に選ぶこと。対角前処理には非零要素を使うため、
`random_sample` が 0 を返したときは引き直す。

#### solve_general

```cpp
#include "math/linalg/bbla_rank_solve.hpp"

bbla::LinearSolution<F::S> bbla::solve_general<F>(
    int rows, int cols, apply, transpose, const vc<F::S>& b,
    random_sample = ...)
```

特異・長方形を含む $Ax=b$ に対し、解を一つ、または解が存在しないことの証明を返す。
`apply`・`transpose` は `rank` と同じ形式、`b` の長さは `rows`。

- `result.solvable == true`：`result.solution` は長さ `cols` の解。`certificate` は空。
- `result.solvable == false`：`result.certificate` は長さ `rows` のベクトル $w$ で、
  $A^\top w=0$、$w^\top b\ne0$。`solution` は空。

```cpp
auto result = bbla::solve_general<F>(rows, cols, apply, transpose, b);
if (result.solvable) {
  auto x = result.solution;  // A x = b
} else {
  auto w = result.certificate;  // 左から掛けると 0 = w^T b != 0 となる
}
```

返す解・証明は元の作用で検算する Las Vegas 法。核の基底全体は返さない。
零の右辺には、作用を呼ばず零の解を返す。
1 試行の時間 $O(d(\mathrm{rows}+\mathrm{cols}+T))$、作業領域 $O(\mathrm{rows}+\mathrm{cols})$。
乱択の条件は `rank` と同じ。不適切なサンプラーや小さい体では停止しない場合がある。

#### characteristic_polynomial

```cpp
#include "math/linalg/bbla_invariant_factors.hpp"

vc<F::S> bbla::characteristic_polynomial<F>(
    int n, linear_map, random_sample = ...)
```

$\det(xI-A)$ の係数を昇冪順・monic で返す。$n=0$ には `{F::e1()}`。
`linear_map` は長さ $n$ の `vc<F::S>&` を $Av$ に変更する関数。転置作用は不要。
従来の `minimal_polynomial` の最高次係数 $-1$ とは符号の規約が異なる。

最初の射影最小多項式が $n$ 次ならそれを使うため、時間 $O(n^2+nt)$、作業領域 $O(n)$。
それ以外は決定的な Krylov 基底の構築に移り、時間 $O(n^3+nt)$、作業領域 $O(n^2)$。
ここで $t$ は `linear_map` の計算量。**返り値は常に正しい**。
小さい体や射影に失敗するサンプラーでも、決定的な部分で計算できる。

#### invariant_factors

```cpp
#include "math/linalg/bbla_invariant_factors.hpp"

vc<vc<F::S>> bbla::invariant_factors<F>(
    int n, linear_map, random_sample = ...)
```

$xI-A$ の非定数の不変因子 $f_1\mid f_2\mid\cdots\mid f_k$ をこの順で返す。
各多項式は昇冪順・monic。次数の和は $n$。$n=0$ には空配列。
`linear_map` は `characteristic_polynomial` と同じ形式で、転置作用は不要。

- 全要素の積が特性多項式。
- 最後の要素が monic な最小多項式（$n>0$）。
- 例えば $I_3$ には `{{-1, 1}, {-1, 1}, {-1, 1}}`、
  3 次の nilpotent Jordan block には `{{0, 0, 0, 1}}` を返す。

各 Krylov ブロックの独立性・閉性・因子間の整除関係を検査する Las Vegas 法。
最初の射影で $n$ 次を得れば時間 $O(n^2+nt)$、作業領域 $O(n)$。
それ以外は通常、時間 $O(n^3+nt)$、作業領域 $O(n^2)$。
乱数は十分大きい集合から独立に選ぶこと。不適切なサンプラーでは停止しない場合がある。
行列の全要素は不要だが、一般ケースでは長さ $n$ の基底を $n$ 本保持する。
