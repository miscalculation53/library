## 概要

modint を有理数に

## 詳細なドキュメント

#### mint_to_rat

```cpp
pair<decltype(mint(0).val()), decltype(mint(0).val())> mint_to_rat(mint x)
```

`mint` の値を、小さい有理数 `p / q` として復元する。

返り値 `(p,q)` は

$$
p \equiv xq \pmod{\mathrm{mint::mod}()}
$$

を満たす短いベクトルの一つで、$q \geq 0$ になるように符号を揃える。

```cpp
auto [p, q] = mint_to_rat(x);
```

##### 制約

- `mint::mod()` と `x.val()` が同じ符号付き整数型で扱える
- `mint::mod() > 0`

##### 計算量

法を $m$ として、

- $O(\log m)$ 回の整数演算
