## 概要

列の添字を値ごとに分類し、各添字列を昇順に保持する。内部では CSR を使う。

`GroupIndex grp(a)` は座標圧縮を行い、負数、大きい整数、文字列などをそのまま扱える。検索には元の値を指定する。`GroupIndexRaw grp(a)` は、小さい非負整数を直接 CSR の行番号に使う非圧縮版。どちらも値の型を引数から推論する。

どちらの版も、範囲 for で「値と添字列」の組を値の昇順に列挙できる。列挙するのは入力に出現した値だけ。

用途：

- 値ごとの処理（例：連結成分の番号から、頂点を成分ごとにまとめる）。
- ある値の出現位置の前後検索、出現回数の計算。

## 使用例

```cpp
vl a = {10000000000LL, -3, 10000000000LL, 7};
GroupIndex grp(a);
for (auto [value, indices] : grp)
{
  // (-3, {1}), (7, {3}), (10000000000, {0, 2}) の順
  for (auto i : indices) { /* a[i] == value */ }
}
auto is = grp.idxs(10000000000LL); // {0, 2}
ll count = grp.in_cnt(10000000000LL, 1, 4); // 1

vc<int> ids = {0, 2, 2, 5};
GroupIndexRaw dense(ids);
for (auto [value, indices] : dense)
{
  // (0, {0}), (2, {1, 2}), (5, {3}) の順
}
```

## 詳細なドキュメント

### GroupIndex

`GroupIndex<T = ll, bool compress = true, I = ll>` の `T` は値の型、`I` は添字列の要素と検索結果の型。`GroupIndex grp(a)` では `T` が自動で推論される。添字を `int` で保持する場合は `GroupIndex<T, compress, int>` と指定する。

`GroupIndexRaw<T = ll, I = ll>` は `GroupIndex<T, false, I>` を継承した非圧縮版。`GroupIndexRaw grp(a)` で `T` を推論でき、以下の関数をすべて同じように使える。添字を `int` で保持する場合は `GroupIndexRaw<T, int>` と指定する。

以降、入力の長さを $n$、値の種類数を $k$ とする。非圧縮版では $m = \max(a)+1$ とし、空列なら $m = 0$ とする。計算量は値の比較・コピーを $O(1)$ として記す。

#### コンストラクタ

```cpp
(1) GroupIndex()
(2) GroupIndex(const vc<T>& a)
```

- (1)：空の構造を作る。
- (2)：列 $a$ の添字を値ごとに分類する。

##### 制約

- `I` は符号付き整数型。
- `compress = true`：`T` は `<` と等値比較を使え、それらが整合する全順序を持つ。
- `compress = false`：`T` は整数型で、各要素は非負。$m$ に比例する配列を確保・走査できること。

##### 計算量

- (1)：$O(1)$
- (2)、圧縮版：$O(n \log(n+1))$
- (2)、非圧縮版：$O(n+m)$

#### idxs

```cpp
auto idxs(const T& val) const
```

$a_i = \mathrm{val}$ である添字 $i$ を昇順に並べた、読み取り専用の CSR の行を返す。該当する添字がなければ空。`begin`, `end`, `size`, `empty`, `[]`, `front`, `back`, `to_v` を使える。

返された行は元の `GroupIndex` のデータを参照する。行を使う間は元のオブジェクトを保持する。

##### 計算量

- 圧縮版：$O(\log(k+1))$
- 非圧縮版：$O(1)$

#### lt, leq, gt, geq

```cpp
(1) I lt_max(const T& val, int i) const
(2) I leq_max(const T& val, int i) const
(3) I gt_min(const T& val, int i) const
(4) I geq_min(const T& val, int i) const
(5) I lt_cnt(const T& val, int i) const
(6) I leq_cnt(const T& val, int i) const
(7) I gt_cnt(const T& val, int i) const
(8) I geq_cnt(const T& val, int i) const
(9) I in_cnt(const T& val, int l, int r) const
```

それぞれ次の値を返す。

- (1)：$\max \lbrace j < i \mid a_j = \mathrm{val} \rbrace$、存在しなければ $-1$
- (2)：$\max \lbrace j \leq i \mid a_j = \mathrm{val} \rbrace$、存在しなければ $-1$
- (3)：$\min \lbrace j > i \mid a_j = \mathrm{val} \rbrace$、存在しなければ $n$
- (4)：$\min \lbrace j \geq i \mid a_j = \mathrm{val} \rbrace$、存在しなければ $n$
- (5)：$\# \lbrace j < i \mid a_j = \mathrm{val} \rbrace$
- (6)：$\# \lbrace j \leq i \mid a_j = \mathrm{val} \rbrace$
- (7)：$\# \lbrace j > i \mid a_j = \mathrm{val} \rbrace$
- (8)：$\# \lbrace j \geq i \mid a_j = \mathrm{val} \rbrace$
- (9)：$\# \lbrace l \leq j < r \mid a_j = \mathrm{val} \rbrace$

「$a_i = \mathrm{val}$ は何番目の $\mathrm{val}$ か」は (5) と一致する。

##### 制約

- (9)：$l \leq r$

##### 計算量

$\mathrm{val}$ の出現回数を $c$ として、

- 圧縮版：$O(\log(k+1)+\log(c+1))$
- 非圧縮版：$O(\log(c+1))$

#### size, empty

```cpp
I size() const
bool empty() const
```

`size()` は値の種類数 $k$、`empty()` は入力が空かを返す。

##### 計算量

- $O(1)$

#### begin, end

```cpp
Iterator begin() const
Iterator end() const
```

範囲 for で `for (auto [value, indices] : grp)` と書ける。入力に出現した値を昇順に列挙し、`indices` は `idxs(value)` と同じ読み取り専用の行を参照する。

イテレータと行を使う間は元の `GroupIndex` を保持する。代入により保持データを置き換えると、それまでのイテレータと行は無効になる。

##### 計算量

- `begin`, `end`、イテレータの参照・インクリメント：$O(1)$
- すべての組の列挙：$O(k)$
- 各添字列の中身も含めた全走査：$O(n+k)$

#### to_csr, to_vv

```cpp
const CSR<I>& to_csr() const
vvc<I> to_vv() const
```

`to_csr()` は内部の CSR への参照、`to_vv()` はその内容のコピーを返す。

- 圧縮版：値を昇順に並べたときの $j$ 番目の値が第 $j$ 行に対応し、行数は $k$。
- 非圧縮版：値 $v$ が第 $v$ 行に対応し、行数は $m$。入力に出現しない値の行は空。

値と添字列の対応を扱う場合は、範囲 for が使える。

##### 計算量

- `to_csr()`：$O(1)$
- `to_vv()`、圧縮版：$O(n+k)$
- `to_vv()`、非圧縮版：$O(n+m)$
