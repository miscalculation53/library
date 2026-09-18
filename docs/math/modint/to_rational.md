## 概要

modint の値を小さい有理数として復元する。

`dump(x | cp::rat())` とすると、復元した有理数を `1/2` のように表示できる。
`cp::rat()` は C++23 以降で利用でき、`dump` で表示するには `LOCAL` を定義する。

## 使用例

```cpp
#include "math/modint/modint.hpp"
#include "math/modint/to_rational.hpp"

using mint = modint998244353;
mint x = mint(1) / mint(2);
auto [p, q] = mint_to_rat(x); // p = 1, q = 2

dump(x | cp::rat()); // 1/2
dump(mint(3) | cp::rat()); // 3
vector<mint> a{x, -x};
dump(a | cp::rat()); // [ 1/2, -1/2 ]
dump(a | cp::rat() | cp::index()); // [ 0: 1/2, 1: -1/2 ]
dump(a | cp::index() | cp::rat()); // [ 0: 1/2, 1: -1/2 ]
dump(a | cp::front(1) | cp::rat() | cp::index()); // [ 0: 1/2, ... ]
vector<int> b{1, 2};
dump(b | cp::rat()); // [ 1, 2 ]
dump(string("abc") | cp::rat()); // "abc"
map<int, mint> m{{1, x}, {2, -x}};
dump(m | cp::rat()); // { 1: 1/2, 2: -1/2 }
auto t = tuple{string("half"), x, 7};
dump(t | cp::rat()); // ( "half", 1/2, 7 )
```

## 詳細なドキュメント

#### mint_to_rat

```cpp
pair<decltype(mint(0).val()), decltype(mint(0).val())> mint_to_rat(mint x)
```

`mint` の値を、小さい有理数 `p / q` として復元する。

返り値 `(p,q)` は

$$
p \equiv xq \pmod{\mathrm{mint::mod}()}
$$

を満たす短いベクトルの一つで、$q \geq 0$ になるように符号を揃える。

```cpp
auto [p, q] = mint_to_rat(x);
```

##### 制約

- `mint::mod()` と `x.val()` が同じ符号付き整数型で扱える
- `mint::mod() > 0`

##### 計算量

法を $m$ として、

- $O(\log m)$ 回の整数演算

#### cp::rat

```cpp
dump(x | cp::rat());
```

modint の値を `mint_to_rat` で復元し、`p/q` の形式で表示する。
分母が $1$ の場合は `3` や `0` のように分子だけを表示する。
配列、多次元配列、`pair`、`tuple`、`map`、`unordered_map`、`multimap`、`unordered_multimap` の要素を再帰的に処理する。
たとえば `map<int, mint>` ではキーの整数はそのまま、値の modint だけを有理数として表示する。キー側だけ、またはキーと値の両方が modint の場合にも対応する。
map の `{ キー: 値 }` という形や、multimap の同じキーに属する値のグループ表示を保つ。
変換に対応しない型は通常どおり表示するため、整数や文字列などに付けてもコンパイルエラーにならない。

map のキーは通常の `dump` と同じくシアンで表示する。配色の変更は [テンプレート（dump）](../../template/template_dump.md) を参照。

`index()`、`front()`、`back()`、`both_ends()`、`middle()`、`bin()`、`hex()`、map のキー・値への表示指定は、`rat()` の前後どちらにも置ける。
たとえば `dump(a | cp::rat() | cp::index())` と `dump(a | cp::index() | cp::rat())` は同じ表示になる。
`rat()` の前後に複数の表示指定を置いた場合も、表示指定同士の順序を保って合成する。

`bin()` や `hex()` は通常の整数要素に適用する。`rat()` が復元した分数は、分母が $1$ の場合も含めて十進数で表示する。

##### 制約

- C++23 以降
- `dump` で表示するには `LOCAL` が定義されている
- modint の要素は `mint_to_rat` の制約を満たす

##### 計算量

法を $m$ として、

- 表示する modint の要素ごとに $O(\log m)$ 回の整数演算
