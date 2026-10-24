このドキュメントには AI による作成・変更が含まれています。

## 概要

Tonelli–Shanks で実装しており $O(\log^2 p)$。 https://37zigen.com/tonelli-shanks-algorithm/

TODO：

- ちゃんと理解できていないので、$k$ 乗根への一般化もあわせてちゃんと理解したい。
- $O(\log p)$ のアルゴリズムがあるらしい？

事実：

- 平方根が存在するのは $[1, p-1]$ のうち半分の $(p-1)/2$ 個。
- $n \neq 0$ の平方根が存在するとき、それは $\pm x$ の $2$ 個。

## 使用例

```cpp
using mint = modint998244353;
auto root = sqrt_mod(mint(4));
assert(root && (*root) * (*root) == 4);
```

## 詳細なドキュメント

#### sqrt_mod

```cpp
optional<mint> sqrt_mod(mint n)
```

`x * x == n` となる `mint x` を1つ返す。存在しなければ `nullopt`。`n == 0` では値ありの `mint(0)` を返す。

##### 制約

- `mint::mod()` は素数

##### 計算量

$p$ を `mint::mod()` として

- $O(\log^2 p)$
