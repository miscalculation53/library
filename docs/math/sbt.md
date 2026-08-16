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

- (1)：$\dfrac{\mathrm{num}}{\mathrm{den}}$ の頂点を作る。
- (2)：$\dfrac{p+r}{q+s}$ の頂点を作る。
- (3)：`path` のパスをたどって頂点を作る。`Path` は `vc<pair<char, T>>` で、方向（`L`, `R`）と進む個数の組の列。

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

現在の頂点の深さを返す。

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

##### 制約

- `judge` は単調性を持つ
- $\mathrm{max\_value} \geq 1$

##### 計算量

- $O(\log \mathrm{max\_value})$ 回の `judge` の呼び出し
