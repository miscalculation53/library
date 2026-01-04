## テンプレート（vector）

### マクロ

#### ALL

`ALL(x)`：`begin(x), end(x)`

#### SZ

```cpp
T SZ<T=ll>(x)
```

`x.size()` を `T` 型で返す（デフォルトでは `ll`）。

#### LMD

ラムダ式で、返り値が単一の式であるときに短く書けるマクロ。

`LMD(x, f(x))`：`[&](auto x) { return f(x); }`


### vector の生成

#### gen_vec

```cpp
gen_vec(int n, F f)
```

長さ $n$ で、$i$ 番目が $f(i)$ の vector を返す。（あまり使わないかも。）

利点：

- 生成する vector の型を書かなくてもよい
- vector を一時変数のように扱える

マクロとして `GEN_VEC(n, i, f(i))` がある。これは `gen_vec(n, LMD(i, f(i)))`。



### 多次元 vector の生成

#### dvec

多次元 vector を生成する。DP 配列の初期化などで使う。

使用例：

```cpp
// dp[i][j][k] (0<=i<N, 0<=j<M, 0<=k<2)
// 初期値は 0
// 初期値の部分を 0 と書くと値の型が int になるので注意
// N, M が ll の場合、定数は 2LL などとする
auto dp = make_vec({N, M, 2}, 0LL);
```

### 文字列から数値の vector に変換

#### ctol

`T ctol<T = ll>(char c, string s)`

文字と数値の関係を表す文字列 $s$ を使って、文字 $c$ を数値に変換する。たとえば、`ctol(c, "JOI")` なら、`J` $\to 0$, `O` $\to 1$, `I` $\to 2$ という変換をする。

厳密には、$s$ の中に初めて $c$ が現れる index（見つからなければ $-1$）を返す。（基本的に $s$ の文字が異なることを想定しているが、そうでなくても動作する。）

#### stov

```cpp
(1) vc<T = ll> stov(string s, char first)
(2) vc<T = ll> stov(string s, string t)
```

- (1)：$s$ の各文字 `s[i]` を `s[i] - first` として数値に変換し、それを並べた vector を返す。
  - `first` は基本的には `'A'`, `'a'`, `'0'` を使うことが多いと思う。

- (2)：$s$ の各文字 `s[i]` を `ctol(s[i], t)` として数値に変換し、それを並べた vector を返す。

### 結合

#### concat

```cpp
(1) vc<T> concat(vvc<T> vs)
(2) vc<T> concat(vc<T> v, vc<T1> v1, vc<T2> v2, ...)
```

vector を結合したものを返す。(2) では、`v1`, `v2`, $\dots$ の型が必ずしも `T` と一致していなくてもよい（`ll` と `int` など）。

### マージ

#### merged

```cpp
vc<T> merged(vc<T> a, vc<T> b)
```

$a$, $b$ はともに昇順にソートされているとき、$a$ と $b$ をマージして昇順にソートされた vector を返す。（特定の用途での）`std::merge` を使いやすくしたもの。

##### 計算量

- $O(\lvert a \rvert + \lvert b \rvert)$

### 取得

#### vecget

```cpp
T vecget(vc<T> v, int i, T dflt_negative = -INF, T dflt_positive = INF)
```

範囲外も考慮した vector の取得。範囲内なら通常の `[]` や `at` と同じで、範囲外なら負方向は `dflt_negative`, 正方向は `dflt_positive` を返す。

用途としては、ソート済み配列が無限に広がっていると考えたい場合（特に、`binsearch` や `expsearch` に渡すとき）。`LB` や `UB` だと頭が壊れるとき用に。→ 二分探索に lt, leq, gt, geq 系をつけたので出番がないかも。
