# 掃き出し・変換の高速化（2026-09-10）

## 再現方法

```sh
g++-15 -std=c++20 -O2 -I. benchmark/delayed_reduction.cpp -o /tmp/delayed-bench
/tmp/delayed-bench > benchmark/delayed_reduction_static.csv
/tmp/delayed-bench dynamic > benchmark/delayed_reduction_dynamic.csv
/tmp/delayed-bench upper > benchmark/delayed_reduction_upper.csv
```

測定環境は macOS arm64、Homebrew GCC 15.2.0、`-O2`。乱数 seed は 20260910。変更前の処理と変更後の処理を交互の順序で実行し、1 回の warm-up と 5 回の測定の中央値を取った。時間には必要なコピー・確保・返り値全体の checksum を含む。約数・倍数の変換では素数表を測定前に準備する。計測時にはこちらのコンパイル・検証処理を並走させていない。

各比較の前に返り値の一致を検査する。CSV の時間の単位は μs、倍率は変更前 / 変更後なので 1 より大きいほど速い。小さいケースや数 % の差は測定揺れ・コンパイラのコード生成差も含む。別 CPU・コンパイラで同じ倍率を保証するものではない。

比較実装は [delayed_reduction_reference.hpp](delayed_reduction_reference.hpp)、不採用の実験実装は [delayed_reduction_candidates.hpp](delayed_reduction_candidates.hpp) に残した。掃き出しの `original` は元の全列更新、`suffix` は列の右側だけ更新して零係数を飛ばす比較版。変換の `adopted` は採用後の公開 API、`raw64_trial` は 64 bit に変換して最後だけ剰余を取る候補。Hadamard の `direct_subtract_baseline` は直接の減算まで適用した比較版である。

## 採用版の代表値

掃き出しは密な 512×512、集合変換は長さ 65536、約数・倍数変換は 1..65535。

| 処理 | static 998244353 | dynamic 1000000007 | static 2147483647 |
|---|---:|---:|---:|
| 掃き出し（階段形） | 3.52x | 3.81x | 3.30x |
| 掃き出し（簡約階段形） | 4.76x | 5.16x | 4.44x |
| Hadamard | 1.54x | 1.59x | 1.52x |
| 部分集合 Möbius | 1.36x | 1.52x | 1.38x |
| 上位集合 Möbius | 1.31x | 1.44x | 1.29x |
| 約数 Möbius | 1.16x | 1.17x | 1.19x |
| 倍数 Möbius | 1.21x | 1.25x | 1.25x |

掃き出しは `Matrix`・`MatrixArray` の標準の 32 bit modint の体に対し、行数・列数がともに 32 以上なら適用する。Hadamard は長さ 1024 以上で剰余を最後にまとめ、それ未満は直接の減算を使う。Möbius は追加配列を使わず直接の減算だけを採用した。

掃き出しの右側更新・零係数を飛ばす改善を除いても、密な行列で次の差がある。Hadamard も直接の減算との比較を別に示す。

| 処理 | static 998244353 | dynamic 1000000007 | static 2147483647 |
|---|---:|---:|---:|
| 階段形：右側更新の比較版に対して | 2.30x | 2.25x | 2.10x |
| 簡約階段形：右側更新の比較版に対して | 2.31x | 2.29x | 2.11x |
| Hadamard：直接の減算の比較版に対して | 1.18x | 1.21x | 1.15x |

## 特殊な行列

以下は 256×256。零行列・単位行列・約 1% の非零成分・全行を同じにした rank 1 の行列も比較した。疎なケースの大きな改善は、零係数・零行の更新を省く効果が多く、剰余の遅延だけの倍率ではない。

| 処理 | static 998244353 | dynamic 1000000007 | static 2147483647 |
|---|---:|---:|---:|
| 零行列・階段形 | 1.63x | 1.67x | 1.56x |
| 単位行列・階段形 | 37.12x | 31.17x | 41.54x |
| 1% 非零・階段形 | 27.38x | 29.41x | 29.98x |
| rank 1・階段形 | 0.98x | 0.98x | 0.99x |
| rank 1・簡約階段形 | 0.98x | 0.98x | 0.96x |

rank 1 は数 % 遅くなる場合がある。最初の試作では大幅に遅かったため、零と分かった行の先頭部分を記録し、零行の再走査を避けるようにした。長方形（32×256、256×32、128×256）と小さい行列の結果も CSV に収録している。

## 採用しなかった 64 bit 化

以下は元の実装に対する候補の倍率。集合変換は長さ 65536、約数・倍数変換は 1..65535。

| 処理 | static 998244353 | dynamic 1000000007 | static 2147483647 |
|---|---:|---:|---:|
| 部分集合 zeta | 0.99x | 1.12x | 1.01x |
| 上位集合 zeta | 0.94x | 1.11x | 1.01x |
| 部分集合 Möbius | 1.35x | 1.44x | 1.21x |
| 上位集合 Möbius | 1.42x | 1.41x | 1.38x |
| 約数 zeta | 0.81x | 0.89x | 0.83x |
| 倍数 zeta | 0.86x | 0.91x | 0.81x |
| 約数 Möbius | 0.90x | 0.96x | 0.91x |
| 倍数 Möbius | 1.09x | 1.09x | 1.04x |

集合 zeta は効果が小さい。集合 Möbius は直接の減算でも改善でき、64 bit の配列を増やす上積みが小さい。約数・倍数の変換は配列変換とメモリ増加の負担が重い。したがって zeta は従来どおり、Möbius は直接の減算を選んだ。変更していない zeta の `adopted` 行は測定の対照として残している。

## メモリと検証

掃き出しは入力コピーとは別に約 8nm byte と O(n)、長い Hadamard は約 8N byte の作業領域をヒープに確保する。一般の体・群と独自演算は汎用実装を使う。値の上限の説明は [research/delayed_reduction.md](../research/delayed_reduction.md) を参照。

- 追加テストは C++17 / C++20 で実行。素数 2、動的法の切替、31 bit 上限付近、合成数法の加法群、独自演算、特異・長方形、切替境界、逆変換を含む。
- 既存の内積・内積再利用・Matrix・MatrixArray・BBLA・BBLA 拡張の回帰テストも通過。
- Library Checker の保存済みケース 198 件を通過：行列式 25、rank 36、逆行列 26、連立方程式 27、XOR 13、AND/OR 13、GCD 29、LCM 29。連立方程式は解・核・独立性を検査。オンライン提出ではない。
- Apple Clang の ASan/UBSan で、共通の掃き出し本体・MatrixArray・全変換を検証。動的 Matrix は既存の継承コンストラクタ宣言が Apple Clang でコンパイルできないため sanitizer 対象から外し、GCC と問題テストで確認した。
