このドキュメントには AI による作成・変更が含まれています。

## 概要

ACL の modint がベース

素数を法として累乗・逆元を大量に計算する場合は、前計算後に各操作が $O(1)$ になる [fast_modint](modint_fast.md) も使える。

`modint`（`dynamic_modint32<-1>`）などの動的 modint は、値の生成や `mod()` の呼び出しより先に `mint::set_mod(m)` で法を設定する。初期化忘れは `set_mod` の呼び出しを促すメッセージ付きの `assert` で検出する（`NDEBUG` 定義時は無効）。

相違点：

- 入出力ができる
- 他は外から使う分にはあまり変わっていないと思うが、中は一部省略している

制約：

- 動的 modint は使用前に `set_mod(m)` を呼び、$m \geq 1$ とする。
- `dynamic_modint32<INT_MIN>` はライブラリの内部で使用するので使用しないこと

## 使用例

```cpp
using mint = modint;
mint::set_mod(998244353);
mint x = 1;
```


## 詳細なドキュメント

`mint` は `static_modint32<M>` または `dynamic_modint32<id>`。`modint998244353` は固定法のエイリアス。

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
