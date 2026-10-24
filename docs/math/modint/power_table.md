このドキュメントには AI による作成・変更が含まれています。

## 概要

固定された $\mathrm{base}$ の累乗をテーブルに格納するやつ。

普通に書けばいいんだけど地味に面倒に思って $\log$ のつく累乗で横着して TLE、がありがち。これを貼ろう。

## 使用例

```cpp
using mint = modint998244353;
PowerTable<mint> powers(mint(3));
assert(powers.pow(4) == 81);
```

## 詳細なドキュメント

#### コンストラクタ

```cpp
PowerTable<mint> pw(mint base);
```

$\mathrm{base}$ を設定する。

#### reserve

```cpp
void reserve(int n)
```

テーブルを最大 $n$ のところまで前計算する。

なくても動くが、事前に必要な分だけ呼んでおくと定数倍高速化が期待できる。

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n))$

#### pow

```cpp
mint pow(int n)
```

$\mathrm{base}^n$ を計算する。

##### 計算量

$T$ 回呼んだとき、

- $O(T + \max(n))$
