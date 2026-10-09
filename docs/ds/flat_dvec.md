## 概要

DP などに使う固定形状の多次元配列。全要素を一次元の `vector<T>` で保持する。
`FlatDvec<T>` と書くと要素型だけを指定でき、次元数と各次元の長さを構築時に保持する。
`FlatDvec<T, D>` では次元数をコンパイル時に指定できる。

最後の添字が連続する通常配置を使う。`dp(i, j, k)` なら `k` が最も内側になる。
次元の長さから配置を選ぶ方式との速度比較は [DP の計測結果](flat_dvec_benchmark.md) を参照。

添字は宣言した順に渡す。例えば `{N, M, K}` なら `dp(i, j, k)` で参照する。
入れ子の vector が必要な場合は [dvec](../template/template_vector.md#dvec) を使う。

## 使用例

```cpp
#include "ds/flat_dvec.hpp"
#include "math/modint/modint.hpp"
using mint = modint998244353;

ll N = 100;
int M = 50;
FlatDvec<mint> dp({N, M, 2}, 0); // ll と int が混在してもよい
dp(0, 0, 0) = 1;
dp.at(1, 2, 1) += dp(0, 0, 0);

assert(dp.ndim() == 3);        // 次元数
assert(dp.size() == size_t(N) * M * 2); // 全要素数
assert(dp.size(1) == size_t(M)); // 第 1 次元の長さ
assert(dp.stride(2) == 1);     // 最後の次元が連続する

dp.fill(0);                   // メンバ関数
fill(dp, 0);                  // dvec と共通の書き方
for (auto &x : dp) x = 1;     // 内部の配置順に全要素を走査する
```

## 詳細なドキュメント

次元数を $r$、各次元の長さを $n_0,\ldots,n_{r-1}$、全要素数を $P$ とする。
$r>0$ なら $P=\prod_d n_d$、$r=0$ なら $P=0$。

### FlatDvec

#### コンストラクタ

```cpp
(1) FlatDvec<T, D = 0>()
(2) FlatDvec<T, D = 0>(const I (&sz)[R], const T& init = T())
(3) FlatDvec<T, D = 0>(const array<I, R>& sz, const T& init = T())
```

- (1)：空配列を構築する。`D=0` なら次元数も 0、`D>0` なら全次元の長さが 0。
- (2), (3)：各次元の長さを `sz[d]` とし、全要素を `init` で初期化する。長さ 0 の次元があれば空配列になる。

`D` の省略時（`D=0`）は次元数を構築時に保持する。
例えば `FlatDvec<mint> flat({N, 2}, 0)` では、初期値 `0` を `mint` に変換して全要素を初期化する。
`FlatDvec<mint> flat({N, 2})` と初期値を省略すると `mint()` で初期化する。

初期値から要素型と次元数を推論する `FlatDvec dp({N, M, 2}, 0LL)` も使える。この型は `FlatDvec<ll, 3>` になる。
`{N, M, 2}` の長さは `ll` として受け取れるため、`N, M` が `ll` でも定数は `2` と書ける。
長さを既存の整数配列や `std::array` で渡すこともできる。

##### 制約

- (2), (3)：$R \geq 1$。`D>0` を指定する場合は $R=D$
- (2), (3)：`I` は整数型、各 `sz[d]` は非負
- `T` は `vector<T>` の要素として、`init` によるコピーで初期化できる型

##### 計算量

- (1)：$O(r+1)$
- (2), (3)：$O(r+P)$

#### operator() / at

```cpp
reference operator()(I... indices)
const_reference operator()(I... indices) const
reference at(I... indices)
const_reference at(I... indices) const
```

指定した添字の要素への参照を返す。`at` は各次元の範囲を `assert` で確認する。
`operator()` は範囲確認を省いてアクセスする。
`D=0` の場合は、両方の操作で添字の個数が次元数と等しいかを `assert` で確認する。
`D>0` の場合は添字の個数をコンパイル時に確認する。

返り値は `vector<T>::reference` / `vector<T>::const_reference`。
通常は `T&` / `const T&`、`T=bool` の場合は `vector<bool>` と同じ参照型になる。

##### 制約

- 添字は整数型で、個数は $r$
- 各次元 $d$ の添字 $i_d$ は $0 \leq i_d < n_d$

##### 計算量

- $O(r)$

#### ndim / size / empty / shape / stride

```cpp
size_t ndim() const
size_t size() const
size_t size(size_t d) const
bool empty() const
const shape_type& shape() const
size_t stride(size_t d) const
```

- `ndim()`：次元数 $r$ を返す。
- `size()`：全要素数 $P$ を返す。
- `size(d)`：第 $d$ 次元の長さ $n_d$ を返す。
- `empty()`：全要素数が 0 かを返す。
- `shape()`：宣言順に並べた各次元の長さを返す。
- `stride(d)`：第 $d$ 次元の添字を 1 進めたときの、一次元配列上での移動幅を返す。

要素の位置は $\sum_d i_d\,\mathrm{stride}(d)$。
要素を持つ配列の stride は、末尾の次元から累積積で割り当てる。
例えば `{N, M, K}` なら stride は `{M*K, K, 1}` になる。
次元を持つ空配列の stride は末尾が 1、他が 0。

`shape_type` は `D=0` なら `vector<size_t>`、`D>0` なら `array<size_t, D>`。

##### 制約

- `size(d)`, `stride(d)`：$0 \leq d < r$

##### 計算量

- $O(1)$

#### fill

```cpp
void fill(const T& value)
```

全要素を `value` にする。
[共通の fill](../template/template_vector.md#fill) を使い、`fill(dp, value)` と書くこともできる。

##### 計算量

- $O(P)$

#### begin / end / content

```cpp
iterator begin()
iterator end()
const_iterator begin() const
const_iterator end() const
const vector<T>& content() const
```

`begin`, `end` は内部の配置順で全要素を走査するイテレータを返す。
`content` は内部の一次元 vector を定数参照で返す。

##### 計算量

- $O(1)$
