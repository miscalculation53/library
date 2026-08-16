## 概要

$[0, 2^{\mathrm{width}})$ の整数をキーとする疎な列を Binary Trie で管理する。

各キーには可換モノイド `M` の要素が対応する。更新されていないキーの値は `M::e()` である。

キーの型 `Key` は符号付き整数型で、キー自体は非負とする。該当するキーが存在しない場合の番兵には、左端の外側を表す $-1$ と右端の外側を表す $2^{\mathrm{width}}$ を使う。どちらを返すかは各関数の説明に記載する。デフォルトの `Key = ll` では `width <= 63` であり、`uint64_t` の全域は扱わない。

```cpp
struct M
{
  using S = ...;
  static S op(const S& a, const S& b);
  static S e();
};
```

以下で「和」と呼ぶ値は、一般には `M::op` でまとめた値を表す。

$N = 2^{\mathrm{width}}$ とする。$0 \leq l \leq r \leq N$ に対し、XOR 後のキーが $[l,r)$ に入る値の和を次で表す。

$$
P_x(l,r) = \mathop{\bigoplus}_{l \leq (\mathrm{key} \mathbin{\mathtt{xor}} x) < r} \operatorname{get}(\mathrm{key})
$$

`xor_value` を受け取る問い合わせは、`key ^ xor_value` の大小でキーを並べる。キーを返す関数の戻り値は XOR 前の `key` である。XOR 後の値が必要なら、呼び出し側で戻り値に `xor_value` を XOR する。

`xor_all(x)` を呼ぶと、それ以降はキー `key` が `key ^ x` に移動したものとして扱う。キーに対応する値も一緒に移動する。

順位や要素の存在に関する問い合わせには、モノイドの値を個数へ写す関数 `cnt` を渡す。`cnt` は次を満たす必要がある。

- `cnt(M::e()) == 0`
- `cnt(M::op(a, b)) == cnt(a) + cnt(b)`
- 各キーについて `cnt(get(key))` は非負整数

`M::count` のようなモノイド固有の関数は要求しない。通常の個数を値として持つ場合は `MonoidAdd<ll>` と `identity{}` を使える。

二分探索の返り値がキー範囲内にある場合は、他の問い合わせと同様に XOR 前のキーへ戻して返す。番兵 $-1$ と $N$ には XOR を適用しない。`max_right_ok_by`, `min_left_ok_by` は `cnt` が正である実在キーだけを返す。

## 使用例

```cpp
BinaryTrie<30> bt;
const auto cnt = identity{};

bt.set(3, 2);  // 3 を 2 個持つ
bt.add(5, 1);  // 5 を 1 個追加

assert(cnt(bt.all_sum()) == 3);
assert(cnt(bt.leq_sum(3)) == 2);
assert(bt.kth_by(2LL, cnt) == 5);
assert(bt.min_element_by(cnt, 6) == 5);  // key ^ 6 が最小になる XOR 前のキー

auto pred = [](ll sum) { return sum <= 2; };
assert(bt.max_right_ng(0, pred) == 5);
assert(bt.max_right_ok_by(0, pred, cnt) == 3);

bt.set(3, 0);  // 3 を削除
```

## 詳細なドキュメント

### BinaryTrie

```cpp
template <
  int width,
  class M = MonoidAdd<ll>,
  class Key = ll
>
struct BinaryTrie
```

`Key` の下位 `width` bit をキーとして使用する。

```cpp
using Boundary = conditional_t<
  width == numeric_limits<Key>::digits,
  larger_int_t<Key>,
  Key
>;
```

`Boundary` は二分探索4関数の返り値型である。通常は `Key` と同じであり、$2^{\mathrm{width}}$ が `Key` に収まらない場合だけ `larger_int_t<Key>` になる。

#### コンストラクタ

```cpp
BinaryTrie()
```

すべてのキーに `M::e()` が対応する Binary Trie を構築する。

##### 制約

- `Key` は符号付き整数型
- $1 \leq \mathrm{width} \leq$ `numeric_limits<Key>::digits`
- `M` は可換モノイド

##### 計算量

- $O(1)$

#### reserve

```cpp
void reserve(int n)
```

少なくとも `n` 頂点分の領域をあらかじめ確保する。

##### 計算量

- 最悪 $O(V)$。$V$ は現在の頂点数

#### clear

```cpp
void clear()
```

すべての値と `xor_all` の効果を消去する。

##### 計算量

- $O(V)$。$V$ は現在の頂点数

#### get

```cpp
S get(Key key) const
```

