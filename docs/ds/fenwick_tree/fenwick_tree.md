## Fenwick Tree

可換群に対して一点加算・区間取得ができるデータ構造。（取得が prefix だけでよいなら群でなくモノイドでもよい）

できることはセグ木の下位互換だが、定数倍がよく、実装が軽い。

### コンストラクタ

```cpp
(1) FenwickTree<G>(int n)
(2) FenwickTree<G>(vc<G::S> v)
```

- (1) 長さ $n$、全要素単位元で初期化する。
- (2) vector $v$ で初期化する。

#### 制約

- `G` は**可換**
  - 群であることが多いが、モノイドでも一部機能は OK


### メンバ関数

#### size

```cpp
I size<I=ll>()
```

$n$ を返す。

##### 計算量

- $O(1)$


#### sum

```cpp
(1) G::S sum(int r)
(2) G::S sum(int l, int r)
```

- (1)：$[0, r)$ の和を返す。モノイドでよい。
- (2)：$[l, r)$ の和を返す。群であることが必要。

##### 制約

- (1)：$0 \leq r \leq n$
- (2)：$0 \leq l \leq r \leq n$

##### 計算量

- $O(\log n)$


#### get

```cpp
G::S get(int i)
```

$i$ 番目の値を返す。群であることが必要。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(\log n)$


#### add

```cpp
void add(int i, G::S x)
```

$i$ 番目の要素に $x$ を加算する。モノイドでよい。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(\log n)$


#### set

```cpp
void set(int i, G::S x)
```

$i$ 番目の要素を $x$ にする。群であることが必要。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(\log n)$


#### leq_max, lt_max, gt_min, geq_min

```cpp
(1) I leq_max<I=ll>(G::S w)
(2) I lt_max<I=ll>(G::S w)
(3) I gt_max<I=ll>(G::S w)
(4) I geq_max<I=ll>(G::S w)
```

`G` が整数の通常の足し算で、要素が非負であるとする。このとき、$[0, r)$ の要素の和に関して二分探索する：

- (1)：$\mathrm{sum}[0, r) \leq w$ となる $r$ の最大値（なければ $-1$）を返す。
- (2)：$\mathrm{sum}[0, r) \lt w$ となる $r$ の最大値（なければ $-1$）を返す。
- (3)：$\mathrm{sum}[0, r) \gt w$ となる $r$ の最大値（なければ $n$）を返す。
- (4)：$\mathrm{sum}[0, r) \geq w$ となる $r$ の最大値（なければ $n$）を返す。

##### 計算量

- $O(\log n)$


#### kth_of_multiset

```cpp
I kth_of_multiset<I=ll>(G::S k)
```

Fenwick Tree は $[0, n)$ の要素からなる多重集合を管理するのにも使える： `G` は整数の通常の足し算にして、要素 $i$ を $c$ 個追加（削除）するときに $i$ 番目の要素に $c$ を加算（減算）すればよい。（クエリ先読み + 座標圧縮と組み合わせることもしばしば）

このとき、「多重集合の $k$ 番目に小さい要素」を返す（なければ $n$ を返す）。なおこれは `gt_min` と一致する。

##### 計算量

- $O(\log n)$
