## 概要

斉次一次式 $f_i(x,y)=a_i x+b_i y$ を追加し、指定した $(x,y)$ における最小値または最大値と、それを達成する一次式を求める。

内部では $y\neq 0$ のとき

$$
a_i x+b_i y=y\left(a_i\frac{x}{y}+b_i\right)
$$

と変形し、係数・座標型が `Rational<T>`、評価値型が `Rational<U>` のCHTを最小値用・最大値用に1個ずつ持つ。$y>0$ ではそのまま、$y<0$ では最小と最大を交換する。$y=0$ のときは係数 $a_i$ の端点を直接使う。

- `ConvexHullTrickDotProduct`：係数 $a$ を任意順に追加できる
- `ConvexHullTrickDotProductMonotoneSlope`：係数 $a$ を単調に追加する

`min_query_monotone`, `max_query_monotone` は、$x/y$ が広義単調なクエリ列を内部 CHT の `query_monotone` へ委譲する。$y$ の符号が変わってもよい。

`T` は係数 $a,b$ とクエリ座標 $x,y$ の型で、必ず指定する。`U` は内積の計算と返り値の型で、既定値は `larger_int_t<T>` である。例えば `T = ll` なら `U = i128` になる。内積の中間計算も `ll` に収まる場合は `ConvexHullTrickDotProduct<ll, ll>` と指定できる。型引数の順序は `T, U, inftyU` である。

公開引数 `inftyU` は空集合の返り値に使う。省略時（`nullptr`）は `U` の[既定の無限大](../utils/default_infty.md)を使う。例えば `ConvexHullTrickDotProduct<ll>` では `U = i128` なので `INF * INF` になる。従来と同じ有限値を返したい場合は、第3引数に `INF` を明示する。

内部CHTの `query` または `query_monotone` が返す `(値, 直線)` の直線を使い、内積を `U(a) * U(x) + U(b) * U(y)` で計算する。内部でも $a(x/y)+b$ を `Rational<U>` で評価するため、分子・分母の中間計算や符号反転も含め、`U` に十分な余裕を持って収まることを仮定する。

交点の分子・分母と符号の正規化は `T`、有理数比較の交差積は `larger_int_t<T>` を使う。`U` を広げても、これらの型は変わらない。`T = ll, U = i128` の場合、比較は `Rational<ll>` の処理として `i128` で行い、`larger_int_t<i128>` は使わない。

## 使用例

```cpp
ConvexHullTrickDotProduct<ll> cht;
cht.add(2, -3);     // id = 0
cht.add(-1, 4, 20);

auto [min_value, min_form] = cht.min_query(5, 2);
auto [max_value, max_form] = cht.max_query(5, 2);
// min_value と max_value は i128、一次式の係数は ll
```

返り値を `ll` にする場合は、次のように指定する。

```cpp
ConvexHullTrickDotProduct<ll, ll> small_value_cht;
```

整数係数の直線 $at+b$ に、有理数座標 $t=p/q$ をクエリする場合は、$q>0$ として `(p,q)` を渡す。取得した内積が答えの分子になる。

```cpp
auto [numerator, form] = cht.min_query(1, 2);
Rational<i128> value(numerator, i128(2));  // t = 1/2 における最小値
```

この場合も公開型は `T = ll, U = i128` であり、`Rational<ll>, Rational<i128>` は内部CHTに渡す型である。

$y\neq 0$ かつ $x/y$ が単調な場合は次のように使う。

```cpp
for (auto [x, y] : ratio_monotone_queries)
  answer.eb(cht.min_query_monotone(x, y).first);
```

別の単調クエリ列を始める場合は、間で状態をリセットする。

```cpp
cht.reset_monotone_query();
```

## 詳細なドキュメント

### ConvexHullTrickDotProduct

```cpp
template <
  class T,
  class U = larger_int_t<T>,
  auto inftyU = nullptr
>
struct ConvexHullTrickDotProduct
```

#### コンストラクタ

```cpp
ConvexHullTrickDotProduct()
```

空のDot Product CHTを構築する。

##### 制約

