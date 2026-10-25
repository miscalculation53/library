このドキュメントには AI による作成・変更が含まれています。

## 概要

時系列上の追加・削除を、追加・undo で処理する。undo 可能 UnionFind による動的連結性や、連結成分の集約値などに使える。

`OfflineDynamicConnectivity<Obj>` に `add(t, obj)`・`erase_if(t, obj)` を時刻の昇順に登録し、`run(add, undo, query)` で実行する。同じ対象の重複追加、初期状態の対象、最後まで残る対象を扱える。

追加・削除操作は配列に蓄積し、`spans()`・`run()` で対象ごとに並べ替えて生存期間を生成する。対象の比較には `<` を使う。

生存区間を直接登録する場合は、比較演算を持たない `Obj` も使える。`add_span` と区間配列のコンストラクタだけを使うと、`run` は保持した区間配列を直接処理する。

追加する順序を変えても、取得する答えが同じになることが必要。無向辺は `(min(u, v), max(u, v))` のように表現を統一する。

## 使用例

クエリ `0 u v` で辺を追加、`1 u v` で辺を 1 本削除、`2 u v` で連結性を取得する。

```cpp
#include "ds/offline_dynamic_connectivity.hpp"
#include "ds/uf/uf_undo.hpp"

int n, q;
cin >> n >> q;
OfflineDynamicConnectivity<pair<int, int>> dc(q);
vc<pair<int, int>> queries(q, {-1, -1});
repi(t, q)
{
  int type, u, v;
  cin >> type >> u >> v;
  if (u > v) swap(u, v);
  if (type == 0) dc.add(t, {u, v});
  else if (type == 1) dc.erase_if(t, {u, v});
  else queries[t] = {u, v};
}

UnionFindUndo<UFDataEmpty<>> uf(n);
dc.run(
    [&](auto edge) { uf.merge(edge.first, edge.second); },
    [&] { uf.undo(); },
    [&](int t)
    {
      auto [u, v] = queries[t];
      if (u >= 0) cout << uf.same(u, v) << '\n';
    });
```

初期辺は入力クエリを登録する前に `dc.add(0, edge)` で追加する。頂点への永続的な加算も、対応する `Obj` を `add(t, obj)` で登録できる。

生存区間の配列が既にある場合は、移動して渡せる。

```cpp
vc<tuple<int, int, Obj>> span_obj = /* 生存区間 */;
OfflineDynamicConnectivity<Obj> dc(tmax, std::move(span_obj));
dc.run(add, undo, query);
```

## 詳細なドキュメント

### OfflineDynamicConnectivity

#### コンストラクタ

```cpp
(1) OfflineDynamicConnectivity<Obj>(int tmax)
(2) template <class I = int>
    OfflineDynamicConnectivity<Obj>(int tmax, vc<tuple<I, I, Obj>> span_obj)
```

時刻の範囲を $[0, t_{\max})$ として初期化する。(2) は生存期間 $[l, r)$ の対象 `obj` を `(l, r, obj)` として受け取る。

配列は値渡しで保持する。`I = int` の配列を `std::move` して渡すと、対象をコピーせず配列ごと受け取る。別の整数型の時刻は `int` に変換する。

初期化後も `add`・`erase_if`・`add_span` で登録を追加できる。

##### 制約

- $t_{\max} \geq 0$
- 各区間は $0 \leq l \leq r \leq t_{\max}$
- `I` は整数型
- `Obj` はコピー構築可能

##### 計算量

区間数を $s$ として

- (1): $O(1)$
- (2): $O(s)$

#### add / erase_if

```cpp
void add(int t, const Obj &obj)
void erase_if(int t, const Obj &obj)
```

`add` は時刻 $t$ から `obj` を 1 個追加する操作、`erase_if` は時刻 $t$ から存在する `obj` を 1 個削除する操作を登録する。存在判定は `spans()`・`run()` による変換時に行う。

追加時刻 $l$、削除時刻 $r$ の対象は $[l, r)$ に存在する。最後まで残る対象の右端は $t_{\max}$ とする。

