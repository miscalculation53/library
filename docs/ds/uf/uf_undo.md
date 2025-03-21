## undo 可能 Unionfind

「直前の辺追加を undo する」ができる。また snapshot/rollback もできる。

経路圧縮ができないので $\log$ がつくことに注意。

### コンストラクタ

```cpp
UnionFindPartiallyPersistent<class UFData>(int n)
```

頂点数 $n$ で初期化する。`UFData` は通常の Unionfind と同じ仕様。

ただし、**`GData` は (辺の追加回数) 倍のメモリを消費する**ことに注意。たとえば $O(n)$ のメモリを `GData` に持たせてはいけない。

### メンバ変数

public なのは

- `UFData::GData gdat`

### メンバ関数

leader, size, same, get_vdata, merge, group_ids は Unionfind と同じ（経路圧縮できないので計算量 $O(\log n)$）

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

- $O(1)$