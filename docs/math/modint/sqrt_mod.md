## mod 素数の平方根

Tonelli–Shanks で実装しており $O(\log^2 p)$。 https://37zigen.com/tonelli-shanks-algorithm/

TODO：

- ちゃんと理解できていないので、$k$ 乗根への一般化もあわせてちゃんと理解したい。
- $O(\log p)$ のアルゴリズムがあるらしい？

事実：

- 平方根が存在するのは $[1, p-1]$ のうち半分の $(p-1)/2$ 個。
- $n \neq 0$ の平方根が存在するとき、それは $\pm x$ の $2$ 個。

#### sqrt_mod

```cpp
pair<bool, mint> sqrt_mod(mint n)
```

`x * x == n` となる `mint x` が存在すれば `(true, x)`（のひとつ）を、存在しなければ `(false, 0)` を返す。

##### 制約

- `mint::mod()` は素数

##### 計算量

$p$ を `mint::mod()` として

- $O(\log^2 p)$
