## 概要

半環 `SR` などの内積（積和）を計算する。

```cpp
SR::S dot_product<SR>(int n, It1 a, It2 b)
```

それぞれの iterator から `n` 要素ずつ読み、$\sum_{i=0}^{n-1}a_i b_i$ を返す。
`n >= 0`。`n=0` は零元。逆向き iterator にも対応する。
時間 $O(n)$、追加空間 $O(1)$。

必要なインターフェースは `SR::S`、`SR::e0()`、`SR::add(a, b)`、`SR::mul(a, b)` のみ。
逆元・符号反転・乗法の単位元は使わないので、体である必要はない。
積は `SR::mul(a_i, b_i)` の順に計算し、零元から入力順に加算する。

```cpp
vc<ll> a{4, 10, -2}, b{7, 1, 8};
auto x = dot_product<RingAddSubMul<ll>>(3, a.begin(), b.begin());  // 22
auto y = dot_product<SemiRingMinPlus<ll>>(3, a.begin(), b.begin()); // 6
```

`S` が `static_modint32<mod>` または `dynamic_modint32<id>` で、
`SR` が次のいずれかなら専用の高速化を適用する。

- `FieldAddSubMulDiv<S>`
- `RingAddSubMul<S>`（合成数の法でも使える）
- `SemiRingFromMonoidMonoid<MonoidAdd<S>, MonoidMul<S>>`

通常の正規化済みの値を 64 bit 整数で積和し、法が $2^{30}$ 以下なら 16 項、
それより大きい正の `int` の法なら 4 項ごとに剰余を取る。
各ブロックの初期値が `mod` 未満でもオーバーフローしない大きさになっている。

64 bit modint、Montgomery 表現、独自の代数構造には `SR::add`・`SR::mul` を使う一般実装を適用する。
型が modint でも、`SR` 自体が独自の演算を定義していれば専用処理には入らない。

## ライブラリ内の利用例

- [Matrix / MatrixArray](linalg/matrix.md)：行列とベクトルの積、密な行列積。
- [畳み込み](convolution/convolution.md)：1 係数の取得と、小規模・密な入力の積。
- [多項式環](../algebra/polynomial_ring.md)：各係数を、片方の列を逆順にした内積として計算する。
- [subset convolution](set/subset_convolution.md)：集合の大きさごとの多項式の積。
- [Kronecker 冪](set/kronecker_power.md)：各段の行列とベクトルの積。

短い内積や疎な入力では、零を飛ばす従来の処理と使い分ける。