`key` に対応する値を返す。まだ更新されていない場合は `M::e()` を返す。

##### 制約

- $0 \leq \mathrm{key} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### modify

```cpp
template <class F>
void modify(Key key, const F& f)
```

`key` に対応する値への参照を `f` に渡して変更する。未更新の場合は `M::e()` から始める。`get` と `set` を続けて呼ぶ場合と異なり、木の走査は $1$ 回で済む。

```cpp
bt.modify(key, [&](S& value)
{
  value.cnt++;
});
```

##### 制約

- $0 \leq \mathrm{key} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### set

```cpp
void set(Key key, const S& value)
```

`key` に対応する値を `value` に置き換える。

##### 制約

- $0 \leq \mathrm{key} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### add

```cpp
void add(Key key, const S& value)
```

`key` に対応する現在値を `M::op(get(key), value)` に置き換える。

##### 制約

- $0 \leq \mathrm{key} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### all_sum

```cpp
S all_sum() const
```

すべてのキーに対応する値の和を返す。

##### 計算量

- $O(1)$

#### lt_sum

```cpp
S lt_sum(Key value, Key xor_value = 0) const
```

`(key ^ xor_value) < value` を満たすキーに対応する値の和を返す。

##### 制約

- $0 \leq \mathrm{value}, \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### leq_sum

```cpp
S leq_sum(Key value, Key xor_value = 0) const
```

`(key ^ xor_value) <= value` を満たすキーに対応する値の和を返す。

##### 制約

- $0 \leq \mathrm{value}, \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### geq_sum

```cpp
S geq_sum(Key value, Key xor_value = 0) const
```

`(key ^ xor_value) >= value` を満たすキーに対応する値の和を返す。

##### 制約

- $0 \leq \mathrm{value}, \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### gt_sum

```cpp
S gt_sum(Key value, Key xor_value = 0) const
```

`(key ^ xor_value) > value` を満たすキーに対応する値の和を返す。

##### 制約

- $0 \leq \mathrm{value}, \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### max_right_ng

```cpp
template <class F>
Boundary max_right_ng(
  Key lower,
  const F& f,
  Key xor_value = 0
) const
```

次の $R$ を考える。

$$
R = \max\left\{r \in \mathbb{Z} \mathrel{\middle|} \mathrm{lower} \leq r \leq N,\ f(P_{\mathrm{xor\_value}}(\mathrm{lower},r)) = \mathrm{true}\right\}
$$

$R<N$ なら `(key ^ xor_value) == R` であるキーを返す。$R=N$ なら番兵 $N$ を返す。すなわち、右へ累積したときに条件を初めて壊すキーを返す。

##### 制約

- $0 \leq \mathrm{lower}, \mathrm{xor\_value} < 2^{\mathrm{width}}$
- `f(M::e()) == true`
- $r$ を `lower` から $N$ へ増やしたとき、$f(P_{\mathrm{xor\_value}}(\mathrm{lower},r))$ は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\mathrm{width})$

#### max_right_ok_by

```cpp
template <class F, class Count>
Boundary max_right_ok_by(
  Key lower,
  const F& f,
  const Count& cnt,
  Key xor_value = 0
) const
```

`max_right_ng` の定義に用いた $R$ に対し、次を満たすキーのうち `key ^ xor_value` が最大のものを返す。

$$
\mathrm{lower} \leq (\mathrm{key} \mathbin{\mathtt{xor}} \mathrm{xor\_value}) < R,
\qquad
\mathrm{cnt}(\operatorname{get}(\mathrm{key})) > 0
$$

該当するキーがなければ `-1` を返す。

##### 制約

- `max_right_ng` の制約を満たす
- `cnt` は概要に記載した条件を満たす

##### 計算量

- $O(\mathrm{width})$

#### min_left_ng

```cpp
template <class F>
Boundary min_left_ng(
  Key upper,
  const F& f,
  Key xor_value = 0
) const
```

$U=\mathrm{upper}+1$ とし、次の $L$ を考える。

$$
L = \min\left\{l \in \mathbb{Z} \mathrel{\middle|} 0 \leq l \leq U,\ f(P_{\mathrm{xor\_value}}(l,U)) = \mathrm{true}\right\}
$$

$L>0$ なら `(key ^ xor_value) == L - 1` であるキーを返す。$L=0$ なら番兵 `-1` を返す。すなわち、左へ累積したときに条件を初めて壊すキーを返す。

##### 制約

