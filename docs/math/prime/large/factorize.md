このドキュメントには AI による作成・変更が含まれています。

## 概要

素因数分解

## 使用例

```cpp
auto fac = factorize(12);
assert(fac.size() == 2);
assert(fac[0].p == 2 && fac[0].e == 2 && fac[1].p == 3);
```

## 詳細なドキュメント

#### factorize

与えられた整数をポラード・ロー法で素因数分解し、素因数の小さい順に素べきを格納した vector を返す。

```cpp
vc<PrimePower<ll>> factorize(ll n)
```

ポラード・ロー法についての参考リンク：https://qiita.com/Kiri8128/items/eca965fe86ea5f4cbb98

##### 制約

- $n \geq 1$

##### 計算量

- $O(n^{1/4})$ と考えられている

-----

prime_power にある divisors とかと組み合わせると「$n \leq 10^{18}$ が与えられるので約数を列挙する」とかもできる
