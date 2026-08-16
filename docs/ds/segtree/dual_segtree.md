## 概要

双対セグメント木。区間作用・一点取得ができる。

## 詳細なドキュメント

### DualSegmentTree

作用 `A` は型 `S`, `F` と次の関数を持つ：

- `S mapping(F f, S x)`
- `F composition(F f, F g)`
- `F id()`

`composition(f, g)` は、先に `g`、次に `f` を作用させる合成を返す。長さ指定のコンストラクタを使う場合は `S e()` も必要。

#### コンストラクタ

```cpp
(1) DualSegmentTree<A>()
(2) DualSegmentTree<A>(int n)
(3) DualSegmentTree<A>(Iter first, Iter last)
(4) DualSegmentTree<A>(const vc<T>& a)
```

- (1)：デフォルトコンストラクタ。
- (2)：`A::e()` を $n$ 個並べた列で初期化する。
- (3), (4)：与えられた列で初期化する。各要素は `S` に変換できればよい。

##### 制約

- $n \geq 0$
- `A::mapping(A::composition(f, g), x) == A::mapping(f, A::mapping(g, x))`
- `A::id()` は作用の合成に関する単位元

##### 計算量

- $O(n)$

#### set

```cpp
void set(int p, const S& x)
```

`a[p]` を `x` に変更する。

##### 制約

- $0 \leq p < n$

##### 計算量

- $O(\log n)$

#### get

```cpp
S get(int p)
```

保留中の作用を反映した `a[p]` を返す。

##### 制約

- $0 \leq p < n$

##### 計算量

- $O(\log n)$

#### apply

```cpp
(1) void apply(int p, const F& f)
(2) void apply(int l, int r, const F& f)
```

- (1)：`a[p]` を `A::mapping(f, a[p])` に変更する。
- (2)：区間 $[l, r)$ の各要素に `f` を作用させる。

##### 制約

- (1)：$0 \leq p < n$
- (2)：$0 \leq l \leq r \leq n$

##### 計算量

- $O(\log n)$

#### content

```cpp
vc<S> content()
```

すべての作用を反映した列を返す。

##### 計算量

- 時間：$O(n \log n)$

### ActCompoundAssignment

```cpp
ActCompoundAssignment<M>
```

モノイド `M` の積を双対セグメント木の作用として使うための補助型。`S = F = M::S` であり、`f` の作用は `M::op(f, x)`。

#### mapping

```cpp
S mapping(const F& f, const S& x)
```

`M::op(f, x)` を返す。

##### 計算量

- `M::op` $1$ 回

#### composition

```cpp
F composition(const F& f, const F& g)
```

`M::op(f, g)` を返す。

##### 計算量

- `M::op` $1$ 回

#### id

```cpp
F id()
```

`M::e()` を返す。

##### 計算量

- $O(1)$
