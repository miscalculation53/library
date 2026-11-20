このドキュメントには AI による作成・変更が含まれています。

## 概要

初期値 $x_0$ と遷移 $x_{i+1}=f(x_i)$ で定まる反復列の周期を検出する。周期に入る場合は、前周期長 $\mu$ と最小周期 $\lambda$ により

$$
x_{\mu+j}=x_{\mu+(j\bmod\lambda)} \qquad (j\geq 0)
$$

と表せる。周期検出には Brent の方法を使う。単発の問い合わせを処理する関数は状態と集約値を定数個保持する。初期値と遷移を固定した多クエリには、状態を保存する `Orbit` と、先頭区間の積を前計算する `OrbitPrefixProd` を使える。

## 使用例

```cpp
#include "algo/orbit_cycle.hpp"
#include "algebra/algebra_basic_ops.hpp"

auto next = [](ll x) { return (2 * x + 1) % 12; };
// 0, 1, 3, 7, 3, 7, ...
auto [preperiod, period] = orbit_cycle(0LL, next);
assert(preperiod == 2 && period == 2);
assert(orbit_at(0LL, 1000000000000000000LL, next) == 3);
assert(orbit_prefix_prod<GroupAddSub<ll>>(0LL, 6, next) == 21);

const Orbit orbit(0LL, next);
const OrbitPrefixProd<GroupAddSub<ll>> sums(orbit);
assert(orbit[1] == 1 && orbit[1000000000000000000LL] == 3);
assert(sums.prod(6) == 21 && sums.prod(7) == 24);
```

## 詳細なドキュメント

共通して、`next(const T&)` に初期値と同じ型の状態を渡し、同じ入力には同じ次の状態が得られることを前提とする。状態型はコピー構築・代入ができ、`next` の戻り値を状態に代入できる型とする。既定の等値判定は `operator==`。`equal(const T&, const T&)` を渡す場合は同値関係とし、等しい状態の遷移先も等しいことを要する。

### OrbitCycle

#### メンバ変数

```cpp
ull preperiod, period;
```

`preperiod` は周期に入る最初の添字 $\mu$、`period` は最小周期 $\lambda$。

#### reduce_index

```cpp
template <class I> constexpr I reduce_index(I k) const;
```

第 $k$ 項と等しい状態を持つ最小の添字を返す。$k<\mu$ なら $k$、それ以降は $\mu+((k-\mu)\bmod\lambda)$。

##### 制約

- `k` は非負整数。添字・項数には `i128`・`u128` も使える。
- `preperiod` と `period` は対象の列の前周期長と最小周期。

##### 計算量

- $O(1)$。

#### orbit_cycle

```cpp
template <class T, class Next, class Equal = equal_to<>>
constexpr OrbitCycle orbit_cycle(const T &initial, Next &&next, Equal &&equal = {});
```

初期値を第 $0$ 項として、前周期長と最小周期を返す。初期値が周期上にある場合、`preperiod` は `0`。

##### 制約

- 反復列は有限回の遷移で周期に入る。

##### 計算量

- 遷移と等値判定をそれぞれ $O(\mu+\lambda)$ 回。

#### orbit_at

```cpp
template <class T, class I, class Next, class Equal = equal_to<>>
constexpr T orbit_at(T initial, I k, Next &&next, Equal &&equal = {});
```

第 $k$ 項と等しい状態を返す。`k == 0` のときは初期値を返す。周期を見つけた場合は残りの遷移回数を周期で割った余りだけ進む。遷移は高々 $k$ 回。

##### 制約

- `k` は非負整数。

##### 計算量

- 周期に入る列では遷移と等値判定をそれぞれ $O(\min(k,\mu+\lambda))$ 回。周期のない列では $O(k)$ 回。

#### orbit_prefix_prod

```cpp
template <class M, class T, class I, class Next,
          class Project = Identity, class Equal = equal_to<>>
constexpr typename M::S orbit_prefix_prod(T initial, I k, Next &&next,
                                          Project &&project = {}, Equal &&equal = {});
```

$x_0,\ldots,x_{k-1}$ を `project` でモノイド $M$ の要素に写し、列の順に `M::op` で結合する。既定の `project` は恒等写像。`k == 0` なら `M::e()` を返す。$k>0$ なら遷移は高々 $k-1$ 回。

##### 制約

- `k` は非負整数。
- `project(const T&)` は等しい状態を同じモノイド要素に写す。

##### 計算量

