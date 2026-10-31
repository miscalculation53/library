このドキュメントには AI による作成・変更が含まれています。

## 概要

01 列に対する Fenwick Tree は、長さ $B = 64$ の 01 列を `ull` で持つことで、長さ $n/64$ の Fenwick Tree に対する処理で行え、多少の高速化になる。（$\log_2 n$ が $\log_2(n/64) = \log_2 n - 6$ になる。）

使用場面の例：

- 転倒数
- 集合を管理する

## 使用例

```cpp
#include "ds/fenwick_tree/fenwick_tree_01.hpp"

FenwickTree01<> fw(vc<int>{0, 1, 0, 1, 1});
assert(fw.sum(1, 4) == 2);
fw.set(2, true);
assert(fw.sum(5) == 4);
assert(fw.get(2));
```

## 詳細なドキュメント

### FenwickTree01

#### コンストラクタ

```cpp
(1) FenwickTree01<T = ll, Word = ull>(int n)
(2) FenwickTree01<T = ll, Word = ull>(vc<U> v)
```

`T` は和を管理する整数型。`int` で足りるが、使いやすさのためデフォルトでは `ll` にしている。

- (1) 長さ $n$、全要素単位元で初期化する。
- (2) vector $v$ で初期化する。

##### 計算量

- (1)：$O(n)$
- (2)：$O(n)$

#### size

```cpp
I size<I=ll>()
```

$n$ を返す。

##### 計算量

- $O(1)$

#### sum

```cpp
(1) T sum(int r)
(2) T sum(int l, int r)
```

- (1)：$[0, r)$ の和を返す。
- (2)：$[l, r)$ の和を返す。

両端が同じワードに収まる区間は、ビットマスクと popcount で直接求める。

##### 制約

- (1)：$0 \leq r \leq n$
- (2)：$0 \leq l \leq r \leq n$

##### 計算量

- (1)：$O\left(\log \dfrac{n}{B}\right)$
- (2)：同じワード内なら $O(1)$、それ以外は $O\left(\log \dfrac{n}{B}\right)$

#### get

```cpp
bool get(int i)
```

$i$ 番目の値を返す。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(1)$

#### set

```cpp
void set(int i, bool b)
```

$i$ 番目の要素を $b$ にする。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$

#### leq_max, lt_max, gt_min, geq_min

```cpp
(1) I leq_max<I=ll>(T w)
(2) I lt_max<I=ll>(T w)
(3) I gt_min<I=ll>(T w)
(4) I geq_min<I=ll>(T w)
```

$[0, r)$ の要素の和に関して二分探索する：

- (1)：$\mathrm{sum}[0, r) \leq w$ となる $r$ の最大値（左番兵 `-1`）を返す。
- (2)：$\mathrm{sum}[0, r) \lt w$ となる $r$ の最大値（左番兵 `-1`）を返す。
- (3)：$\mathrm{sum}[0, r) \gt w$ となる $r$ の最小値（右番兵 `n+1`）を返す。
- (4)：$\mathrm{sum}[0, r) \geq w$ となる $r$ の最小値（右番兵 `n+1`）を返す。

有効な接頭辞長は $0 \leq r \leq n$。左側の検索には `-1` を表せる符号付き整数型 `I` を使う。

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$

---

$[0, n)$ の要素からなる集合を管理するのにも使える： 要素 $i$ が存在するかどうかをデータ構造の $i$ 番目に持てばよい。（クエリ先読み + 座標圧縮と組み合わせることもしばしば）

以下は、この使い方をしたとき用のメソッドである。

（`sum` を使うと「$x$ 以上の値の個数」などがわかるがこれは別のメソッドにはしていない。）

#### size_of_set

```cpp
S size_of_set()
```

集合のサイズを返す。`sum(n)` に一致。

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$

#### order_in_set

```cpp
pair<I, I> order_in_set<I=ll>(int x)
```

$x$ が集合に存在するとき、$x$ が何番目かを返す。存在しないときは（も）、「$x$ 以上で最小の要素」が何番目かを返す。

##### 制約

- $0 \leq x \lt n$

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$

#### kth_in_set

```cpp
optional<I> kth_in_set<I=ll>(int k)
```

「集合の $k$ 番目に小さい要素」を返す。範囲外の `k` は `nullopt` を返す。

有効な `k` では `leq_max` と一致する。

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$

#### lt_max_in_set など

```cpp
(1) I lt_max_in_set<I=ll>(T x)
(2) I leq_max_in_set<I=ll>(T x)
(3) I geq_min_in_set<I=ll>(T x)
(4) I gt_min_in_set<I=ll>(T x)
```

- (1)：$x$ 未満で最大の要素が**小さい方から何番目か**を返す。左番兵は `-1`。
- (2)：$x$ 以下で最大の要素が**小さい方から何番目か**を返す。左番兵は `-1`。
- (3)：$x$ 以上で最小の要素が**小さい方から何番目か**を返す。右番兵は `size_of_set()`。
- (4)：$x$ 超過で最小の要素が**小さい方から何番目か**を返す。右番兵は `size_of_set()`。

左側の検索には `-1` を表せる符号付き整数型 `I` を使う。順位は $0$ 始まり。

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$
