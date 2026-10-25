このドキュメントには AI による作成・変更が含まれている可能性があります。

## 概要

clamp・加算による最終位置と、左右の補正距離に重みを付けた費用・利得を、関数合成で扱うモノイド。

区間 $I=[L,R]$ からの距離を $d(x,I)=\max(L-x,0,x-R)$ とする。要素は 4 値 `lo`, `hi`, `bias`, `base` を持ち、次の関数対を表す。

$$
\begin{aligned}
p(x)&=\operatorname{clamp}(x,L,R)+a,\\
c(x)&=b+\alpha\max(L-x,0)+\beta\max(x-R,0).
\end{aligned}
$$

`MonoidClampAddCost<T, chmax_cost, chmin_cost>` で全要素に共通の重み $\alpha,\beta$ を指定する。負の重みも使える。

| `chmax_cost` | `chmin_cost` | 用途 |
| --- | --- | --- |
| `1` | `1` | 通路に沿う移動距離、clamp による両側の補正量 |
| `-1` | `-1` | 移動予定量から飽和で失った量を引き、実際の移動量を集計 |
| `0` | `1` | 上限による減少量だけを費用とする |
| `1` | `0` | 下限による増加量だけを費用とする |
| $\alpha$ | $\beta$ | 下限・上限の補正にそれぞれ単位費用 $\alpha,\beta$ を設定 |

`op(f, g)` は先に `g`、次に `f` を適用する。位置は $p_f(p_g(x))$、費用・利得は $c_g(x)+c_f(p_g(x))$ になる。左から右へ操作列を実行するセグメント木には `OppositeMonoid<M>` を使う。

同じ集約を SWAG の操作列や木のパスにも使える。両方向の積を保持する場合は `NormalAndOppositeMonoid<M>` が対応する。逆順の積は、操作を逆順に実行する意味を持つ。

`infty` の省略時は[型ごとの既定値](../utils/default_infty.md)を使う。端点の `-infty`, `infty` は、それぞれ下限・上限の開放を表す。

### 関連する構造の使い分け

| 取得するもの・操作 | ライブラリ | 用途 |
| --- | --- | --- |
| 最終位置 | [MonoidClampAdd](clamp_add.md) | 加算・chmin・chmax・代入の合成 |
| 最終位置と距離型の費用 | `MonoidClampAddCost<T, 1, 1>` | 区間を順に通る最短移動、clamp の補正量 |
| 最終位置と重み付き費用 | `MonoidClampAddCost<T, α, β>` | 下限・上限の補正に別々の料金を設定 |
| 最終位置と距離型の利得 | `MonoidClampAddCost<T, -1, -1>` | 境界で止まる移動の総移動量 |
| 最終位置と片側の補正量 | `MonoidClampAddCost<T, 0, 1>` / `MonoidClampAddCost<T, 1, 0>` | 時間を戻す回数、片方向だけ有料の移動 |
| 配列の区間最小値・最大値 | [ActedMonoidClampMinMax](acted_monoid/clamp_min_max.md) | 区間 chmin・chmax・加算を通常の遅延セグメント木で処理 |
| 配列の区間和・最小値・最大値 | [ActedMonoidChminChmaxAddMinMaxSum](acted_monoid/chmin_chmax_add_min_max_sum.md) | Segment Tree Beats |

本モノイドは操作列が一つの初期値に及ぼす効果をまとめる。複数の初期値への作用後の和は、値の分布に応じた情報が必要になる。一般の折れ線凸関数を足す場合は傾きの変化点を保持する方法が対応する。ここでは費用の折れ点が位置関数の区間端点と一致するため、4 値に閉じる。

## 使用例

### 区間を順に通過する移動距離

