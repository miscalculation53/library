## 概要

数列の添字を値で分類し、`vvc` のような構造（実際には CSR）を作る。値が存在しない行も保持するため、行数は値の種類数ではなく、空列なら $0$、そうでなければ $\max(a)+1$ となる。

用途：
- 普通に値ごとに処理するとき（例：連結成分の番号から、頂点を連結成分ごとにまとめる）。
- 二分探索などの操作を行うと、rank, select, pred, succ,（ある値の）range frequency などの操作ができる。

もしかして：「ある範囲にある値の range frequency」→ wavelet matrix

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) GroupIndex()
(2) GroupIndex(vc<T> a)
```

- (2)：数列 $a$ から、添字を値で分類した構造を作る。

##### 制約

- (2)
  - `T` は整数型
  - $a$ の各要素は $0$ 以上かつ大きくない

##### 計算量

$a$ の長さを $n$、最大値を $m$ として

- $O(n+m)$

#### idxs

```cpp
idxs(int val)
```

$a_i = \mathrm{val}$ である添字 $i$ を小さい順に並べた CSR の行（vector のように扱える）を返す。存在しない場合は空。

##### 計算量

- $O(1)$

#### lt, leq, gt, geq

```cpp
(1) lt_max(int val, int i)
(2) leq_max(int val, int i)
(3) gt_min(int val, int i)
(4) geq_min(int val, int i)
(5) U=ll lt_cnt(int val, int i)
(6) U=ll leq_cnt(int val, int i)
(7) U=ll gt_cnt(int val, int i)
(8) U=ll geq_cnt(int val, int i)
(9) U=ll in_cnt(int val, int l, int r)
```

それぞれ次の値を返す。

- (1)：$\max \lbrace j \lt i \mid a_j = \mathrm{val} \rbrace$、存在しなければ $-1$
- (2)：$\max \lbrace j \leq i \mid a_j = \mathrm{val} \rbrace$、存在しなければ $-1$
- (3)：$\min \lbrace j \gt i \mid a_j = \mathrm{val} \rbrace$、存在しなければ $n$
- (4)：$\min \lbrace j \geq i \mid a_j = \mathrm{val} \rbrace$、存在しなければ $n$
- (5)：$ \\# \lbrace j \lt i \mid a_j = \mathrm{val} \rbrace $
- (6)：$ \\# \lbrace j \leq i \mid a_j = \mathrm{val} \rbrace $
- (7)：$ \\# \lbrace j \gt i \mid a_j = \mathrm{val} \rbrace $
- (8)：$ \\# \lbrace j \geq i \mid a_j = \mathrm{val} \rbrace $
- (9)：$ \\# \lbrace l \leq j \lt r \mid a_j = \mathrm{val} \rbrace $

「$a_i = \mathrm{val}$ は何番目の $\mathrm{val}$ か」は (5) と一致する。

##### 計算量

- $O(\log n)$

#### value_bound

```cpp
I value_bound()
```

値として使える半開区間 $[0, m)$ の上端 $m$ を返す。空列なら $0$、そうでなければ $\max(a)+1$ を返す。したがって、すべての値について処理するときは `rep(val, grp.value_bound())` と書ける。

##### 計算量

- $O(1)$

#### to_vv

```cpp
vvc<I> to_vv()
```

`vvc` での表現にして返す。

##### 計算量

- $O(n+m)$
