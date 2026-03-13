## Black Box Linear Algebra

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

### ライブラリ

定数倍が割と死んでます（実装が悪いかもしれない）

#### minimal_polynomial

```cpp
bbla::minimal_polynomial<F>(int n, auto linear_map, auto random_sample)
```

$S^n$ から $S^n$ への線型写像に対し、最小多項式を求める。

##### 使い方

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


##### 使い方

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

##### 使い方

- `linear_map` は `vc<F::S>` を参照で受け取って破壊的に変更する関数として定義する。
- `random_sample` は `F::S` の要素をランダムに選ぶ関数（`F::S` が modint のときはデフォルトでよい）。

##### 制約

- `F` は体
- `linear_map` は長さ $n$ の vector に作用する
- `random_sample` のプールは十分大きい

##### 計算量

- $t$ を `linear_map` の計算量として、$O(n^2 + nt)$

---

### 例題

- https://judge.yosupo.jp/problem/sparse_matrix_det ： 疎行列の行列式そのもの。
- https://yukicoder.me/problems/no/310 ： BEST 定理を疎グラフに対して用いることになるので、疎行列の行列式が登場する。
- https://codeforces.com/contest/963/problem/E ： マルコフ連鎖（というかただの期待値 DP）で、立式すると線型方程式を解く問題になる（遷移が少ないので疎行列）。想定解は掃き出しを工夫する方針だが BBLA でも通る。

