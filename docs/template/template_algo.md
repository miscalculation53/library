## テンプレート（アルゴリズム）

### 集約

#### SUM

`U SUM(vc<T> v)`

$v$ の要素の総和を（型 `U` の値として）返す。`U` はデフォルトでは `ll`。

##### 計算量

- $O(|v|)$

#### MAX, MIN

```cpp
(1) T MAX(vc<T> v, T dflt = INF)
(2) T MIN(vc<T> v, T dflt = -INF)
```

それぞれ、$v$ の要素の最大値、最小値を返す。

##### 制約

- $|v| > 0$

##### 計算量

- $O(|v|)$

#### ARGMAX, ARGMIN

```cpp
(1) T ARGMAX(vc<T> v)
(2) T ARGMIN(vc<T> v)
```

それぞれ、$v$ の要素が最大値、最小値をとるインデックス（のうち最小のもの）を返す。$v$ が空のときは $0$ を返す。

##### 計算量

- $O(|v|)$

#### mex

`U mex(vc<T> v)`

$v$ の要素の mex を（型 `U` の値として）返す。`U` はデフォルトでは `ll`。

$v$ の要素はすべて非負整数であることを仮定。負の数がある場合、それを無視した上で動作する。

##### 計算量

- $O(|v|)$

### 順列

#### permid

`vc<T = ll> permid(int n, int base_index = 0)`

長さ $n$ の恒等順列 $(0, 1, \dots, n-1)$ を返す。

##### 計算量

- $O(n)$

#### perminv

`vc<T> perminv(vc<T> p)`

順列 $p$ の逆順列を返す。すなわち、$q_{p_i} = i$ を満たす順列 $q$ を返す。

$p$ が $(0, 1, \dots, |p|-1)$ の順列でない場合は、$p_i \geq 0$ なるすべての $i$ に対し $q_{p_i} = i$ を満たす配列 $q$ をどれか一つ返す。

##### 計算量

- $O(|p| + \max(p))$

#### permuted

`vc<T> permuted(vc<T> a, vc<U> p)`

配列 $a$ を配列 $p$ にしたがって並べ替えた配列を返す。すなわち、$(a_{p_0}, a_{p_1}, \dots, a_{p_{|p|-1}})$ を返す。特に、$p$ が順列の場合や $a, p$ ともに順列の場合の使用を想定している。

##### 制約

- $0 \leq p_i < |a|$

##### 計算量

- $O(|p|)$


### string や vector の操作

#### reversed

```cpp
(1) string reversed(string s)
(2) vc<T> reversed(vc<T> v)
```

列を逆順にしたものを返す（破壊的変更は行わない）。

使用例：

- 回文判定が `s == reversed(s)` と書ける
- 左右から見る系の処理で `reversed(f(reversed(a)))` などと書ける

##### 計算量

列の長さを $n$ として

- $O(n)$

#### SORT, sorted

```cpp
(1) string sorted(string s, ...)
(2) vc<T> sorted(vc<T> v, ...)
```

列をソートしたものを返す（破壊的変更は行わない）。

C++17 以前ならば `std::sort`, C++20 以降ならば `std::ranges::sort` の引数を後ろにとれる。

##### 制約

`std::sort` に準じる。

##### 計算量

`std::sort` に準じる。具体的には、列の長さを $n$ として

- $O(n \log n)$

#### unique, uniqued, sortunique, sortuniqued

`std::unique` をもとにしている。

```cpp
(1) void unique(string &s)
(2) void unique(vc<T> &v)
(3) string uniqued(string s)
(4) vc<T> uniqued(vc<T> v)
(5) void sortunique(string &s)
(6) void sortunique(vc<T> &v)
(7) string sortunique(string s)
(8) vc<T> sortunique(vc<T> v)
```

- (1), (2)：重複する隣接要素を削除する。
- (3), (4)：重複する隣接要素を削除した列を返す。
- (5), (6)：ソートした後、重複する隣接要素を削除する。
- (7), (8)：「ソートした後、重複する隣接要素を削除する」操作を行った結果の列を返す。

##### 計算量

列の長さを $n$ として

- (1), (2), (3), (4)：$O(n)$
- (5), (6), (7), (8)：$O(n \log n)$

#### rotate, rotated

`std::rotate` をもとにしている。

```cpp
(1) void rotate(string &s, U k)
(2) void rotate(vc<T> &v, U k)
(3) string rotated(string s, U k)
(4) vc<T> rotated(vc<T> v, U k)
```

- (1), (2)：列を**右に** $k$ 個 cyclic-shift する。
- (3), (4)：列を**右に** $k$ 個 cyclic-shift してできる列を返す。

