## テンプレート（vector）

### マクロ

- `ALL(x)`
- `SZ(x)`：vector の長さを `ll` 型で
- `SZI(x)`：vector の長さを `int` 型で

### vector の生成

#### gen_vec

`gen_vec(int n, F f)`

長さ $n$ で、$i$ 番目が $f(i)$ の vector を返す。

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