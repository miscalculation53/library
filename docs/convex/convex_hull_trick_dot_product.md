## 概要

斉次一次式 $f_i(x,y)=a_i x+b_i y$ を追加し、指定した $(x,y)$ における最小値または最大値を求める。

係数 $(a_i,b_i)$ を平面上の点とみなすと、方向 $(x,y)$ との内積が最小・最大になる凸包頂点を求めている。

内部では $y\neq 0$ のとき $a_i x+b_i y=y(a_i(x/y)+b_i)$ と変形し、`ConvexHullTrick<Rational<T>>` を上側・下側に1個ずつ持つ。係数、切片、交点、$t=x/y$ はすべて `Rational<T>` で管理する。$y=0$ のときは係数 $a_i$ の端点を直接使う。

線の選択には通常CHTの `query_line` を使い、選ばれた一次式の内積を `Calc` で計算する。このため、内積が `T` の範囲を超えても `Calc` に収まればよい。一方、CHTの交点を作る切片差・傾き差は `Rational<T>` に格納するため `T` の範囲に収まる必要がある。交点同士の比較では `larger_int_t<T>` を使う。

上側と下側のCHTを常に両方持つため、1つのインスタンスで最小値と最大値の両方を処理できる。`convex_hull()` は上下のCHTに残った係数点から凸包を構築し、頂点を反時計回りに返す。単調クエリはこの凸包上の最適頂点をクエリの偏角に合わせて順に動かす。

- `ConvexHullTrickDotProduct`：係数を任意順に追加できる
- `ConvexHullTrickDotProductMonotoneCoefficient`：係数 $a$ を単調に追加する

`min_query_monotone`, `max_query_monotone` は偏角が単調なクエリを処理する。時計回り・反時計回りは自動で判定する。最小値用と最大値用の現在位置は別々に保持するため、同じ方向列に対して両方を交互に呼べる。一次式を追加すると単調クエリの状態は自動でリセットされる。

`Calc` は内積、係数点の外積、単調クエリでの比較に使い、既定で `larger_int_t<T>` になる。例えば `T = ll` なら `Calc = i128` である。

## 使用例

```cpp
ConvexHullTrickDotProduct<ll> cht;
cht.add(2, -3);     // id = 0
cht.add(-1, 4, 20);

i128 min_value = cht.min_query(5, 2);
i128 max_value = cht.max_query(5, 2);
auto [value, form] = cht.min_query_with_form(5, 2);
int id = form.id;
```

偏角が単調な場合は次のように使う。

```cpp
for (auto [x, y] : directions)
{
  min_answer.eb(cht.min_query_monotone(x, y));
  max_answer.eb(cht.max_query_monotone(x, y));
}
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
- Rational の比較に現れる交差積が `larger_int_t<T>` で正しく計算できる
- 内積と係数点の外積が `Calc` の範囲に収まる
- `infty` は空集合の最小値クエリで返してよい十分大きい正の値

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

一次式 $f(x,y)=ax+by$ を追加する。`id` を省略した場合は、`clear` 以降の追加順を0始まりで自動採番する。明示指定した追加も追加順には数える。`id` を指定した場合はその値をそのまま保持し、重複も許す。単調クエリの状態はリセットされる。

##### 計算量

- $O(\log H)$ 償却

#### min_query・max_query

```cpp
Calc min_query(T x, T y)
Calc max_query(T x, T y)
```

$(x,y)$ における最小値または最大値を返す。

空の場合、`min_query` は `infty`、`max_query` は `-infty` を返す。

$(x,y)=(0,0)$ の場合、空でなければ $0$ を返す。

##### 計算量

- $O(\log H)$

#### min_query_with_form・max_query_with_form

```cpp
pair<Calc, LinearForm> min_query_with_form(T x, T y)
pair<Calc, LinearForm> max_query_with_form(T x, T y)
```

$(x,y)$ における最小値または最大値と、それを達成する一次式および `id` を返す。空の場合の一次式は `{0,0,-1}` になる。$(x,y)=(0,0)$ の場合は最初に追加した一次式を返す。

##### 計算量

- $O(\log H)$

#### min_query_monotone・max_query_monotone

```cpp
Calc min_query_monotone(T x, T y)
Calc max_query_monotone(T x, T y)
```

偏角が単調な $(x,y)$ における最小値または最大値を返す。時計回り・反時計回りは自動で判定する。

##### 制約

- $(x,y) \neq (0,0)$
- `reset_monotone_query` または最後の `add` 以降、各関数へ渡す偏角は広義単調
- 偏角の列は正の $x$ 軸を境界とする一周をまたがない

##### 計算量

- 追加後の最初の単調クエリ：$O(H\log H)$
- もう一方の最小・最大クエリを初めて呼ぶとき：$O(V)$
- 以降：最小・最大それぞれについて、クエリ $Q$ 回全体で $O(V+Q)$

#### min_query_monotone_with_form・max_query_monotone_with_form

```cpp
pair<Calc, LinearForm> min_query_monotone_with_form(T x, T y)
pair<Calc, LinearForm> max_query_monotone_with_form(T x, T y)
```

偏角が単調な $(x,y)$ における最小値または最大値と、それを達成する一次式および `id` を返す。単調性の状態は値だけを返す関数と共有する。

##### 制約

- $(x,y) \neq (0,0)$
- `reset_monotone_query` または最後の `add` 以降、偏角は広義単調
- 偏角の列は正の $x$ 軸を境界とする一周をまたがない

##### 計算量

- `min_query_monotone`, `max_query_monotone` と同じ

#### reset_monotone_query

```cpp
void reset_monotone_query()
```

最小値用・最大値用の単調クエリの現在位置と回転方向を両方消去する。

##### 計算量

- $O(1)$

#### convex_hull

```cpp
vc<LinearForm> convex_hull()
```

係数点の凸包頂点を反時計回りに並べた列として返す。通常CHTの `segments` に相当する区間列はないため、係数点の凸包を取得する用途にはこの関数を使う。追加後の最初の呼び出しでは凸包全体を構築する。

##### 計算量

- 追加後の最初の呼び出し：$O(H\log H)$
- 2回目以降：$O(V)$

#### clear

```cpp
void clear()
```

すべての一次式と単調クエリの状態を消去し、自動採番を0に戻す。

##### 計算量

- $O(H)$

### ConvexHullTrickDotProductMonotoneCoefficient

```cpp
template <
  class T = ll,
  class Calc = larger_int_t<T>,
  Calc infty = Calc(INF)
>
struct ConvexHullTrickDotProductMonotoneCoefficient
```

`ConvexHullTrickDotProduct` と同じ公開関数を持つ。追加する係数 $a$ の増加・減少方向は最初の異なる2係数から自動で判定する。

##### 制約

- 追加する $a$ は広義単調

##### 計算量

- `add`：$O(1)$ 償却
- その他：`ConvexHullTrickDotProduct` と同じ
