## 概要

`default_infty<T>()` は型 `T` の既定の無限大を返す。CHT、最短路、min/max 系モノイドなどで `infty` を省略した場合は、この値を使う。

| 型 | 既定値 |
|---|---|
| `ll` など64ビット整数 | 定義されている `INF` |
| `i128`, `u128` | 128ビット整数で計算した `INF * INF` |
| `int`, `unsigned int` など32ビット整数 | `(1 << 30) - 1`（`1'073'741'823`） |
| さらに小さい整数型（`bool` を除く） | `numeric_limits<T>::max() / 2` |
| `float`, `double`, `long double` | `numeric_limits<T>::infinity()` |
| `Rational<V>` など `T::infty()` を持つ型 | `T::infty()` |
| その他 | 既定値なし。`infty` の明示指定が必要 |

`T::infty()` がある場合は、その値を優先する。`Rational` の既定値は分母0の正の無限大であり、分子・分母の型が `ll`、`i128`、`BigInteger` のいずれでも $1/0$ になる。`BigInteger` 自体には自然な無限大がないため、有限の番兵値を返す関数などを明示する。

通常の `INF = 4'000'000'000'000'000'037LL` では、各整数型の既定値は2回足してもその型に収まる。一般の中間計算までオーバーフローしないことを保証するものではない。`INF` を変更する場合は、このヘッダを含むライブラリをインクルードする前に定義する。

## 使用例

```cpp
static_assert(default_infty<ll>() == INF);
static_assert(default_infty<i128>() == i128(INF) * INF);
static_assert(default_infty<int>() == (1 << 30) - 1);

using R = Rational<ll>;
auto inf = default_infty<R>();  // 1/0
GraphDirected<R> g(n, edges);
ShortestPath sp(g);  // 到達不能は 1/0
ConvexHullTrick<ll> cht;  // 境界は INF、空集合の評価値は i128 の INF * INF
```

## 詳細なドキュメント

### default_infty

```cpp
template <class T>
T または const T& default_infty()
```

組み込み数値型では値を返し、定数式としても使える。`T::infty()` を使う場合は、最初の呼び出し時に一度だけ構築し、以後は同じ値への参照を返す。

#### 制約

- 上記の既定値が定義されている型である
- `T::infty()` を持つ型では、その関数が利用可能であり、返り値から `T` を構築できる

#### 計算量

- 組み込み数値型：$O(1)$
- `T::infty()` を使う型：初回はその関数と構築の計算量、以後は $O(1)$