- $0 \leq \mathrm{upper}, \mathrm{xor\_value} < 2^{\mathrm{width}}$
- `f(M::e()) == true`
- $l$ を $U$ から $0$ へ減らしたとき、$f(P_{\mathrm{xor\_value}}(l,U))$ は `true` から `false` へ高々 $1$ 回だけ変化する

##### 計算量

- $O(\mathrm{width})$

#### min_left_ok_by

```cpp
template <class F, class Count>
Boundary min_left_ok_by(
  Key upper,
  const F& f,
  const Count& cnt,
  Key xor_value = 0
) const
```

`min_left_ng` の定義に用いた $L,U$ に対し、次を満たすキーのうち `key ^ xor_value` が最小のものを返す。

$$
L \leq (\mathrm{key} \mathbin{\mathtt{xor}} \mathrm{xor\_value}) < U,
\qquad
\mathrm{cnt}(\operatorname{get}(\mathrm{key})) > 0
$$

該当するキーがなければ番兵 $N$ を返す。

##### 制約

- `min_left_ng` の制約を満たす
- `cnt` は概要に記載した条件を満たす

##### 計算量

- $O(\mathrm{width})$

#### kth_by

```cpp
template <class Count, class F>
Key kth_by(
  Count k,
  const F& cnt,
  Key xor_value = 0
) const
```

各キーを `cnt(get(key))` 個ずつ並べ、`key ^ xor_value` の昇順に並べたときの $k$ 番目のキーを返す。戻り値は XOR 前のキーである。

##### 制約

- `cnt` は概要に記載した条件を満たす
- $0 \leq k <$ `cnt(all_sum())`
- $0 \leq \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### min_element_by

```cpp
template <class F>
Key min_element_by(
  const F& cnt,
  Key xor_value = 0
) const
```

`cnt(get(key))` が正であるキーのうち、`key ^ xor_value` が最小であるものを返す。

##### 制約

- `cnt` は概要に記載した条件を満たす
- `cnt(all_sum()) > 0`
- $0 \leq \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### max_element_by

```cpp
template <class F>
Key max_element_by(
  const F& cnt,
  Key xor_value = 0
) const
```

`cnt(get(key))` が正であるキーのうち、`key ^ xor_value` が最大であるものを返す。

##### 制約

- `cnt` は概要に記載した条件を満たす
- `cnt(all_sum()) > 0`
- $0 \leq \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### lt_max_by

```cpp
template <class F>
Key lt_max_by(
  Key value,
  const F& cnt,
  Key xor_value = 0
) const
```

`cnt(get(key))` が正かつ `(key ^ xor_value) < value` であるキーのうち、`key ^ xor_value` が最大であるものを返す。該当するキーがなければ `-1` を返す。

##### 制約

- `cnt` は概要に記載した条件を満たす
- $0 \leq \mathrm{value}, \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### leq_max_by

```cpp
template <class F>
Key leq_max_by(
  Key value,
  const F& cnt,
  Key xor_value = 0
) const
```

`cnt(get(key))` が正かつ `(key ^ xor_value) <= value` であるキーのうち、`key ^ xor_value` が最大であるものを返す。該当するキーがなければ `-1` を返す。

##### 制約

- `cnt` は概要に記載した条件を満たす
- $0 \leq \mathrm{value}, \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### geq_min_by

```cpp
template <class F>
Key geq_min_by(
  Key value,
  const F& cnt,
  Key xor_value = 0
) const
```

`cnt(get(key))` が正かつ `(key ^ xor_value) >= value` であるキーのうち、`key ^ xor_value` が最小であるものを返す。該当するキーがなければ `-1` を返す。

##### 制約

- `cnt` は概要に記載した条件を満たす
- $0 \leq \mathrm{value}, \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### gt_min_by

```cpp
template <class F>
Key gt_min_by(
  Key value,
  const F& cnt,
  Key xor_value = 0
) const
```

`cnt(get(key))` が正かつ `(key ^ xor_value) > value` であるキーのうち、`key ^ xor_value` が最小であるものを返す。該当するキーがなければ `-1` を返す。

##### 制約

- `cnt` は概要に記載した条件を満たす
- $0 \leq \mathrm{value}, \mathrm{xor\_value} < 2^{\mathrm{width}}$

##### 計算量

- $O(\mathrm{width})$

#### xor_all

```cpp
void xor_all(Key value)
```

すべてのキー `key` を `key ^ value` に移動する。キーに対応する値も一緒に移動する。

##### 制約

- $0 \leq \mathrm{value} < 2^{\mathrm{width}}$

##### 計算量

- $O(1)$
