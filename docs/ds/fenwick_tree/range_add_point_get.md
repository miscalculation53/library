## 区間加算一点取得

Fenwick Tree で累積和配列を管理するとできる。

### コンストラクタ

```cpp
(1) RangeAddPointGet<G>(n)
(2) RangeAddPointGet<G>(vc<T> v)
```

- (1)：長さ $n$、全要素単位元で初期化する。
- (2)：vector $v$ で初期化する。

##### 制約

- `G` は**可換**群

##### 計算量

- (1)：$O(n)$
- (2)：$O(n\log n)$


### メンバ関数

#### add

```cpp
(1) void add(int p, G::S x)
(2) void add(int l, int r, G::S x)
```

- (1)：$p$ 番目の要素に $x$ を加算する。
- (2)：$[l, r)$ 番目の要素に $x$ を加算する。

##### 制約

- (1)：$0 \leq p \lt n$
- (2)：$0 \leq l \leq r \leq n$

##### 計算量

- $O(\log n)$


#### set

```cpp
void set(int p, G::S x)
```

- $p$ 番目の要素を $x$ に変更する。

##### 制約

- $0 \leq p \lt n$

##### 計算量

- $O(\log n)$


#### get

```cpp
G::S get(int p)
```

$p$ 番目の要素を取得する。

##### 制約

- $0 \leq p \lt n$

##### 計算量

- $O(\log n)$

