このドキュメントには AI による作成・変更が含まれています。

## 概要

半開区間の追加・削除と、指定区間に交差する登録区間の検索を行う。
ここで交差とは、2区間の端点が $a<c<b<d$ または $c<a<d<b$ の順に並ぶことをいう。
包含関係、同一区間、端点で接する区間はラミナー性を保つ。空区間も扱える。

更新する区間を構築時にすべて渡し、入力順の ID で追加・削除する。
同一区間を複数渡すと、それぞれ独立した ID として扱う。
任意の端点を指定した検索もできる。
交差相手は ID と両端点を持つ `optional<Interval>` で返す。

`can_insert` で追加可否を判定してから `insert` すると、ラミナーな区間族を維持できる。
`insert` 自体は交差を含む状態も保持するので、追加後に判定する尺取り法にも使える。
各操作は厳密な判定で、前処理 $O(n\log n)$、検索・追加・削除は各 $O(\log n)$。空間計算量は $O(n)$。

左端で整列した区間列に右端位置の最大値を、右端で整列した区間列に左端位置の最小値を持つ。
指定区間 $[a,b)$ の内側に左端を持ち右端が $b$ を超える区間と、内側に右端を持ち左端が $a$ より小さい区間を、2本のセグメント木で検索する。

## 使用例

```cpp
#include "ds/interval_crossing.hpp"

vc<pair<int, int>> intervals{{0, 5}, {1, 3}, {2, 7}, {5, 8}, {1, 3}};
IntervalCrossing ds(intervals);
ds.insert(0);
assert(ds.can_insert(1));  // 包含
ds.insert(1);
if (auto crossing = ds.find_crossing(2))
{
  auto [id, l, r] = *crossing;  // 交差相手の ID と区間 [l, r)
  cout << id << ' ' << l << ' ' << r << '\n';
  // crossing->id, crossing->l, crossing->r でも取得できる。
}
assert(ds.can_insert(3));  // 端点で接する
assert(ds.can_insert(4));  // 同一区間は別 ID で登録できる
ds.erase(1);
assert(!ds.find_crossing(6, 9));
```

[AALC001 P](https://atcoder.jp/contests/aalc001/tasks/aalc001_p) は、既存の `algo/two_pointers.hpp` と組み合わせる。

```cpp
#include "algo/two_pointers.hpp"
#include "ds/interval_crossing.hpp"

IntervalCrossing ds(intervals);
int n = intervals.size();
auto add_r = [&](int, int r) { ds.insert(r); };
auto del_l = [&](int l, int) { ds.erase(l); };
// [l, r-1) はラミナー。最後に追加した区間との交差だけを調べる。
auto judge = [&](int l, int r) { return l == r || ds.can_insert(r - 1); };
auto rok = right_ok<int>(n, add_r, del_l, judge);
// 添字区間 [l, r) の区間族がラミナーである条件は r <= rok[l]。
```

追加前の窓はラミナーで、左端の削除後も右端の区間を除いた族はラミナーである。
このため `judge` では右端の区間との交差だけを調べればよい。
前処理 $O(n\log n)$、各クエリ $O(1)$。

## 詳細なドキュメント

### IntervalCrossing

`T` は端点の型。以降、$n$ は構築時に渡す区間数とする。
端点の比較とコピーを $O(1)$ として計算量を記す。

#### コンストラクタ

```cpp
template <class T = ll>
IntervalCrossing()
explicit IntervalCrossing(const vc<pair<T, T>>& intervals)
```

`intervals[i]` を ID `i` の更新候補とする。初期状態ではすべて未登録。
既定コンストラクタは候補数0として構築する。
端点には比較だけを用い、整数型の最小値・最大値も扱える。

##### 制約

- `T` はコピー・代入ができ、`operator<` が端点に全順序を与える。
- 各区間は $l\leq r$ を満たす。

##### 計算量

- 既定コンストラクタ：$O(1)$
- 区間列からの構築：$O(n\log n)$

#### find_crossing / can_insert

```cpp
struct Interval
{
  int id;
  T l, r;
};

optional<Interval> find_crossing(int id) const
optional<Interval> find_crossing(const T& l, const T& r) const
bool can_insert(int id) const
bool can_insert(const T& l, const T& r) const
```

- `find_crossing`：指定区間と交差する登録区間を1つ返す。返り値は `IntervalCrossing<T>::Interval` の `optional` で、該当する区間がなければ `nullopt`。
- `can_insert`：指定区間と交差する登録区間が0個なら `true`。現在の区間族がラミナーなら、指定区間を加えた族もラミナーであることに相当する。

`Interval` の `id` は登録区間の ID、`l, r` はその区間の両端点。
`result->id`, `result->l`, `result->r` または `auto [id, l, r] = *result` で取得する。
返り値は区間のコピーなので、取得後の追加・削除や元オブジェクトの破棄後も保持できる。

どちらも登録中の ID を指定できる。自身や同一区間との包含関係も通常どおり扱う。
端点を指定する形式は、構築時の候補に含まれる区間と、それ以外の区間をともに検索できる。
空区間に対する結果は `nullopt` / `true`。
交差相手が複数ある場合の返却順序は任意。

##### 制約

- ID 指定：$0\leq id<n$
- 端点指定：$l\leq r$

##### 計算量

- 各 $O(\log n)$

#### insert / erase

```cpp
void insert(int id)
void erase(int id)
```

`insert` は ID `id` の区間を追加し、`erase` は削除する。
削除後の ID は再び追加できる。空区間も登録数に含める。
交差を含む区間族でも操作できる。
ラミナー性を維持する場合は、追加前に `can_insert(id)` を確認する。

##### 制約

- $0\leq id<n$
- `insert`：ID `id` が未登録である。
- `erase`：ID `id` が登録中である。

##### 計算量

- 各 $O(\log n)$

#### contains / size / empty

```cpp
bool contains(int id) const
int size() const
bool empty() const
```

`contains` は ID の登録状態、`size` は登録数、`empty` は登録数が0かを返す。

##### 制約

- `contains`：$0\leq id<n$

##### 計算量

- 各 $O(1)$

#### content

```cpp
vc<int> content() const
```

登録中の ID を昇順にコピーして返す。デバッグ出力用。

##### 計算量

- $O(n)$
