このドキュメントには AI による作成・変更が含まれています。

## 概要

「直前の辺追加を undo する」ができる。また snapshot/rollback もできる。

経路圧縮ができないので $\log$ がつくことに注意。

## 使用例

```cpp
#include "ds/uf/uf_undo.hpp"

UnionFindUndo<UFDataEmpty<>> uf(3);
uf.merge(0, 1);
uf.snapshot();
uf.merge(1, 2);
assert(uf.same(0, 2));
uf.rollback();
assert(uf.same(0, 1) && !uf.same(0, 2));
```

## 詳細なドキュメント

### UnionFindUndo

#### コンストラクタ

```cpp
UnionFindUndo<class UFData>(int n)
```

頂点数 $n$ で初期化する。`UFData` は通常の Unionfind と同じ仕様。

ただし、**`GData` は (辺の追加回数) 倍のメモリを消費する**ことに注意。たとえば $O(n)$ のメモリを `GData` に持たせてはいけない。

#### メンバ変数

public なのは

- `UFData::GData gdat`

leader, size, same, get_vdata, merge, group_ids は Unionfind と同じ（経路圧縮できないので計算量 $O(\log n)$）

`content() const` も通常の [UnionFind](uf.md#content) と同じ形式で使え、$O(n)$ で現在の連結成分ごとの頂点一覧を返す。undo の履歴を維持する。

#### undo

```cpp
void undo(int times = 1)
```

$\mathrm{times}$ 回分の辺追加を undo する。

##### 制約

- すでに $\mathrm{times}$ 回の辺追加が行われている

##### 計算量

- $O(\mathrm{times})$

#### snapshot

```cpp
void snapshot()
```

現在の状態の snapshot を撮る。

##### 計算量

- $O(1)$

#### rollback

```cpp
void rollback()
```

直前に snapshot を撮った状態（なければ初期状態）に戻す。

##### 計算量

- snapshot 以降の辺追加回数を $k$ として $O(k)$
