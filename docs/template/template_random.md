## テンプレート（ランダム生成）

主にランダムテストで使うことを目的としているが、解法が乱択の場合にも便利かも。

### グローバル変数

- `mt19937_64 mt`
  - `LOCAL` では seed は固定となり、オンラインジャッジでは毎回異なる seed となる（ようにテンプレの `init` 関数に書いている）

### 関数

これらはすべて `mt` を使っている。

#### randint, randrange

```cpp
(1) ll randint(ll l, ll r)
(2) ll randrange(ll l, ll r)
```

- (1)：$[l, r]$ から一様ランダムに選んだ整数を $1$ つ返す。
- (2)：$[l, r)$ から一様ランダムに選んだ整数を $1$ つ返す。

##### 制約

- (1)：$l \leq r$
- (2)：$l \lt r$

##### 計算量

- $O(1)$


#### random_sample_range

```cpp
(1) array<ll, k> random_sample_range_array<int k, bool does_sort>(ll l, ll r)
(2) vc<ll> random_sample_range_vector<bool does_sort>(ll l, ll r, int k)
```

$[l, r)$ の範囲から相異なる $k$ 個の整数を一様ランダムに選び、(1) は array で、(2) は vector で返す。`does_sort` が true ならソートされて返り、false ならシャッフルされて返る。

##### 制約

- $r - l \geq k$

##### 計算量

- $O(k\log k)$

##### 使用例

$1 \leq a \lt b \leq n$ なる $a, b$ を生成したいとき

```cpp
auto [a, b] = random_sample_range_array<2, true>(1, n + 1)
```