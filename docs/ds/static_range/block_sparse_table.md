このドキュメントには AI による作成・変更が含まれています。

## 概要

静的列に対するモノイドの区間積クエリを、列を幅 $B$ のブロックに分割し、sparse table または disjoint sparse table と組み合わせて処理する。前計算は $O\left(N+\dfrac{N}{B} \log \dfrac{N}{B}\right)$ 時間・空間、クエリは最悪 $O(B)$ 時間。$B = \Theta(\log N)$ ととると、前計算の時空間は線形となる。

各ブロックの prefix・suffix の積を前計算し、ブロック全体の積には suffix の値を再利用する。

クエリに関しては幅 $B$ 以上のときは $O(1)$ 時間で、幅 $B$ 未満のときに愚直 $O(B)$ 時間ということなので、（モノイド積の軽さにもよるが）額面の印象より軽いと思う。

## 使用例

```cpp
#include "algebra/algebra_basic_ops.hpp"
#include "ds/static_range/block_sparse_table.hpp"

vc<ll> a{5, 2, 7, 1};
BlockSparseTable<MonoidMin<ll>> rmq(a);
BlockDisjointSparseTable<MonoidAdd<ll>> sums(a);
assert(rmq.prod(0, 3) == 2);
assert(sums.prod(1, 4) == 10);
```

## 詳細なドキュメント

### BlockSparseTable / BlockDisjointSparseTable

#### コンストラクタ

```cpp
(1) BlockSparseTable<M, int B = 16>(vc<M::S> v)
(2) BlockDisjointSparseTable<M, int B = 16>(vc<M::S> v)
```

##### 制約

- `M` はモノイド
  - (1) においては `M` は冪等である必要がある

##### 計算量

`vec` の長さを $n$ として

- $O\left(n+\dfrac{n}{B} \log \dfrac{n}{B} \right)$

#### get

```cpp
M::S get(int p)
```

$v_p$ の値を返す。

##### 制約

- $0 \leq p \lt n$

##### 計算量

- $O(1)$

#### prod

```cpp
M::S prod(int l, int r)
```

$v_l \bullet \cdots \bullet v_{r-1}$ の値を返す。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- $r - l \lt B$ のとき、$O(B)$
- $r - l \geq B$ のとき、$O(1)$

#### content

```cpp
vc<M::S> content() const
```

構築時の全要素を添字順に並べた配列のコピーを返す。空列では空の配列を返す。
`BlockSparseTable` と `BlockDisjointSparseTable` の両方で使える。

##### 計算量

- $O(n)$
