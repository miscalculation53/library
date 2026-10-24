このドキュメントには AI による作成・変更が含まれています。

## 概要

グラフの隣接リストに代表されるような `vvc<T>` の構造の代替。一次元の vector のみでよく、高速・省メモリ。

`vvc<T>` と比較してできないことは `v[i].push_back(x)`、それ以外は（実装しているかはともかく）大体できるような気がする（要出典）

## 使用例

```cpp
#include "ds/csr.hpp"

CSR<int> rows(vvc<int>{{2, 4}, {}, {7}});
assert(rows.size() == 3);
assert(rows[0][1] == 4);
assert(rows[1].empty());
assert(rows[2].content() == (vc<int>{7}));
```

## 詳細なドキュメント

### CSR

#### コンストラクタ

```cpp
(1) CSR()
(2) CSR(vc<int> row_sizes)
(3) CSR(int n, vc<pair<I, T>> ies)
(4) CSR(vvc<T> vv)
```

- (1)：行数 $0$ の空の CSR を構築する。
- (2)：`row_sizes[i]` 個の値初期化された要素を持つ第 $i$ 行を構築する。`get_elist()` と `offset()` を使って要素を直接書き込める。
- (3)：$n$ は $\mathrm{vv}$ の長さを表す。$\mathrm{ies}$ の要素 $(i, e)$ は、$\mathrm{vv}[i]$ に $e$ が含まれることを表す。この表現を CSR 表現に変換する。
- (4)：`vvc` での表現を CSR 表現に変換する。

##### 計算量

全要素数を $m$ として

- $O(n+m)$

##### 制約

- (2)：`row_sizes` の要素は非負
- (3)
  - $n > 0$
  - 各 $(i, e)$ に対し $0 \leq i < n$

#### operator[], at

`vvc<T>` と同じように扱えるが、変更はできない。（変更できるように実装することも可能ではあるはず。）

`operator[]` は範囲チェックをしない。`at` に関しては、$1$ 段階目の `at` は範囲外であれば空の行を返し、$2$ 段階目の `at` は範囲外であれば RE にする。

#### size

```cpp
I=ll size()
```

$n$ を返す。

##### 計算量

- $O(1)$

#### offset

```cpp
int offset(int i)
```

第 $i$ 行の先頭が平坦な要素列 `get_elist()` の何番目にあるかを返す。`offset(size())` は全要素数になる。

##### 制約

- $0 \leq i \leq \mathrm{size}()$

##### 計算量

- $O(1)$

#### sortunique

```cpp
void sortunique()
```

各行を個別にソートし、同じ行にある重複要素を削除する。

##### 計算量

第 $i$ 行の要素数を $m_i$ として、

- $O\left(\sum_i m_i \log m_i\right)$

#### content / to_vv

```cpp
vvc<T> content() const
vvc<T> to_vv() const
```

現在の各行を `vvc` にコピーして返す。空の行も維持し、`pop_back` で削除した要素は除く。
各行にも `content() const` と `to_v() const` があり、その行を `vc<T>` にコピーできる。

##### 計算量

- $O(n+m)$
- 行の取得：その行の要素数に比例
