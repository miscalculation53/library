## modint (64 bit)

- `static_modint64`
- `dynamic_modint64_odd`（mod が奇数のみ）

の $3$ つを用意。`modint61` は `static_modint64<(1LL << 61) - 1>` のエイリアス。

ACL の modint にあるメソッドと入出力はだいたい揃っているが、dynamic には `raw` が存在しない。

---

内部の手法としては、dynamic は montgomery reduction を利用（あんまり理解してない。真の値 $a \bmod m$ の代わりに $a \cdot 2^{64} \bmod m$ を保持しているということだけ理解した。誤差評価はやりたくないので放置して信じることに……）。

参考リンク：
- https://rsk0315.hatenablog.com/entry/2022/11/27/060616
- https://nyaannyaan.github.io/library/modint/arbitrary-montgomery-modint.hpp
- https://www.mathenachia.blog/even-mod-montgomery-impl/
  - これに書いてある mod 偶数 ver も実装しようとしたが、初期化がよくわからなくなった　後で考えたいかも

##### 制約

- mod の上限は、`ll` に収まるなら OK なはず（証明したわけではない）
  - $2^{63}-1 = 9.223372036854775807 \times 10^{18}$