## $2$ 次元セグメント木（疎、クエリ点先読み）

一点更新・矩形モノイド積ができる（ただし、一点更新が起こる点は（本ライブラリでは）先読みする必要がある）。log ふたつ。なお、$2$ 次元目にセグメント木以外のデータ構造も載せられるようになっており、たとえば更新がない場合は (Disjoint) Sparse Table や静的 RMQ なども載せることができて計算量の改善になる。

中身について：普通に点を二分探索で探そうとするとそのパートに log ふたつがついてしまうが、Fractional Cascading を使うと回避できる。これは各ノードに $y$ 座標の列に加え、子に潜ったときのその列でのインデックスも持っておくというアイデア。再帰セグ木でやる必要がある。

### コンストラクタ

```cpp
SegmentTree2DSparse<DS, M, I>(vc<tuple<I, I, M::S>> xyws)
```

- `DS` は $2$ 次元目に載せるデータ構造（典型的には `SegmentTree<M>`）
- `M` はモノイド
- `I` は座標の型
- `xyws` は `add` クエリが飛んでくる点たち（最初の重みも与える）

**x 方向の座圧は自動でやらないので注意！**

##### 計算量

$n = \max(x)$ として

- $O(n \log n)$

### メンバ関数

#### set

```cpp
void set(I x, I y, M::S val)
```

点 $(x, y)$ の値を $\mathrm{val}$ にする。

##### 制約

- $x, y$ は `xyws` のいずれかに含まれる
- `DS` はメソッドに `set` を持つ

##### 計算量

$n = \max(x)$、$f(n)$ を `DS` の `set` メソッドの計算量として

- $O(\log(n) \cdot f(n))$

#### get

```cpp
M::S get(I x, I y)
```

点 $(x, y)$ の値を取得する。最初に与えられていない点に関しては `M::e()` を返す。

##### 制約

- `DS` はメソッドに `get` を持つ

##### 計算量

$n = \max(x)$、$f(n)$ を `DS` の `get` メソッドの計算量として

- $O(\log(n) + f(n))$

#### prod

```cpp
(1) M::S prod(I x, I ly, I ry)
(2) M::S prod(I lx, I rx, I ly, I ry)
```

- (1) 矩形領域 $(-\infty, \mathrm{x}) \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの積を求める。
- (2) 矩形領域 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の点の重みの積を求める。

##### 制約

- `DS` はメソッドに `prod` を持つ
- (1) では $\mathrm{ly} \leq \mathrm{ry}$、(2) ではそれに加え $\mathrm{lx} \leq \mathrm{rx}$

##### 計算量

$n = \max(x)$、$f(n)$ を `DS` の `prod` メソッドの計算量として

- (1)：$O(\log(n) + f(n))$
- (2)：$O(\log(n) \cdot f(n))$


#### all_prod

```cpp
M::S all_prod()
```

すべての点の重みの積を求める。

##### 制約

- `DS` はメソッドに `all_prod` を持つ

##### 計算量

$n = \max(x)$、$f(n)$ を `DS` の `all_prod` メソッドの計算量として

- $O(f(n))$


#### content

```cpp
vc<tuple<I, I, M::S>> content()
```

$(x\ 座標, y\ 座標, 重み)$ の組のうち、重みが `S::e()` でないものを格納した vector を返す。デバッグ用を想定。

##### 制約

- `DS` はメソッドに `get` を持つ

##### 計算量

$n = \max(x)$、$f(n)$ を `DS` の `content` メソッドの計算量として

- $O(n \log(n) \cdot f(n))$


