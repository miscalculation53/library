## 概要

$1$ 次元 imos 法

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) Imos<G = GroupAddSub<ll>>()
(2) Imos<G = GroupAddSub<ll>>(int n)
(3) Imos<G = GroupAddSub<ll>>(const vc<G::S>& a)
```

- (1)：デフォルトコンストラクタ。
- (2)：単位元を $n$ 個並べた列で初期化する。
- (3)：列 `a` で初期化する。

##### 制約

- `G` は可換群
- (2)：$n \geq 0$

##### 計算量

- (2)：$O(n)$
- (3)：$O(n)$

#### add

```cpp
void add(int l, int r, const S& v)
```

区間 $[l, r)$ の各要素に $v$ を作用させる。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- $O(1)$

#### content

```cpp
vc<S> content()
```

すべての区間作用を反映した長さ $n$ の列を返す。

##### 計算量

- 時間：$O(n)$