同じ `obj` を複数個追加できる。削除は直近に追加された 1 個と対応づける。同じ時刻の操作は登録順に適用し、その時刻のコールバックには操作をすべて適用した状態を渡す。

辺の更新では、周辺の辺の削除を登録してから、更新後に存在する辺を追加できる。

```cpp
odc.erase_if(t, edge);
if (new_edge_exists) odc.add(t, edge);
```

##### 制約

- $0 \leq t \leq t_{\max}$
- `add`・`erase_if` 全体で時刻は広義単調増加
- `Obj` は `<` による比較が可能で、その比較は狭義弱順序をなす
- 比較上同じ `Obj` は、追加処理において同じ対象を表す

##### 計算量

- 償却 $O(1)$

#### add_span

```cpp
void add_span(int l, int r, const Obj &obj)
```

生存期間 $[l, r)$ の `obj` を 1 個登録する。空区間は読み飛ばす。

区間は任意の順序で登録できる。`add`・`erase_if` で管理する対象とは独立に扱う。

##### 制約

- $0 \leq l \leq r \leq t_{\max}$

##### 計算量

- 償却 $O(1)$

#### spans

```cpp
vc<tuple<int, int, Obj>> spans() const
```

登録内容を `(l, r, obj)` の配列として返す。残っている対象の右端は $t_{\max}$ とする。空区間を除き、各コピーについて 1 区間を返す。区間の順序は任意。

登録内容を保持するため、取得後も追加・削除を登録できる。

##### 計算量

`add`・`erase_if` の登録回数を $k$、コンストラクタ・`add_span` で登録した区間数を $s$ として

- $O(s + k\log k)$

#### run

```cpp
void run(auto add, auto undo, auto query) const
```

登録した対象を追加・undo で処理する。`add`・`erase_if` を登録した場合は、先に生存期間を生成する。生存区間だけを登録した場合は、保持した区間配列を直接処理する。

- `add(Obj obj)`：対象を 1 個反映する
- `undo()`：直前の反映を 1 回分戻す
- `query(int t)`：時刻 $t$ の答えを取得する

`query(t)` を $t = 0, 1, \dots, t_{\max}-1$ の順に呼ぶ。各時刻では、その時刻に存在する対象が各コピーについて 1 回ずつ追加された状態になる。終了時には追加処理をすべて undo して呼び出し前の状態に戻す。登録内容を保持し、繰り返し実行できる。

時間軸は内部で 2 の累乗に切り上げる。右端が $t_{\max}$ の区間は、その余白まで延ばして登録する。全期間有効な対象は根に 1 回だけ登録する。

##### 制約

- 追加する順序を変えても、取得する答えが同じになる
- 各 `add` は `undo` で 1 回分戻せるように履歴を保存する
- `query` による状態の変更は、コールバック内で元に戻す

##### 計算量

`add`・`erase_if` の登録回数を $k$、コンストラクタ・`add_span` で登録した区間数を $s$、生成した生存区間数を $m$ として、コールバックの処理を除き

- $O(s + k\log k + t_{\max} + m\log t_{\max})$
- `add`・`undo` をそれぞれ $O(m\log t_{\max})$ 回、`query` を $t_{\max}$ 回呼ぶ

#### add_del_to_span

```cpp
vc<tuple<I, I, Obj>> add_del_to_span(
    int tmax, const vc<pair<Time, Obj>> &time_obj)
```

初期状態を空として、各 `(time, obj)` を「存在しなければ追加、存在すれば削除」と解釈し、生存期間に変換する。時刻の昇順に並べ替えて処理する。返す時刻の型 `I` は既定で `ll`。

追加・削除が明示される入力や、同じ対象を複数個扱う入力には `OfflineDynamicConnectivity` を使う。

##### 制約

- $t_{\max} \geq 0$
- $0 \leq \mathrm{time} \leq t_{\max}$
- `Obj` は比較可能
- `Time` は整数型
- `I` は $[0, t_{\max}]$ の時刻を表現できる整数型

##### 計算量

$m = \lvert\mathrm{time\_obj}\rvert$ として

- $O(m\log m)$
