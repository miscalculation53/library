## 概要

一次関数 $f_i(x)=a_i x+b_i$ を追加し、指定した $x$ における最小値または最大値と、それを達成する直線を求める。

- `ConvexHullTrick`：傾きを任意順に追加できる。内部は `multiset`
- `ConvexHullTrickMonotoneSlope`：傾きを単調に追加する。内部は `deque`
- `Compare = less<>`：最小値
- `Compare = greater<>`：最大値

係数 $a,b$、クエリ座標 $x$、交点の境界は `T` で管理し、評価値 $ax+b$ は `U` で計算して返す。`T` は必須で、`U` の既定値は `larger_int_t<T>` である。例えば `T = ll` なら `U = i128`、`T = Rational<ll>` なら `U = Rational<i128>` になる。

型引数は `T, U, Compare, inftyT, inftyU` の順に指定する。評価の中間計算も `ll` に収まる場合は `ConvexHullTrick<ll, ll>` とできる。`U` を広げても、係数の符号反転や交点を作る差の計算は `T` のままである。

`inftyT` は境界と空集合で返す直線の切片に、`inftyU` は空集合で返す評価値に使う。省略時（`nullptr`）は、それぞれ `T`, `U` の[既定の無限大](../utils/default_infty.md)を使う。例えば `ConvexHullTrick<ll>` の境界は `INF`、空集合の評価値は `i128` の `INF * INF` になる。値や関数を明示した場合はその指定を使う。非空のクエリ結果をこれらの値で打ち切る処理は行わない。

`T` が整数型の場合は、2直線の交点を floor した整数境界を持つ。厳密な有理数座標を使う場合は `T = Rational<ll>`、浮動小数点数を使う場合は `T = long double` などとする。

`query_monotone` は広義単調な $x$ に対して、前回選んだ直線から現在位置を動かす。増加・減少方向は最初の異なる2クエリから自動で判定する。

## 使用例

```cpp
ConvexHullTrick<ll> cht;
cht.add_line(2, 3);     // id = 0
cht.add_line(-1, 8, 20);

auto [value, line] = cht.query(4);
// value は i128、line.a と line.b は ll
int id = line.id;
```

返り値を `ll` にする場合や、最大値を求める場合は次のように指定する。

```cpp
ConvexHullTrick<ll, ll> small_value_cht;
ConvexHullTrick<ll, i128, greater<>> max_cht;
```

境界用と返り値用の無限大は型ごとに選ばれる。有理数では指定を省略すると $1/0$ になる。片方だけを明示することもできる。

```cpp
ConvexHullTrick<int> integer_cht;  // 境界は (1 << 30) - 1、返り値用は ll の INF
using R = Rational<ll>;
using W = Rational<i128>;
ConvexHullTrick<R, W> rational_cht;  // 境界用・返り値用ともに 1/0
ConvexHullTrick<R, W, less<>, nullptr, INF> finite_empty_cht;
```

`INF` を明示すると、有理数でも有限値 `INF/1` として扱う。従来の有限の既定値を両方に使いたい場合は、第4・第5引数に `INF, INF` を指定する。

傾きとクエリがどちらも単調な場合は次のように使う。増加・減少のどちらでもよいが、途中で方向を変えてはいけない。

```cpp
ConvexHullTrickMonotoneSlope<ll> cht;
repi(i, lines.size()) cht.add_line(lines[i].first, lines[i].second, i);
for (ll x : queries) answer.eb(cht.query_monotone(x).first);
```

同じ直線集合に対して別の単調クエリ列を始める場合は、間で状態をリセットする。

```cpp
for (ll x : increasing_queries) answer1.eb(cht.query_monotone(x).first);
cht.reset_monotone_query();
for (ll x : decreasing_queries) answer2.eb(cht.query_monotone(x).first);
```

有理数を使う場合は、係数もクエリも `Rational` にする。整数係数に有理数座標をクエリする場合も、係数を分母1の `Rational` として追加する。

```cpp
using R = Rational<ll>;
ConvexHullTrick<R> cht;
cht.add_line(R(2), R(3));
auto [value, line] = cht.query(R(1, 2));
// value は Rational<i128>、line.a と line.b は Rational<ll>
```

返り値も `Rational<ll>` にする場合は `ConvexHullTrick<R, R>` と指定する。異なる `Rational` 型への変換は明示的に行う。

```cpp
Rational<i128> expected(R(4));
assert(value == expected);
```

直線だけが必要な場合も `query(x).second` を使う。従来の `query_line(x)` は `query(x).second`、`query_line_monotone(x)` は `query_monotone(x).second` に置き換える。どちらも値の計算を行うため、中間計算と結果が `U` に収まる必要がある。

```cpp
auto best_line = cht.query(R(1, 2)).second;
```

## 詳細なドキュメント

### ConvexHullTrick

```cpp
template <class T, class U = larger_int_t<T>, class Compare = less<>,
          auto inftyT = nullptr, auto inftyU = nullptr>
struct ConvexHullTrick
```

