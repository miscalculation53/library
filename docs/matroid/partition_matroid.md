このドキュメントには AI による作成・変更が含まれています。

## 概要

台集合をグループに分け、各グループから選ぶ個数に上限を設ける分割マトロイド。台集合は `group` の添字。

色ごとの採用数制限や、二部マッチングの各側の次数制限に使える。マトロイド交叉用の fundamental circuit を求める oracle を備える。

選択した要素を [CSR](../ds/csr.md) の各グループの行にまとめて保持する。入力のグループ番号が直接行番号になるため、値の圧縮や出現位置の検索を備える `GroupIndex` よりも CSR を直接使う形にしている。行の領域は構築時に確保し、`build` では選択要素の部分だけを書き換える。

線形表現は、このヘッダの `linear_representation()` メンバから取得できる。点の列は自動生成でき、明示的な指定も使える。既定の係数体は `modint998244353`。引数・条件は [線形表現](linear_representation.md) を参照。

`build` の要素番号の配列には、`i128`・`u128` を含む任意の整数型を使える。返す要素番号は `int`。

## 使用例

```cpp
vc<ll> group{0, 0, 1, 1};
vc<unsigned> capacity{1, 2};
PartitionMatroid m(group, capacity);
m.build({0, 2});
auto c = m.circuit(1); // {0}
assert(!m.circuit(3)); // グループ 1 には追加できる
```

## 詳細なドキュメント

### PartitionMatroid

#### コンストラクタ

```cpp
template<class G = int, class C = int>
PartitionMatroid(const vc<G>& group, const vc<C>& capacity);
```

`G`・`C` はそれぞれ独立に選べる整数型。容量は台集合の要素数を超えてもよい。

要素 `e` はグループ `group[e]` に属する。グループ `g` からは `capacity[g]` 個まで選べる。初期状態の独立集合は空集合。以下、台集合の要素数を $n$、グループ数を $k$ とする。

##### 制約

- `0 <= group[e] < capacity.size()`。
- `capacity[g] >= 0`。

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
template<class I = int>
void build(const vc<I>& independent);
```

現在の独立集合 $I$ を置き換える。

##### 制約

- 各要素は台集合に属し、互いに異なる。
- 各グループの選択数は容量以下。

##### 計算量

- $O(n+k)$。

#### circuit

```cpp
optional<vc<int>> circuit(int e) const;
```

`e` のグループに余裕があれば `nullopt` を返す。上限に達していれば、そのグループで選択済みの要素を返す。この列は fundamental circuit から `e` を除いたもの。容量が $0$ なら値ありの空配列を返す。

##### 制約

- `e` は台集合に属し、$I$ の外にある。

##### 計算量

- $O(\lvert C\rvert)$。$C$ は返す要素列。

#### sparse_linear_representation

`linear_representation` と同じ引数・係数体で、行を座標、列を要素とする `SparseMatrix<F>` を返す。構築の条件と計算量は [線形表現](linear_representation.md) を参照する。
