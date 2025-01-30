## 素因数分解

#### factorize

与えられた整数をポラード・ロー法で素因数分解し、素因数の小さい順に素べきを格納した vector を返す。

```cpp
vc<PrimePower<ll>> factorize(ll n)
```

ポラード・ロー法についての参考リンク：https://qiita.com/Kiri8128/items/eca965fe86ea5f4cbb98

##### 制約

- 特になし（`ll` に収まっていれば OK）

##### 計算量

- $O(n^{1/4})$ と考えられている


#### factors

整数の素因数分解形を受け取り、その相異なる素因数を返す。

```cpp
vc<P> divisors(vc<PrimePower<P>> fac)
```

##### 計算量

- $O(\lvert \mathrm{fac} \rvert)$


#### divisors

整数の素因数分解形を受け取り、そのすべての約数を小さい順に格納した vector を返す。

```cpp
vc<ll> divisors(vc<PrimePower<ll>> fac)
```

##### 制約

- `fac` は素因数分解形として正しい
- `fac` が表す整数は `ll` に収まる

##### 計算量

- $O(\sigma_0(n))$
