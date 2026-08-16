## 概要

高々二次の方程式の整数解を求める。

## 使用例

```cpp
auto [cnt, sol] = quadratic_equation_integer(0, 2, -6);
assert(cnt == 1 && sol[0] == 3);
```

## 詳細なドキュメント

#### quadratic_equation_integer

```cpp
pair<int, array<T, 2>>
quadratic_equation_integer<T=ll, U=larger_int_t<T>>(T a, T b, T c)
```

高々二次の方程式 $ax^2 + bx + c = 0$ の整数解をすべて求める。

`first` には整数解の個数を、`second` には整数解を格納する。`second` のうち整数解の個数を超えた部分は未定義。$a=b=c=0$ のときはすべての整数が解なので、`first` に $-1$ を格納する。

計算途中の積には `U` を使う。

##### 制約

- `T`, `U` は符号つき整数型
- $a,b,c$ および整数解は `T` に収まる
- 判別式などの計算途中の値は `U` に収まる

##### 計算量

- $a \neq 0$ のとき `iroot(k=2)` と同じ
- $a = 0$ のとき $O(1)$
