## 二次方程式の整数解

```cpp
pair<int, array<T, 2>> quadratic_equation_integer<T=ll, U=i128>(T a, T b, T c)
```

二次方程式 $ax^2 + bx + c$ の整数解をすべて求める。

first には整数解の個数を、second には整数解を格納する。second のうち整数解の個数を超えた部分は未定義。

##### 制約

- $a, b, c$ は余裕を持って `T` に収まる
- `U` は `T` の $2$ 乗が収まる

##### 計算量

- `iroot(k=2)` がボトルネック