## 概要

素数を法とする `fast_modint<p>`。前計算後の `pow`・`inv`・除算が $O(1)$ になる。

[maspy さんの記事「O(1) mod inv, mod pow」](https://maspypy.com/o1-mod-inv-mod-pow) の手法を用いる。前計算表は同じ `p` の値同士で共用し、各値の大きさは通常の 32 bit modint と同じ。

前計算の時間は期待 $O(p^{2/3} + p^{5/6}/\log p)$、空間は $O(p^{2/3})$。多数の累乗・逆元クエリを処理するときに使う。四則演算・入出力・`val()`・`raw()` は既存の [modint](modint.md) と同じように使える。NTT・畳み込みにも対応する。

## 使用例

```cpp
#include "math/modint/modint_fast.hpp"

using mint = fast_modint<998244353>;
mint::precompute(); // 省略すると最初の pow・inv・除算で前計算する

mint a = 3;
auto x = a.pow(1000000000000000000ULL);
auto y = a.inv();
auto z = mint(10) / a;
assert(a * y == 1);
```

## 詳細なドキュメント

### fast_modint

#### コンストラクタ

```cpp
fast_modint<p> x;
fast_modint<p> x(T v);
```

引数を省略すると $0$、整数 `v` を渡すとその剰余で初期化する。負数や 128 bit 整数も渡せる。前計算は `precompute`・`pow`・`inv`・除算で行う。

##### 制約

- `p` は素数。$p=2$ にも対応する。
- `T` は整数型。

##### 計算量

- $O(1)$

#### precompute

```cpp
static void mint::precompute();
```

分数・逆元・離散対数・原始根の累乗の表を構築する。同じ `p` に対する 2 回目以降の呼び出しは構築済みの表を使う。

##### 計算量

- 初回は期待 $O(p^{2/3} + p^{5/6}/\log p)$。
- 2 回目以降は $O(1)$。

#### pow

```cpp
template <class T>
mint x.pow(T n) const;
```

$x^n$ を返す。$0^0=1$ とする。`n` には 64 bit・128 bit の整数も渡せる。

##### 制約

- `T` は整数型で、$n \geq 0$。

##### 計算量

- 前計算後は $O(1)$。

#### inv・除算

```cpp
mint x.inv() const;
mint x / y;
mint& x /= y;
```

`inv` は $x$ の逆元、除算は $x y^{-1}$ を求める。

##### 制約

- `inv` では $x \neq 0$。
- 除算では $y \neq 0$。

##### 計算量

- 前計算後は $O(1)$。

### 前計算の内容

$n=\Theta(p^{1/3})$ として Farey 数列を列挙し、各区間に分母 $b \leq n$ の分数 $a/b$ を割り当てる。区間内の $x$ に対して $t=bx-ap$ とすると $|t|=O(p^{2/3})$ なので、$x=t/b$ の逆元・離散対数を小さい整数の表から求められる。

離散対数は $\sqrt p$ 以下の素数について BSGS で計算し、baby step の表を共用する。$i>\sqrt p$ では

$$i \equiv -\frac{p \bmod i}{\lfloor p/i\rfloor}\pmod p$$

を使うと、計算済みの小さい整数の対数から求められる。原始根の累乗は指数を二分して表に格納する。

表の最大添字を $K=\Theta(p^{2/3})$、$\sqrt p$ 以下の素数の個数を $Q$ とし、BSGS の幅を $B=\max(1,\min(2K,\lceil\sqrt{(p-1)Q}\rceil))$ としている。前計算の時間は期待 $O(K+B+pQ/B)$。期待値はハッシュ表と原始根の探索による。

### 計測例

手元の環境で `g++-15 -O2 -DNDEBUG`、ランダムな 100 万クエリを 5 回測った中央値。クエリ時間は前計算を除く。

| 法 | 前計算 | 通常の pow | fast_modint の pow | 通常の inv | fast_modint の inv |
|---|---:|---:|---:|---:|---:|
| 998244353 | 149 ms | 87.6 ns | 34.5 ns | 93.0 ns | 11.3 ns |
| 1000000007 | 130 ms | 86.5 ns | 28.3 ns | 87.4 ns | 13.2 ns |
| 2147483647 | 316 ms | 104.3 ns | 32.0 ns | 118.7 ns | 22.4 ns |

計測コードは `benchmarks/modint_fast.cpp`、結果は `benchmarks/modint_fast.csv`。
