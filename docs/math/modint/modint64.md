このドキュメントには AI による作成・変更が含まれています。

## 概要

- `static_modint64`
- `dynamic_modint64_odd`（mod が奇数のみ）
- `dynamic_modint64`（mod が偶数も OK）

の $3$ つを用意。`modint61` は `static_modint64<(1LL << 61) - 1>` のエイリアス。

`static_modint64<M>` は $M\le2^{32}-1$ のとき積と剰余を `ull` で計算する。計算幅はコンパイル時に決まり、値の格納型と `val()` の返り値は従来どおり 64 bit。

動的 modint は、値の生成や `mod()` の呼び出しより先に `mint::set_mod(m)` で法を設定する。初期化忘れは `set_mod` の呼び出しを促すメッセージ付きの `assert` で検出する（`NDEBUG` 定義時は無効）。

値の取得・四則演算・累乗・逆元・入出力を提供する。

## 使用例

```cpp
using mint = dynamic_modint64<7>;
mint::set_mod(1000000000000000000LL);
mint x = 123456789;
assert((x * 10).val() == 1234567890);
```

## 詳細なドキュメント

`mint` は概要にある3種類のいずれかを表す。`dynamic_modint64_odd` の `set_mod(m)` は奇数の法を受け取る。

#### 値の生成、val, mod

```cpp
(1) mint(T x)
(2) auto val() const
(3) static auto mod()
```

整数から剰余を生成する。`val()` は $0$ 以上法未満の整数、`mod()` は法を返す。

##### 計算量

- $O(1)$。

#### 四則演算、pow, inv

```cpp
(1) mint operator+(mint y), operator-(mint y), operator*(mint y), operator/(mint y)
(2) mint pow(T k) const
(3) mint inv() const
```

法上の演算、非負整数乗、乗法逆元を返す。複合代入と入出力にも対応する。

##### 制約

- $k\ge0$。
- 除数と `inv()` の対象は法と互いに素。

##### 計算量

- 加減乗算：$O(1)$。
- `pow(k)`：$O(\log k)$。
- 除算・`inv()`：法を $p$ として $O(\log p)$。

#### set_mod

```cpp
static void set_mod(m)
```

動的 modint の法を設定する。値を作る前に呼ぶ。法を変更した場合、既存の値を使った計算は新しい値を作り直してから行う。
異なる `id` の型は法を独立して保持する。

##### 制約

- $m\ge1$。

##### 計算量

- $O(1)$。

## 内部計算

---

内部の手法としては、dynamic は montgomery reduction を利用（あんまり理解してない。真の値 $a \bmod m$ の代わりに $a \cdot 2^{64} \bmod m$ を保持しているということだけ理解した。誤差評価はやりたくないので放置して信じることに……）。

参考リンク：
- https://rsk0315.hatenablog.com/entry/2022/11/27/060616
- https://nyaannyaan.github.io/library/modint/arbitrary-montgomery-modint.hpp
- https://www.mathenachia.blog/even-mod-montgomery-impl/
  - mod 偶数 ver はこれ
  - 初期化は、$R = 2^{64}$ として $\mathrm{REDC}(x) \equiv xR^{-1} \pmod {M_x}, \mathrm{REDC}(x) \equiv x \pmod {2^b}$ の逆 $\mathrm{IREDC}$ を $\mathrm{REDC}$ で表せばよくて、$y \equiv R^2 \pmod {M_x}, y \equiv x \pmod {2^b}$ を解けばよい。

制約：

- 動的 modint は使用前に `set_mod(m)` を呼び、$m \geq 1$ とする。`dynamic_modint64_odd` では $m$ を奇数とする。
- `dynamic_modint64_odd<INT_MIN>` はライブラリの内部で使用するので使用しないこと
