# コンテスト用の転置コード生成

**C++ のライブラリやテンプレートを使って順方向を書く場合は、[C++ 版](CPP.md)を参照。**

順方向の線形な処理を短い Python 風の記法で書き、**順方向・転置・検査用の C++ 関数**を生成する。
ループと再帰を保って出力するので、実行中の演算履歴を保存するメモリは使わない。
Python 3.10 以上と C++17 以上で利用でき、追加の Python パッケージは不要。

## 最初に使う

以下のコマンドはライブラリのルートで実行する。

```sh
python3 tools/transpose.py tools/transpose_examples/basic.lin.py \
  -o tools/transpose_examples/basic.generated.hpp
```

`range_sum` は固定した各区間の和を求める。その転置は、各区間へ重みを加算した配列を返す。

```cpp
#include "tools/transpose_examples/basic.generated.hpp"

int main() {
  using T = modint998244353;
  std::vector<int> left{0, 1}, right{2, 3};
  auto sums = range_sum<T>({1, 2, 3}, left, right); // {3, 5}
  auto adds = range_sum_transpose<T>({10, 20}, 3, left, right); // {10, 30, 20}
  assert(range_sum_check<T>(3, left, right));
}
```

転置関数の第 2 引数は**順方向の入力長**。転置の入力長から一意に決まらないため、明示して渡す。

## コンテスト中の流れ

1. 求めたい線形写像と、その転置に当たる順方向の計算を考える。
2. 順方向を `solve.lin.py` に書く。変数として動かす配列を第 1 引数 `x: Vec` にし、ほかの引数を固定する。
3. 生成器を実行し、C++ から `solve_transpose` を呼ぶ。
4. 小さい入力で素朴解と比較し、`solve_check` で内積の恒等式も確認する。
5. 通常の `submit_code.py expand` または `oj-b` で提出用ファイルへ展開する。

```sh
# コンテストディレクトリから。LIBRARY はこのライブラリへのパス。
python3 "$LIBRARY/tools/transpose.py" solve.lin.py -o solve.generated.hpp
g++-15 -std=c++17 -O2 -I "$LIBRARY" main.cpp -o main
python3 "$LIBRARY/tools/submit_code.py" expand main.cpp -o bundle.cpp
```

`main.cpp` には `#include "solve.generated.hpp"` と書く。
提出用 C++ は生成器なしで実行できる。生成済みファイルの更新漏れは、生成コマンドに `--check` を付けて検査できる。

## 記法

入力は Python の構文解析器で読み取る専用の記述で、生成時に実行することはない。
既存の C++ は、線形な部分をこの記法に移して使う。

```python
def prefix_sum(x: Vec) -> "len(x)":
    y = zeros(len(x))
    s = scalar()
    for i in range(len(x)):
        s += x[i]
        y[i] = s
    return y
```

これは累積和と、その転置である後ろからの累積和を生成する。

### 引数と線形性

- 第 1 引数の `Vec` は `std::vector<T>` になる。
- 固定引数には `n: "int"`, `a: "const std::vector<T>&"` のように C++ の型を書く。
- 戻り値の注釈には、入力長と固定引数から決まる出力長を書く。
- 最後に、関数の外側のスコープにある配列を `return` する。
- `T` は modint などの可換環。除算を使う場合、分母の逆元が必要。
- 配列の要素と `scalar()` で作った変数は、入力に依存する値として扱う。
- 添字・係数・分岐条件・ループの範囲は固定値にする。入力配列の `len` も利用できる。
- 固定値は生成後の C++ の規則で計算する。`//` は整数の `/` に変換されるため、負数は 0 方向に丸める。

ここでの「固定」はコンパイル時定数という意味ではない。たとえば区間端点や木の構造は、
問題の入力から読み取ってよい。線形写像の入力 `x` を変えたときに同じ値・構造を保つものを固定する。

### 対応する処理

