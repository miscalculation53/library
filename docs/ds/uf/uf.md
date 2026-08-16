## 概要

UnionFind

## 詳細なドキュメント

#### コンストラクタ

```cpp
UnionFind<class UFData, bool compress = true>(int n)
```

頂点数 $n$ で初期化する。`compress` は経路圧縮を行うかどうか。

#### UFData について

`UFData` の仕様が少し複雑だが、特にカスタマイズしないのであれば

- ACL 相当の機能で十分なら `UFDataEmpty<EWeight_ = ll>` を渡す
- いろいろな機能を使いたいなら `UFDataEverything<EWeight_ = ll, need_vlist = false>` を渡す
  - `need_vlist` は頂点集合の vector が欲しいかどうか（これをやると $\log$ がつく）

で使える。

`UFDataEverything` が持っているものは

- `VData`
  - `vsum`：連結成分の頂点数（`VData` のコンストラクタを変更することで、連結成分の頂点の重みの和にできる）
  - `esum`：連結成分の辺重みの和（辺重みはデフォルトで $1$ なので、その場合辺数）
  - `vlist`：連結成分に属する頂点番号の vector（`need_vlist` が `true` のときのみ）
- `GData`
  - `cmp_cnt`：連結成分数
  - `min_leader`, `max_leader`：代表元になっている頂点番号の最小値・最大値
    - これは「頂点 $v$ と連結でない頂点を $1$ つ求める」をしたいときに便利（この $2$ つを調べればよい） https://yukicoder.me/problems/no/2290

#### UFData の詳細な仕様

`UFData` をカスタマイズするときは `UFDataEverything` を参考に編集するとよい。

ちゃんと書いておくと、実装するものは

- `VData`：連結成分の代表元となる頂点が管理するデータ。次のコンストラクタを実装する：
  - 引数が空のコンストラクタ
  - 引数が頂点番号 $i$ のコンストラクタ
- `GData`：グラフ全体が管理するデータ。次のコンストラクタを実装する：
  - 引数が空のコンストラクタ
  - 引数が頂点数 $n$ のコンストラクタ
- `EWeight`：辺重みの型
- `static void add_edge_diff(UF &uf, int x, int y, EWeight w)`
  - $x$ が代表元の連結成分と $y$ が代表元の連結成分に重み $w$ の辺を追加するのに伴って、`uf` の `vdat` および `gdat` を書き換える関数
    - 新たな代表元は $x$ になる
      - 計算量が $O(\mathrm{size}(y))$ で済むならマージテクの計算量になる
    - `uf` の `vdat`, `gdat` **以外**は**マージされた後の状態**であることに注意
- `static void add_edge_same(UF &uf, int x, EWeight w)`
  - $x$ が代表元の連結成分の内部で重み $w$ の辺を追加するのに伴って、`uf` の `vdat` および `gdat` を書き換える関数

#### メンバ変数

public なのは

- `UFData::GData gdat`

#### leader

```cpp
int leader(int x)
```

頂点 $x$ が属する連結成分の代表元を返す。

##### 制約

- $0 \leq x \lt n$

##### 計算量

- `compress` が `true` の場合、ならし $O(\alpha(n))$
- `compress` が `false` の場合、$O(\log n)$

#### size

```cpp
I=ll size(int x)
```

頂点 $x$ が属する連結成分の頂点数を返す。

##### 制約

- $0 \leq x \lt n$

##### 計算量

- `leader` と同じ

#### same

```cpp
bool same(int x, int y)
```

頂点 $x$ と頂点 $y$ が同じ連結成分に属するかどうかを返す。

##### 制約

- $0 \leq x,y \lt n$

##### 計算量

- `leader` と同じ

##### 計算量

- `leader` と同じ

#### get_vdata

```cpp
UFData::VData get_vdata(int x)
```

頂点 $x$ が属する連結成分の `UFData::VData` を取得する。

##### 制約

- $0 \leq x \lt n$

##### 計算量

- `leader` と同じ

#### merge

```cpp
I=ll merge(int x, int y, UFData::EWeight w = 1)
```

頂点 $x$ と頂点 $y$ の間に重み $w$ の辺を追加し、この連結成分の新たな代表元を返す。

`EWeight` が整数型などの場合はデフォルトで重みが $1$ になるが、そうでない場合は重みを指定しなければならない。

##### 制約

- $0 \leq x,y \lt n$

#### group_ids

```cpp
vc<I=ll> group_ids()
```

長さ $n$ の vector を返す。$i$ 番目の要素には頂点 $i$ が属する連結成分の番号が格納される。番号の順番は未定義。

ACL の `groups()` 相当のものが欲しい場合、これに `group_index` を噛ませること。

##### 計算量

- `compress` が `true` の場合、$O(n)$
- `compress` が `false` の場合、$O(n \log n)$
