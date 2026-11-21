このドキュメントには AI による作成・変更が含まれています。

## 概要

二値変数や、有限個の整数値を取る変数について、制約を満たす範囲で追加した関数の和を最小化する。`solve()` は最小値とそれを達成する割り当ての組を返す。制約が矛盾する場合は `nullopt`。

- `ProjectSelectionProblem<Cost>` では、各変数は $0$ または $1$。二変数関数は劣モジュラ性を満たすものを扱う。
- `K_ProjectSelectionProblem<Cost>` では、変数 $i$ の値域を $[0,k_i)$ と指定する。二変数関数は Monge 性を満たすものを扱う。

`add1` と `add2` の `nullopt` は、その値や値の組を禁止する。項の全入力を禁止した場合は `solve()` が `nullopt` を返す。

求解には最小カットを使う。 [MaxFlow](maxflow.md)

二値変数の一変数・二変数関数の最小カットへの帰着については次を参照。 [Kolmogorov–Zabih, §4](https://www.cs.cornell.edu/~rdz/Papers/KZ-PAMI04.pdf)

順序のある有限値域から二値変数への帰着については次を参照。 [Schlesinger–Flach, §§3–4](https://wwwpub.zih.tu-dresden.de/~ds24/publications/tr_kto2.pdf)

## 使用例

```cpp
#include "flow/project_selection.hpp"

ProjectSelectionProblem<ll> p(2);
p.add2(0, 1, 3, 5, nullopt, 2);  // (1, 0) を禁止し、残りの組にそれぞれ値を加える
const auto solution = p.solve();
assert(solution);
const auto &[value, x] = *solution;
assert(value == 2);
assert((x == vc<int>{1, 1}));

K_ProjectSelectionProblem<ll> q(2, {4, 3});
q.add1(0, {6, 5, 3, 0});
q.add1(1, {5, 2, 0});
q.add2(0, 1, [](int a, int b) -> optional<ll> {
  if (a >= 3 && b < 2) return nullopt;  // x[0] >= 3 なら x[1] >= 2
  return abs(a - b);
});
assert((q.solve() == pair{1LL, vc<int>{3, 2}}));
q.add1(0, {nullopt, nullopt, nullopt, 0});  // x[0] = 3 に固定
q.add1(1, {nullopt, 0, nullopt});          // x[1] = 1 に固定
assert(!q.solve());
```

## 詳細なドキュメント

### ProjectSelectionProblem

#### コンストラクタ・size

```cpp
template <class Cost> class ProjectSelectionProblem;
ProjectSelectionProblem();
explicit ProjectSelectionProblem(int n);
template <class I = ll> I size() const;
```

$n$ 個の二値変数を用意する。既定コンストラクタでは $n=0$。`size()` は $n$ を返す。

##### 制約

- $n\ge 0$。
- `Cost` は符号付き整数型。`i128` に対応する。
- 追加する関数の各値と、和の最小値が `Cost` に収まる。

##### 計算量

- $O(1)$。

#### add_constant・add1

```cpp
void add_constant(Cost value);
void add1(int i, optional<Cost> f0, optional<Cost> f1);
template <class F> void add1(int i, const F &f);
```

`add_constant` は定数を加える。`add1` は $x_i=0$ のとき `f0`、$x_i=1$ のとき `f1` を取る関数を加える。数値はそのまま渡せる。`nullopt` を指定した値は禁止する。例えば `add1(i, nullopt, 0)` は $x_i=1$ に固定する。関数を渡す形式では `f(0)` と `f(1)` が `Cost` または `optional<Cost>` を返す。

##### 制約

- $0\le i<n$。

##### 計算量

- 償却 $O(1)$。関数の評価時間を除く。

#### add2

```cpp
void add2(int i, int j, optional<Cost> f00, optional<Cost> f01,
          optional<Cost> f10, optional<Cost> f11);
template <class F> void add2(int i, int j, const F &f);
```

$(x_i,x_j)=(a,b)$ のとき `fab` または `f(a, b)` を取る関数を加える。`nullopt` を指定した組は禁止する。例えば `add2(i, j, f00, f01, nullopt, f11)` は $(1,0)$ を禁止し、残りの組に指定した値を加える。

関数を渡す形式では `f(a, b)` が `Cost` または `optional<Cost>` を返す。`i == j` では対角の組を使う。`LOCAL` では劣モジュラ性の違反時に変数番号と4組の引数を表示する。

##### 制約

- $0\le i,j<n$。
- $i\ne j$ で $(0,1)$ と $(1,0)$ を両方許す場合は、$(0,0)$ と $(1,1)$ も許し、劣モジュラ性 $f_{00}+f_{11}\le f_{01}+f_{10}$ を満たす。

##### 計算量

- 償却 $O(1)$。関数の評価時間を除く。

#### add_all0・add_all1

```cpp
template <class I = int>
void add_all0(const vc<I> &vertices, Cost f_all0, Cost f_exists1);
template <class I = int>
void add_all1(const vc<I> &vertices, Cost f_all1, Cost f_exists0);
```

`add_all0` は、指定した変数がすべて $0$ のとき `f_all0`、それ以外のとき `f_exists1` を取る関数を加える。`add_all1` は、指定した変数がすべて $1$ のとき `f_all1`、それ以外のとき `f_exists0` を取る関数を加える。空配列では第2引数を定数として加える。変数番号の重複も許す。

##### 制約

- `vertices` の各要素が $[0,n)$ にある。`I` は `i128`・`u128` を含む整数型。
- `f_all0 <= f_exists1`、`f_all1 <= f_exists0`。

##### 計算量

- 配列の長さに対して償却線形時間。

#### solve

```cpp
optional<pair<Cost, vc<int>>> solve() const;
```

制約を満たす割り当てについて、関数の和の最小値と、その値を達成する元の $n$ 変数の割り当てを返す。割り当ては変数番号順。制約が矛盾する場合は `nullopt`。変数が $0$ 個の場合も、制約を満たせば空の割り当てを含む組を返す。

禁止に対応する辺の容量は、有限容量の総和に $1$ を加えて求解時に設定する。

##### 計算量

- 元の変数・補助変数・始点・終点の総数を $N$、有限辺数を $A$、制約辺数を $H$、有限容量の総和を $B$、`Cost` に対応する符号なし整数型の最大値を $U$ とする。生成される辺数は $M=O(A+H\lceil (B+1)/U\rceil)$。
- $O(N+M+qT(N,M))$。$T(N,M)$ は最大流1回の時間計算量、$q$ は流量を $U$ 以下に分けて最大流を呼ぶ回数。

### K_ProjectSelectionProblem

#### コンストラクタ

```cpp
template <class Cost, Cost = Cost(0)> class K_ProjectSelectionProblem;
K_ProjectSelectionProblem();
template <class I = int> K_ProjectSelectionProblem(int n, const vc<I> &ks);
template <class F> K_ProjectSelectionProblem(int n, const F &count);
```

$n$ 個の整数変数を用意し、変数 $i$ の値域を $[0,k_i)$ とする。`ks[i]` または `count(i)` で $k_i$ を指定する。既定コンストラクタでは $n=0$。

第2テンプレート引数は旧版との互換用。順序制約に必要な容量は追加した関数から自動で決まる。

##### 制約

- $n\ge 0$。各 $k_i\ge 1$。
- 配列で指定する場合、長さは $n$。$k_i$ は `i128`・`u128` を含む整数型で与える。
- `Cost` の条件は `ProjectSelectionProblem` と同じ。

##### 計算量

- $O(\sum_i k_i)$。`count` の評価時間を除く。

#### add1

```cpp
template <class T = optional<Cost>> void add1(int i, const vc<T> &f);
template <class T> void add1(int i, initializer_list<T> f);
template <class F> void add1(int i, const F &f);
```

$x_i=a$ のとき `f[a]` または `f(a)` を取る関数を加える。`nullopt` を指定した値は禁止する。配列の要素や `f(a)` の戻り値には `Cost` と `optional<Cost>` の両方を使える。

##### 制約

- $0\le i<n$。
- 配列で指定する場合、長さは $k_i$。

##### 計算量

- 償却 $O(k_i)$。関数の評価時間を除く。

#### add2

```cpp
template <class T = optional<Cost>> void add2(int i, int j, const vvc<T> &f);
template <class T = Cost> void add2(int i, int j, initializer_list<vc<T>> f);
template <class F> void add2(int i, int j, const F &f);
```

$(x_i,x_j)=(a,b)$ のとき `f[a][b]` または `f(a,b)` を取る関数を加える。`nullopt` を指定した組は禁止する。`i == j` では対角の組を使う。表の要素や `f(a,b)` の戻り値には `Cost` と `optional<Cost>` の両方を使える。

`LOCAL` では Monge 性の違反時に変数番号と該当する行・列を表示する。不等式に違反した場合は4つの値も表示する。

##### 制約

- $0\le i,j<n$。
- 配列で指定する場合、$k_i$ 行 $k_j$ 列。
- $i\ne j$ のとき、$a<a^{\prime}$、$b<b^{\prime}$ で $(a,b^{\prime})$ と $(a^{\prime},b)$ を両方許すなら、$(a,b)$ と $(a^{\prime},b^{\prime})$ も許し、次の Monge 不等式を満たす。

$$
f(a,b)+f(a^{\prime},b^{\prime})\le f(a,b^{\prime})+f(a^{\prime},b).
$$

全組を許す場合、この条件は隣接する $2\times 2$ の区画だけで判定できる。

##### 計算量

- 償却 $O(k_i k_j)$。`i == j` では償却 $O(k_i)$。関数の評価時間を除く。

#### solve

```cpp
optional<pair<Cost, vc<int>>> solve() const;
```

制約を満たす割り当てについて、関数の和の最小値と、その値を達成する割り当てを変数番号順に返す。制約が矛盾する場合は `nullopt`。

##### 計算量

- $\sum_i (k_i-1)$ 個の二値変数からなる問題の求解に加え、割り当ての復元に $O(n+\sum_i \log k_i)$。