| 記述 | 意味 |
| --- | --- |
| `y = zeros(n)`, `s = scalar()` | 零配列、零スカラーを宣言 |
| `s = x[i] + 2 * x[j]` | 線形なスカラーを宣言 |
| `x[i] = a * x[j] + b * x[k]` | 上書き代入。左右の添字が一致しても使える |
| `s += x[i]`, `x[i] -= s` | 加算・減算 |
| `x[i] *= a`, `x[i] /= a` | 固定係数による乗算・除算 |
| `n = len(x)` | 固定値を宣言。宣言後は不変 |
| `for i in range(l, r, step)` | 固定範囲のループ。1～3 引数、降順にも対応 |
| `for i in indices` | 固定配列の要素を順に処理 |
| `if condition: ... else: ...` | 固定条件で分岐 |
| `assert condition` | そのスコープでの固定条件の検査 |

ループと分岐はブロックスコープを作る。内部で作った変数はそのブロックで利用する。
スカラーや要素への上書きは可能で、配列全体の演算結果には新しい変数名を付ける。
配列のコピー `y = x` は別の配列を作る。
`x[i] * x[j]`、`x` の値に依存する分岐、零以外の定数項の加算などは、行番号付きのエラーになる。

固定式では算術・ビット演算・比較・`len` と固定値だけを引数にした関数呼び出しを使える。
呼び出す C++ 関数は、同じ固定引数に同じ結果を返し、状態を変更しないものを使う。
生成器は固定値の宣言をスコープの先頭へ移して、順方向と転置で同じ形状・係数を作る。
対応する C++ の宣言は生成ヘッダを読み込む前に用意する。

### 配列演算の部品

| 記述 | 条件・結果 |
| --- | --- |
| `copy(x)` | コピー |
| `slice(x, l, r)` | 半開区間 `[l,r)` を取り出す |
| `resize(x, n)` | 切り詰め・零埋めで長さを変更 |
| `reverse(x)` | 反転 |
| `convolution(x, fixed)` | 固定多項式との畳み込み |
| `poly_mod(x, fixed)` | 固定した monic 多項式による剰余。出力長は除数の次数 |
| `ntt(x)`, `intt(x)` | ライブラリの変換。`intt` は正規化前 |
| `f(x, fixed_args...)` | 同じ入力ファイルで定義した関数。再帰も利用可能 |

畳み込み・多項式の剰余・NTT は、既存ライブラリの実装と
[転置用の部品](../../math/linalg/linear_transpose.hpp)を呼び出す。
これにより、ライブラリ内部の高速化を保ったまま関数全体を転置できる。
添字や再帰の終了条件、多項式演算の法・長さなどは、順方向の計算で成立するように与える。

### 生成される関数

`def f(x: Vec, parameter: "P") -> "..."` から次を生成する。

```cpp
template<class T> std::vector<T> f(std::vector<T> x, P parameter);
template<class T> std::vector<T> f_transpose(
    const std::vector<T>& seed, int input_size, P parameter);
template<class T> int f_output_size(int input_size, P parameter);
template<class T> bool f_check(int input_size, P parameter, int trials = 8);
```

`f_check` は $\langle F(x),y\rangle=\langle x,F^{\mathsf T}(y)\rangle$ を乱択で検査する。
順方向のアルゴリズムが問題を正しく解くことは、素朴解との比較でも確認する。

## 転置コードができる仕組み

生成器はまず、入力に依存する値と固定値を区別し、線形な式を
「入力要素と固定係数の組」の列に分解する。その後、処理とループの順を逆にする。

たとえば `y[i] = a * x[j] + b * x[k]` の転置は、次の処理になる。
以下の `gx`, `gy` は転置側の配列を表す。

```cpp
T t = gy[i];
gy[i] = 0;
gx[j] += a * t;
gx[k] += b * t;
```

上書き前の `y[i]` は出力に使われないので、転置側ではいったん零にする。
最初に `t` へ保存することで、`i == j` などの重なりも扱える。
`+=` の場合は元の値も使うため、零にする処理を省く。

関数の列 $A_1,A_2,\ldots,A_k$ の転置は $A_k^{\mathsf T},\ldots,A_1^{\mathsf T}$ の順になる。
同じ規則を配列演算・ループ・再帰にも適用する。
畳み込みなどの部品の転置はあらかじめ実装してあり、生成器がそれらを組み合わせる。

