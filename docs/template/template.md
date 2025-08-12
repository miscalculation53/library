## テンプレート（全体）

### LOCAL

手元では `LOCAL` が define された状態で実行する。オンラインジャッジ等では `LOCAL` が define されていない想定。

デバッグのときは簡単なものであれば `dump` を使うだけでよいが、少し複雑になった場合 `local()` で囲むと `LOCAL` が define された環境のみで出力されるようになる。（`#ifdef LOCAL` で囲むのでもよいが）

### テストケースの形式

- `SINGLE_TESTCASE`：単一テストケース

- `MULTI_TESTCASE`：複数テストケース（最初にケース数が入力される）

- `AOJ_TESTCASE`：複数テストケース（最初にケース数が入力されない）
  - このときは `main2` の内部で最後のケースのとき `exit(0)` をする

問題に応じて手動で切り替える。

`LOCAL` では `SINGLE_TESTCASE` は `AOJ_TESTCASE` になる（サンプルを試したりデバッグしたりをしやすくする目的）。

### 入出力高速化

`FAST_IO` が define されているとき、例の `cin.tie(0); ios::sync_with_stdio(false);` をする。`FAST_IO` は `LOCAL` では外れる。

普段は `PRINT` 系での改行で flush しない。`INTERACTIVE` が define されているとき、flush する。


### 定数

これらは必要に応じて書き換える。

- `INF` $= 4 \times 10^{18} + 37$
  - $\in [2^{61}, 2^{62})$
  - `INF + INF` はオーバーフローしない
  - 一応素数

- `EPS` $= 10^{-11}$ (double)

### ランダムテスト

`test()` をいい感じに書き換える。

`test()` は `LOCAL` のみで実行される。
