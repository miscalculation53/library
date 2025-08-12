## 根つき木クラス

### 思想（抜粋）

この思想は SSRS さんに直接聞いたことなども参考にしている。

- グラフクラスとは別で作る。
- グラフクラス同様、すべての辺を受け取ってから直接構築し、**`add_edge` は使わない**。
- 同じ木でも、根が異なれば別のインスタンスとして扱う。
- 構築すると同時に、多くの場面で必要になる諸々を計算する。具体的には次の $6$ つ：
  - 親
  - 子配列
  - BFS 順
  - 深さ
  - 根からの距離（`Cost` ベース）
  - 部分木サイズ

### 使用例

#### 木 DP

```cpp
fec(v : reversed(bfs_ordered_vertices()))
{
  // dp[v] を計算する
  fec(c : children(v))
  {
    // dp[c] を使って dp[v] を更新する
  }
}
```

----

### コンストラクタ

```cpp
(1) RootedTree<class Cost, bool need_dist = true>(vc<I> p, Cost dflt_cost = 1)
(2) RootedTree<class Cost, bool need_dist = true>(Graph<is_directed, Cost> g, int root)
(3) RootedTree<class Cost, bool need_dist = true>(int n, vc<pair<I, I>> es, int root)
(4) RootedTree<class Cost, bool need_dist = true>(int n, vc<tuple<I, I, Cost>> es, int root)
```

- (1)：親配列 `p` から構築する。根については `p[root] = -1` として指定する。
- (2)：グラフ `g` と根 `root` から構築する。`g` が木（有向グラフのときは、`root` から出る有向木）であることを仮定。
- (3), (4)：グラフを構築できる `n`, `es` と根 `root` から構築する。仮定については (2) と同様（自動的に無向グラフとして解釈する）。

構築と同時に、基本的な情報を前計算する。

##### 計算量

- $O(n)$
  - 定数倍はやや大きい

### メンバ関数

#### size

```cpp
I size<I = ll>()
```

頂点数 $n$ を返す。

##### 計算量

- $O(1)$

#### root

```cpp
int root()
```

根の頂点番号を返す。

##### 計算量

- $O(1)$

#### parent

```cpp
Edge<Cost> parent(int v)
```

頂点 $v$ からその親への辺を返す。$v$ が根のときは `Edge<Cost>(v, -1, dflt_cost)` を返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(1)$

#### children

```cpp
CSR<Edge<Cost>> children(int v)
```

頂点 $v$ から各子に出る辺の vector を返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- （この関数を呼ぶだけなら）$O(1)$

#### depth

```cpp
I depth<I = ll>(int v)
```

頂点 $v$ の深さを返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(1)$


#### dist

```cpp
Cost dist(int v)
```

根から頂点 $v$ までの距離を返す。

##### 制約

- $0 \leq v < n$
- `need_dist` が `true`

##### 計算量

- $O(1)$

#### subtree_size

```cpp
I subtree_size<I = ll>(int v)
```

頂点 $v$ を根とする部分木のサイズ（$v$ も含んだ頂点数）を返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(1)$

#### cut_and_subtree_size

```cpp
pair<I, I> cut_and_subtree_size<I = ll>(int u, int v)
```

頂点 $u$ と $v$ の間の辺を切断したときの、$u$ 側、$v$ 側の部分木のサイズを返す。

これは $\mathrm{dep}(u) < \mathrm{dep}(v)$ のとき $(n - \mathrm{siz}(v), \mathrm{siz}(v))$ である。

##### 制約

- $0 \leq u, v < n$
- $u$ と $v$ の間に辺が存在する

##### 計算量

- $O(1)$


#### get_edge

```cpp
Edge<Cost> get_edge(int eid)
```

辺番号が $\mathrm{eid}$ の辺を返す。辺は親から子の向き。

##### 制約

- $0 \leq \mathrm{eid} < n - 1$

##### 計算量

- $O(1)$


#### edges

```cpp
vc<Edge<Cost>> edges()
```

すべての辺を格納した vector を返す。辺は親から子の向き。

##### 計算量

- $O(n)$

#### path_to_root

```cpp
vc<Edge<Cost>> path_to_root(int v)
```

頂点 $v$ から根までのパスを辺の vector として返す。

##### 制約

- $0 \leq v < n$

##### 計算量

- $O(\mathrm{dep}(v))$


#### path

```cpp
vc<Edge<Cost>> path(int u, int v)
```

頂点 $u$ から $v$ までのパスを辺の vector として返す。

##### 制約

- $0 \leq u, v < n$

##### 計算量

- $O(\mathrm{dep}(u) + \mathrm{dep}(v))$


#### bfs_ordered_vertices

```cpp
vc<I> bfs_ordered_vertices<I = ll>()
```

BFS 順に頂点番号を格納した vector を返す（長さ $n$）。

##### 計算量

- $O(n)$


#### bfs_ordered_edges

```cpp
vc<Edge<Cost>> bfs_ordered_edges()
```

BFS 順に辺を格納した vector を返す（長さ $n-1$）。辺は親から子の向き。

##### 計算量

- $O(n)$


#### dfs_ordered_edges

```cpp
vc<Edge<Cost>> dfs_ordered_edges()
```

帰るのも含めた DFS 順に辺を格納した vector を返す（長さ $2(n-1)$）。

##### 計算量

- $O(n)$


#### edges_vt

```cpp
vc<tuple<I, I, Cost>> edges_vt()
```

すべての辺を $(\mathrm{from}, \mathrm{to}, \mathrm{cost})$ の tuple として格納した vector を返す。辺は親から子の向き。

用途：グラフや根つき木のコンストラクタに渡す。

##### 計算量

- $O(n)$


#### rerooted_tree

```cpp
RootedTree<Cost, need_dist> rerooted_tree(int new_root)
```

新たに根を付け替えた根つき木を返す。

##### 制約

- $0 \leq \mathrm{new\_root} < n$

##### 計算量

- $O(n)$


#### to_graph

```cpp
Graph<is_directed, Cost> to_graph<bool is_directed>()
```

根つき木をグラフに変換して返す。

##### 計算量

- $O(n)$