## 概要

$f(x)=\operatorname{clamp}(x,\mathrm{lo},\mathrm{hi})+\mathrm{bias}$ という形の単調非減少関数を、関数合成について扱うモノイド。

加算、`chmin`、`chmax`、区間への clamp、定数関数を表現でき、これらを任意の順に合成しても同じ形になる。AtCoder ABC196 E - Filters のように、加算・下限設定・上限設定からなる操作列をまとめる用途や、遅延セグメント木の作用として利用できる。

`op(f, g)` は $f \circ g$、すなわち先に `g`、次に `f` を適用する。

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
```

## 詳細なドキュメント

### MonoidClampAdd

```cpp
MonoidClampAdd<T, infty = INF>
```

要素型 `S` は `lo`, `hi`, `bias` を持ち、$x \mapsto \operatorname{clamp}(x,\mathrm{lo},\mathrm{hi})+\mathrm{bias}$ を表す。

#### S のコンストラクタ

```cpp
S(T lo, T hi, T bias = T{})
```

対応する clamp・加算関数を構築する。

##### 制約

- `lo <= hi`
- `-infty` は取り得るすべての値以下、`infty` は取り得るすべての値以上
- 合成中の加減算結果が `T` に収まる

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

#### op

```cpp
S op(const S& f, const S& g)
```

合成関数 $f \circ g$ を返す。

##### 制約

- 合成中の加減算結果が `T` に収まる

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

恒等関数 $x\mapsto x$ を返す。

##### 計算量

- $O(1)$
