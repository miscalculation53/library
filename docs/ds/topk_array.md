## 概要

要素の追加・上位 $k$ 個の管理。`SameKey` を指定すると、同じキーの候補を最良の 1 個にまとめ、異なるキーを高々 $k$ 個保持する。

最短路・DP・セグメント木に共通する候補管理だけを担当し、グラフや DP の遷移には依存しない。

```cpp
using Candidate = pair<ll, int>; // (値, 色)
struct SameColor {
  bool operator()(const Candidate &a, const Candidate &b) const {
    return a.second == b.second;
  }
};
using Best = TopKArray<Candidate, 2, greater<Candidate>, SameColor>;
Best best;
best.add({10, 1});
best.add({9, 1});  // 同じ色で劣るので保持しない
best.add({8, 2});
// {{10, 1}, {8, 2}} を保持する。
```

ここで普通の上位 2 個では `{10,1}, {9,1}` となり、「色 1 以外の最大値」に答えられない。
色を指定して高々 1 種類除外するなら、異なる上位 2 色があれば十分。
最大 $r$ 種類のキーを除外する問い合わせには、異なる上位 $r+1$ 個を使える。

## DP・区間クエリでの使い方

例えば「直前と異なる色を選ぶ」DP の $\max_{c'\ne c}dp[c']$ は、色をキーとした上位 2 候補から計算できる。
[ABC345 E - Colorful Subsequence の公式解説](https://atcoder.jp/contests/abc345/editorial/9590)にも、この圧縮を使う DP がある。
無効な状態は候補として追加せず、番兵の色や $-\infty$ を必須にしない。

```cpp
using M = TopKArrayMonoid<Candidate, 2, greater<Candidate>, SameColor>;
auto merged = M::op(left, right);
```

`merged` をセグメント木・累積積・全方位木 DP の集約に使えば、区間内の指定色以外の最大値や、
特定の子の寄与を除いた最大値を求められる。キーを要素 ID にすれば同じ要素の重複排除にもなる。

ただし、削除済み候補の復活を要する操作や、ラベルごとの加点によって順位が変わる操作には対応しない。
圧縮した DP が正しいことは、遷移ごとに確かめる必要がある。

## 詳細なドキュメント

### TopKArray

要素を追加しながら、`Compare` で小さい順の先頭 $k$ 個だけを保持する固定長配列。デフォルトの `less<T>` では小さい方から $k$ 個を保持する。

#### コンストラクタ

```cpp
(1) TopKArray<T, k, Compare = less<T>, SameKey = void>()
(2) TopKArray<T, k, Compare = less<T>, SameKey = void>(Iter first, Iter last)
(3) TopKArray<T, k, Compare = less<T>, SameKey = void>(initializer_list<T> init)
(4) TopKArray<T, k, Compare = less<T>, SameKey = void>(const V& v)
```

- (1)：空で初期化する。
- (2)--(4)：与えられた全要素を順に追加して初期化する。

##### 制約

- $k \geq 1$
- `Compare` は `T` 上の strict weak ordering
- `SameKey=void` なら重複をまとめない（従来どおり）。それ以外では `SameKey()(a,b)` が同じキーかを返す同値関係。
- 比較器・同値判定はデフォルト構築できること。
- 保持される同点候補を安定させるには、`Compare` の tie-break にキーも含める。上の `pair` の比較はこの条件を満たす。

##### 計算量

入力要素数を $n$ として、

- (1)：$O(k)$
- (2)--(4)：$O(nk)$

#### add

```cpp
bool add(const T& val)
```

`val` を追加し、`Compare` で小さい方から高々 $k$ 個を保持する。
`SameKey` がある場合は同じキー内の最良の候補だけを残す。
保持している候補が変われば `true`、同じキーの改善でない候補や圏外の候補なら `false`。
戻り値を使わない既存の呼び出しはそのまま使える。

##### 計算量

- $O(k)$

#### size

```cpp
I size<I = ll>()
```

現在保持している要素数を返す。

##### 計算量

- $O(1)$

#### get

```cpp
T get(int i)
```

保持している要素を `Compare` の順に並べたときの `i` 番目を返す。

##### 制約

- $0 \leq i < \mathrm{size}()$

##### 計算量

- $O(1)$

#### merged

```cpp
TopKArray merged(const TopKArray& other) const
```

`*this` と `other` の要素を合わせたものから、`Compare` で小さい方から高々 $k$ 個を持つ新しい `TopKArray` を返す。
`SameKey` がある場合は、両方に含まれる同じキーも最良の 1 個へまとめる。

##### 計算量

- `SameKey=void`：$O(k)$。
- `SameKey` あり：$O(k^2)$。$k=2$ のような小さい定数を想定。

#### merge

```cpp
void merge(const TopKArray& other)
```

`*this` を `merged(other)` で置き換える。

##### 計算量

- `SameKey=void`：$O(k)$、それ以外：$O(k^2)$。

#### content

```cpp
vc<T> content()
```

保持している要素を `Compare` の順に格納した vector を返す。

##### 計算量

- 時間：$O(k)$

### TopKArrayMonoid

```cpp
TopKArrayMonoid<T, k, Compare = less<T>, SameKey = void>
```

`TopKArray` の `merged` を積とするモノイド。セグメント木などに載せるために使う。
同点の選択まで含めて結合則を満たすため、異なるキーを `Compare` の同値にせず、
同じキーで `Compare` 上も同値の候補は同一視できるものとする。
`SameKey` がある場合は、同じ集合を 2 回まとめても結果が変わらない冪等な演算になる。

#### op

```cpp
S op(const S& a, const S& b)
```

`a.merged(b)` を返す。

##### 計算量

- `SameKey=void`：$O(k)$、それ以外：$O(k^2)$。

#### e

```cpp
S e()
```

空の `TopKArray` を返す。

##### 計算量

- $O(k)$
