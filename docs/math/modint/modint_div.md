## 零の重複度つき modint

https://kmyk.github.io/blog/blog/2021/04/23/modulo-with-zero/

素数 $p$ について、有理数 $r$ を

- 既約分数で表したときに分母も分子も $p$ で割り切れない有理数 $b$
- 整数 $c$

を用いて $r = bp^c$ の形で表現し、$(b, c)$ を（$b$ は modint で）持つ。これにより乗除算が $r = 0$（$b = 0$）を除いて常にできるようになる。（加減算はできない）

$p$ の倍数と $0$ は modint では同一視されるが、この表現では異なることに注意（$b = 0$ と $b \neq 0$）。たとえば「modint の列の区間積を累積和で求める」などは、$r \equiv 0$ を $(b, c) = (1, 1)$ だと思うことでできる。modint からの初期化はこの方法で実装している。

### コンストラクタ

```cpp
(1) ModintDiv<mint>(mint r)
(2) ModintDiv<mint>(mint b, ll c)
```

(1) は、$r \equiv 0$ なら $(b, c) = (1, 1)$、$r \not\equiv 0$ なら $(b, c) = (r, 0)$ で初期化する。


### メンバ変数

- `mint b`
- `ll c`


### メンバ関数

#### operator

`ModintDiv` との `*=`, `*`, `/=`, `/`, `==`, `!=` を実装。

##### 制約

- `/=`, `/` において、`rhs.b` $\neq 0$

##### 計算量

- `/=`, `/` は $O(\log p)$
- それ以外は $O(1)$


#### inv

```cpp
ModintDiv inv()
```

$b \neq 0$ のとき、逆元 $(b^{-1}, -c)$ を返す。

別の方法で逆元を計算したい場面（前計算できる等）であれば、$(b^{-1}, -c)$ を自分で計算するとよい。

##### 制約

- $b \neq 0$


##### 計算量

- $O(\log p)$


#### to_modint

```cpp
pair<bool, mint> to_modint()
```

(modint で表現できるか、表現できるときの値) を返す。

- $c \gt 0$ なら $0$
- $c = 0$ なら $b$
- $c \lt 0$ なら表現不能

##### 計算量

- $O(1)$


### フレンド関数

#### modint_to_modint_div

```cpp
ModintDiv modint_to_modint_div(mint r)
```

- $r \equiv 0$ なら $(b, c) = (1, 1)$
- $r \not\equiv 0$ なら $(b, c) = (r, 0)$

を返す。

##### 計算量

- $O(1)$
