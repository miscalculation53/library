# dot_product の他ライブラリへの適用

2026-09-10、macOS、GCC 15.2.0、`modint998244353`、`-O2`。
変更前の主要ループを [dot_product_reuse.cpp](dot_product_reuse.cpp) に残し、変更後の実際の API と比較する。

```sh
g++-15 -std=c++20 -O2 -Wall -Wextra -I. benchmark/dot_product_reuse.cpp -o /tmp/dot-reuse-bench
/tmp/dot-reuse-bench
```

入力生成は計測外。ウォームアップ 1 回後、5 回の中央値。
新旧を交互に先に実行する。全出力の一致を検査し、計測内でも全出力の checksum を取る。
転置や作業配列の確保も計測に含める。`NDEBUG` を付けずに実行する。
[全計測値（CSV）](dot_product_reuse.csv) に小さい入力や疎な入力も含めて記録した。

## 主な結果

時間の単位は µs。速度比は変更前 / 変更後。

| ケース | 変更前 | 変更後 | 速度比 |
| --- | ---: | ---: | ---: |
| Matrix・ベクトル積、128×128 | 17.020 | 6.630 | 2.57x |
| Matrix・ベクトル積、256×256 | 63.330 | 26.020 | 2.43x |
| Matrix・密行列積、128×128 | 1803.000 | 775.000 | 2.33x |
| Matrix・密行列積、256×256 | 14292.500 | 7083.000 | 2.02x |
| MatrixArray・密行列積、128×128 | 1975.333 | 791.000 | 2.50x |
| 畳み込みの中央 1 係数、長さ 60 | 0.056 | 0.025 | 2.24x |
| 素朴な畳み込み、60×60 | 3.390 | 1.970 | 1.72x |
| 素朴な畳み込み、60×4096、密 | 188.700 | 105.600 | 1.79x |
| PolynomialRingVector、長さ 256 | 51.580 | 29.560 | 1.74x |
| PolynomialRingArray、長さ 128、打切り積 | 7.840 | 3.640 | 2.15x |
| subset convolution、$2^{16}$ 要素 | 16548.500 | 13412.500 | 1.23x |
| Kronecker 冪、$k=8$、$8^5$ 要素 | 1262.000 | 960.667 | 1.31x |
| Kronecker 冪、$k=16$、$16^4$ 要素 | 4293.000 | 2857.333 | 1.50x |

通常の畳み込みでは非零数に応じて NTT を選ぶので、上の素朴な畳み込みの速度比を全サイズには適用できない。
多項式環の計算量は従来と同じ二乗時間。subset convolution では zeta・Möbius 変換の時間が残る。

## 小さい入力・疎な入力

| ケース | 変更前 | 変更後 | 速度比 |
| --- | ---: | ---: | ---: |
| Matrix 積、8×8 | 1.199 | 1.195 | 1.00x |
| Matrix 積、128×128、左辺が零 | 45.000 | 29.000 | 1.55x |
| Matrix 積、128×128、左辺の非零率 1% | 70.500 | 79.000 | 0.89x |
| Matrix 積、128×128、左辺の非零率 10% | 229.000 | 230.000 | 1.00x |
| 畳み込み、60×4096、非零率 1% | 5.800 | 5.900 | 0.98x |
| PolynomialRingArray、長さ 8 | 0.030 | 0.030 | 1.00x |
| Kronecker 冪、$k=2$、$2^{16}$ 要素 | 1791.000 | 1732.667 | 1.03x |

小さい入力と疎な入力には従来の処理を残した。ただし、行列の密度判定の追加走査などのコストはあり、
非零率 1% の行列積では約 11% 遅くなった。測定が数十 ns のケースは時間の分解能や実行状況の影響も大きい。

## 検証

- `verify/mytest/ai/dot_product_reuse.test.cpp` を C++17 / C++20 で実行。
  行列・多項式は逐次の積和、subset convolution は部分集合の列挙、Kronecker 冪は陽なテンソル積と比較。
- 空配列、零次元、長方形、疎・密な入力、切替境界、逆順 iterator、範囲外の係数を検査。
- 整数環、min-plus 半環、零元と演算を変更した独自の modint 環を検査。
  法 1、12、$2^{30}$、$2^{30}+1$、$2^{31}-1$ と 64 bit modint も確認。
- 整数係数の PolynomialRingArray の `constexpr` 利用を確認。
- 保存済み Library Checker：行列積 22、行列累乗 43、通常の畳み込み 53、任意 mod の static / dynamic 各 48、
  subset convolution 11、Kronecker 関連各 13、計 251 ケースを通過。
- 既存の convolution、polynomial_ring、matrix、matrix_array、BBLA 本体・拡張の回帰テストを通過。
- MatrixArray・畳み込み・多項式環・subset convolution・Kronecker 冪には Apple Clang の ASan / UBSan も適用。
  動的 Matrix は既存の継承コンストラクタの宣言が Apple Clang でコンパイルできないため、GCC の比較テストで検査。

Kronecker 冪の非破壊版も検査した際、内部呼出しで `int k` と `array` の `size_t` の推論が合わない既存の問題を確認し、
`k` を明示して呼ぶように修正した。オンラインへの提出は行っていない。

候補と適用条件の整理は [調査メモ](../research/dot_product_reuse.md) を参照。
