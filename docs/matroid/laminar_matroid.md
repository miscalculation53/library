このドキュメントには AI による作成・変更が含まれています。

## 概要

木の各頂点に容量を設け、その部分木に割り当てられた要素の選択数を制限する laminar matroid。容量の対象集合は包含関係にあるか、互いに素になる。部門・チームなどの階層的な採用数制限に使える。

fundamental circuit から追加要素を除くと、最も近い飽和した祖先の部分木で選択済みの要素になる。

[`add_virtual_root`](../tree/add_virtual_root.md) と [`RootedTree`](../tree/rooted_tree.md) により、森の親・部分木・集計順序を管理する。仮想根には容量を設けない。

## 使用例

```cpp
// 頂点 0 が全体、頂点 1, 2 がその子。
LaminarMatroid m({-1, 0, 0}, {2, 1, 2}, {1, 1, 2, 2});
m.build({0, 2});
auto c = m.circuit(1); // {0}: 子 1 の容量
c = m.circuit(3);      // {0, 2}: 全体の容量
```

## 詳細なドキュメント

### LaminarMatroid

#### コンストラクタ

```cpp
LaminarMatroid(vc<int> parent, vc<int> capacity, vc<int> group);
```

`parent` はグループの包含関係を表す森の親配列で、根の親は `-1`。要素 `e` は頂点 `group[e]` に割り当てる。頂点 `v` 自身とその子孫に割り当てられた要素から、合計 `capacity[v]` 個まで選べる。台集合は `group` の添字。初期状態の独立集合は空集合。

以下、台集合の要素数を $n$、グループ数を $k$、森の高さを $h$ とする。

##### 制約

- `parent` は有効な森の親配列。
- `capacity.size() == parent.size()`、`capacity[v] >= 0`。
- `0 <= group[e] < parent.size()`。

##### 計算量

- $O(n+k)$。

#### size

```cpp
int size() const;
```

台集合の要素数を返す。

##### 計算量

- $O(1)$。

#### build

```cpp
void build(const vc<int>& independent);
```

現在の独立集合 $I$ を置き換え、各部分木の選択数を記録する。

##### 制約

- 各要素は台集合に属し、互いに異なる。
- 各部分木の選択数が容量以下。

##### 計算量

- $O(n+k)$。

#### circuit

```cpp
optional<vc<int>> circuit(int e) const;
```

$I+e$ が独立なら `nullopt`、従属なら fundamental circuit から `e` を除いた要素列を返す。`e` の所属する部分木の容量が $0$ なら値ありの空配列になる。返す列の順序は未規定。

##### 制約

- `e` は台集合に属し、$I$ の外にある。

##### 計算量

- $O(h+\lvert I\rvert)$。
