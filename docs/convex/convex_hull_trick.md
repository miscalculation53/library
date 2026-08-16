## 概要

一次関数 $f_i(x)=a_i x+b_i$ を追加し、指定した $x$ における最小値または最大値を求める。

- `ConvexHullTrick`：傾きを任意順に追加できる。内部は `multiset`
- `ConvexHullTrickMonotoneSlope`：傾きを単調に追加する。内部は `deque`
- `Compare = less<>`：最小値
- `Compare = greater<>`：最大値

`query_monotone` は単調な $x$ に対するクエリを処理する。増加・減少方向は最初の異なる2クエリから自動で判定する。直線を追加すると単調クエリの状態は自動でリセットされる。

係数型 `T` と、交点およびクエリ座標の型 `X` を別々に指定できる。既定では `X = T` である。整数係数に有理数座標を渡す場合は `X = Rational<ll>`、浮動小数点数で交点を管理する場合は `X = long double` とする。返り値の型 `Value` は `decltype(T() * X() + T())` になる。

`T` が整数型かつ `X = T` の場合だけ、交点を floor した整数境界で管理する。それ以外の場合は `X(切片差) / X(傾き差)` を境界にする。したがって `Rational<ll>` なら厳密な有理数境界になる。

`X = long double` は無条件に厳密ではない。係数と、有理数クエリ $u/v$ に現れる整数の絶対値がすべて $B$ 以下、`long double` の仮数精度が $p$ bit、除算が最近接丸めである場合、$8B^2 < 2^p$ なら交点同士および交点とクエリの順序は丸めで逆転しない。交点の分子・分母の絶対値は $2B$ 以下であり、この範囲の異なる2分数の相対的な間隔は $1/(4B^2)$ 以上だからである。この条件が保証するのは選ばれる直線であり、`query` が返す値自体は浮動小数点数による近似値になる。この実装は $B$ を受け取らないため、利用者が条件を確認する。

## 使用例

```cpp
ConvexHullTrick<ll, less<>> cht;
cht.add_line(2, 3);     // id = 0
cht.add_line(-1, 8, 20);

ll value = cht.query(4);
auto [value2, line] = cht.query_with_line(4);
int id = line.id;
```

傾きとクエリがどちらも単調な場合は次のように使う。傾きとクエリは増加・減少のどちらでもよいが、途中で方向を変えてはいけない。

```cpp
ConvexHullTrickMonotoneSlope<ll, less<>> cht;
repi(i, lines.size()) cht.add_line(lines[i].first, lines[i].second, i);
for (ll x : queries) answer.eb(cht.query_monotone(x));
```

係数を整数のまま保ち、有理数 $x$ を渡す場合は次のようにする。

```cpp
using R = Rational<ll>;
ConvexHullTrick<ll, less<>, INF, R> cht;
cht.add_line(2, 3);
R value = cht.query(R(1, 2));
```

## 詳細なドキュメント

### ConvexHullTrick

```cpp
template <class T = ll, class Compare = less<>, auto infty = INF, class X = T>
struct ConvexHullTrick
```

#### コンストラクタ

```cpp
ConvexHullTrick()
```

空のCHTを構築する。

##### 制約

- `T` と `X` に必要な四則演算および大小比較が定義されている
- `T` が整数型なら符号つきである
- `Compare` は `less<>`, `less<T>`, `greater<>`, `greater<T>` のいずれか
- `infty` は空集合の最小値クエリで返してよい十分大きい正の値

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
  X left, right;
};
```

`Line` は直線 $y=ax+b$ と追加時に指定した `id` を保持する。`Segment` はその直線と、CHTがその直線を選ぶ境界区間 $(\mathrm{left},\mathrm{right}]$ を保持する。

#### add_line

```cpp
void add_line(T a, T b)
void add_line(T a, T b, int id)
```

直線 $f(x)=ax+b$ を追加する。`id` を省略した場合は、`clear` 以降の追加順を0始まりで自動採番する。明示指定した追加も追加順には数える。`id` を指定した場合はその値をそのまま保持し、重複も許す。単調クエリの状態はリセットされる。

##### 制約

- 係数の符号反転、係数差、切片差が `T` の範囲に収まる

##### 計算量

- $O(\log H)$ 償却

#### query・query_line・query_with_line

```cpp
Value query(const X& x)
Line query_line(const X& x)
pair<Value, Line> query_with_line(const X& x)
```

`x` における最適値を返す。`query_line` は最適な直線だけを、`query_with_line` は値と直線の両方を返す。`query_line` は $ax+b$ を計算しないため、最適な直線だけが必要な場合に使える。

空の場合、最小値クエリでは `infty`、最大値クエリでは `-infty` を返す。`query_line` と `query_with_line` の直線はそれぞれ `{0, infty, -1}`、`{0, -infty, -1}` になる。

##### 制約

- `query` と `query_with_line` では $ax+b$ が `Value` の範囲に収まる

##### 計算量

- $O(\log H)$

#### query_monotone・query_monotone_line・query_monotone_with_line

```cpp
Value query_monotone(const X& x)
Line query_monotone_line(const X& x)
pair<Value, Line> query_monotone_with_line(const X& x)
```

単調な $x$ における最適値または直線を返す。増加・減少方向は自動で判定する。同じ値を繰り返してもよい。

##### 制約

- `reset_monotone_query` または最後の `add_line` 以降に渡す $x$ は広義単調

##### 計算量

- 最初のクエリ：$O(\log H)$
- 以降：クエリ $Q$ 回全体で $O(H+Q)$

#### reset_monotone_query

```cpp
void reset_monotone_query()
```

単調クエリの現在位置と増減方向を消去する。

##### 計算量

- $O(1)$

#### segments

```cpp
vc<Segment> segments()
```

各直線と、その直線をCHTが選ぶ境界区間 $(\mathrm{left},\mathrm{right}]$ を、$x$ の増加順に返す。最初の `left` は `-infty`、最後の `right` は `infty` になる。境界で複数の直線が同じ値を取る場合、そのうち1本に割り当てる。

##### 制約

- 有限な境界の絶対値は `infty` 以下

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
template <class T = ll, class Compare = less<>, auto infty = INF, class X = T>
struct ConvexHullTrickMonotoneSlope
```

`ConvexHullTrick` と同じ公開関数を持つ。追加する傾きの増加・減少方向は最初の異なる2傾きから自動で判定する。

##### 制約

- 追加する傾きは広義単調

##### 計算量

- `add_line`：$O(1)$ 償却
- その他：`ConvexHullTrick` と同じ