## 問題例: Do Use FFT

[Codeforces Gym 102978D](https://codeforces.com/gym/102978/problem/D) の

$$
S_k=\sum_{i=0}^{N-1} C_i\prod_{j=0}^{k-1}(A_i+B_j),\qquad 1\leq k\leq N
$$

を解く実行例が `do_use_fft.cpp`。入力は `N`, 配列 `A`, `B`, `C` の順。

```sh
python3 tools/transpose.py tools/transpose_examples/polynomial.lin.py \
  -o tools/transpose_examples/polynomial.generated.hpp
g++-15 -std=c++17 -O2 -I . tools/transpose_examples/do_use_fft.cpp \
  -o /tmp/do_use_fft
python3 tools/submit_code.py expand tools/transpose_examples/do_use_fft.cpp \
  -o /tmp/do_use_fft_bundle.cpp
# コード長も短くする場合
python3 tools/submit_code.py cleanup /tmp/do_use_fft_bundle.cpp --std c++17
```

生成元では次の順方向だけを書く。

1. `newton_basis`: $u$ から $H(X)=\sum_k u_k\prod_{j<k}(X+B_j)$ の係数列を作る。
2. `evaluate_tree`: その多項式を各 $A_i$ で評価する。

これらを合成した行列の $(i,k)$ 成分は $\prod_{j<k}(A_i+B_j)$。
したがって転置に $C$ を渡すと、欲しい $S_k$ が得られる。

```cpp
auto coefficients = evaluate_tree_transpose<mint>(weights, m, pa, 1, m);
auto answers = newton_basis_transpose<mint>(coefficients, m, pb, 1);
```

`m` は `N+1` 以上の最小の 2 冪。積木を作り、評価点の余分な重みを零にしておく。
結果の `answers[1]` から `answers[N]` を使う。評価点や `B` の値が重複しても使える。
計算量は $O(N\log^2 N)$、空間計算量は $O(N\log N)$。

### 確認方法

```sh
python3 tools/test_transpose.py
g++-15 -std=c++20 -O2 -I . verify/mytest/ai/linear_transpose.test.cpp \
  -o /tmp/linear_transpose_test
/tmp/linear_transpose_test
python3 tools/transpose_examples/stress_do_use_fft.py /tmp/do_use_fft
```

- 小さいランダム入力は素朴解と全回答を比較。
- 最大規模のテストは `N=250000`、`A` をランダムな 7 種類の値にして、別の $O(7N)$ の計算と全回答を比較。
- 2026-09-27 の手元環境（macOS、GCC 15.2、`-O2`）では実行部分が約 2.4～3.1 秒、最大 RSS は約 144 MiB。公式ジャッジへの提出は行っていない。
- 提出用に展開・短縮した C++17 のファイルは 49,698 バイト。単独でコンパイルし、同じ最大規模のテストを通している。
- 生成器のテストは、上書き・同じ添字への代入・降順ループ・分岐・関数呼び出しについて、Python で作った行列の転置と生成 C++ を比較する。

## 講義の例との対応

[AtCoder の転置原理入門](https://info.atcoder.jp/entry/algorithm_lectures/transposition_principle)の基本例は `basic.lin.py` にまとめた。

| 順方向 | 生成した転置で求めるもの |
| --- | --- |
| `range_sum` | 各区間への加算をまとめた配列 |
| `subset_zeta` | 上位集合のゼータ変換 |
| `divisor_zeta` | 倍数のゼータ変換 |
| `fenwick_queries` | 区間和の出力に重みを付けたときの、初期値・各更新値の係数 |

`divisor_zeta` には昇順の素数列を渡し、添字 1 以降を使う（添字 0 はそのまま）。
`fenwick_queries` の入力は初期値 `n` 個と各時刻の更新値をつなげた配列。
`kind[t] == 0` は一点加算、`1` は半開区間の和で、加算の時刻の出力は零にする。
クエリの順番・位置を固定すると、これらも線形写像として扱える。
