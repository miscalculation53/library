## 概要

閉半平面 $a_i x+b_i y+c_i\leq 0$ を追加し、その共通部分を求める。境界が垂直な場合も同じ形式で扱える。

内部では境界線に方向 $(-b_i,a_i)$ を与える。このとき残す半平面は境界線の左側になる。境界線を偏角順に並べ、新しい半平面の外側にある両端の交点を deque から取り除く。

共通部分が空・非有界・有界のいずれかを返す。非有界性を判定するため、内部では $[-\mathrm{infty},\mathrm{infty}]^2$ との共通部分を計算する。結果に残った内部枠の辺は公開しない。

整数型では交点の内外判定を分数のまま `Calc` で行い、頂点を返すときだけ `Real` に変換する。実数型では `eps` を許容誤差として使う。

## 使用例

```cpp
HalfPlaneIntersection<ll> hpi;
hpi.add(1, 0, -3);   // x <= 3, id = 0
hpi.add(-1, 0, -2);  // x >= -2, id = 1
hpi.add(0, 1, -5);   // y <= 5, id = 2
hpi.add(0, -1, -4);  // y >= -4, id = 3

auto res = hpi.intersection();
if (res.status == decltype(hpi)::Status::Bounded)
  for (auto [x, y] : res.vertices) answer.eb(x, y);
```

## 詳細なドキュメント

### HalfPlaneIntersection

```cpp
template <
  class T = ll,
  class Calc = larger_int_t<T>,
  class Real = long double
>
struct HalfPlaneIntersection
```

#### コンストラクタ

```cpp
HalfPlaneIntersection(T infty = T(INF), Real eps = Real(EPS))
```

空の半平面集合を構築する。`infty` は非有界判定用の内部の枠、`eps` は実数比較の許容誤差になる。

##### 制約

- `T` は符号つき整数型または浮動小数点数型
- `infty > 0`
- `eps >= 0`
- `infty` は、共通部分を内部の正方形で切ったときに元の境界がすべて現れ、すべての有限頂点が枠の内部に入るほど十分大きい

##### 計算量

- $O(1)$

#### HalfPlane・Point・Status・Result

```cpp
struct HalfPlane {
  T a, b, c;
  int id;
};

struct Point {
  Real x, y;
};

enum class Status {
  Empty,
  Unbounded,
  Bounded,
};

struct Result {
  Status status;
  vc<HalfPlane> boundaries;
  vc<Point> vertices;
};
```

`HalfPlane` は閉半平面 $ax+by+c\leq 0$ と識別用の `id` を保持する。

`boundaries` は共通部分の境界に残る、内部枠を除いた半平面を境界方向の偏角順に保持する。同じ向きの境界では最も強い制約だけが残る。

`status == Bounded` のとき、`vertices` は共通部分の頂点を反時計回りに保持する。`status == Unbounded` のときは、隣接する元の境界線同士から得られる有限頂点だけを同じ順序で保持する。

#### add

```cpp
void add(T a, T b, T c)
void add(T a, T b, T c, int id)
```

閉半平面 $ax+by+c\leq 0$ を追加する。`id` を省略した場合は、`clear` 以降の追加順を0始まりで自動採番する。明示指定した追加も追加順には数える。`id` の重複は許す。

$a=b=0$ の場合、$c\leq 0$ なら制約を無視し、$c>0$ なら共通部分は空になる。

##### 計算量

- $O(1)$

#### intersection

```cpp
Result intersection()
```

追加した半平面の共通部分を求める。何度呼んでもよく、半平面を追加した後に再び呼ぶこともできる。

##### 制約

- 共通部分は空、または内部を持つ。点・線分・直線だけからなる共通部分は対象外
- 行列式、交点の内外判定に現れる積と和が `Calc` の範囲に収まる
- 整数型の場合、有限頂点を `Real` に変換できる

##### 計算量

- 半平面の個数を $N$ として $O(N\log N)$

#### clear

```cpp
void clear()
```

すべての半平面を消去し、自動採番を0に戻す。

##### 計算量

- $O(N)$