#### コンストラクタ

```cpp
ConvexHullTrick()
```

空のCHTを構築する。

##### 制約

- `T` に必要な四則演算および大小比較が定義されている
- `U` に `T` からの明示変換、加算、乗算、符号反転が定義されている
- `T`, `U` が整数型なら符号つきである
- `Compare` は `less<>`, `less<T>`, `greater<>`, `greater<T>` のいずれか
- `inftyT` は、すべての有限な境界の絶対値より大きい正の値で、変換後の値とその符号反転が `T` に収まる
- `inftyU` は空集合の最小値クエリで返してよい正の値で、変換後の値とその符号反転が `U` に収まる
- 各引数には、対応する型に変換できる値、またはそのような値を返す引数なし関数を指定できる
- `inftyT` が `U` に収まる必要も、`inftyU` が `T` に収まる必要もない

##### 計算量

- $O(1)$

#### Line・Segment

```cpp
struct Line {
  T a, b;
  int id;
};

struct Segment {
  Line line;
  T left, right;
};
```

`Line` は直線 $y=ax+b$ と追加時に指定した `id` を保持する。`Segment` はその直線と、CHTがその直線を選ぶ境界区間 $(\mathrm{left},\mathrm{right}]$ を保持する。

#### add_line

```cpp
void add_line(T a, T b)
void add_line(T a, T b, int id)
```

直線 $f(x)=ax+b$ を追加する。`id` を省略した場合は、`clear` 以降の追加順を0始まりで自動採番する。明示指定した追加も追加順には数える。単調クエリの状態は自動でリセットされる。

##### 制約

- 係数の符号反転、係数差、切片差、交点が `T` の範囲に収まる
- `T = Rational<V>` の場合、未約分の分子・分母が `V` に収まり、比較の交差積が `larger_int_t<V>` に収まる

##### 計算量

- $O(\log H)$ 償却

#### query

```cpp
pair<U, Line> query(const T &x) const
```

$x$ における最適値と、それを達成する直線を返す。同じ最適値を取る直線が複数ある場合は、そのうち1本を返す。

評価は `U(a) * U(x) + U(b)` で行う。返される `Line` の係数は `T` のままである。

空の場合、最小値クエリでは `{eU, {0, eT, -1}}`、最大値クエリでは `{-eU, {0, -eT, -1}}` を返す。`eT` は `inftyT` を `T` として解決した値、`eU` は `inftyU` を `U` として解決した値である。空集合を表す直線の切片と返り値は、それぞれの無限大を使うため一致するとは限らない。

##### 制約

- 評価の中間計算と結果が `U` の範囲に収まる
- `U = Rational<V>` の場合、評価の未約分の分子・分母が `V` の範囲に収まる

##### 計算量

- $O(\log H)$

#### query_monotone

```cpp
pair<U, Line> query_monotone(const T &x)
```

広義単調な $x$ における最適値と直線を返す。同じ $x$ を繰り返してもよい。増加・減少方向は自動で判定する。

##### 制約

- 評価値については `query` と同じ制約を満たす
- `reset_monotone_query` または最後の `add_line` 以降、`query_monotone` に渡す $x$ は広義単調

##### 計算量

- 最初のクエリ：$O(\log H)$
- 以降：クエリ $Q$ 回全体で $O(H+Q)$

#### reset_monotone_query

```cpp
void reset_monotone_query()
```

`query_monotone` の現在位置、最後の $x$、増減方向を消去する。直線集合は変更しない。前の単調列と無関係な新しい単調列を始める前に呼ぶ。`add_line` と `clear` は自動でこの状態をリセットする。

##### 計算量

- $O(1)$

#### segments

```cpp
vc<Segment> segments() const
```

各直線と、その直線をCHTが選ぶ境界区間 $(\mathrm{left},\mathrm{right}]$ を、$x$ の増加順に返す。最初の `left` は `-eT`、最後の `right` は `eT` になる。ここで `eT` は `inftyT` を `T` として解決した値である。

##### 制約

- 有限な境界の絶対値は、`inftyT` を `T` として解決した値より小さい

##### 計算量

- $O(H)$

#### clear

```cpp
void clear()
```

すべての直線と単調クエリの状態を消去し、自動採番を0に戻す。

##### 計算量

- $O(H)$

### ConvexHullTrickMonotoneSlope

```cpp
template <class T, class U = larger_int_t<T>, class Compare = less<>,
          auto inftyT = nullptr, auto inftyU = nullptr>
struct ConvexHullTrickMonotoneSlope
```

`ConvexHullTrick` と同じ公開関数を持つ。追加する傾きの増加・減少方向は最初の異なる2傾きから自動で判定する。

##### 制約

- 追加する傾きは広義単調

##### 計算量

- `add_line`：$O(1)$ 償却
- その他：`ConvexHullTrick` と同じ
