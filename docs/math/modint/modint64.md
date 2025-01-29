## modint (64 bit)

- `static_modint64`
- `dynamic_modint64_odd`（mod が奇数のみ）
- `dynamic_modint64`（mod が偶数も OK）

の $3$ つを用意。`modint61` は `static_modint64<(1LL << 61) - 1>` のエイリアス。

ACL の modint にあるメソッドと入出力はだいたい揃っているが、dynamic には `raw` が存在しない。

---

内部の手法としては、dynamic は montgomery reduction を利用（あんまり理解してない。真の値 $a \bmod m$ の代わりに $a \cdot 2^{64} \bmod m$ を保持しているということだけ理解した。誤差評価はやりたくないので放置して信じることに……）。

参考リンク：
- https://rsk0315.hatenablog.com/entry/2022/11/27/060616
- https://nyaannyaan.github.io/library/modint/arbitrary-montgomery-modint.hpp
- https://www.mathenachia.blog/even-mod-montgomery-impl/
  - mod 偶数 ver はこれ
  - 初期化は、$R = 2^{64}$ として $\mathrm{REDC}(x) \equiv xR^{-1} \pmod {M_x}, \mathrm{REDC}(x) \equiv x \pmod {2^b}$ の逆 $\mathrm{IREDC}$ を $\mathrm{REDC}$ で表せばよくて、$y \equiv R^2 \pmod {M_x}, y \equiv x \pmod {2^b}$ を解けばよい。

##### 制約

- `dynamic_modint64_odd<INT_MIN>` はライブラリの内部で使用するので使用しないこと
- mod の上限は、`ll` に収まるなら OK なはず（証明したわけではない）
  - $2^{63}-1 = 9.223372036854775807 \times 10^{18}$