- 周期に入る列では遷移・等値判定・射影・結合をそれぞれ $O(\min(k,\mu+\lambda))$ 回。周期をまとめるときは周期の積のべき乗を追加で行う。指数型 `I` に対応する `M::pow` があれば使い、二分累乗を使う場合は $O(\log k)$ 回結合する。周期のない列では $O(k)$ 回。

### Orbit

#### コンストラクタ

```cpp
template <class Next, class Equal = equal_to<>>
Orbit(const T &initial, Next &&next, Equal &&equal = {});
```

前周期長と最小周期を検出し、前周期と周期一周の状態を順に保存する。遷移と等値判定は構築時に評価する。

##### 制約

- 反復列は有限回の遷移で周期に入る。

##### 計算量

- 遷移・等値判定・状態のコピーをそれぞれ $O(\mu+\lambda)$ 回。保持する状態も $O(\mu+\lambda)$ 個。

#### operator[]

```cpp
template <class I> const_reference operator[](I k) const;
```

第 $k$ 項と等しい状態を返す。添字の縮約には `OrbitCycle::reduce_index` を使う。`const_reference` は状態を格納する `vc<T>` の型に従い、通常は `const T&`、`bool` では `bool`。

##### 制約

- `k` は非負整数。`i128`・`u128` も使える。

##### 計算量

- $O(1)$。

#### cycle, states

```cpp
OrbitCycle cycle() const;
const vc<T> &states() const;
```

`cycle()` は前周期長と最小周期を返す。`states()` は第 $0$ 項から第 $\mu+\lambda-1$ 項までの表を返す。

##### 計算量

- $O(1)$。

### OrbitPrefixProd

#### コンストラクタ

```cpp
template <class T, class Next, class Project = Identity, class Equal = equal_to<>>
OrbitPrefixProd(const T &initial, Next &&next, Project &&project = {}, Equal &&equal = {});

template <class T, class Project = Identity>
explicit OrbitPrefixProd(const Orbit<T> &orbit, Project &&project = {});
```

前周期と周期一周について、それぞれ先頭区間の積を作る。各状態を `project` でモノイド $M$ の要素に写し、列の順に `M::op` で結合する。既定の `project` は恒等写像。

初期値と遷移を渡した場合は、構築時に周期を検出して状態を順に射影する。保存済みの `Orbit` を渡した場合は、その周期情報と状態を使う。構築後はモノイド積の表を持ち、元の `Orbit` とは独立に使える。

##### 制約

- 反復列は有限回の遷移で周期に入る。
- `project(const T&)` は等しい状態を同じモノイド要素に写す。

##### 計算量

- 射影と結合をそれぞれ $O(\mu+\lambda)$ 回。保持するモノイド要素も $O(\mu+\lambda)$ 個。
- 初期値と遷移を渡した場合は、遷移と等値判定をそれぞれ $O(\mu+\lambda)$ 回。

#### prod

```cpp
template <class I> S prod(I k) const;
```

第 $0$ 項から第 $k-1$ 項までの射影を順に結合した値を返す。`k == 0` なら `M::e()` を返す。

前周期を過ぎた分の項数を $k-\mu=q\lambda+r$ とすると、前周期全体の積、周期一周の積の $q$ 乗、周期の先頭 $r$ 項の積をこの順に結合する。

##### 制約

- `k` は非負整数。`i128`・`u128` も使える。

##### 計算量

- 表参照と定数回の結合。$k>\mu+\lambda$ なら周期の積のべき乗を追加で行う。指数型 `I` に対応する `M::pow` があれば使い、二分累乗を使う場合は $O(\log k)$ 回結合する。

#### cycle

```cpp
OrbitCycle cycle() const;
```

前周期長と最小周期を返す。

##### 計算量

- $O(1)$。

## 周期の検出方法

長さ $h=1,2,4,\ldots$ の区間ごとに、始点 $x_{h-1}$ と、その後の高々 $h$ 項を順に比較する。始点が周期上にあり、$h\geq\lambda$ なら距離 $\lambda$ で初めて一致する。それ以前の区間で一致した場合も、その区間の始点は周期上にあり、初めて一致した距離が $\lambda$ になる。

初期値から $\lambda$ 項離した二つの状態を同時に進めると、先頭側が最初に周期に入った時点で一致する。これで $\mu$ が分かる。第 $k$ 項の取得や集約では、最初の一致時点で残りが周期的だと分かるため、$\mu$ の計算を省ける。

## 参考

- 数学的手法：[Richard P. Brent, “An improved Monte Carlo factorization algorithm”, BIT 20 (1980), 176–184](https://maths-people.anu.edu.au/~brent/pub/pub051.html)。
