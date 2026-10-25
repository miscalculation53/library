このドキュメントには AI による作成・変更が含まれています。

## 概要

定義されている型はソースコードの通り。

注意点だけ書いておく：

- `ld` は浮動小数点の型だが、これは `EPS` の型で決まる。`EPS` は後ろに `L` をつけると `long double` になり、つけないと `double` になる。
- `f128` は常に IEEE binary128 の四倍精度。`std::float128_t` が提供される C++23 ではその型を使い、それ以外では `__float128` を使う。`LIBRARY_HAS_FLOAT128` は `1`。
- `pql` は **昇順**、`pqg` は **降順** の優先度つきキュー。C++ の `priority_queue` はデフォルトが降順で `greater` をつけると昇順になるという（直感に反する）仕様になっているが、命名がそれに追従していない（特に、他の人のテンプレートと異なる場合がある）ので注意。

## 使用例

```cpp
#include "template/template_all_but_modint.hpp"

f128 x = f128(1) / 3;
f128 y = sqrt(x);
PRINT(y);
```

`f128` を使った計算は、手元とジャッジの両方で仮数部 $113$ bit（十進約 $33$ 桁）の精度で行う。`numeric_limits<f128>::digits` は `113`。

`ld` も `f128` に揃える場合は、テンプレートを読み込む前に `#define EPS f128(1e-25L)` のように定義する。定数は `f128(1) / 3` のように計算するか、`READ` で読み込むと選択された型の精度を使える。

macOS / Homebrew GCC では libquadmath を使い、コンパイル時に `-lquadmath` を付ける。普段の実行用 `tools/run_cpp.py`、提出用コードの検証、verify-helper の設定では常に付ける。

数学関数は `sqrt(x)`, `sin(x)`, `pow(x, 2)` のように名前空間を指定せず呼ぶ。三角関数・指数対数・丸め・分類関数など、実数用の関数を用意する。`cin` / `cout`、`READ` / `PRINT`、`dump` も四倍精度に対応する。`std::sqrt` など名前空間を指定した呼び出しや、標準ライブラリ全体の拡張型対応は処理系に従う。

AtCoder の [C++23 (GCC 15.2.0)](https://img.atcoder.jp/file/language-update/2025-10/language-list.html) のように標準の数学関数と文字列変換が揃う環境では、それらを使うため追加のリンク設定を要しない。`LIBRARY_USE_QUADMATH` は libquadmath を使う環境で `1`、標準機能を使う環境で `0`。この選択で精度は共通。

### 制約

- IEEE binary128 を扱える GCC と標準ライブラリを使う。macOS では x86_64 版 Homebrew GCC 15.2 で検証している。
- `LIBRARY_USE_QUADMATH == 1` の環境では libquadmath をリンクする。

提出用コードを `tools/submit_code.py` で展開・短縮した場合も、この環境判定を保持する。

## 詳細なドキュメント

### 型の別名

| 名前 | 型・用途 |
| --- | --- |
| `ll`, `ull` | `long long`, `unsigned long long` |
| `uint` | `unsigned int` |
| `i128`, `u128` | 符号付き・符号なし $128$ ビット整数 |
| `ld`, `f128` | `EPS` の型、IEEE binary128 |
| `pll`, `tlll`, `tllll` | `ll` を $2$・$3$・$4$ 個持つ組 |
| `vc<T>`, `vvc<T>`, `vvvc<T>` | $1$・$2$・$3$ 次元の `vector` |
| `vb`, `vl`, `vstr`, `vpll` | `bool`, `ll`, `string`, `pll` の `vector` |
| `pql<T>`, `pqg<T>` | 最小値・最大値を `top()` で返す優先度付きキュー |

### 128 ビット整数の文字列変換

```cpp
i128 stoi128(const string& s)
string i128tos(i128 x)
```

`stoi128` は十進文字列を整数へ変換し、`i128tos` は符号付き十進表記へ変換する。`i128` にはストリーム入出力も使える。

##### 制約

- `stoi128` の入力は、`i128` に収まる十進整数の表記。負数には先頭に `-` を付ける

##### 計算量

- 文字数を $k$ として $O(k)$
