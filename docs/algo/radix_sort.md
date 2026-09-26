## 概要

整数キーの昇順に並べる安定な基数ソート。同じキーの要素は、入力時の順序を保つ。
符号付き・符号なし整数と `i128`, `u128` に対応する。

`radix_sort` は列を並べ替え、`radix_argsort` は並べ替え後の添字列を返す。
構造体などの列には、整数キーを取り出す関数を渡せる。

キーを一度取り出して保存し、下位の桁から安定に振り分ける。
全要素で等しい桁は省略する。32 要素以下では挿入ソートを使う。
それ以外は通常 8 bit ずつ、65536 要素以上では最大 16 bit ずつ処理する。

## 使用例

```cpp
#include "algo/radix_sort.hpp"

vc<ll> a = {5, -2, 5, 0};
auto ord = radix_argsort(a); // {1, 3, 0, 2}
radix_sort(a);               // {-2, 0, 5, 5}

vc<pair<int, string>> records = {{2, "A"}, {-1, "B"}, {2, "C"}};
radix_sort(records, [](const auto &x) { return x.first; });
// {{-1, "B"}, {2, "A"}, {2, "C"}}
```

## 詳細なドキュメント

#### radix_argsort

```cpp
(1) vc<int> radix_argsort(const vc<I>& a)
(2) vc<int> radix_argsort(const vc<T>& a, const Key& key)
```

- (1)：`a[ord[i]]` が昇順になる添字列 `ord` を返す。
- (2)：`key(a[ord[i]])` が昇順になる添字列を返す。`key` は各要素につき高々 1 回呼ぶ。

同じキーでは元の添字の昇順になる。入力 `a` は保持する。
空の列には空の添字列を返す。

##### 制約

- (1) の `I`、(2) のキーの型は `bool` を除く整数型。`i128`, `u128` も使える
- (2) の `key` は `const T&` を受け取り、値または参照として整数キーを返す
- `key` は入力の要素を保持する

##### 計算量

$n=|a|$、キーのビット幅を $w$、1 回に処理するビット数を $d$ とする。

- $O(n + \lceil w/d\rceil(n+2^d))$。キー取得の時間は別に加算する
- 固定幅の整数型では $O(n)$

#### radix_sort

```cpp
(1) void radix_sort(vc<I>& a)
(2) void radix_sort(vc<T>& a, const Key& key)
```

(1) は値の昇順、(2) は `key(a[i])` の昇順に `a` を安定ソートする。
`radix_argsort` で添字列を求めてから、巡回置換ごとに要素を移動する。
コピー・デフォルト構築を持たない要素型にも使える。

##### 制約

- キーについての条件は `radix_argsort` と同じ
- 要素型はムーブ構築とムーブ代入が可能

##### 計算量

- `radix_argsort` と同じ時間に加えて $O(n)$ 回の要素の移動
