このドキュメントには AI による作成・変更が含まれています。

## 概要

`infty` の省略時（`nullptr`）は、[型ごとの既定の無限大](../utils/default_infty.md)を `T` に対して使う。値や関数を明示した場合は、その指定を使う。

$f(x)=\operatorname{clamp}(x,\mathrm{lo},\mathrm{hi})+\mathrm{bias}$ という形の単調非減少関数を、関数合成について扱うモノイド。

加算、`chmin`、`chmax`、区間への clamp、定数関数を表現でき、これらを任意の順に合成しても同じ形になる。AtCoder ABC196 E - Filters のように、加算・下限設定・上限設定からなる操作列をまとめる用途や、遅延セグメント木の作用として利用できる。

`op(f, g)` は $f \circ g$、すなわち先に `g`、次に `f` を適用する。

操作列を左から順に実行するセグメント木には `OppositeMonoid<MonoidClampAdd<T>>` を使う。最終位置と移動距離・利得を一緒に取得する場合は [MonoidClampAddCost](clamp_add_cost.md) を使う。

端点の `-infty`, `infty` は、それぞれ下限・上限の開放を表す。合成時には開放端点を保持し、有限の加減算の中間計算には `larger_int_t<T>` を使う。

## 使用例

```cpp
using M = MonoidClampAdd<ll>;
using F = M::S;

F f = M::e();
f = M::op(F::add(3), f);
f = M::op(F::chmax(0), f);
f = M::op(F::chmin(10), f);
assert(f(-5) == 0);
assert(f(20) == 10);

// 加算してから clamp する関数を直接構築する。
auto g = F::shift_then_clamp(3, 0, 10);
assert(g(5) == 8);
```

## 詳細なドキュメント

### MonoidClampAdd

```cpp
MonoidClampAdd<T, infty = nullptr>
```

要素型 `S` は `lo`, `hi`, `bias` を持ち、$x \mapsto \operatorname{clamp}(x,\mathrm{lo},\mathrm{hi})+\mathrm{bias}$ を表す。

#### S のコンストラクタ

```cpp
S(T lo, T hi, T bias = T{})
```

対応する clamp・加算関数を構築する。

##### 制約

- `lo <= hi`
- `T` は符号付き整数型、または必要な大小比較・加減算・型変換を持つ数値型
- 有限の端点・評価する座標は `-infty` より大きく、`infty` より小さい
- 保存する端点・`bias`・評価結果は `T` で表現できる
- `i128` など拡張後も同じ幅となる型では、中間計算もその型で表現できる

##### 計算量

- $O(1)$

#### operator()

```cpp
T operator()(T x) const
```

$\operatorname{clamp}(x,\mathrm{lo},\mathrm{hi})+\mathrm{bias}$ を返す。

##### 計算量

- $O(1)$

#### chmin / chmax / add / clamp / set

```cpp
static S chmin(T v)
static S chmax(T v)
static S add(T v)
static S clamp(T lo, T hi)
static S set(T v)
```

それぞれ $x\mapsto\min(x,v)$、$x\mapsto\max(x,v)$、$x\mapsto x+v$、$x\mapsto\operatorname{clamp}(x,lo,hi)$、$x\mapsto v$ を返す。

##### 制約

- `clamp` では `lo <= hi`

##### 計算量

- $O(1)$

#### shift_then_clamp

```cpp
static S shift_then_clamp(T v, T lo, T hi)
```

$x\mapsto\operatorname{clamp}(x+v,lo,hi)$ を構築する。`op(S::clamp(lo, hi), S::add(v))` と同じ関数になる。

内部表現は `{lo-v, hi-v, v}`。開放端点の `-infty`, `infty` はそのまま保持する。

##### 制約

- `lo <= hi`
- 有限の `lo-v`, `hi-v` は `-infty` と `infty` の間に収まる

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& f, const S& g)
```

合成関数 $f \circ g$ を返す。定数関数は複数の表現を持つため、関数としての値を使って比較する。

##### 制約

- 合成後に保存する端点・`bias` は `T` で表現できる

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

恒等関数 $x\mapsto x$ を返す。

##### 計算量

- $O(1)$

#### pow

```cpp
template <class I>
static constexpr S pow(const S& x, I k);
```

同じ関数を `k` 回合成する。移動量と境界での飽和をまとめて求める。両端が有限なら、移動量の積を区間幅で打ち切って計算する。指数 $k$ は非負整数とし、`k = 0` なら `e()` を返す。型と整数倍の条件は [pow の共通仕様](monoid_power.md) を参照。

##### 計算量

- $O(1)$。整数倍を使う場合は、その演算を $O(1)$ とする。