```cpp
#include "algebra/algebra_base.hpp"
#include "algebra/clamp_add_cost.hpp"
#include "ds/segtree/segtree.hpp"

using M = MonoidClampAddCost<ll, 1, 1>;
using S = M::S;
vector<S> intervals{S::clamp(2, 5), S::clamp(8, 10), S::clamp(1, 4)};
SegmentTree<OppositeMonoid<M>> seg(intervals);
auto [y, distance] = seg.prod(0, 3)(3);
assert(y == 4 && distance == 9); // 3 -> 3 -> 8 -> 4
```

[ABC365 F の解説](https://atcoder.jp/contests/abc365/editorial/10582)に対応する。列 $s_x$ から $t_x$ へ進むとき、途中の列を clamp として合成する。最終縦座標を $y$、累積縦移動を $c$ とすると、答えは $(t_x-s_x)+c+|y-t_y|$。逆方向の質問は始点と終点を交換して処理できる。

### 境界で止まる移動の総移動量

```cpp
using M = MonoidClampAddCost<ll, -1, -1>;
using S = M::S;
// [0, 10] 上で +7、-4 の順に移動する。
auto f = M::op(S::shift_then_clamp(-4, 0, 10, 4),
               S::shift_then_clamp(7, 0, 10, 7));
auto [y, gain] = f(8);
assert(y == 6 && gain == 6); // 8 -> 10 -> 6、移動量は 2+4
```

[AAL Contest 001 Q の解説](https://atcoder.jp/contests/aalc001/editorial/25876)に対応する。移動量を $v$ とすると、葉は `S::shift_then_clamp(v, 0, M, abs(v))`。区間内から出発した場合、$|v|$ から clamp の補正量を引いた値が実際の移動量になる。点更新と区間取得は各 $O(\log N)$。

### 下限・上限の補正に別々の料金を設定する

```cpp
// 下限による増加は 1 単位につき 2、上限による減少は 1 単位につき 3。
using M = MonoidClampAddCost<ll, 2, 3>;
auto f = M::S::shift_then_clamp(3, 0, 10, 5);
assert((f(-5) == pair<ll, ll>{0, 9}));   // 基本料金 5 + 下限補正 2*2
assert((f(9) == pair<ll, ll>{10, 11}));   // 基本料金 5 + 上限補正 3*2
```

料金は同じ型の全要素で共通。`base` は操作ごとに異なる定数料金として指定できる。

### 時間を戻す回数だけを集計する

```cpp
using M = MonoidClampAddCost<ll, 0, 1>;
using S = M::S;
// 出発可能時刻が [3, 7]、通過に 1 秒かかる道路。
S road(3, 7, 1);
assert((road(1) == pair<ll, ll>{4, 0}));  // 2 秒待機して通過
assert((road(10) == pair<ll, ll>{8, 3})); // 3 秒戻って通過
```

位置は両方向に clamp し、費用は chmin 側だけを計上する。加算 `bias` に相当する通常の経過時間は `base` と分けて扱う。

ビ太郎の道路は `S(L, R-1, 1)`。道路列の合成を時刻 $b$ で評価して `{到着時刻 y, タイムリープ回数 c}` を得たら、指定到着時刻 $d$ に対する答えは $c+\max(y-d,0)$。両方向の道路列は `NormalAndOppositeMonoid<M>` で保持できる。

### 同じ情報から求められる量

- $\alpha,\beta\geq0$ なら `[lo, hi]` で最小値 `base`、$\alpha,\beta\leq0$ なら同区間で最大値 `base` を取る。初期位置を区間で制限した場合、`[lo, hi]` と交われば `base`、離れていれば近い端点で極値を取る。
- 片側費用の最小値も `base`。重みが `(0, 1)` なら `x <= hi`、`(1, 0)` なら `lo <= x` で達成する。
- 実際の総移動量を $D$、初期位置を $x$、最終位置を $y$ とすると、正方向の移動量は $(D+y-x)/2$、負方向は $(D-y+x)/2$。方向別の料金も計算できる。
- 各操作の費用・利得が非負なら、累積値がしきい値以下となる最大の接頭区間を `SegmentTree::max_right_ok` で $O(\log N)$ で求められる。

### 類似の出題例

| 問題 | 対応する構造・処理 |
| --- | --- |
| [ABC196 E - Filters](https://atcoder.jp/contests/abc196/tasks/abc196_e) | `MonoidClampAdd` で加算・chmin・chmax を順に合成。全体 $O(N+Q)$ |
| [ARC082 F - Sandglass](https://atcoder.jp/contests/arc082/tasks/arc082_d) | パーツ A の砂量を状態にし、経過時間に応じた加算と `[0,X]` への clamp を合成。時刻順に処理して $O(K+Q)$ |
| [JOISC2019 - 時をかけるビ太郎](https://atcoder.jp/contests/joisc2019/tasks/joisc2019_i) | 道路を `S(L, R-1, 1)` とし、`NormalAndOppositeMonoid<MonoidClampAddCost<ll, 0, 1>>` で両方向の移動を集約。更新・質問とも $O(\log N)$ |
| [IOI2014 - Wall](https://www2.ioi-jp.org/ioi/2014/tasks/wall-jp.pdf) | `MonoidClampAdd` の chmax・chmin を双対セグメント木で区間に作用させ、最後に各点の値を取得 |
| [yukicoder No.2890 - Chiffon](https://yukicoder.me/problems/no/2890) | 最小ピース幅を二分探索し、次の切断位置を `shift_then_clamp` で表す。上限による減少量を取り出して実現可能性を判定。全体 $O(N\log N)$ |

ABC196 E、ARC082 F、JOISC2019、Chiffon の検証用プログラムを `verify` 以下に置いている。

#### ARC082 F の状態の選び方

常に「パーツ A の砂量」を状態にする。砂時計の反転ではこの値を保ち、パーツ A の上下だけを切り替える。$d$ 秒間の変化は、A が上なら $a\mapsto\operatorname{clamp}(a-d,0,X)$、下なら $a\mapsto\operatorname{clamp}(a+d,0,X)$。それぞれ `S::shift_then_clamp(-d, 0, X)`、`S::shift_then_clamp(d, 0, X)` で表せる。

「上側の容器の砂量」を状態にした場合、$d$ 秒後に反転する遷移は $a\mapsto X-\operatorname{clamp}(a-d,0,X)$ になる。これは単調減少関数を含むため、その状態のまま扱うには反転に対応した表現への拡張が必要になる。

#### 片方向の補正量の取り出し

重み `(1, 1)` から片側の補正量を復元することもできる。加算と clamp を繰り返す操作列について、通常の加算量の合計を $A$、chmax による増加量を $U$、chmin による減少量を $D$ とする。各操作の `base` を $0$ にすると、評価結果 $(y,C)$ は

$$
y=x+A+U-D,\qquad C=U+D
$$

を満たす。したがって

$$
U=\frac{C+y-x-A}{2},\qquad
D=\frac{C-y+x+A}{2}
$$

で片側だけの補正量を求められる。$A$ は別途保持する量で、定数関数になった後の `bias` と区別する。

ビ太郎では道路数を $k$ とすると $A=k$。開始時刻 $b$ から合成して $(y,C)$ を得たとき、道路通過までのタイムリープ回数は $(C-y+b+k)/2$、指定到着時刻 $d$ までの追加回数は $\max(y-d,0)$ になる。

Chiffon では各ステップの加算量が候補幅 $w$ なので $A=Kw$。chmin による減少量が $0$ であることと、一周の長さに収まることを確認する。

### 別の設計が必要な出題例

本モノイドは、各操作の clamp 端点を具体的な値として持ち、初期位置 $x$ に対する効果を合成する。次の問題には、別の集約情報や解法が必要になる。

#### IOI2021 - Distributing Candies

[問題文](https://ioinformatics.org/files/ioi2021problem1.pdf)の操作は、対象区間の各箱について $x_i\gets\operatorname{clamp}(x_i+v,0,c_i)$。箱ごとに容量 $c_i$ が異なる。

容量が共通なら `MonoidClampAdd` と双対セグメント木で処理できる。容量が異なる場合、同じ更新区間内でも作用する関数が異なるため、共通の `S` をそのまま遅延作用として使う方法は適用範囲外となる。容量の違いを扱うオフライン処理などの追加が必要で、この問題の専用解法は現在未整備。汎用セグメント木などを再利用して実装することは可能である。

#### ABC217 H - Snuketoon

[公式解説](https://atcoder.jp/contests/abc217/editorial/2620)では、到達位置ごとの最小費用を凸な折れ線関数として扱う。遷移は、移動可能範囲における最小値の取得と、$\max(0,x-a)$ または $\max(0,a-x)$ の加算からなる。

これは費用関数を扱う近縁の問題で、折れ点の数は操作回数に応じて増える。本モノイドは折れ点を位置関数の区間端点に置く 4 値の表現を使うため、ここでは Slope Trick のように複数の折れ点を保持する構造が必要になる。現在のライブラリには Slope Trick の専用実装はない。

## 詳細なドキュメント

### MonoidClampAddCost

```cpp
MonoidClampAddCost<T, chmax_cost, chmin_cost, infty = nullptr, Cost = T>
```

`chmax_cost` が $\alpha$、`chmin_cost` が $\beta$。値または引数なしの関数を指定し、[resolved_value](../utils/resolved_value.md) で計算型へ変換する。関数を指定した場合は初回の返り値を保持するので、重みは全要素で共通となる。

位置・区間端点・平行移動量を `T`、累積費用・利得を `Cost` に保持する。例えば `MonoidClampAddCost<int, 1, 1, nullptr, ll>` とすれば位置と累積値の型を分けられる。座標の中間計算には `larger_int_t<T>`、費用には `larger_int_t<common_type_t<T, Cost>>` を使う。費用を浮動小数点で持つ場合も、整数座標の計算は整数型で行う。

##### 制約

- `T`, `Cost` は符号付き整数型、または必要な大小比較・加減乗算・相互変換を持つ数値型
- 重みの値または関数の返り値を、計算型 `larger_int_t<common_type_t<T, Cost>>` で表現できる
- 有限の区間端点・評価する座標は `-infty` より大きく、`infty` より小さい
- 保存する端点・`bias`・評価結果は `T`、保存する `base`・累積値は `Cost` で表現できる
- `i128` など拡張後も同じ幅となる型では、中間計算もその型で表現できる

#### S のコンストラクタ

```cpp
S(T lo, T hi, T bias = T{}, Cost base = Cost{})
```

$p(x)=\operatorname{clamp}(x,lo,hi)+bias$ と、累積値 $base+\alpha\max(lo-x,0)+\beta\max(x-hi,0)$ を構築する。

##### 制約

- `lo <= hi`

##### 計算量

- $O(1)$

#### operator()

```cpp
pair<T, Cost> operator()(T x) const
```

初期位置 `x` に対する `{最終位置, 累積費用または利得}` を返す。

##### 計算量

- $O(1)$

#### add / chmin / chmax / clamp / set

```cpp
static S add(T v, Cost base = Cost{})
static S chmin(T v, Cost base = Cost{})
static S chmax(T v, Cost base = Cost{})
static S clamp(T lo, T hi, Cost base = Cost{})
static S set(T v, Cost base = Cost{})
```

それぞれ加算、上限設定、下限設定、区間への clamp、一点への clamp を構築する。

`add` の累積値は常に `base`。残りは `base` に、左右の補正距離と各重みの積を加える。特に `set(v, base)` の累積値は $base+\alpha\max(v-x,0)+\beta\max(x-v,0)$。

##### 制約

- `clamp` では `lo <= hi`

##### 計算量

- $O(1)$

#### shift_then_clamp

```cpp
static S shift_then_clamp(T v, T lo, T hi, Cost base = Cost{})
```

$x\mapsto\operatorname{clamp}(x+v,lo,hi)$ と、累積値 $base+\alpha\max(lo-x-v,0)+\beta\max(x+v-hi,0)$ を構築する。内部表現は `{lo-v, hi-v, v, base}`。開放端点はそのまま保持する。

##### 制約

- `lo <= hi`
- 有限の `lo-v`, `hi-v` は `-infty` と `infty` の間に収まる

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& f, const S& g)
```

先に `g`、次に `f` を適用し、費用・利得を加算した要素を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

位置を保持し、累積値が $0$ となる単位元 `{-infty, infty, 0, 0}` を返す。

##### 計算量

- $O(1)$

### 合成の導出

`g` の後に `f` を適用する。$I=[L_g,R_g]$、$J=[L_f-a_g,R_f-a_g]$ と置く。これは後半の区間を、前半の入力座標へ平行移動したもの。

新しい区間は

$$
K=[\operatorname{clamp}(L_J,L_g,R_g),
   \operatorname{clamp}(R_J,L_g,R_g)].
$$

となる。区間が重なると $K=I\cap J$、離れていると $K$ は $I$ のうち $J$ に近い端点になる。

区間 $I$ の下限による補正量を $u_I(x)=\max(L_I-x,0)$、上限による補正量を $v_I(x)=\max(x-R_I,0)$ とすると、

$$
\begin{aligned}
u_I(x)+u_J(\operatorname{clamp}(x,I))&=u_K(x)+\delta_+,\\
v_I(x)+v_J(\operatorname{clamp}(x,I))&=v_K(x)+\delta_-,\\
\delta_+&=\max(L_J-R_I,0),\\
\delta_-&=\max(L_I-R_J,0)
\end{aligned}
$$

が成り立つ。すべての要素で重み $\alpha,\beta$ が共通なので、新しい `base` は

$$
b_g+b_f+\alpha\delta_++\beta\delta_-
$$

となる。この計算は重みの符号によらず成り立つ。実装では開放端点を含む無限大同士の減算を分岐で避ける。

重なる場合、新しい `bias` は $a_g+a_f$。離れている場合は最終位置が定数になり、その位置から $K$ の一点を引いた値を `bias` とする。こうして位置と累積値が同じ $K$ を使う形に保たれる。

合成は「位置と加算する値を持つ状態遷移」の合成なので結合則を満たす。

### 重みを選ぶタイミング

本実装では重みを全操作で共通にする。片側費用・両側費用・利得はすべてこの特殊化で、4 値・$O(1)$ 合成で扱える。

共通の重みを評価時に選びたい場合は、設計を 5 値 $(L,R,a,b_+,b_-)$ へ広げられる。上下の補正量を

$$
U(x)=b_++\max(L-x,0),\qquad D(x)=b_-+\max(x-R,0)
$$

と別々に保持し、評価時に $\alpha U(x)+\beta D(x)$ を計算する方法になる。この 5 値版は本実装には含めていない。操作ごとの定数料金も独立に保持するなら、さらに 1 値を加える。

一方、操作ごとに単位費用を変えると、一般には折れ点が増える。例えば `chmin(2)` を単位費用 $1$、続いて `chmin(1)` を単位費用 $2$ で行うと、費用は

$$
c(x)=\max(x-2,0)+2\max(\min(x,2)-1,0)
$$

になる。位置は $\min(x,1)$ だが、費用は $x=1,2$ で傾きが変わる。重みを各要素に持たせるだけでは、この 4 値の表現に閉じなくなる。

### 定数関数になったときの表現

最終位置が一定になっても、累積値は初期位置に依存する。例えば `[0, 1]`、`[3, 4]` の順に clamp すると、最終位置は常に $3$、移動距離は $2+d(x,[1,1])$ になる。表現は `{1, 1, 2, 2}`。

ABC365 F の解説では、出力位置の clamp 区間と費用の極値区間を別々に持つ。定数関数の場合に出力位置と極値位置の差を `bias` に移すことで、両方を同じ区間で表せる。これが 4 値で扱える理由である。
