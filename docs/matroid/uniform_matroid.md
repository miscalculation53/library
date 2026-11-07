このドキュメントには AI による作成・変更が含まれています。

## 概要

要素数が $r$ 以下の集合を独立とする一様マトロイド。台集合は $0,\ldots,n-1$。

`build` で独立集合を指定し、`circuit` で要素追加に伴う fundamental circuit を取得する。マトロイド交叉の oracle として使える。

線形表現は、このヘッダの `linear_representation<F>(...)` メンバから取得できる。引数・条件は [線形表現](linear_representation.md) を参照。

## 使用例

```cpp
UniformMatroid m(5, 2);
m.build({0, 3});
auto c = m.circuit(4); // {0, 3}: どちらかを除いて 4 を追加できる
m.build({0});
assert(!m.circuit(4)); // {0, 4} は独立
```

## 詳細なドキュメント

### UniformMatroid

#### コンストラクタ

```cpp
UniformMatroid(int n, int rank);
```

台集合の要素数を `n`、独立集合の要素数の上限を `rank` として構築する。初期状態の独立集合は空集合。

##### 制約

- `n >= 0`、`rank >= 0`。

##### 計算量

- $O(n)$。

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

現在の独立集合 $I$ を置き換える。

##### 制約

- 各要素は台集合に属し、互いに異なる。
- $\lvert I\rvert\le r$。

##### 計算量

- $O(n)$。

#### circuit

```cpp
optional<vc<int>> circuit(int e) const;
```

$I+e$ が独立なら `nullopt`、従属なら fundamental circuit から `e` を除いた要素列を返す。返された要素のいずれかを除くと `e` を追加できる。`rank == 0` では値ありの空配列を返す。

##### 制約

- `e` は台集合に属し、$I$ の外にある。

##### 計算量

- $O(\lvert I\rvert)$。
