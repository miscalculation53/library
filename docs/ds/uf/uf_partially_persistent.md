## 概要

部分永続とは、「ある時点での状態を取得する」ができるということ。

（全永続はこれよりも強く、「ある時点に変更を加えたものを今の状態とする」ができる。大変（多分）。）

本ライブラリでは、辺が $t$ 本のときの状態を時刻 $t$ と呼ぶことにして管理する（特に、初期状態は時刻 $0$）。また、時刻 $0$ より小さい時刻は時刻 $0$ とみなされ、現在時刻より大きい時刻は現在時刻とみなされる。

なお、経路圧縮はできないので $\log$ がつくことに注意。

## 詳細なドキュメント

#### コンストラクタ

```cpp
UnionFindPartiallyPersistent<class UFData>(int n)
```

頂点数 $n$ で初期化する。`UFData` は通常の Unionfind と同じ仕様。

ただし、**`GData` は (辺の追加回数) 倍のメモリを消費する**ことに注意。たとえば $O(n)$ のメモリを `GData` に持たせてはいけない。

#### leader

```cpp
int leader(int t, int x)
```

時刻 $t$ において、頂点 $x$ が属する連結成分の代表元を返す。

##### 制約

- $0 \leq x \lt n$

##### 計算量

- $O(\log n)$

#### same

```cpp
bool same(int t, int x, int y)
```

時刻 $t$ において、頂点 $x$ と頂点 $y$ が同じ連結成分に属するかどうかを返す。

##### 制約

- $0 \leq x,y \lt n$

##### 計算量

- $O(\log n)$

#### get_vdata

```cpp
UFData::VData get_vdata(int t, int x)
```

時刻 $t$ において、頂点 $x$ が属する連結成分の `UFData::VData` を取得する。

**`UnionFind` と違って、size が存在しない仕様になっている**。そのため、「（時刻 $t$ における）連結成分のサイズ」が欲しい場合、`VData` にサイズの情報を入れること。

##### 制約

- $0 \leq x \lt n$

##### 計算量

- $O(\log n + \log(現在時刻))$

#### get_gdata

```cpp
UFData::GData get_gdata(int t)
```

時刻 $t$ における `UFData::GData` を取得する。

##### 制約

- $0 \leq t \leq (現在時刻)$

##### 計算量

- $O(1)$

#### merge

```cpp
pair<V=ll, Time=ll> merge(int x, int y, UFData::EWeight w = 1)
```

頂点 $x$ と頂点 $y$ の間に重み $w$ の辺を追加し、(この連結成分の新たな代表元, 追加後の時刻) を返す。

`EWeight` が整数型などの場合はデフォルトで重みが $1$ になるが、そうでない場合は重みを指定しなければならない。

#### group_ids

```cpp
vc<I=ll> group_ids()
```

長さ $n$ の vector を返す。$i$ 番目の要素には頂点 $i$ が属する連結成分の番号が格納される。番号の順番は未定義。

ACL の `groups()` 相当のものが欲しい場合、これに `group_index` を噛ませること。

##### 計算量

- $O(n \log n)$
