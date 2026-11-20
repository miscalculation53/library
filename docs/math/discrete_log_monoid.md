このドキュメントには AI による作成・変更が含まれています。

## 概要

モノイドの累乗 $a^n=b$、およびモノイド作用 $a^n\cdot s=t$ を満たす、区間 $[l,r)$ 内の最小の整数 $n$ を求める。非可逆な元や作用にも対応し、合成数を法とする累乗にも使える。

結果は `optional<ll>` で返す。$n=0$ も有効な解となり、区間内に解がなければ `nullopt` を返す。

左端 $l$ まで累乗で移動し、幅 $r-l$ の範囲で baby-step giant-step 法を使う。ハッシュ表には状態そのものを格納し、等値判定で照合する。

デフォルト構築・コピー代入ができる `bool` 以外の状態型には [HashSet](../ds/hash_set.md) を使い、それ以外は `std::unordered_set` を使う。どちらも指定された `hash`・`equal` を使って照合する。

有限集合への作用では、区間幅を状態数にすると、時刻 $l$ 以降に到達可能な目標の最小指数をすべて探索できる。

## 使用例

```cpp
#include "math/discrete_log_monoid.hpp"
#include "algebra/algebra_basic_ops.hpp"
#include "algebra/affine_function.hpp"
#include "math/modint/modint.hpp"

using mint = dynamic_modint32<0>;
using M = MonoidMul<mint>;
mint::set_mod(8);
assert(discrete_log_monoid<M>(mint(2), mint(0), 0, 8) == 3);
assert(discrete_log_monoid<M>(mint(2), mint(1), 0, 8) == 0);
assert(discrete_log_monoid<M>(mint(2), mint(0), 0, 3) == nullopt);
assert(discrete_log_monoid<M>(mint(3), M::e(), 1, 3) == 2);
assert(discrete_log_monoid<M>(mint(3), M::e(), 3, 7) == 4);

// f(x) = 2x+1 を 0 に繰り返し作用させる。0 -> 1 -> 3 -> 7。
mint::set_mod(11);
using A = MonoidAffineFunction<mint>;
auto act = [](const A::S &f, const mint &x) { return f.b * x + f.c; };
assert(discrete_log_action<A>(A::S(2, 1), mint(0), mint(7), 2, 11, act) == 3);
```

## 詳細なドキュメント

#### discrete_log_monoid

```cpp
template <class M, class Hash = safe_hash, class Equal = equal_to<typename M::S>>
optional<ll> discrete_log_monoid(
    const typename M::S &a, const typename M::S &b, ll l, ll r,
    Hash hash = {}, Equal equal = {});
```

`M::op` を積、`M::e()` を単位元とするモノイドについて、$a^n=b$ を満たす最小の $l\le n<r$ を返す。解がなければ `nullopt`。`l == r` は空区間。

`discrete_log_action` に初期状態 `M::e()` と左からの乗算を渡して計算する。位数は、目標を `M::e()` とし、区間の左端を $1$ にして求める。

##### 制約

- `0 <= l && l <= r`。
- `M` は要素型 `S`、結合的な `op`、単位元を返す `e` を持つ。
- `M::S` は値としてコピー・代入できる。
- `hash` と `equal` は `M::S` を受け取り、等しい要素には同じハッシュ値を返す。
- `Hash` と `Equal` はコピー構築できる。独自型の既定ハッシュへの対応は [safe_hash](../ds/safe_unordered_map.md) を参照。
- 入力・中間値・結果は、使用する型に十分な余裕を持って収まる。

##### 計算量

- `discrete_log_action` と同じ。二分累乗を使い、各演算を $O(1)$ とすると、非空区間で期待時間 $O(\log r+\sqrt{r-l})$、追加領域 $O(\sqrt{r-l})$。

#### discrete_log_action

```cpp
template <class M, class S, class Act,
          class Hash = safe_hash, class Equal = equal_to<S>>
optional<ll> discrete_log_action(
    const typename M::S &a, S s, const S &t, ll l, ll r,
    Act &&act, Hash hash = {}, Equal equal = {});
```

`act(g, x)` を $g\cdot x$ として、$a^n\cdot s=t$ を満たす最小の $l\le n<r$ を返す。解がなければ `nullopt`。`l == r` は空区間。

`M::S` は作用する元の型、`S` は状態の型。`act` は参照で受け取り、状態を持つ関数オブジェクトやムーブ専用の関数オブジェクトも渡せる。`hash` と `equal` には状態を持つオブジェクトも渡せる。

##### 制約

- `0 <= l && l <= r`。`M` はモノイドの要件を満たす。
- `act(const M::S&, const S&)` は入力の値を保持し、作用後の `S` を返す。
- `act(M::e(), x) == x`、`act(M::op(g, h), x) == act(g, act(h, x))` が成り立つ。
- `M::S` と `S` は値としてコピー・代入できる。
- `equal` は状態の等値関係を表し、等しい状態への同じ作用は等しい状態を返す。等しい状態には `hash` が同じ値を返す。
- `Hash` と `Equal` はコピー構築できる。
- 入力・中間値・結果は、使用する型に十分な余裕を持って収まる。

##### 計算量

- 非空区間では、モノイドの累乗 $a^l$ と $a^{\lceil\sqrt{r-l}\rceil}$ を求める。$l=0$ なら初期状態をそのまま使う。累乗には `pow_monoid` を使い、二分累乗の場合はモノイドの積 $O(\log r)$ 回。
- 作用は $O(\sqrt{r-l})$ 回。ハッシュ・等値判定・値のコピーを含む各操作を $O(1)$ として、二分累乗を使う場合の期待時間は $O(\log r+\sqrt{r-l})$、追加領域は $O(\sqrt{r-l})$。

## アルゴリズム

$f(x)=a\cdot x$ とし、始点を $a^l\cdot s$ に移す。この始点から $0\le j<r-l$ で目標に到達する最小の $j$ を求め、$l+j$ を返す。

$B=\lceil\sqrt{r-l}\rceil$ とする。目標からの前進列 $f(t),\ldots,f^B(t)$ を集合に格納し、移した始点から $B$ ステップずつ進める。$j$ の区間 $[kB,(k+1)B)$ に解があれば、その右端の状態はこの集合に含まれる。一致した区間だけ、左端から順に目標との一致を確認する。

初めて右端が一致した時点で、始点側の軌道は目標の前進軌道に入っている。それ以降に目標へ到達できるなら、目標は周期上にある。周期上では $f$ が置換として働くため、次に右端が一致した区間には必ず目標が現れる。したがって、詳しく確認する区間は高々 $2$ 個で済む。末尾の区間では $j<r-l$ を満たす位置だけを確認する。

## 参考

- 数学的手法：[maspy「モノイド作用に関する離散対数問題」](https://maspypy.com/モノイド作用に関する離散対数問題)。
