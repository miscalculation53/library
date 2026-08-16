## 概要

二元一次連立方程式

$$
ax+by=c,\qquad dx+ey=f
$$

の整数解全体を、1つの解と整数係数の基底で表す。

行列式が $0$ でなければクラメルの公式を使い、行列式が $0$ なら拡張ユークリッド互除法で一次不定方程式を解く。

## 使用例

```cpp
auto res = linear_equations_integer(2, 3, 7, 1, -1, 1);
assert(res.dim == 0);
assert((res.sol == pair<ll, ll>{2, 1}));

auto line = linear_equations_integer(2, 4, 6, 1, 2, 3);
assert(line.dim == 1);
// すべての解は line.sol + k * line.basis[0] (k は整数)
```

## 詳細なドキュメント

#### linear_equations_integer

```cpp
LinearEquationsIntegerResult<T>
linear_equations_integer<T=ll, U=larger_int_t<T>>(a, b, c, d, e, f)
```

$ax+by=c$, $dx+ey=f$ の整数解全体を返す。返り値を `res` とすると、各メンバは次の意味を持つ。

- `res.dim == -1`：整数解は存在しない
- `res.dim == 0`：整数解は `res.sol` の1つだけ
- `res.dim == 1`：整数解は `res.sol + k * res.basis[0]` $(k\in\mathbb Z)$
- `res.dim == 2`：整数解は `res.sol + k_0 * res.basis[0] + k_1 * res.basis[1]` $(k_0,k_1\in\mathbb Z)$

積や行列式の計算には `U` を使い、解と基底は `T` に格納する。

##### 制約

- `T`, `U` は符号つき整数型
- $a,b,c,d,e,f$ および返す解と基底は `T` に収まる
- 積や拡張ユークリッド互除法の計算途中の値は `U` に収まる

##### 計算量

- 係数行列の行列式が $0$ でないとき $O(1)$
- 行列式が $0$ のとき $O(1+\log\min(|p|,|q|))$
  - $(p,q)$ は採用した非零な方程式の係数
