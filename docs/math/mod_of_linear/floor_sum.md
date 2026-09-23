## 概要

一次式の床関数の和をユークリッド互除法型の処理で求める。負の係数と負の添字を扱える。

一次式の剰余が指定した値域に入る項数を求める `count_mod_of_linear` も同じヘッダに含む。剰余の最小値は [min_of_mod_of_linear.hpp](min_of_mod_of_linear.md) にある。

テンプレート引数には答えの型 `R` を必ず指定し、各入力の整数型は独立に推論する。`int`、`ll`、`ull`、`i128`、`u128` などを混在させられる。

**整数型 `R` には、そのビット幅 $w$ に対する $\bmod 2^w$ の答えを返す。** 総和は対応する unsigned で計算する。signed の戻り値は $[-2^{w-1},2^{w-1})$ の代表元となり、同じ幅の unsigned の戻り値と合同になる。総和が型の範囲を超える場合も扱える。

`R` には `modint` や `BigInteger<>` も指定できる。この場合は `R` 上で集約する。整数の除算を先に済ませるため、`R` 上での除算は不要。

商・剰余を求める部分は、正規化後の値の大きさで計算幅を選ぶ。各累積和の入口で、添字の絶対値と法がともに $2^{32}-1$ 以下なら、積と除算を `ull` で行う。入力を `ll` や `u128` に格納していても、この経路を使える。係数は先に正規化するため、$a,b$ が大きい場合も対象となる。

それより大きく、添字の絶対値と法が 64 bit に収まる場合は、積を `u128` に拡張する。128 bit 入力の積が収まらない場合は、商・剰余を保ちながら各 bit を処理する。答えの集計は、どの経路でも `R` に対応する型で行う。

各反復で添字の絶対値と法は増えず、正規化後の積は $an+b\le(m-1)(n+1)$ となるため、入口の判定だけで計算幅を決められる。[速度比較と他のライブラリの調査](../runtime_integer_width.md)。

## 使用例

```cpp
#include "math/mod_of_linear/floor_sum.hpp"

auto x = floor_sum<ll>(4, 5, 3, -2);  // -1+0+0+1 = 0
auto y = floor_sum<ll>(-2, 3u, 5ULL, 3, -2);  // 添字 [-2,3) の和
auto u = floor_sum<ull>(n, m, a, b);  // mod 2^64 の非負の代表元
auto s = floor_sum<ll>(n, m, a, b);   // ull(s) == u
auto z = floor_sum<modint998244353>(n, m, a, b);
// #include "math/bigint.hpp"
auto w = floor_sum<BigInteger<>>(n, m, a, b);  // 正確な整数の総和
assert(count_mod_of_linear(4, 9, 4, 3, 2, 7) == 3);  // 3,7,2,6 のうち [2,7)
```

## 詳細なドキュメント

#### floor_sum

```cpp
template <class R, class N, class M, class A, class B>
R floor_sum(N n, M m, A a, B b)

template <class R, class L, class H, class M, class A, class B>
R floor_sum(L l, H r, M m, A a, B b)
```

それぞれ $\sum_{i=0}^{n-1}\lfloor(ai+b)/m\rfloor$、$\sum_{i=l}^{r-1}\lfloor(ai+b)/m\rfloor$ を `R` の規則で返す。空区間では $0$。負の端点や、signed と unsigned を混ぜた区間も整数として扱う。

##### 制約

- $m\ge1$。
- 長さを指定する形式は $n\ge0$。区間を指定する形式は $l\le r$。
- 入力は `bool` を除く組み込み整数型。`i128`、`u128` にも対応する。
- `R` は `bool` を除く整数型、または内部の unsigned 整数からの構築・加算・減算・乗算を持つ型。

##### 計算量

- 64 bit 以下の入力では $O(1+\log m)$。
- 128 bit 入力では、ビット幅を $w=128$ として $O(w(1+\log m))$。
- いずれも `R` の演算時間を $O(1)$ とした場合。

#### count_mod_of_linear

```cpp
ll count_mod_of_linear(ll n, ll m, ll a, ll b, ll l, ll r)
ll count_mod_of_linear(ll n, ll m, ll a, ll l, ll r)  // b=0
```

$0\le i<n$ のうち、$l\le(ai+b)\bmod m<r$ を満たす項数を返す。負の $a,b$ を正規化して扱い、周期を超える長さにも対応する。$l=r$ の場合は $0$。

2 つの floor sum の差で計算する。各総和は $\bmod 2^{64}$ で計算してよく、その差は $[0,n]$ に収まるため正確な個数が得られる。

##### 制約

- $n\ge0$、$m\ge1$。
- $0\le l\le r\le m$。

##### 計算量

- $O(1+\log m)$。

#### internal の役割

数学的な本体は `floor_sum_unsigned_core` の約 20 行。符号や型変換などの汎用処理は `utils` に分け、ここには floor sum の計算と計算幅の選択を置く。以下の `C` は集計する型、`U` は添字・法・剰余を持つ unsigned の型。

| 名前 | 役割 |
| --- | --- |
| `floor_sum_divmod` | 正規化後の $an+b$ の商・剰余を求める。入口で小さい値と判定済みなら 64 bit の積を使い、それ以外は `utils` の `mul_add_divmod` を呼ぶ |
| `floor_sum_triangle` | $n(n-1)/2$ を `C` で返す。偶数の因子を先に 2 で割るため、整数の剰余演算でも正しく、`modint` の 2 の逆元も不要 |
| `floor_sum_unsigned_core` | 非負入力に対する本体。$a/m,b/m$ の寄与を答えに加え、添字数と法をユークリッド互除法型に縮小する |
| `floor_sum_unsigned` | 入口で値の大きさを調べ、本体の計算幅を選ぶ。反復中の幅判定を省くため、本体と分けている |

汎用処理は次の関数を使う。

| 名前 | 役割 |
| --- | --- |
| [`unsigned_arithmetic_t`](../../utils/integer_arithmetic.md) | 整数なら整数昇格後も unsigned となる計算型、それ以外なら元の型を選ぶ |
| [`is_negative` / `unsigned_abs`](../../utils/integer_arithmetic.md) | signed / unsigned を含む整数の符号判定と絶対値。signed の最小値も扱える |
| [`wrapping_cast`](../../utils/integer_arithmetic.md) | 下位 bit が等しい値に整数型を変換する |
| [`mul_add_divmod`](../../utils/mul_add_divmod.md) | 積と定数の和の商・剰余。128 bit の積があふれる場合にも対応する |

公開関数内の `normalize` は $a=q_am+r_a$、$b=q_bm+r_b$ に分解する。`prefix` は $F(0)=0$、$F(x+1)-F(x)=\lfloor(ax+b)/m\rfloor$ を満たす累積和で、答えを $F(r)-F(l)$ として求める。負の端点は添字を反転して扱い、`a*l+b` や `r-l` が入力型を超える場合にも対応する。

128 bit の**入力**で積があふれる場合の処理は `utils/mul_add_divmod.hpp` にある。答えを `i128` にすることと、入力を 128 bit まで扱うことは独立した機能。
