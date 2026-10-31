このドキュメントには AI による作成・変更が含まれています。

## 概要

`pair`, `tuple`, `array` の成分ごとの加算・減算を行う。`+`, `-`, `+=`, `-=`, 単項 `-` を使える。入れ子の各成分にも同じ演算を適用する。

`template_math.hpp`, `template_algo.hpp`, `template_inout.hpp` からも読み込まれる。各成分の演算が定数式で使える場合は、全体も定数式として使える。

## 使用例

```cpp
#include "template/template_arithmetic.hpp"

pll p{7, -2}, q{3, 5};
assert((p + q == pll{10, 3}));
assert((p - q == pll{4, -7}));
assert((-p == pll{-7, 2}));
p += q;
p -= q;

tlll x{1, 2, 3};
x += tlll{4, 5, 6};
assert((x == tlll{5, 7, 9}));

array<ll, 3> a{1, 2, 3};
a -= array<ll, 3>{3, 2, 1};
assert((a == array<ll, 3>{-2, 0, 2}));
```

## 詳細なドキュメント

### 加算・減算

```cpp
S operator+(S a, const R& b)
S operator-(S a, const R& b)
S& operator+=(S& a, const R& b)
S& operator-=(S& a, const R& b)
S operator-(S a)
```

`S` は `pair<T, U>`, `tuple<Ts...>`, `array<T, N>` のいずれか。二項演算では、対応する各成分に `+=` または `-=` を適用する。単項 `-` では、各成分を符号反転する。二項 `+`, `-` と単項 `-` は左辺と同じ型の値を返し、`+=`, `-=` は左辺への参照を返す。

右辺の成分型は左辺と異なっていてもよい。`pair` は `first`, `second`、`array` は添字、`tuple` は `get<I>` で右辺の成分を参照する。

成分を先頭から順に更新する。参照を成分に持つ `pair`・`tuple` では参照先が更新される。左右の成分が参照先を共有する場合、後続の演算はそれまでの更新を反映する。

空の `tuple`・`array` に対する演算も使える。

#### 制約

- 左右の対応する成分に必要な演算が定義され、その結果が左辺の成分型に収まる
- 右辺には左辺と対応する成分がある
- 値で受け取る引数は、その呼び出しに必要なコピーまたはムーブが可能
- 二項 `+`, `-` では結果のコピーが可能

#### 計算量

- 各成分へのアクセス・演算と、必要なコピー・ムーブの計算量の合計
