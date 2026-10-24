このドキュメントには AI による作成・変更が含まれています。

## 概要

二値変数 $x_i\in\lbrace0,1\rbrace$ の費用を、最小カットへの帰着で最小化する。単変数費用、劣モジュラな二変数費用、依存関係、固定値、全変数が指定値のときの利益を追加できる。PSP（Project Selection Problem）として使える。

既存の [MaxFlow](maxflow.md) を使う。**1 は $s$ 側・選択、0 は $t$ 側・非選択**。利益は負の費用として加える。

型 `Cost` は明示指定する。係数と最適費用がその型に収まることを前提とする。容量・定数の途中の総和は同じ幅の符号なし演算と容量の分割で扱う。依存関係の容量は「有限容量の総和 $+1$」として内部で設定する。

二変数費用の最小カットへの帰着については [Kolmogorov–Zabih, §4](https://www.cs.cornell.edu/~rdz/Papers/KZ-PAMI04.pdf) を参照。

## 使用例

```cpp
#include "flow/project_selection.hpp"

ProjectSelection<ll> p(2);
p.add_profit(0, 0, 10); // 事業0を選ぶと利益10
p.add_cost(1, 0, 4);    // 設備1を選ぶと費用4
p.imply(0, 1);          // 事業0を選ぶなら設備1も選ぶ
auto cost = p.solve();
assert(cost && *cost == -6); // 最大利益は6
assert((p.assignment() == vc<bool>{true, true}));
```

## 詳細なドキュメント

### ProjectSelection

#### コンストラクタ

```cpp
template <class Cost> class ProjectSelection;
ProjectSelection();
explicit ProjectSelection(int n);
template <class I = ll> I size() const;
```

$n$ 個の変数を用意する。既定コンストラクタは $n=0$。`size()` は元の変数の数を返す。

##### 制約

- $n\ge0$。
- `Cost` は符号付き整数型。`i128` に対応する。
- 追加する係数と、最適費用が `Cost` に収まる。

##### 計算量

- $O(1)$。

#### 定数・単変数費用・利益

```cpp
void add_constant(Cost cost);
void add_cost(int v, Cost cost0, Cost cost1);
void add_profit(int v, Cost profit0, Cost profit1);
```

それぞれ定数、$\mathrm{cost}_{x_v}$、$-\mathrm{profit}_{x_v}$ を費用へ足す。係数の符号は自由。

##### 制約

- $0\le v<n$。

##### 計算量

- 償却 $O(1)$。

#### 二変数費用

```cpp
void add_penalty(int from, int to, Cost cost);
void add_disagreement_cost(int u, int v, Cost cost);
void add_pairwise_cost(int u, int v, Cost c00, Cost c01, Cost c10, Cost c11);
```

- `add_penalty`：$x_{\mathrm{from}}=1$ かつ $x_{\mathrm{to}}=0$ のときだけ `cost` を足す。
- `add_disagreement_cost`：$x_u\ne x_v$ のときだけ `cost` を足す。
- `add_pairwise_cost`：$(x_u,x_v)=(a,b)$ のとき `cab` を足す。

`u == v` では `c00`, `c11` の単変数費用として扱う。

##### 制約

- 頂点番号が $[0,n)$ にある。
- `add_penalty`・`add_disagreement_cost` は `cost >= 0`。
- `add_pairwise_cost` は $u\ne v$ のとき **$c_{00}+c_{11}\le c_{01}+c_{10}$**。

##### 計算量

- 償却 $O(1)$。

#### 依存関係・固定値

```cpp
void imply(int from, int to);
void fix(int v, bool value);
```

それぞれ $x_{\mathrm{from}}=1\Rightarrow x_{\mathrm{to}}=1$、$x_v=\mathrm{value}$ を必須条件として加える。矛盾は `solve()` で検出する。

##### 制約

- 頂点番号が $[0,n)$ にある。

##### 計算量

- 償却 $O(1)$。

#### 全変数が同じ値のときの利益

```cpp
void add_all_profit(const vector<int>& vertices, bool value, Cost profit);
```

指定した全変数が `value` のときだけ `profit` の利益を足す。補助変数を1個追加する。空配列では条件を常に満たすため、定数の利益となる。頂点番号の重複も許す。

##### 制約

- `profit >= 0`。
- `vertices` の各要素が $[0,n)$ にある。

##### 計算量

- $O(\lvert\mathrm{vertices}\rvert)$。

#### 求解・復元

```cpp
optional<Cost> solve();
vector<bool> assignment() const;
```

必須条件を満たす最小費用を返す。矛盾する場合は `nullopt`。費用 $0$ の解は値ありの `0` として返す。`assignment()` は元の $n$ 変数の最適値。**true は $s$ 側、false は $t$ 側**。

`solve()` は現在までに追加した項を毎回解き直す。成功後に項を追加した場合は、再度 `solve()` してから復元する。

##### 制約

- `assignment()` は直前の `solve()` が成功している。

##### 計算量

- 元・補助変数の総数を $N-2$、生成された有限辺数を $A$、必須条件数を $H$ とする。有限容量の総和を $B$、同じ幅の符号なし整数の最大値を $U$ とすると、生成辺数は $M=O(A+H(1+\lfloor(B+1)/U\rfloor))$。
- `solve()` は $O(N+M+qT(N,M))$。$T$ は既存 `MaxFlow::flow` の計算量、$q$ は流量を $U$ 以下へ分けた呼出し回数。通常の容量範囲では $q=1$。
- `assignment()` は $O(n)$。

有限容量の総和 $+1$ が同じ幅に収まる場合、必須条件1個は1本の辺になる。型を十分な幅にすると補助辺と求解の分割数も抑えられる。作業領域は $O(N+M)$。
