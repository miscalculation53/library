## 概要

一次関数 $f_i(x)=a_i x+b_i$ を追加し、指定した $x$ における最小値または最大値と、それを達成する直線を求める。

- `ConvexHullTrick`：傾きを任意順に追加できる。内部は `multiset`
- `ConvexHullTrickMonotoneSlope`：傾きを単調に追加する。内部は `deque`
- `Compare = less<>`：最小値
- `Compare = greater<>`：最大値

係数、交点、クエリ座標、返り値はすべて `T` で管理する。`T` が整数型の場合は、2直線の交点を floor した整数境界を持つ。厳密な有理数座標を使う場合は `T = Rational<ll>`、浮動小数点数を使う場合は `T = long double` などとする。

`query_monotone` は広義単調な $x$ に対して、前回選んだ直線から現在位置を動かす。増加・減少方向は最初の異なる2クエリから自動で判定する。

## 使用例

```cpp
ConvexHullTrick<ll, less<>> cht;
cht.add_line(2, 3);     // id = 0
cht.add_line(-1, 8, 20);

auto [value, line] = cht.query(4);
int id = line.id;
```

傾きとクエリがどちらも単調な場合は次のように使う。増加・減少のどちらでもよいが、途中で方向を変えてはいけない。

```cpp
ConvexHullTrickMonotoneSlope<ll, less<>> cht;
repi(i, lines.size()) cht.add_line(lines[i].first, lines[i].second, i);
for (ll x : queries) answer.eb(cht.query_monotone(x).first);
```

同じ直線集合に対して別の単調クエリ列を始める場合は、間で状態をリセットする。

```cpp
for (ll x : increasing_queries) answer1.eb(cht.query_monotone(x).first);
cht.reset_monotone_query();
for (ll x : decreasing_queries) answer2.eb(cht.query_monotone(x).first);
```

有理数を使う場合は、係数もクエリも `Rational` にする。

```cpp
using R = Rational<ll>;
ConvexHullTrick<R, less<>> cht;
cht.add_line(R(2), R(3));
auto [value, line] = cht.query(R(1, 2));
```

## 詳細なドキュメント

### ConvexHullTrick

```cpp
template <class T = ll, class Compare = less<>, auto infty = INF>
struct ConvexHullTrick
```

#### コンストラクタ

```cpp
ConvexHullTrick()
```

空のCHTを構築する。

##### 制約

- `T` に必要な四則演算および大小比較が定義されている
- `T` が整数型なら符号つきである
- `Compare` は `less<>`, `less<T>`, `greater<>`, `greater<T>` のいずれか
- `infty` は空集合の最小値クエリで返してよく、すべての有限な境界より大きい正の値
- `infty` には `T` に変換できる値、または `T` に変換できる値を返す引数なし関数を指定できる

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

- 係数の符号反転、係数差、切片差、交点、評価値が `T` の範囲に収まる

##### 計算量

- $O(\log H)$ 償却

#### query

```cpp
pair<T, Line> query(const T &x)
```

$x$ における最適値と、それを達成する直線を返す。同じ最適値を取る直線が複数ある場合は、そのうち1本を返す。

空の場合、最小値クエリでは `{infty, {0, infty, -1}}`、最大値クエリでは `{-infty, {0, -infty, -1}}` を返す。

##### 計算量

- $O(\log H)$

#### query_monotone

```cpp
pair<T, Line> query_monotone(const T &x)
```

広義単調な $x$ における最適値と直線を返す。同じ $x$ を繰り返してもよい。増加・減少方向は自動で判定する。

##### 制約

- `reset_monotone_query` または最後の `add_line` 以降に渡す $x$ は広義単調

##### 計算量

- 最初のクエリ：$O(\log H)$
- 以降：クエリ $Q$ 回全体で $O(H+Q)$

#### reset_monotone_query

```cpp
void reset_monotone_query()
```

`query_monotone` が保持する現在位置、最後の $x$、増減方向を消去する。直線集合は変更しない。前の単調列と無関係な新しい単調列を始める前に呼ぶ。`add_line` と `clear` は自動でこの状態をリセットする。

##### 計算量

- $O(1)$

#### segments

```cpp
vc<Segment> segments()
```

各直線と、その直線をCHTが選ぶ境界区間 $(\mathrm{left},\mathrm{right}]$ を、$x$ の増加順に返す。最初の `left` は `-infty`、最後の `right` は `infty` になる。

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
template <class T = ll, class Compare = less<>, auto infty = INF>
struct ConvexHullTrickMonotoneSlope
```

`ConvexHullTrick` と同じ公開関数を持つ。追加する傾きの増加・減少方向は最初の異なる2傾きから自動で判定する。

##### 制約

- 追加する傾きは広義単調

##### 計算量

- `add_line`：$O(1)$ 償却
- その他：`ConvexHullTrick` と同じ
