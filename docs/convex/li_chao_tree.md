## 概要

一次関数 $f_i(x)=a_i x+b_i$ を全体または区間へ追加し、指定した $x$ における最適値と、それを達成する直線を求める。
両構造体は `convex/li_chao_tree.hpp` に置いている。

| 構造体 | 座標の指定 | 用途 |
| --- | --- | --- |
| `LiChaoTree` | 整数範囲 `[xmin, xmax)` | クエリ座標が逐次決まる場合、通常の直線追加 |
| `LiChaoTreeCompressed` | 全クエリ座標をコンストラクタに渡す | 区間追加、少数の座標への反復取得 |

`Compare = less<>` で最小値、`greater<>` で最大値を求める。
係数と座標は `T`、評価値は `U` で扱う。`U` の既定値は `larger_int_t<T>` なので、`T = ll` なら `U = i128` になる。
圧縮版では浮動小数点数や `Rational` の座標も使える。浮動小数点数の演算には丸め誤差がある。

区間追加は両版とも半開区間 `[l, r)`。傾き・追加順・取得順は任意でよい。
動的版は必要なノードを連続配列に追加し、圧縮版はソート・重複除去した座標に対する配列を持つ。

以下、動的版の範囲内の整数の個数を $C$、圧縮版へ渡した座標数を $Q$、重複除去後の座標数を $M$、作成済みノード数を $K$ とする。
対数を含む計算量では、範囲の大きさが 1 の場合を $O(1)$ とする。

## 使用例

```cpp
#include "convex/li_chao_tree.hpp"

LiChaoTree<ll> tree(-10, 11);  // 整数 -10, ..., 10 が対象
tree.add_line(2, 3);          // y = 2x + 3、id = 0
tree.add_segment(-2, 5, -1, 8, 20);  // [-2, 5) に y = -x + 8

auto [value, line] = tree.query(4);
// value = 4 (i128)、line = {-1, 8, 20}
```

全クエリ座標が分かる場合は圧縮版を使う。追加区間の端点は、登録座標以外の値も指定できる。

```cpp
LiChaoTreeCompressed<ll> tree({10, -3, 2, 2});
// 登録座標は {-3, 2, 10}
tree.add_line(2, 3);
tree.add_segment(0, 5, -1, 8);
auto result = tree.query(2);  // value = 6、id = 1

int k = tree.lower_bound(2);
assert(tree.query_index(k) == result);

int l = tree.lower_bound(-3), r = tree.lower_bound(10);
tree.add_segment_index(l, r, 0, -7);  // 座標 -3, 2 に y = -7
```

最大値、評価型、空の結果の値も指定できる。

```cpp
LiChaoTree<ll, i128, greater<>> max_tree(-100, 101);
LiChaoTreeCompressed<ll, ll> small_values({-1, 0, 1});
LiChaoTree<ll, i128, less<>, 1000, 1000000> custom_empty(-10, 11);
auto empty = custom_empty.query(0);
// empty.first = 1000000、empty.second = {0, 1000, -1}

using R = Rational<ll>;
LiChaoTreeCompressed<R> rational_tree({R(-1, 2), R(0), R(1, 2)});
```

## 詳細なドキュメント

### LiChaoTree

```cpp
template <class T, class U = larger_int_t<T>, class Compare = less<>,
          auto inftyT = nullptr, auto inftyU = nullptr>
struct LiChaoTree
```

#### コンストラクタ

```cpp
LiChaoTree(T xmin, T xmax)
```

整数座標 `[xmin, xmax)` を対象とする空の木を構築する。
問題の制約から分かる座標範囲を指定すると、木の深さを抑えられる。

##### 制約

- `xmin < xmax`
- `T` は符号付き整数型（`i128` も使用可）
- `U` に `T` からの明示変換、加算、乗算、大小比較、符号反転が定義されている
- `U` が整数型なら符号付きである
- `Compare` は `less<>`, `less<T>`, `greater<>`, `greater<T>` のいずれか
- `inftyT`, `inftyU` は、それぞれ `T`, `U` に変換できる正の値、またはその値を返す引数なし関数
- 無限大として指定した値とその符号反転が、対応する型で表現できる

`nullptr` を指定した場合は、対応する型の[既定の無限大](../utils/default_infty.md)を使う。
これらの無限大は、適用される直線がない座標で返す値に使う。

##### 計算量

- $O(1)$

#### Line

```cpp
using Line = LiChaoTreeLine<T>;

struct LiChaoTreeLine {
  T a, b;
  int id;
};
```

直線 $y=ax+b$ の係数と ID を保持する。`==` で全メンバを比較できる。
返される係数は追加時に指定した値である。

#### add_line

```cpp
void add_line(T a, T b)
void add_line(T a, T b, int id)
```

全座標範囲に直線 $y=ax+b$ を追加する。

`id` の省略時は、`clear` 以降に呼んだ追加操作の順番を 0 始まりで使う。
明示 ID、空区間、対象座標を含まない区間の追加も 1 回と数える。
同じ ID を複数の追加に使える。

##### 制約