右シフトなので、たとえば $(0, 1, 2, 3, 4)$ は $1$ 個シフトすると $(1, 2, 3, 4, 0)$ になる。

##### 制約

- `U` は整数
- $k$ は型 `U` の上限・下限付近ではない

##### 計算量

列の長さを $n$ として

- $O(n)$

### 二次元配列の操作

#### top

```cpp
(1) vvc<T> top(vvc<T> a)
(2) vstr top(vstr a)
```

二次元配列 $a$ を転置した二次元配列を返す。すなわち、$a$ のサイズが $n \times m$ であるとき、サイズが $m \times n$ で、$a_{i,j} = b_{j,i} \: (0 \leq i < n, 0 \leq j < m)$ を満たす二次元配列 $b$ を返す。

ただし、サイズが $0 \times m$ の二次元配列は表現できない（$0 \times 0$ とみなされる）ことに注意。

##### 計算量

$a$ のサイズを $n \times m$ として

- $O(nm)$

#### rot90

```cpp
(1) vvc<T> rot90(vvc<T> a, U k = 1)
(2) vstr rot90(vstr a, U k = 1)
```

二次元配列を**反時計回りに** $90$ 度回転することを $k$ 回行った結果の二次元配列を返す。

```
12
34 -> 246
56    135
```

同じく $0 \times m$ には注意。

##### 制約

- `U` は整数
- $k$ は型 `U` の上限・下限付近ではない

##### 計算量

$a$ のサイズを $n \times m$ として

- $O(nm)$

### 累積和・差分

#### cuml, cumr

```cpp
(1) vc<T> cuml(vc<T> v, F op = plus<>(), T e = 0)
(2) vc<T> cumr(vc<T> v, F op = plus<>(), T e = 0)
(3) vc<T> cumlmax(vc<T> v)
(4) vc<T> cumrmax(vc<T> v)
(5) vc<T> cumlmin(vc<T> v)
(6) vc<T> cumrmin(vc<T> v)
```

$|v| = n$ とする。

- (1)：長さ $n + 1$ の配列を返す。$i \: (0 \leq i \leq n)$ 番目には、$v[0, i)$ 全体に二項演算 $\mathrm{op}$ を行った結果を格納する。単位元は $\mathrm{e}$ 。デフォルトでは、$\mathrm{op}$ は $+$ 、$\mathrm{e}$ は $0$ 。
- (2)：長さ $n + 1$ の配列を返す。$i \: (0 \leq i \leq n)$ 番目には、$v[i, n)$ 全体に二項演算 $\mathrm{op}$ を行った結果を格納する。単位元は $\mathrm{e}$ 。デフォルトでは、$\mathrm{op}$ は $+$ 、$\mathrm{e}$ は $0$ 。
- (3), (4), (5), (6)：(1) や (2) で、$(\mathrm{op}, \mathrm{e})$ を $(\max, -\mathrm{INF})$ や $(\min, \mathrm{INF})$ としたもの。

例 (cuml)：$(3, 1, 4, 1) \to (0, 3, 4, 8, 9)$

##### 制約

- $\mathrm{op}$ は可換

##### 計算量

- $O(n)$

#### adjd

`vc<T> adjd(vc<T> v)`

$|v| = n$ とする。

長さ $n+1$ の配列を返す。$i \: (0 \leq i \leq n)$ 番目には、$v_i - v_{i-1}$ が格納される（ただし、$v_{-1}$ や $v_n$ は $0$ とみなす）。

例：$(3, 1, 4, 1) \to (3, -2, 3, -3, -1)$ 

##### 計算量

- $O(n)$

### 全探索

#### direct_product

```cpp
direct_product(vc<T> a)
```

長さ $|a|$ で、$i \: (0 \leq i < |a|)$ 番目が $[0, a_i)$ であるような vector をすべて（辞書順で）列挙する。
  - 例：$a = (2, 1, 3)$ のとき
    - $(0, 0, 0)$
    - $(0, 0, 1)$
    - $(0, 0, 2)$
    - $(1, 0, 0)$
    - $(1, 0, 1)$
    - $(1, 0, 2)$

イテレータを実装している形なので、たとえば範囲 for の中で
```
fec(v : direct_product({2, 1, 3}))
```
のように使う。

##### 計算量

- $1$ 回のイテレーションが償却 $O(1)$

### その他

#### dij4

`const vpll dij4 = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }`

グリッドの $4$ 方向の探索で使う。

$(i, j)$ の $i$ を下方向、$j$ を右方向とする座標系において、`DRUL` の順になっている。