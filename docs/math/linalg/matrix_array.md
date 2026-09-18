## 概要

行列ライブラリとほとんど同じものを実装している。`array` を使って実装しており、特に小さい定数サイズの行列のとき（セグ木に載せるなど）に定数倍高速化が期待できる。


## 32 bit modint の積に関する補足

`Matrix` と同じ条件で `dot_product<F>` を使う。
$n\times m$ 行列と $m\times k$ 行列の密な積では、
返り値に加えて転置用の $O(mk)$ の作業領域をヒープに確保する。
切替条件は [Matrix](matrix.md) を参照。


### 通常の 32 bit modint の掃き出しで使う作業領域

`F = FieldAddSubMulDiv<mint>` で、`mint` が `static_modint32` または `dynamic_modint32` のとき、行数・列数がともに 32 以上なら内部で 64 bit 整数の作業配列を使う。`row_reduction` の入力コピーに加えて、約 $8nm$ byte と $O(n)$ の作業領域をヒープに確保する。`rank`・`det`・`inv`・`solve` からこの掃き出しを呼ぶ場合も同様（`inv`・`solve` では拡大行列の大きさで数える）。
