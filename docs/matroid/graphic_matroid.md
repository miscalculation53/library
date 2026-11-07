このドキュメントには AI による作成・変更が含まれています。

## 概要

無向グラフの辺を台集合とし、森をなす辺集合を独立とする graphic matroid。自己ループ・多重辺に対応する。要素番号は入力の辺の添字。

fundamental circuit は、追加辺の両端を結ぶ森上のパスと追加辺からなる。

選択辺の隣接関係には [`GraphUndirected`](../graph/graph.md)、森の親・深さには [`add_virtual_root`](../tree/add_virtual_root.md) と [`RootedTree`](../tree/rooted_tree.md) を使う。

線形表現は、このヘッダの `linear_representation<F>(...)` メンバから取得できる。引数・条件は [線形表現](linear_representation.md) を参照。

## 使用例

```cpp
GraphicMatroid m(3, {{0, 1}, {1, 2}, {2, 0}, {0, 0}});
m.build({0, 1});
auto c = m.circuit(2); // 0, 1 を含む列
assert(m.circuit(3)->empty()); // 自己ループ
```

## 詳細なドキュメント

### GraphicMatroid

#### コンストラクタ

```cpp
GraphicMatroid(int vertices, vc<pair<int,int>> edges);
```

頂点数と辺列を指定して構築する。初期状態の独立集合は空集合。以下、頂点数を $v$、辺数を $n$ とする。

##### 制約

- `vertices >= 0`。
- 各辺の両端は `0..vertices-1`。

##### 計算量

- $O(v+n)$。

#### size

```cpp
int size() const;
```

台集合の要素数、すなわち辺数を返す。

##### 計算量

- $O(1)$。

#### build

```cpp
void build(const vc<int>& independent);
```

現在の独立集合 $I$ を置き換え、森の親と深さを計算する。

##### 制約

- 各要素は有効な辺番号で、互いに異なる。
- 指定した辺集合は森をなす。

##### 計算量

- $O(v+n)$。

#### circuit

```cpp
optional<vc<int>> circuit(int e) const;
```

`e` の両端が異なる連結成分なら `nullopt`、同じ成分なら両端を結ぶパスの辺番号を返す。返す列は fundamental circuit から `e` を除いたもので、順序は未規定。自己ループは値ありの空配列になる。

##### 制約

- `e` は有効な辺番号で、$I$ の外にある。

##### 計算量

- $O(\lvert C\rvert)$。$C$ は返す要素列。
