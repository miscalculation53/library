## 概要

ACL とだいたい同じだが、次の点が異なる：

- コンストラクタに作用つきモノイドを構造体として渡す（重要）
- コンストラクタにイテレータを渡すこともできる（必須ではないが便利）
  - たとえば（作用つき）モノイドにするときに列の要素に加えて長さなどを持っておかなければならないという場面がある。このような場合でも、列の要素を `S` のコンストラクタに渡せるように実装すれば、もとの vector をイテレータとしてセグメント木のコンストラクタに渡すことができる。

二分探索の名前は、区間積が条件を満たす境界を返すことを明示するため、ACL の `max_right`, `min_left` に `_ok` を付けている。返り値は要素の添字ではなく半開区間の境界である。

## 使用例

```cpp
vc<ll> a = {2, 1, 3, 2};
LazySegmentTree<ActedMonoidAddSum<ll>> seg(a);
auto g = [](const auto& s) { return s.val <= 4; };

assert(seg.max_right_ok(1, g) == 3);
assert(seg.min_left_ok(4, g) == 3);
```

## 詳細なドキュメント

### LazySegmentTree

#### max_right_ok

```cpp
template <class G>
int max_right_ok(int l, const G& g)
```

次の $R$ を返す。

$$
R = \max\left\{x \in \mathbb{Z} \mathrel{\middle|} l \leq x \leq n,\ g(\operatorname{prod}(l, x)) = \mathrm{true}\right\}
$$

すなわち、$[l, R)$ は条件を満たし、$R=n$ でなければ $[l, R+1)$ は条件を満たさない。

##### 制約

- $0 \leq l \leq n$
- `g(AM::e()) == true`
- $x$ を $l$ から $n$ へ増やしたとき、`g(prod(l, x))` は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\log n)$

#### min_left_ok

```cpp
template <class G>
int min_left_ok(int r, const G& g)
```

次の $L$ を返す。

$$
L = \min\left\{x \in \mathbb{Z} \mathrel{\middle|} 0 \leq x \leq r,\ g(\operatorname{prod}(x, r)) = \mathrm{true}\right\}
$$

すなわち、$[L, r)$ は条件を満たし、$L=0$ でなければ $[L-1, r)$ は条件を満たさない。

##### 制約

- $0 \leq r \leq n$
- `g(AM::e()) == true`
- $x$ を $r$ から $0$ へ減らしたとき、`g(prod(x, r))` は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\log n)$