- `id >= 0`
- 対象範囲の座標に対する `U(a) * U(x) + U(b)` の変換・中間計算・結果と、その大小比較が使用する型で扱える

##### 計算量

- $O(\log C)$ 償却

#### add_segment

```cpp
void add_segment(T l, T r, T a, T b)
void add_segment(T l, T r, T a, T b, int id)
```

`[l, r)` と `[xmin, xmax)` の共通部分に直線 $y=ax+b$ を追加する。
ID の扱いは `add_line` と共通。

##### 制約

- `l <= r`
- ID と評価型の条件は `add_line` と同じ

##### 計算量

- $O(\log^2 C)$ 償却

#### query

```cpp
pair<U, Line> query(T x) const
```

$x$ に適用される直線の最適値と、その直線を返す。同値の場合は最適な直線のうち 1 本を返す。

適用される直線がない場合は、最小値なら `{eU, {0, eT, -1}}`、最大値なら `{-eU, {0, -eT, -1}}` を返す。
`eT`, `eU` は `inftyT`, `inftyU` をそれぞれの型として解決した値。
空の判定には `result.second.id == -1` を使う。直線がある場合は、指定した無限大の大きさによらず実際の評価値を返す。

##### 制約

- `xmin <= x && x < xmax`
- 評価型の条件は `add_line` と同じ

##### 計算量

- $O(\log C)$

#### reserve

```cpp
void reserve(int n)
```

少なくとも `n` ノード分の容量を確保する。通常の直線追加では 1 回につき高々 1 ノード増える。
区間追加では 1 回につき $O(\log C)$ ノード増える。

##### 制約

- `n >= 0`

##### 計算量

- $O(K)$

#### clear

```cpp
void clear()
```

全直線を消去し、自動採番を 0 に戻す。座標範囲と確保済みの容量を保持する。

##### 計算量

- $O(K)$

### LiChaoTreeCompressed

```cpp
template <class T, class U = larger_int_t<T>, class Compare = less<>,
          auto inftyT = nullptr, auto inftyU = nullptr>
struct LiChaoTreeCompressed
```

`Line`、最小値・最大値の選択、評価型、ID、空の取得結果は `LiChaoTree` と共通。

#### コンストラクタ

```cpp
explicit LiChaoTreeCompressed(vc<T> xs)
```

取得で使う全座標を受け取り、ソート・重複除去して空の木を構築する。空の座標列も指定できる。
区間追加の端点は、登録座標以外の値も指定できる。

##### 制約

- `T` に大小比較と等値比較が定義されている
- `T` が整数型なら符号付きである
- 係数と座標は有限であり、大小比較が整合的である
- `U`, `Compare`, `inftyT`, `inftyU` の条件は `LiChaoTree` と同じ

##### 計算量

- $O(Q \log Q)$

#### coordinates・lower_bound

```cpp
const vc<T>& coordinates() const
int lower_bound(const T &x) const
```

`coordinates()` は登録座標を昇順・重複なしで返す。
`lower_bound(x)` は、`x` 以上となる最初の登録座標の添字を返す。該当する座標がなければ $M$ を返す。
登録済み座標の添字や、追加区間の端点の添字を取得するときに使う。

##### 計算量

- `coordinates`：$O(1)$
- `lower_bound`：$O(\log M)$

#### add_line

```cpp
void add_line(T a, T b)
void add_line(T a, T b, int id)
```

全登録座標に直線を追加する。ID の扱いは `LiChaoTree` と共通。

##### 制約

- `id >= 0`
- 登録座標での評価と比較が、使用する型で扱える。有理数では分子・分母・比較の交差積などの中間計算も含む

##### 計算量

- $O(\log M)$

#### add_segment・add_segment_index

```cpp
void add_segment(T l, T r, T a, T b)
void add_segment(T l, T r, T a, T b, int id)
void add_segment_index(int l, int r, T a, T b)
void add_segment_index(int l, int r, T a, T b, int id)
```

`add_segment` は座標が `[l, r)` に入る登録点へ追加する。
`add_segment_index` は圧縮後の添字が `[l, r)` に入る登録点へ追加する。
ID の扱いは `add_line` と共通。

##### 制約

- `add_segment`：`l <= r`
- `add_segment_index`：`0 <= l && l <= r && r <= M`
- ID と評価型の条件は圧縮版の `add_line` と同じ

##### 計算量

- $O(\log^2 M)$

#### query・query_index

```cpp
pair<U, Line> query(const T &x) const
pair<U, Line> query_index(int k) const
```

`query(x)` は登録座標 `x`、`query_index(k)` は `coordinates()[k]` における結果を返す。
返り値の扱いは `LiChaoTree::query` と共通。

##### 制約

- `query`：`x` は登録座標に含まれる
- `query_index`：`0 <= k && k < M`
- 評価型の条件は圧縮版の `add_line` と同じ

##### 計算量

- $O(\log M)$

#### clear

```cpp
void clear()
```

全直線を消去し、自動採番を 0 に戻す。登録座標と確保済みの容量を保持する。

##### 計算量

- $O(M)$