- `T` は符号つき整数型または浮動小数点数型
- 係数の符号反転、交点を作る係数差、`Rational<T>(x,y)` の符号正規化後の分子・分母が `T` の範囲に収まる
- 内部の `Rational<T>` の比較で使う交差積が `larger_int_t<T>` の範囲に収まる
- `U` に `T` からの明示変換、加算、乗算、符号反転が定義されている
- 内積の中間計算と結果が `U` の範囲に収まる
- $y\neq 0$ の場合、内部で評価する $a(x/y)+b$ の分子・分母とその中間計算も `U` の範囲に収まる
- `inftyU` は空集合の最小値クエリで返してよい正の値で、符号反転も `U` に収まる
- `inftyU` には `U` に変換できる値、または `U` に変換できる値を返す引数なし関数を指定できる

内部CHTには、境界用の `inftyT` に `Rational<T>::infty`、評価値用の `inftyU` に公開引数の指定を渡す。境界は常に分母0の正の無限大になる。内部の評価値用は、指定が `nullptr` なら `Rational<U>` の既定値 $1/0$、値や関数が指定されていればその値を `Rational<U>` に変換する。公開引数 `inftyU` は境界には使わないため、`T` に収まる必要も、有限な交点より大きい必要もない。空集合のクエリは外側で処理する。

##### 計算量

- $O(1)$

#### LinearForm

```cpp
struct LinearForm {
  T a, b;
  int id;
};
```

斉次一次式 $ax+by$ と、追加時に指定した `id` を保持する。

#### add

```cpp
void add(T a, T b)
void add(T a, T b, int id)
```

一次式 $f(x,y)=ax+by$ を追加する。`id` を省略した場合は、`clear` 以降の追加順を0始まりで自動採番する。明示指定した追加も追加順には数える。単調クエリの状態は自動でリセットされる。

##### 計算量

- $O(\log H)$ 償却

#### min_query・max_query

```cpp
pair<U, LinearForm> min_query(T x, T y) const
pair<U, LinearForm> max_query(T x, T y) const
```

$(x,y)$ における最小値または最大値を `U` で返し、それを達成する一次式を添える。一次式の係数は `T` のままである。

空の場合はそれぞれ `{eU, {0,0,-1}}`, `{-eU, {0,0,-1}}` を返す。ここで `eU` は公開引数 `inftyU` を `U` として解決した値である。$(x,y)=(0,0)$ の場合は、値 $0$ と最初に追加した一次式を返す。

##### 計算量

- $O(\log H)$

#### min_query_monotone・max_query_monotone

```cpp
pair<U, LinearForm> min_query_monotone(T x, T y)
pair<U, LinearForm> max_query_monotone(T x, T y)
```

$x/y$ が広義単調なクエリ列に対し、最小値または最大値と一次式を返す。$x/y$ の増加・減少方向は内部 CHT が自動で判定する。$y$ の符号が変わってもよい。

##### 制約

- $y\neq 0$
- `reset_monotone_query` または最後の `add` 以降、$x/y$ は広義単調

##### 計算量

- 各内部 CHT に対する最初のクエリ：$O(\log H)$
- 以降：クエリ $Q$ 回全体で $O(H+Q)$

#### reset_monotone_query

```cpp
void reset_monotone_query()
```

内部の最小値用・最大値用 CHT が保持する現在位置、最後の $x/y$、増減方向を消去する。追加した一次式は変更しない。別の単調クエリ列を始める前に呼ぶ。`add` と `clear` は自動でこの状態をリセットする。

##### 計算量

- $O(1)$

#### convex_hull

```cpp
vc<LinearForm> convex_hull() const
```

係数点 $(a,b)$ の凸包頂点を反時計回りに返す。最小値用 CHT の下側凸包と最大値用 CHT の上側凸包を `segments()` で取得してつなぐ。

##### 計算量

- $O(H)$

#### clear

```cpp
void clear()
```

すべての一次式と単調クエリの状態を消去し、自動採番を0に戻す。

##### 計算量

- $O(H)$

### ConvexHullTrickDotProductMonotoneSlope

```cpp
template <
  class T,
  class U = larger_int_t<T>,
  auto inftyU = nullptr
>
struct ConvexHullTrickDotProductMonotoneSlope
```

`ConvexHullTrickDotProduct` と同じ公開関数を持つ。$a_i(x/y)+b_i$ における傾き $a_i$ の増加・減少方向は、最初の異なる2係数から自動で判定する。

##### 制約

- 追加する $a$ は広義単調

##### 計算量

- `add`：$O(1)$ 償却
- その他：`ConvexHullTrickDotProduct` と同じ
