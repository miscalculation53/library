## 概要

- https://github.com/philip82148/cpp-dump
  - 解説記事 1： https://zenn.dev/sassan/articles/19db660e4da0a4
  - 解説記事 2： https://zenn.dev/sassan/articles/4878e79272ed61

上記のプログラムを手元で利用する。`LOCAL` が define されているときのみ dump 出力をする（特に、オンラインジャッジでは無効化される）。

---

その他マクロ

- `local()`：`LOCAL` のときのみ `()` 内が存在する
- `oj()`：`LOCAL` でないときのみ `()` 内が存在する
- `local_oj(a, b)`：`LOCAL` のときは `a`, `LOCAL` でないときは `b`

---

関数 `content()`：std の queue または priority_queue を引数に取り、中身を表す vector を返す。`dump(content(que))` のように使う。

`math/modint/to_rational.hpp` を include すると、C++23 以降では `dump(x | cp::rat())` で modint を `1/2` 形式で表示できる。配列、`pair`、`tuple`、map などの内部も再帰的に処理し、変換できない部分は通常どおり表示する。詳細は [modint を有理数に](../math/modint/to_rational.md) を参照。

## 使用例

```cpp
map<int, string> m{{1, "one"}, {2, "two"}};
dump(m); // { 1: "one", 2: "two" }（キーの 1, 2 はシアン）

vector<int> a{1, 2};
dump(a | cp::index() | cp::bin(4)); // 添字付きの二進数表示
dump(a | cp::bin(4) | cp::index()); // 同じ表示
```

## 詳細なドキュメント

#### map のキーの配色

`map`、`unordered_map`、`multimap`、`unordered_multimap` は、キーをシアン、値を従来の配色で表示する。
入れ子の map や、`cp::rat()` で有理数にしたキーにも適用する。
キーが `pair` などの場合は、そのキー全体を同じ色にする。単独の `pair`・`tuple` の配色は変更しない。

```cpp
cp::options::map_key_color = "\x1b[35m"; // キーをマゼンタに変更
cp::options::map_key_color.clear();     // キーの特別な配色を無効にする
```

設定の既定値は `"\x1b[36m"`。
`cp::options::es_style = cp::types::es_style_t::no_es` のときは色を付けない。
表示件数の制限、改行、`cp::front()`、キー・値それぞれの表示コマンドも引き続き利用できる。

##### 制約

- `LOCAL` が定義されている
