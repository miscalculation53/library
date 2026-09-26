# C++ の順方向から転置を作る

通常の C++ テンプレートで順方向を書き、係数型を差し替えて演算を記録する。
既存のマクロ・コンテナ・線形演算のライブラリを利用できる。
実行例は [cpp_forward.cpp](cpp_forward.cpp)、API は [linear_transpose_cpp.hpp](../../math/linalg/linear_transpose_cpp.hpp)。

## 1. 順方向を書く

```cpp
template<class S>
vc<S> range_sum_cpp(const vc<S>& x, const vc<int>& left, const vc<int>& right) {
  FenwickTree<GroupAddSub<S>> tree(x);
  vc<S> y(left.size());
  repi(i, left.size()) y[i] = tree.sum(left[i], right[i]);
  return y;
}
```

`S = modint998244353` なら普通の区間和を計算する。
記録時は `S = linear_transpose::Value<modint998244353>` になり、Fenwick 木の中の加減算が記録される。
`left`, `right` は固定パラメータで、問題の入力から読み込んでよい。

## 2. 転置を構築して使う

```cpp
using mint = modint998244353;
vc<int> left{0, 1}, right{2, 3};
auto forward = [&](const auto& x) { return range_sum_cpp(x, left, right); };

auto program = linear_transpose::record<mint>(3, forward);
auto result = program.transpose({10, 20}); // {10, 30, 20}
```

コンテストでは、この C++ 自体を提出コードに含める。
入力を読んでから順方向を一度記録し、必要な重みを渡して転置を実行できる。
実行例のコンパイルはライブラリのルートで次のとおり。

```sh
g++-15 -std=c++17 -O2 -I . tools/transpose_examples/cpp_forward.cpp \
  -o /tmp/cpp_forward
/tmp/cpp_forward
```

## 3. C++ ソースとして書き出す場合

```cpp
program.write_cpp(out, "generated_range_add", "modint998244353");
```

この関数は、記録時のサイズ・固定係数に特化した関数を出力する。
変化する引数は転置側の入力 `seed` だけになる。

```sh
/tmp/cpp_forward /tmp/generated_range_add.hpp
```

生成される処理の形は次のようになる。

```cpp
std::vector<modint998244353> generated_range_add(
    const std::vector<modint998244353>& seed);
```

ループは実行した回数だけ展開される。入力ごとにサイズ・係数が変わるコンテスト問題には、
上の `record` / `transpose` を提出コードで実行する使い方が向いている。
ループを保ったソース生成には [Python 風の記法による生成器](README.md)も使える。

## Do Use FFT

[do_use_fft_cpp.hpp](do_use_fft_cpp.hpp) は、C++ で書いた順方向だけから問題を解く例。
順方向の中心は次の部分になる。

```cpp
auto forward = [&](const auto& u) {
  auto h = newton_basis_cpp(u, pb);
  return evaluate_tree_cpp(h, pa, 1, m);
};
auto answer = linear_transpose::transpose<mint>(m, weights, forward);
```

`newton_basis_cpp` は固定多項式との畳み込み、`evaluate_tree_cpp` は固定多項式による剰余と再帰で書く。
`vc`, `repi`, `convolution`, `FormalPowerSeries::eval` は既存のライブラリを使っている。
剰余には `linear_transpose::polynomial_mod` を使い、出力長を除数の次数に固定する。

畳み込み・剰余・NTT は、個々の乗算を展開せずに部品として記録する。
転置の実行時に既存ライブラリの対応する演算を呼ぶため、計算量は $O(N\log^2N)$ を保つ。

```sh
g++-15 -std=c++17 -O2 -I . tools/transpose_examples/do_use_fft_cpp.cpp \
  -o /tmp/do_use_fft_cpp
python3 tools/transpose_examples/stress_do_use_fft.py /tmp/do_use_fft_cpp
python3 tools/submit_code.py expand tools/transpose_examples/do_use_fft_cpp.cpp \
  -o /tmp/do_use_fft_cpp_bundle.cpp
python3 tools/submit_code.py cleanup /tmp/do_use_fft_cpp_bundle.cpp --std c++17
```

2026-09-27 の手元環境（macOS、GCC 15.2、`-O2`）で、`N=250000` の検証ケースの全回答が一致。
入力の記録・転置・入出力を含めて約 2.8 秒、最大 RSS は約 474 MiB。
小さいランダム入力は素朴解と照合し、大規模テストは `A` を 7 種類の値にした別の $O(7N)$ の計算と照合する。
検証はローカル実行。
展開・短縮後は 51,435 バイトの C++17 ファイルになり、単独でコンパイルして同じ最大規模のテストを通している。

## 記録の仕組みと利用条件

- 記録用の値は、中間値の番号と固定係数を持つ。
- `y = a * x + b * z` は演算の依存関係を記録する。上書きや `swap` は参照する中間値を変える。
- 転置では、出力の重みを中間値へ配り、演算を逆順にたどって入力の重みを求める。
- 入力値同士の乗算、入力値による分岐、非零の定数項の加算は実行時に検出する。
- サイズ・添字・固定係数だけに依存する `for`, `while`, 分岐、再帰は通常の C++ として書ける。
- `mint` を直接書いている順方向は、動かす係数部分をテンプレート引数 `S` に替える。
- ライブラリ中で係数の値を読み取る処理には、固定した長さ・構造で動く部品を使う。FPS の剰余はそのための対応を用意している。

記録に比例するメモリを使う点は、ループを保つ Python 版との違い。
畳み込みなどを部品にまとめることで、NTT 内部の膨大な演算履歴を省いている。

## 検査

```sh
g++-15 -std=c++17 -O1 -D_GLIBCXX_DEBUG -D_GLIBCXX_ASSERTIONS -I . \
  verify/mytest/ai/linear_transpose_cpp.test.cpp -o /tmp/transpose_cpp_test
/tmp/transpose_cpp_test /tmp/transpose_cpp_blocks.cpp
g++-15 -std=c++17 -O2 -I . /tmp/transpose_cpp_blocks.cpp -o /tmp/transpose_cpp_blocks
/tmp/transpose_cpp_blocks
```

基底ベクトルを通常の順方向に与えて行列を作り、記録した転置と比較する。
Fenwick 木・ゼータ変換・FPS 演算・上書き・同じ変数の複数回出力・非線形な式の検出を含む。
最後の2行は、書き出した C++ が各多項式部品も含めて実行できることの検査。
