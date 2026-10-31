このドキュメントには AI による作成・変更が含まれています。

## 概要

値の和と、添字の $0$ 乗から $D$ 乗までの和を持つモノイド。
多項式加算・代入と区間和取得の作用つきモノイドで使用する。

## 使用例

```cpp
using M = MonoidIndexPowerSum<ll, 2>;
auto s = M::op(M::S(2, 10), M::S(3, 20));
assert(s.val == 30);
assert((s.power_sum == array<ll, 3>{2, 5, 13}));
```

## 詳細なドキュメント

### MonoidIndexPowerSum

```cpp
MonoidIndexPowerSum<T, D>
```

要素型 `S` は値の和 `val` と、`array<T, D + 1>` 型の `power_sum` を持つ。
`power_sum[k]` は含まれる添字の $k$ 乗の和で、`power_sum[0]` は要素数を `T` で表した値。
一点のコンストラクタは添字を `ll` 型で受け取る。

##### 制約

- $D \geq 0$
- `T` は $0$, $1$, 整数からの変換、加算、乗算を持つ可換半環
- 整数型では、値の和と添字の累乗和が `T` に収まる

#### S のコンストラクタ

```cpp
(1) S()
(2) S(ll i, T val)
```

- (1)：値の和・累乗和がすべて $0$ の空の要素を構築する。
- (2)：添字 `i`、値 `val` の一点を構築する。`power_sum[k]` は $i^k$。$i=0$ でも `power_sum[0]` は $1$。

##### 制約

- (2)：$i \geq 0$

##### 計算量

- $O(D)$

#### op

```cpp
S op(const S& a, const S& b)
```

`val` と `power_sum` の各成分を加算する。

##### 計算量

- $O(D)$

#### e

```cpp
S e()
```

空の要素 `S()` を返す。

##### 計算量

- $O(D)$

#### pow

```cpp
template <class I>
static constexpr S pow(const S& x, I k);
```

値の和 `val` と添字の累乗和の各成分を `k` 倍する。添字は元の値を使って集約する。指数 $k$ は非負整数とし、`k = 0` なら `e()` を返す。型と整数倍の条件は [pow の共通仕様](monoid_power.md) を参照。

##### 計算量

- $O(D)$。整数倍を使う場合は、その演算を $O(1)$ とする。
