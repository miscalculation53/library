## 概要

木のパスに対する静的・オフラインなクエリを Mo's algorithm で処理する。頂点の追加・削除で集約状態を更新し、値の種類数や頻度など、集合から決まる答えを求められる。

[RootedTree](rooted_tree.md) を受け取り、行きがけ・帰りがけに各頂点を記録したオイラーツアー上で [Mo's algorithm](../../ds/mo/mo.md) を実行する。区間内に奇数回現れる頂点を管理し、必要に応じて LCA を一時的に追加する。

## 使用例

```cpp
#include "graph/tree/mo_on_tree.hpp"

// 各パス上の色の種類数を求める。
vc<pair<int, int>> es = {{0, 1}, {1, 2}, {1, 3}};
RootedTree g(4, es, 0);
vc<int> color = {0, 1, 0, 2};
vc<pair<int, int>> uvs = {{0, 2}, {2, 3}, {1, 1}};
vc<int> cnt(3), ans(uvs.size());
int distinct = 0;
auto add = [&](int v) { distinct += (cnt[color[v]]++ == 0); };
auto del = [&](int v) { distinct -= (--cnt[color[v]] == 0); };
auto rem = [&](int qid) { ans[qid] = distinct; };
mo_on_tree(g, uvs, add, del, rem);
// ans = {2, 3, 1}
```

## 詳細なドキュメント

#### mo_on_tree

```cpp
void mo_on_tree(
  const RootedTree& g, const vc<pair<I, I>>& uvs,
  const Add& add, const Del& del, const Rem& rem,
  bool edge = false)
```

`uvs[qid] = {u, v}` で指定したパスのクエリを処理する。`rem` には入力順のクエリ番号を渡す。

- `edge == false`：両端を含むパス上の頂点を集約する。
- `edge == true`：パス上の辺を集約する。辺 `(g.parent(v), v)` を子側の頂点 `v` で表し、`add`, `del` には根以外を渡す。辺の値は `g.edge_to_vertex_values` で頂点番号順に変換できる。
- `add(v)`：現在の集合に含まれていない頂点 `v` を追加する。
- `del(v)`：現在の集合に含まれる頂点 `v` を削除する。
- `rem(qid)`：対象のパスが集まった状態で答えを記録する。集約状態を保つ。

追加・削除の途中では任意の頂点集合を扱い、答えは集合の内容だけから求める。パス上の順序に依存する集約には対応しない。

`u == v` のとき、頂点属性では `{u}`、辺属性では空集合を扱う。クエリが空なら、各コールバックの呼び出しも $0$ 回となる。

呼び出し前に集約状態を空にする。終了時の状態は内部の最後の区間に対応するため、再実行する際は集約状態を初期化する。

##### 制約

- `g` は構築済みの `RootedTree`
- 各クエリについて $0 \leq u,v < g.\mathrm{size}()$
- 各コールバックの実行中は `g` と `uvs` を保つ
- `add`, `del` は任意の頂点の追加・削除に対応し、各頂点の有無と集約状態を一致させる

##### 計算量

頂点数を $n$、クエリ数を $q$ とする。$q \geq 1$ のとき、

- 前処理・クエリの並べ替え：$O(n + q\log n + q)$
- `add`, `del` の呼び出し回数：$O(n\sqrt{q} + q)$
- `rem` の呼び出し回数：$q$

各コールバックが $O(1)$ なら、全体で $O(n + q\log n + q + n\sqrt{q})$。$q = 0$ のときは $O(1)$。
