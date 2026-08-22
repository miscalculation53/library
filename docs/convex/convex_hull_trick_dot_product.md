## 概要

斉次一次式 $f_i(x,y)=a_i x+b_i y$ を追加し、指定した $(x,y)$ における最小値または最大値と、それを達成する一次式を求める。

内部では $y\neq 0$ のとき

$$
a_i x+b_i y=y\left(a_i\frac{x}{y}+b_i\right)
$$

と変形し、`ConvexHullTrick<Rational<Calc>>` を最小値用・最大値用に1個ずつ持つ。$y>0$ ではそのまま、$y<0$ では最小と最大を交換する。$y=0$ のときは係数 $a_i$ の端点を直接使う。

- `ConvexHullTrickDotProduct`：係数 $a$ を任意順に追加できる
- `ConvexHullTrickDotProductMonotoneSlope`：係数 $a$ を単調に追加する

`min_query_monotone`, `max_query_monotone` は、$x/y$ が広義単調なクエリ列を内部 CHT の `query_monotone` へ委譲する。$y$ の符号が変わってもよい。

`Calc` は最終的な内積と内部の有理数計算に使い、既定で `larger_int_t<T>` になる。例えば `T = ll` なら `Calc = i128` である。

## 使用例

```cpp
ConvexHullTrickDotProduct<ll> cht;
cht.add(2, -3);     // id = 0
cht.add(-1, 4, 20);

auto [min_value, min_form] = cht.min_query(5, 2);
auto [max_value, max_form] = cht.max_query(5, 2);
```

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
  class T = ll,
  class Calc = larger_int_t<T>,
  Calc infty = Calc(INF)
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
- CHTの交点を作る係数差が `T` の範囲に収まる
- 内部の `Rational<Calc>` の演算結果が `Calc` の範囲に収まる
- 内積が `Calc` の範囲に収まる
- `infty` は空集合の最小値クエリで返してよく、内部 CHT のすべての有限な境界より大きい正の値

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
pair<Calc, LinearForm> min_query(T x, T y)
pair<Calc, LinearForm> max_query(T x, T y)
```

$(x,y)$ における最小値または最大値と、それを達成する一次式を返す。

空の場合はそれぞれ `{infty, {0,0,-1}}`, `{-infty, {0,0,-1}}` を返す。$(x,y)=(0,0)$ の場合は、値 $0$ と最初に追加した一次式を返す。

##### 計算量

- $O(\log H)$

#### min_query_monotone・max_query_monotone

```cpp
pair<Calc, LinearForm> min_query_monotone(T x, T y)
pair<Calc, LinearForm> max_query_monotone(T x, T y)
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
vc<LinearForm> convex_hull()
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
  class T = ll,
  class Calc = larger_int_t<T>,
  Calc infty = Calc(INF)
>
struct ConvexHullTrickDotProductMonotoneSlope
```

`ConvexHullTrickDotProduct` と同じ公開関数を持つ。$a_i(x/y)+b_i$ における傾き $a_i$ の増加・減少方向は、最初の異なる2係数から自動で判定する。

##### 制約

- 追加する $a$ は広義単調

##### 計算量

- `add`：$O(1)$ 償却
- その他：`ConvexHullTrickDotProduct` と同じ
