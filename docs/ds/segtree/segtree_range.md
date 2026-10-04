## 概要

セグ木のノード番号・区間・点の対応と、区間分解・祖先の列挙を扱う。

`SegmentTreeRange seg(n)` で列の長さを指定する。葉の数を $s = \operatorname{bit\_ceil}(n)$ とし、根は `1`、ノード番号は $[1, 2s)$、点 $p$ の葉は $s+p$。`SegmentTree` と同じ番号付けを使う。$n=0$ のときは $s=1$。

構造体と列挙範囲は定数個の整数を保持する。範囲 for で順次列挙でき、必要に応じて `to_v()` で配列を作れる。

## 使用例

```cpp
SegmentTreeRange seg(5); // 葉の数は 8
ll v = seg.point_to_node(3);   // 11
int p = seg.node_to_point(v);  // 3
auto [l, r] = seg.node_to_range(5); // [2, 4)
ll u = seg.range_to_node(2, 4);     // 5

for (ll i : seg.range_to_nodes_from_left(1, 5)) {
  // 9, 5, 12 の順：[1, 2), [2, 4), [4, 5)
}
for (ll i : seg.range_to_nodes_from_bottom(1, 5)) {
  // 9, 12, 5 の順
}
for (ll i : seg.point_to_nodes_from_bottom(3)) {
  // 11, 5, 2, 1 の順。点更新後の再計算などに使う。
}
for (ll i : seg.point_to_nodes_from_top(3)) {
  // 1, 2, 5, 11 の順。遅延評価の伝播などに使う。
}

auto nodes = seg.range_to_nodes_from_left(1, 5); // 保存して再走査できる
vl ids = nodes.to_v();                 // {9, 5, 12}
```

## 詳細なドキュメント

### SegmentTreeRange

長さと点の添字は `int`、ノード番号とノードの区間端点は `ll` で扱う。

#### コンストラクタ

```cpp
explicit SegmentTreeRange(int n)
```

長さ $n$ の列に対応する構造を作る。

##### 制約

- $n \geq 0$

##### 計算量

- $O(1)$

#### size, leaf_size, node_count

```cpp
int size() const
ll leaf_size() const
ll node_count() const
```

それぞれ元の列の長さ $n$、余白を含む葉の数 $s$、全ノード数 $2s-1$ を返す。ノード番号を添字とする配列には `node_count() + 1` 要素を用意する。

##### 計算量

- $O(1)$

#### point_to_node, node_to_point

```cpp
ll point_to_node(int p) const
int node_to_point(ll i) const
```

点 $p$ と、それに対応する葉のノード番号を相互変換する。

##### 制約

- `point_to_node`：$0 \leq p < n$
- `node_to_point`：$s \leq i < s+n$

##### 計算量

- $O(1)$

#### node_to_range, range_to_node

```cpp
pair<ll, ll> node_to_range(ll i) const
ll range_to_node(ll l, ll r) const
```

ノード番号と、そのノードが表す区間 $[l,r)$ を相互変換する。

区間は余白 $[n,s)$ を含む完全二分木上のもの。例えば $n=5$ の根は $[0,8)$ を表す。実際の列との共通部分は、両端を `min<ll>(端点, seg.size())` で切り詰めると得られる。

##### 制約

- `node_to_range`：$1 \leq i < 2s$
- `range_to_node`：$0 \leq l < r \leq s$、$r-l$ は $2$ の冪、$l$ は $r-l$ の倍数

##### 計算量

- $O(1)$

#### depth, range_length, is_leaf

```cpp
int depth(ll i) const
ll range_length(ll i) const
bool is_leaf(ll i) const
```

それぞれ根からの深さ（根は $0$）、ノードの区間長、葉かどうかを返す。区間長と葉の判定は余白も含めた完全二分木に基づく。

用途：

- `depth`：段ごとに OR と XOR を交互に使う集約など、深さに応じて処理を変える。
- `range_length`：実区間に収まるノードへの一様加算で、区間和を `sum[i] += x * seg.range_length(i)` と更新する。
- `is_leaf`：葉の値を設定する処理と、子の値から再計算する処理を分ける。遅延値の伝播では内部ノードを選ぶ。

##### 制約

- $1 \leq i < 2s$

##### 計算量

- $O(1)$

#### range_to_nodes_from_left, range_to_nodes_from_bottom

```cpp
auto range_to_nodes_from_left(int l, int r) const
auto range_to_nodes_from_bottom(int l, int r) const
```

$[l,r)$ を重複なく覆う最小個数のノードを列挙する範囲を返す。$l=r$ なら空。

- `range_to_nodes_from_left`：区間の左から順に列挙する。非可換な積の集計にも使える。
- `range_to_nodes_from_bottom`：深いノードから順に列挙する。同じ深さでは左から順。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- `range_to_nodes_from_left` の範囲の取得・イテレータの移動：$O(1)$
- `range_to_nodes_from_bottom` の範囲の取得・イテレータの移動：それぞれ最悪 $O(\log(n+1)+1)$
- どちらも範囲の取得から全列挙まで：$O(\log(n+1)+1)$

#### point_to_nodes_from_bottom, point_to_nodes_from_top

```cpp
auto point_to_nodes_from_bottom(int p) const
auto point_to_nodes_from_top(int p) const
```

点 $p$ を含むノードを列挙する範囲を返す。`from_bottom` は葉から根へ、`from_top` は根から葉へ進む。どちらも葉と根を含む。

##### 制約

- $0 \leq p < n$

##### 計算量

- 範囲の取得・イテレータの移動：$O(1)$
- 全列挙：$O(\log(n+1)+1)$

#### ancestors_from_bottom, ancestors_from_top

```cpp
auto ancestors_from_bottom(ll i) const
auto ancestors_from_top(ll i) const
```

ノード $i$ と根を結ぶパス上のノードを列挙する範囲を返す。`from_bottom` は $i$ から根へ、`from_top` は根から $i$ へ進む。どちらも $i$ 自身と根を含む。

内部ノードが表す区間を直接読み書きする場合に使う。例えば $n=8$ では、区間 $[2,4)$ のノードは `5`。その区間和を読む前に `1, 2` の順で遅延値を伝播し、ノード `5` を更新した後は `2, 1` の順で祖先を再計算する。列挙結果はそれぞれ `1, 2, 5` と `5, 2, 1` なので、この用途では `i` 自身を除いて処理する。

##### 制約

- $1 \leq i < 2s$

##### 計算量

- 範囲の取得・イテレータの移動：$O(1)$
- 全列挙：$O(\operatorname{depth}(i)+1)$

#### 列挙範囲の共通操作

```cpp
auto begin() const
auto end() const
bool empty() const
vc<ll> to_v() const
```

各列挙メソッドの返す範囲で使える。`to_v()` は列挙順に並んだノード番号の配列を返す。

範囲とイテレータは必要な状態を値で保持する。元の構造体や範囲を破棄した後も走査できる。再走査・入れ子の走査・`break`・`continue` に対応する。イテレータは C++17 の入力イテレータとして利用できる。

##### 計算量

- `begin`, `end`, `empty`、イテレータの参照・比較・コピー：$O(1)$
- `to_v`：全列挙と同じ
