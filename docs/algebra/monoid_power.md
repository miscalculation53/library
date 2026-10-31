このドキュメントには AI による作成・変更が含まれています。

## 概要

同じ要素を繰り返し結合する `pow(x, k)` の共通仕様。`pow_monoid<M>(x, k)` は `M::pow` があればそれを使うため、疎なセグメント木で同じ初期値が続く区間の集約にも高速化が反映される。

加法的な成分は整数倍で計算する。組み込み数値型は通常の乗算を使う。独自係数型では、整数を波括弧で渡せる乗算を選び、指数や係数のコンストラクタでの縮小変換を防ぐ。それ以外は二分累乗に相当する繰り返し加算を使う。浮動小数点数では、演算順序の変更に伴って丸め誤差が変わることがある。

## 使用例

```cpp
#include "algebra/min_max_sum.hpp"

using M = MonoidMinMaxSum<ll>;
auto x = pow_monoid<M>(M::S(7), 1000000000LL);
assert(x.mn == 7 && x.mx == 7);
assert(x.len == 1000000000LL && x.sum == 7000000000LL);
```

## 詳細なドキュメント

#### pow

```cpp
template <class I>
static constexpr S pow(const S& x, I k);
```

`e()` に `x` を `k` 回結合した結果を返す。`k = 0` なら `e()` を返す。`GroupTrivial` の第1引数は値渡し、従来の `MonoidLenSum` と一部の作用つきモノイドの指数型は `ll` である。

##### 制約

- `k` は非負整数。テンプレートの指数型は `i128`・`u128` にも対応する。
- 入力・結果・中間計算が、それぞれの係数型・個数型に十分な余裕を持って収まる。
- 係数の整数倍は繰り返し加算と整合する。

##### 計算量

係数演算と整数倍を $O(1)$ とする。

| 対象 | 計算量 | 計算方法 |
| --- | --- | --- |
| `MonoidMinMax`, `MonoidUpd`, `MonoidUpdFlag`, `ActedMonoidAriUpdMin/Max/MinMax` | $O(1)$ | 正の指数では元の要素を返す |
| `GroupTrivial` | $O(1)$ | 単位元を返す |
| `MonoidLenSum`, `MonoidMinSum`, `MonoidMaxSum`, `MonoidMinMaxSum` | $O(1)$ | 和と長さを整数倍する |
| `MonoidMinMaxSumBeats` | $O(1)$ | 和・長さ・極値の個数を整数倍する |
| `MonoidPrefixMin/Max/MinMax` | $O(1)$ | 総和の整数倍と、反復する列の最初・最後の累積和から求める |
| `MonoidPowerSum`, `MonoidIndexPowerSum` | $O(D)$ | 次数 $D$ までの各成分を整数倍する |
| `MonoidMultivariatePowerSum` | $O(\prod_j(D_j+1))$ | 各成分を整数倍する |
| `MajorityVote` | $O(1)$ | 同じ候補の個数を整数倍する |
| `MonoidClampAdd`, `MonoidClampAddCost` | $O(1)$ | 同じ向きへの移動と境界での飽和をまとめる |

繰り返し加算で整数倍を計算する場合は、該当する項目に $O(\log k)$ の係数が付く。`MajorityVote` は有効な非負個数の状態を対象とし、個数 $0$ は単位元にそろえる。

### 作用つきモノイドと逆順用の型

対応する作用つきモノイドは、値の集約を行う `M` の `pow` を公開する。引数は集約値 `S` で、`op` による自己反復を計算する。

`OppositeMonoid`・`OppositeGroup` は元の要素の `pow` を、`NormalAndOppositeMonoid`・`NormalAndOppositeGroup` は各成分の `pow` を使う。同じ要素の反復では、結合順序の反転によって結果が変わらない性質を利用する。

転送先が指数型に対応する `pow` を持つ場合は、その計算量を引き継ぐ。それ以外は元の指数型を保って二分累乗し、$O(\log k)$ 回結合する。群の負の指数は、従来どおり `pow_group` が逆元を取って処理する。
