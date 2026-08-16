## 概要

木のオイラーツアーとセグ木でパスのモノイド積クエリを処理（非可換でも OK）

非可換にするために逆順で積を取らないといけない

## 詳細なドキュメント

### PathProd

```cpp
PathProd<G, need_lca = true, DS1, DS2, RMQ>
```

`need_lca = false` とすると LCA の前計算を省略できるが、二頂点間の `prod(u, v)` は使用できない。

#### コンストラクタ

```cpp
(1) PathProd<G>(int n, const vc<I>& par, const vc<T>& a = {})
(2) PathProd<G>(int n, const vc<P>& es, int root, const vc<T>& a = {})
```

- (1)：親配列 `par` から構築する。
- (2)：無向木の辺集合 `es` と根 `root` から構築する。
- `a` を省略した場合は、全頂点を `G::e()` で初期化する。

##### 制約

- `G` は群
- $n \geq 1$
- (1)：`par` の長さは $n$ で、ちょうど一つの根を持つ木を表す
- (2)：`es` の長さは $n-1$ で、頂点 $0,\ldots,n-1$ の木を表す
- `a` は空、または長さ $n$

##### 計算量

- `need_lca = false`：$O(n)$
- `need_lca = true`：`EulerTour` の RMQ 構築に加えて $O(n)$
- デフォルトの `RMQ` では $O(n \log n)$

#### get

```cpp
S get(int v)
```

頂点 `v` の現在値を返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(1)$

#### set

```cpp
void set(int v, const S& x)
```

頂点 `v` の値を `x` に変更する。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(\log n)$

#### prod

```cpp
(1) S prod(int v)
(2) S prod(int u, int v)
```

- (1)：根から `v` までの頂点値を、パスの順に積んだ値を返す。
- (2)：`u` から `v` までの頂点値を、パスの順に積んだ値を返す。

##### 制約

- $0 \leq u, v < n$
- (2)：`need_lca = true`

##### 計算量

- $O(\log n)$
