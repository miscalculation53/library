## 概要

C++ のテンプレート関数として書いた順方向から、転置を構築して実行する。
`vc<S>` の係数型 `S` を記録用の `Value<T>` に差し替え、通常の C++ コンパイラで処理する。
`repi`、標準コンテナ、既存の Fenwick 木やゼータ変換などを利用できる。

順方向のサイズ・固定パラメータは実行時に与えてよい。
演算を記録して逆順にたどる方式で、ループ・再帰は実行された分だけ記録される。
畳み込み・多項式の剰余・NTT はひとつの演算として記録し、既存の高速な転置部品を呼ぶ。
記録に使うメモリは、スカラー演算数と各部品に渡した配列長の合計に比例する。

[C++ 版の使い方と Do Use FFT の例](../../../tools/transpose_examples/CPP.md)を参照。

## 使用例

```cpp
#include "math/linalg/linear_transpose_cpp.hpp"
#include "ds/fenwick_tree/fenwick_tree.hpp"

template<class S>
vc<S> forward(const vc<S>& x, const vc<int>& left, const vc<int>& right) {
  FenwickTree<GroupAddSub<S>> tree(x);
  vc<S> y(left.size());
  repi(i, left.size()) y[i] = tree.sum(left[i], right[i]);
  return y;
}

// 区間和の転置で、各区間へ重みを加算する。
using T = modint998244353;
vc<int> left{0, 1}, right{2, 3};
auto program = linear_transpose::record<T>(3,
  [&](const auto& x) { return forward(x, left, right); });
auto result = program.transpose({10, 20}); // {10, 30, 20}
```

## 詳細なドキュメント

以下はすべて `linear_transpose` 名前空間にある。

#### record / transpose

```cpp
Program<T> record<T>(int n, const Forward& forward)
vc<T> transpose<T>(int n, const vc<T>& seed, const Forward& forward)
```

`record` は長さ $n$ の記録用配列を `forward` に渡し、線形写像の計算手順を記録する。
`transpose` は `record<T>(n, forward).transpose(seed)` の省略形。
固定パラメータはラムダのキャプチャで渡せる。記録に必要な係数は `Program` にコピーする。

##### 制約

- $n\geq0$。
- `forward` は `vc<Value<T>>` を受け取り、同じ記録から得た値の配列を返す。
- `forward` は入力について線形。添字・長さ・分岐・ループ回数は固定パラメータで決まること。
- 係数には、必要な演算が定義された modint 型、または演算結果が収まる整数型を使う。除算には逆元が必要。
- 多項式・NTT の部品には、既存ライブラリが対応する modint 型を使う。

入力に依存する値同士の乗算、非零の定数項の加算、入力値による分岐、別の記録の値との混在は `std::logic_error` になる。
固定値だけの演算や比較は通常どおり計算する。

##### 計算量

- `record`: 順方向の C++ の実行と記録。ブロック化した多項式演算は配列の長さ・参照・固定係数を記録する。
- `transpose`: 記録時間と、下記 `Program::transpose` の実行時間の合計。

### Program<T>

#### transpose

```cpp
vc<T> transpose(const vc<T>& seed) const
```

記録した写像を $F(x)=Ax$ とすると、$A^{\mathsf T}\,\mathrm{seed}$ を返す。
同じ `Program` に別の `seed` を与えて繰り返し使える。

##### 制約

- `seed.size() == output_size()`。
- 記録時と同じ係数演算を使うこと。動的 modint の法も同じにする。

##### 計算量

- $O(L+\sum_b T_b)$。$L$ は記録したスカラー演算と配列要素の総数、$T_b$ は各部品の転置の計算量。

#### input_size / output_size / node_count

```cpp
int input_size() const
int output_size() const
int node_count() const
```

順に、順方向の入力長・出力長・中間値も含めた記録上の変数数を返す。

##### 計算量

- $O(1)$。

#### write_cpp

```cpp
void write_cpp(std::ostream& out, const std::string& name,
               const std::string& coefficient_type) const
```

記録したサイズ・固定係数に特化した、転置を計算する C++ 関数を書き出す。
生成関数の引数は `seed` のみ。スカラー演算は展開され、多項式演算はライブラリ呼び出しになる。
生成した関数は `linear_transpose.hpp` に依存し、`Recorder` や `Value` は使わない。

##### 制約

- `name` は有効な C++ 関数名。
- `coefficient_type` は `modint998244353` など、生成先で参照できる型名。記録時と同じ係数演算を持つこと。
- `T` のストリーム出力が `T(出力内容)` で復元できる整数表記になること。

##### 計算量

- 出力する文字数に比例。

#### 配列演算の対応

| 順方向の処理 | 記録時の扱い |
| --- | --- |
| `convolution(active, fixed)` | 畳み込みの部品として記録 |
| `convolution(active, active)` | 非線形として検出 |
| `linear_transpose::polynomial_mod(active, fixed)` | monic 多項式による剰余の部品として記録 |
| `ntt(active)`, `intt(active)` | 変換の部品として記録 |
| `FormalPowerSeries<S>::eval`, `diff`, 加減算、固定多項式との乗算 | 既存のテンプレートを通して演算を記録 |

`convolution`, `ntt`, `intt` は名前空間を限定せずに呼ぶと、C++ の ADL によって記録用のオーバーロードが選ばれる。
`FormalPowerSeries::operator%` には末尾の零を調べる処理があるため、記録時の剰余は長さを固定した `linear_transpose::polynomial_mod` を使う。
`Recorder` と `Value` は `record` 内部の記録に使う型で、値はその記録の実行中に利用する。
