このドキュメントには AI による作成・変更が含まれています。

## 概要

モノイドの累乗 $a^n=b$、およびモノイド作用 $a^n\cdot s=t$ を満たす最小の $0\le n<\mathrm{limit}$ を求める。可逆でない元・作用にも対応し、合成数を法とする累乗にも使える。

解を `optional<ll>` で返す。$n=0$ も有効な解で、範囲内に解がなければ `nullopt` を返す。

演算・作用・ハッシュ・等値判定・値のコピーを $O(1)$ とすると、期待時間・追加空間は $O(\sqrt{\mathrm{limit}})$。ハッシュ表には状態そのものを格納し、等値判定で照合する。

デフォルト構築・コピー代入ができる `bool` 以外の状態型には [HashSet](../ds/hash_set.md) を使い、それ以外は `std::unordered_set` を使う。どちらも指定された `hash`・`equal` を使って照合する。

有限集合への作用では `limit` に状態数を指定すると、到達可能な目標の最小指数をすべて探索できる。

## 使用例

```cpp
#include "math/discrete_log_monoid.hpp"
#include "algebra/algebra_basic_ops.hpp"
#include "algebra/affine_function.hpp"
#include "math/modint/modint.hpp"

using mint = dynamic_modint32<0>;
using M = MonoidMul<mint>;
mint::set_mod(8);
assert(discrete_log_monoid<M>(mint(2), mint(0), 8) == 3);
assert(discrete_log_monoid<M>(mint(2), mint(1), 8) == 0);
assert(discrete_log_monoid<M>(mint(2), mint(0), 3) == nullopt);

// f(x) = 2x+1 を 0 に繰り返し作用させる。0 -> 1 -> 3 -> 7。
mint::set_mod(11);
using A = MonoidAffineFunction<mint>;
auto act = [](const A::S &f, const mint &x) { return f.b * x + f.c; };
assert(discrete_log_action<A>(A::S(2, 1), mint(0), mint(7), 11, act) == 3);
```

## 詳細なドキュメント

#### discrete_log_monoid

```cpp
template <class M, class Hash = safe_hash, class Equal = equal_to<typename M::S>>
optional<ll> discrete_log_monoid(
    const typename M::S &a, const typename M::S &b, ll limit,
    Hash hash = {}, Equal equal = {});
```

`M::op` を積、`M::e()` を単位元とするモノイドについて、$a^n=b$ を満たす最小の $0\le n<\mathrm{limit}$ を返す。解がなければ `nullopt`。`limit == 0` は常に `nullopt` を返す。

`discrete_log_action` に初期状態 `M::e()` と左からの乗算を渡して計算する。

##### 制約

- `limit >= 0`。
- `M` は要素型 `S`、結合的な `op`、単位元を返す `e` を持つ。
- `M::S` は値としてコピー・代入できる。
- `hash` と `equal` は `M::S` を受け取り、等しい要素には同じハッシュ値を返す。
- `Hash` と `Equal` はコピー構築できる。独自型の既定ハッシュへの対応は [safe_hash](../ds/safe_unordered_map.md) を参照。
- 入力・中間値・結果は、使用する型に十分な余裕を持って収まる。

##### 計算量

- 演算・ハッシュ・等値判定・値のコピーを $O(1)$ として、期待 $O(\sqrt{\mathrm{limit}})$。

#### discrete_log_action

```cpp
template <class M, class S, class Act,
          class Hash = safe_hash, class Equal = equal_to<S>>
optional<ll> discrete_log_action(
    const typename M::S &a, S s, const S &t, ll limit,
    Act &&act, Hash hash = {}, Equal equal = {});
```

`act(g, x)` を $g\cdot x$ として、$a^n\cdot s=t$ を満たす最小の $0\le n<\mathrm{limit}$ を返す。解がなければ `nullopt`。`limit == 0` は常に `nullopt` を返す。

`M::S` は作用する元の型、`S` は状態の型。`act` は参照で受け取り、状態を持つ関数オブジェクトやムーブ専用の関数オブジェクトも渡せる。`hash` と `equal` には状態を持つオブジェクトも渡せる。

##### 制約

- `limit >= 0`。`M` はモノイドの要件を満たす。
- `act(const M::S&, const S&)` は入力の値を保持し、作用後の `S` を返す。
- `act(M::e(), x) == x`、`act(M::op(g, h), x) == act(g, act(h, x))` が成り立つ。
- `M::S` と `S` は値としてコピー・代入できる。
- `equal` は状態の等値関係を表し、等しい状態への同じ作用は等しい状態を返す。等しい状態には `hash` が同じ値を返す。
- `Hash` と `Equal` はコピー構築できる。
- 入力・中間値・結果は、使用する型に十分な余裕を持って収まる。

##### 計算量

- モノイドの積 $O(\log\mathrm{limit})$ 回、作用 $O(\sqrt{\mathrm{limit}})$ 回。累乗には既存の `pow_monoid` を使う。
- ハッシュ・等値判定・値のコピーを含む各操作を $O(1)$ として、期待 $O(\sqrt{\mathrm{limit}})$。

## アルゴリズム

$f(x)=a\cdot x$、$B=\lceil\sqrt{\mathrm{limit}}\rceil$ とする。目標からの前進列 $f(t),\ldots,f^B(t)$ を集合に格納し、始点側は $B$ ステップずつ進める。指数の区間 $[kB,(k+1)B)$ に解があれば、その右端の状態はこの集合に含まれる。一致した区間だけ、左端から順に目標との一致を確認する。

初めて右端が一致した時点で、始点側の軌道は目標の前進軌道に入っている。それ以降に目標へ到達できるなら、目標は周期上にある。周期上では $f$ が置換として働くため、次に右端が一致した区間には必ず目標が現れる。したがって、詳しく確認する区間は高々 $2$ 個で済む。末尾の区間では `limit` 未満だけを確認する。

## 参考

- 数学的手法：[maspy「モノイド作用に関する離散対数問題」](https://maspypy.com/モノイド作用に関する離散対数問題)。
