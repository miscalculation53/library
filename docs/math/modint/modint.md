## 概要

ACL の modint がベース

`modint`（`dynamic_modint32<-1>`）などの動的 modint は、値の生成や `mod()` の呼び出しより先に `mint::set_mod(m)` で法を設定する。初期化忘れは `set_mod` の呼び出しを促すメッセージ付きの `assert` で検出する（`NDEBUG` 定義時は無効）。

相違点：

- 入出力ができる
- 他は外から使う分にはあまり変わっていないと思うが、中は一部省略している

制約：

- 動的 modint は使用前に `set_mod(m)` を呼び、$m \geq 1$ とする。
- `dynamic_modint32<INT_MIN>` はライブラリの内部で使用するので使用しないこと
- mod の上限は、`int` に収まるなら OK なはず（証明したわけではない）
  - $2^{31} - 1 = 2.147483647 \times 10^9$

## 使用例

```cpp
using mint = modint;
mint::set_mod(998244353);
mint x = 1;
```
