このドキュメントには AI による作成・変更が含まれています。

## 概要

$1$ 以上 $n$ 以下の整数の集合の整除関係に関する約数・倍数 ゼータ・メビウス変換。

約数・倍数 ゼータ・メビウス変換の定義は、「$m$ の約数」側のドキュメントを参照のこと。

LCM/GCD 畳み込みについてはそちら側のドキュメントを参照のこと。

内部で線形篩を用いている。関数を呼び出す前に `LinearSieve::reserve(n)` しておくと定数倍高速化が期待できる。

## 使用例

```cpp
vc<ll> a{0, 1, 2, 3, 4}; // 添字 0 は使わない
auto sums = zeta_divisor<MonoidAdd<ll>>(a);
assert(sums[4] == 7); // a[1] + a[2] + a[4]
assert(mobius_divisor<GroupAddSub<ll>>(sums) == a);
```

## 詳細なドキュメント

#### zeta_divisor

```cpp
DivisorMap<M::S> zeta_divisor(DivisorMap<M::S> a)
```

1-indexed（`a[0]` は使わない）。

##### 制約

- `M` は**可換**モノイド

##### 計算量

$\lvert a \rvert = n$ として

- $O(n \log \log n)$

#### mobius_divisor

```cpp
DivisorMap<G::S> mobius_divisor(DivisorMap<G::S> a)
```

1-indexed（`a[0]` は使わない）。

##### 制約

- `G` は**可換**群

##### 計算量

$\lvert a \rvert = n$ として

- $O(n \log \log n)$

----

zeta_multiple, mobius_multiple も同様（省略）。
