## 概要

各頂点に正の有理数 $\dfrac{p+r}{q+s}$ が一対一対応する二分探索木。性質：

- $\dfrac{p+r}{q+s}$ の左の子は $\dfrac{p+(r+p)}{q+(s+q)}$、右の子は $\dfrac{(p+r)+r}{(q+s)+s}$
- $\dfrac{p+r}{q+s}$ の子孫として現れる有理数の範囲は $\left(\dfrac{p}{q}, \dfrac{r}{s} \right)$
- パスを RLE したものの長さは $O(\log 値)$

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) SBTNode<T>(T num, T den)
(2) SBTNode<T>(T p, T q, T r, T s)
(3) SBTNode<T>(Path path)
```

- (1)：$\dfrac{\mathrm{num}}{\mathrm{den}}$ の頂点を作る。ユークリッド互除法の商から境界を直接構築する。
- (2)：$\dfrac{p+r}{q+s}$ の頂点を作る。
- (3)：`path` のパスをたどって頂点を作る。`Path` は `vc<pair<char, T>>` で、方向（`L`, `R`）と進む個数の組の列。

##### 制約

- (1)：$\mathrm{num}, \mathrm{den} > 0$
- (2)：`p, q, r, s` は Stern–Brocot 木の頂点の両境界を表す
- (3)：各方向は `L` または `R`、進む個数は $0$ 以上

##### 計算量

- (1)：$O(\log \max(\mathrm{num}, \mathrm{den}))$
- (2)：$O(1)$
- (3)：$O(|\mathrm{path}|)$

#### メンバ変数

```cpp
T p, q, r, s
```

#### num, den

```cpp
(1) T num()
(2) T den()
```

##### 計算量

- $O(1)$

#### descend

```cpp
void descend(char dir, T d)
```

`dir` 方向に $d$ だけ潜る（頂点自体が変更される）。

##### 制約

- `dir` は `L` または `R`
- $d \geq 0$

##### 計算量

- $O(1)$

#### ascend

```cpp
void ascend(T d)
```

親方向に $d$ だけ上がる（頂点自体が変更される）。

##### 制約

- $0 \leq d \leq (頂点の深さ)$

##### 計算量

- $O(\log (p+q+r+s))$

#### depth

```cpp
T depth()
```

現在の頂点の深さを返す。ユークリッド互除法の商の和から求める。

##### 計算量

- $O(\log (p+q+r+s))$

#### encode_path

```cpp
Path encode_path()
```

根から現在の頂点までのパスを `Path` として返す。

##### 計算量

- $O(\log (p+q+r+s))$

#### lca

```cpp
SBTNode<T> lca(SBTNode<T> rhs)
```

現在の頂点と `rhs` の頂点の LCA を求める。

なお、$\dfrac{a}{b}$ と $\dfrac{c}{d}$ の LCA は、$\dfrac{a}{b} \leq \dfrac{p}{q} \leq \dfrac{c}{d}$ を満たす $\dfrac{p}{q}$ のうち分母 $q$ が最小のもの。

##### 計算量

$v$ を現在の頂点と `rhs` の頂点の分子・分母の最大値として

- $O(\log v)$

#### sbt_search

```cpp
SBTNode<T> sbt_search(auto judge, T max_value)
```

`judge(T num, T den)` は $[0/1, 1/0]$ から `bool` への関数で、単調性を持つ、すなわちある実数 $\alpha$ が存在して、$\alpha$ を境に `true` と `false` が切り替わるとする（$\alpha$ で `true` か `false` かは問わない）。このとき、分母・分子がともに $\mathrm{max\_value}$ 以下の有理数のうち、下側・上側それぞれで $\alpha$ に最も近いものを求める。

返り値の `p/q` が下側、`r/s` が上側の境界を表す。それぞれ `judge(0, 1)`、`judge(1, 0)` と同じ判定値を持ち、境界には $0/1$ と $1/0$ も含む。媒介分数 `(p+r)/(q+s)` は分子・分母のいずれかが `max_value` を超えるため、結果は `p, q, r, s` から取得する。

同方向へ進む距離を指数探索と二分探索で求め、向きが変わる際の判定結果を再利用する。次の一段で向きが変わる場合は加算と比較だけで処理する。`judge` に渡す分子・分母はともに $0$ 以上 $\mathrm{max\_value}$ 以下。

##### 制約

- `judge` は単調性を持つ
- `judge(0, 1) != judge(1, 0)`
- $\mathrm{max\_value} \geq 1$

##### 計算量

- $O(\log \mathrm{max\_value})$ 回の `judge` の呼び出し
