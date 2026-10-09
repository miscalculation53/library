## 概要

テンプレート（vector）

## 詳細なドキュメント

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
サイズには `ll` と `int` を混在させられる。要素型は `dvec<T>` で指定でき、省略時は初期値から推論する。
型を指定した場合、初期値を `T` に変換して使う。

一次元の vector で保持する多次元配列には [FlatDvec](../ds/flat_dvec.md) がある。

使用例：

```cpp
// dp[i][j][k] (0<=i<N, 0<=j<M, 0<=k<2)
// 初期値は 0
// 初期値の部分を 0 と書くと値の型が int になるので注意
// N が ll、M が int でもそのまま書ける
auto dp = dvec({N, M, 2}, 0LL);
fill(dp, -1); // 全要素を -1 にする

auto dp_mod = dvec<mint>({N, 2}, 0); // 要素型を mint に指定する
```

### 全要素の代入

#### fill

```cpp
void fill(V& v, const T& value)
```

配列の全要素を `value` にする。入れ子の `vector` や `array`、C 配列を再帰的に処理する。
[FlatDvec](../ds/flat_dvec.md) にも同じ書き方を使える。
値を代入できる要素の型に達したところで、各要素に代入する。
例えば `vector<string>` に文字列を渡すと、各文字列全体を置き換える。

```cpp
auto dp = dvec({3, 4, 5}, 0LL);
fill(dp, -1);           // 全要素を -1 にする。int から ll への代入も可能
fill(dp[1], 0);         // 第 1 次元の添字が 1 の部分だけを更新する

array<array<int, 3>, 2> a{};
fill(a, 7);
```

##### 制約

- `v` は変更可能で、`begin` / `end` による走査が可能
- 各要素に `value` を代入できる、または各要素が同じ条件を満たす配列

##### 計算量

処理するコンテナと代入先の要素の総数を $S$ として、

- $O(S)$。値の代入にかかる時間は別途必要

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
T vecget(vc<T> v, int i, T dflt_negative = -default_infty<T>(), T dflt_positive = default_infty<T>())
```

範囲外も考慮した vector の取得。範囲内なら通常の `[]` や `at` と同じで、範囲外なら負方向は `dflt_negative`, 正方向は `dflt_positive` を返す。

省略時は `T` の[既定の無限大](../utils/default_infty.md)の符号を変えた値を返す。例えば `i128` なら `±(INF * INF)`、`Rational` なら `±1/0` になる。明示指定した範囲外の値はそのまま使う。

用途としては、ソート済み配列が無限に広がっていると考えたい場合（特に、`binsearch` や `expsearch` に渡すとき）。`LB` や `UB` だと頭が壊れるとき用に。→ 二分探索に lt, leq, gt, geq 系をつけたので出番がないかも。
