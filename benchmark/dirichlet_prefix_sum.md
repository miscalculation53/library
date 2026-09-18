## 測定対象

この節は積の高速化時の測定記録。現在は `K` の引数を廃止し、平方根までを内部で保持する。
商の高速化後の測定は末尾に記載する。

2026-09-10、手元の macOS / x86_64、Homebrew GCC 15.2.0 で測定。
`-std=c++17 -O2 -Wall -Wextra`、`NDEBUG` は定義していない。

[Dirichlet Convolution and Prefix Sums](https://judge.yosupo.jp/problem/dirichlet_convolution_and_prefix_sums)
と同じ形式で、$N=10^{12}$ の商集合上に二つの累積和を与えた。
各値は `random.Random(18631).randrange(1, 998244353)` で生成し、
係数環は `RingAddSubMul<modint998244353>`、$K=\lfloor\sqrt N\rfloor=10^6$。

`direct` は各商位置を双曲線分割で計算する比較用の実装、`auto` はライブラリの `f * g`。
両方とも、小さい側の係数の積には `DirichletSeries` の自動選択を使う。
別プロセスで交互に各 3 回実行し、入出力を含む実時間と `wait4` の最大 RSS を記録した。
測定中に別のテストやベンチマークは実行していない。

## 結果

| 方法 | 時間計算量 | 実時間の中央値 | CPU 時間の中央値 | 最大 RSS の最大値 |
| --- | --- | ---: | ---: | ---: |
| 直接の双曲線分割 | $O(N^{3/4})$ | 6.688 秒 | 6.557 秒 | 99.1 MiB |
| 自動選択（区間集約） | $O(N^{2/3})$ | 3.001 秒 | 2.980 秒 | 112.2 MiB |

実時間で約 **2.23 倍**。空間計算量はいずれも $O(\sqrt N)$ で、長さ $N^{2/3}$ の配列は使わない。
直接分割の比較用実装は非零添字の抽出を省いており、配列の確保方法まで旧ライブラリと同一ではない。

各実行の実時間は `direct` が 7.238 / 6.688 / 6.548 秒、
`auto` が 3.001 / 3.057 / 2.991 秒だった。
全 1,999,999 個の出力が全ての実行で一致した。
出力の SHA-256 は `727d0f67aad28efda64d498966aaca001ee371cf010210b4b99ba59fc8da5143`。

元の提出コードについても、$K$ を平方根に直した同じソースでヘッダ変更前後を各一回測定し、
8.009 秒・100.7 MiB から 3.527 秒・103.4 MiB になった。
こちらの出力も上記と一致した。オンラインジャッジへの提出は行っていない。

## 正当性の確認

- C++17・20 の Dirichlet 積・商・逆数のテスト。
- 区間集約を通る立方数の前後、全保持位置での直接分割との比較。
  当時の拡張範囲 $K>\sqrt N$ も検証したが、現在の API にはその指定はない。
- 整数環、modint、算術演算子を持たない要素型、XOR/AND 環、乗法性フラグの保持。
- 乗法的関数の和・一般化した Euler 積の既存テスト。
- この問題のサンプルと、上記の最大規模入力。

## 再現方法

リポジトリのルートで実行する。

```sh
g++-15 -std=c++17 -O2 -Wall -Wextra -I. benchmark/dirichlet_prefix_sum.cpp -o /tmp/dirichlet_bench
python3 benchmark/dirichlet_prefix_sum.py /tmp/dirichlet_bench --n 1000000000000 --repeat 3
```

入力生成は測定時間に含めない。入力・出力の処理とプロセスの起動・終了は含める。
Python は各方式の出力を比較し、JSON Lines で時間とメモリを出力する。
標準ライブラリだけを使う macOS / Linux 用のランナー。

アルゴリズムと計算量の導出は
[DirichletPrefixSum の説明](../docs/math/prime/sieve/dirichlet_prefix_sum.md) を参照。

## 商の高速化・K 廃止後の測定

同じ環境・コンパイル条件・入力生成方法で、$N=10^{12}$ の `f / g` を測定した。
環は `FieldAddSubMulDiv<modint998244353>`。入力の $g(1)$ は非零なので逆元が存在する。
`divide-direct` は従来の双曲線分割による商、`divide` は新しい区間更新による商。
両方とも商集合だけを保持し、配列の大きさは $O(\sqrt N)$。
入出力を含む別プロセスを交互に各 3 回実行した。測定中に別のテストは実行していない。

| 方法 | 時間計算量 | 実時間の中央値 | CPU 時間の中央値 | 最大 RSS の最大値 |
| --- | --- | ---: | ---: | ---: |
| 直接の双曲線分割 | $O(N^{3/4})$ | 8.832 秒 | 8.571 秒 | 94.9 MiB |
| 商集合への区間更新 | $O(N^{2/3})$ | 2.990 秒 | 2.956 秒 | 87.2 MiB |

実時間で約 **2.95 倍**。各回は直接方式が 9.282 / 8.467 / 8.832 秒、
区間更新が 2.950 / 2.990 / 2.992 秒。
全 1,999,999 個の出力が全実行で一致し、SHA-256 は
`7d00ed5797ad30e8a816d8d87064365034ef5d01ed6cb2f41afd4910aaf43a27`。

```sh
g++-15 -std=c++17 -O2 -Wall -Wextra -I. benchmark/dirichlet_prefix_sum.cpp -o /tmp/dirichlet_bench
python3 benchmark/dirichlet_prefix_sum.py /tmp/dirichlet_bench --n 1000000000000 --operation division --repeat 3
```

`inv()` は単位元を分子とする同じ商の実装を使うため、時間・空間の上界も同じ。
上の実測値はランダムな分子・分母に対する商であり、逆数そのものの測定値ではない。

## 根付き商集合への一般化後の測定

2026-09-10、同じ macOS / x86_64・GCC 15.2.0・C++17・`-O2 -Wall -Wextra` で測定。
`NDEBUG` は定義していない。小さい入力で起動を一度済ませてから、
別プロセスで交互に各 3 回実行した。測定中に別のテスト・コンパイルは実行していない。
入出力とプロセスの起動・終了を含む実時間の中央値、および最大 RSS の最大値を示す。

### Counting Squarefrees

[検証コード](../verify/yosupo/counting_squarefrees.test.cpp) は
`DirichletPrefixSum<RingAddSubMul<ll>, 2>` の逆数から Möbius 関数の累積和を求め、
$\sum_{i\le\sqrt N}\mu(i)\lfloor N/i^2\rfloor$ を同じ商の区間ごとに集約する。
比較対象は [Library Checker の参照実装](https://github.com/yosupo06/library-checker-problems/blob/master/number_theory/counting_squarefrees/sol/correct.cpp)。
そのソースを同じオプションでコンパイルした。

| 方法 | $N$ | 実時間の中央値 | CPU 時間の中央値 | 最大 RSS の最大値 |
| --- | ---: | ---: | ---: | ---: |
| 一般化した Dirichlet 逆数 | $10^{18}$ | 1.503 秒 | 1.489 秒 | 111.6 MiB |
| 参照実装 | $10^{18}$ | 0.922 秒 | 0.873 秒 | 343.9 MiB |

一般化した実装は $O(N^{2/5})$ 時間・$O(N^{1/3})$ 空間。
参照実装は小さい側の Möbius 関数を $N^{2/5}$ 程度まで前計算する。
出力は全て `607927101854022750` で一致した。
$N=10^{12},10^{15}$ でも参照実装との一致を確認した。
オンラインジャッジへの提出は行っていない。

```sh
g++-15 -std=c++17 -O2 -Wall -Wextra -I. verify/yosupo/counting_squarefrees.test.cpp -o /tmp/counting_squarefrees
echo 1000000000000000000 | /usr/bin/time -l /tmp/counting_squarefrees
```

### D=1 の比較

この一般化の直前・直後のヘッダで `benchmark/dirichlet_prefix_sum.cpp` をコンパイルし、
前節と同じ $N=10^{12}$・seed 18631 のランダムな累積和を入力した。
係数環は `FieldAddSubMulDiv<modint998244353>`。

| 演算 | 一般化前の実時間 | 一般化後の実時間 | 一般化前の CPU 時間 | 一般化後の CPU 時間 |
| --- | ---: | ---: | ---: | ---: |
| 積 | 2.824 秒 | 2.814 秒 | 2.792 秒 | 2.787 秒 |
| 商 | 2.687 秒 | 2.832 秒 | 2.663 秒 | 2.799 秒 |

積は同程度、商は今回の中央値で約 5% 増。時間計算量は従来通り $O(N^{2/3})$。
全 1,999,999 個の出力が全実行で一致し、SHA-256 は前節の値と同じ。
`D=1` の座標変換は追加配列を使わない。

### 正当性の確認

- C++17・20 で、既存の Dirichlet 級数・累積和、および一般化した累積和のテストが成功。
- `D=1..5` の小さい入力、完全冪の前後、根の重複、コールバックの呼び出し回数を確認。
- `D=2,3,4` の区間集約を通る入力で、通常の係数配列による積・商と全保持位置を照合。
- 整数環の割り切れる商、有限体の任意の非零の第 1 係数、算術演算子を持たない環に対応。
- `LLONG_MAX` 付近や `D=62,63,100,INT_MAX` でも整数根・座標変換を確認。
- 配列の境界検査と符号付き整数オーバーフローの検査を有効にした
  `-D_GLIBCXX_ASSERTIONS -ftrapv` でも一般化のテストが成功。
- 乗法的関数の和・一般化した Euler 積の既存テストも C++17・20 で成功。
