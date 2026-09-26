## 概要

閉区間 $[l, r]$ に含まれる整数の素因数を区間篩で求める。

`segmented_sieve(l, r)` は素因数と整数の組をイテレータで列挙する。
`segmented_factorize(l, r)` は各整数の素因数分解を配列にまとめて返す。

$\sqrt{r}$ 以下の素数で区間内の整数を割り、最後に残った $1$ より大きい因数も取り出す。
以下では $n = r-l+1$、$m = \max(n, \sqrt{r})$ とする。
列挙に使うメモリは $O(m)$、素因数分解を配列で取得する場合はさらに出力の要素数に比例するメモリを使う。

## 使用例

```cpp
for (auto [p, x] : segmented_sieve(10, 14)) {
  // (2, 10), (2, 12), (2, 14), (3, 12), (5, 10), (7, 14)
}

const auto range = segmented_sieve(94, 94);
vc<pair<ll, ll>> pairs(range.begin(), range.end());  // {{2, 94}, {47, 94}}

const ll l = 10, r = 14;
const auto fac = segmented_factorize(l, r);
for (auto pp : fac[12 - l]) {
  // (pp.p, pp.e, pp.pe) は (2, 2, 4), (3, 1, 3)
}
// fac[13 - l] は {{13, 1, 13}}
// segmented_factorize(1, 1)[0] は空配列
```

## 詳細なドキュメント

### segmented_sieve

#### コンストラクタ

```cpp
segmented_sieve(ll l, ll r);
```

次の条件を満たす組 $(p, x)$ をそれぞれ一度ずつ列挙する範囲を作る。

- $l \leq x \leq r$
- $p$ は $x$ の素因数
- $p \neq x$

まず $\sqrt{r}$ 以下の素因数について $p$ の昇順、同じ $p$ では $x$ の昇順に列挙する。
その後、残りの素因数を $x$ の昇順に列挙する。

##### 制約

- $1 \leq l \leq r$

##### 計算量

- $O(1)$

#### begin / end

```cpp
Iterator begin() const;
Iterator end() const;
```

`begin()` で篩の作業領域を用意し、先頭の組を指す入力イテレータを返す。
`end()` は列挙の終端を返す。
`*it` は `pair<ll, ll>` 型の $(p, x)$ で、`++it` によって篩を進める。

イテレータのコピーは走査状態を共有する。独立に走査するときは、改めて `begin()` を呼ぶ。
範囲オブジェクトの寿命が終わった後も、取得済みのイテレータは使える。
列挙中に `LinearSieve` の共有テーブルを拡張したり、別の区間を列挙したりできる。

##### 計算量

- `begin()`：$O(m)$
- `end()`、イテレータのコピー、値の取得、比較：$O(1)$
- `begin()` から終端までの列挙全体：$O(m\log\log(m+2))$

### 関数

#### segmented_factorize

```cpp
vvc<PrimePower<ll>> segmented_factorize(ll l, ll r);
```

長さ $r-l+1$ の配列 `res` を返す。`res[x-l]` は $x$ の素因数分解で、
各要素に素因数 $p$、指数 $e$、素べき $p^e$ を持ち、$p$ の昇順に並ぶ。

$x=1$ の要素は空配列。$x$ が素数なら `PrimePower<ll>(x, 1, x)` を一つ持つ。

##### 制約

- $1 \leq l \leq r$

##### 計算量

- $O(m\log\log(m+2))$
