## 概要

基数 $B$ を固定して、文字列 $s$ に対するハッシュを $s_0 B^{n-1} + s_1 B^{n-2} + \dots + s_{n-2} B + s_{n-1}$ で定義する（向きは流派があるが本ライブラリではこちら）。これには $2$ つの使い方がある：

- 文字列全体のハッシュを持っておきながら文字列の結合を行っていくような場面。(ハッシュ値, 文字列の長さ) に適切な演算を入れるとモノイドになる。
- 部分文字列のハッシュを求める場面。文字列 $s$ に対して prefix $s[0, i)$ のハッシュ $h[i]$ を持っておけば、$s[l, r)$ のハッシュは $h[r] - h[l] B^{r-l}$ となる。これは push_back, pop_back にも対応できる。
  - 部分文字列の一致判定ができる。
  - 一致判定を用いて二分探索すると、LCP が求められる（別ファイル `lcp_compare.hpp` で実装しているほか、典型的な場面についてはこのファイルでも `calc_lcp_rh` 関数として実装）。
  - LCP を用いると、部分文字列の辞書順比較ができる（別ファイル `lcp_compare.hpp` で実装しているほか、典型的な場面についてはこのファイルでも `compare_substr_rh` 関数として実装）。

本ライブラリでは、前者を扱う `RollingHash<mint, id>` と、後者を扱う `RollingHashSubstring<mint, id>` を実装している。

---

RollingHash

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) RollingHash<mint=modint61, int id=INT_MIN>(mint val, int len)
(2) RollingHash<mint=modint61, int id=INT_MIN>(V s)
```

`id` が同じなら基数は同じになる。`LOCAL` ではすべて基数は $1000$ になる。

- (1)：ハッシュ値が $\mathrm{val}$ で長さが $\mathrm{len}$ の文字列を管理するインスタンスを作成する。
- (2)：文字列あるいは数列 $s$ を管理するインスタンスを作成する。

#### base

```cpp
static mint base()
```

基数を返す静的メンバ関数。

##### 計算量

- $O(1)$

#### hash

```cpp
mint hash()
```

ハッシュ値を返す。

##### 計算量

- $O(1)$

#### size

```cpp
I size()
```

文字列の長さを返す。

##### 計算量

- $O(1)$

#### 演算子

- `+=`, `+`：文字列の結合。通常の文字列結合と同様、交換則が成り立たないので注意。
- `==`, `!=`：（確率的な）一致判定。

##### 計算量

- $O(1)$

---

RollingHashSubstring

#### コンストラクタ

```cpp
RollingHashSubstring<mint=modint61, int id=INT_MIN>(V s)
```

`id` が同じなら基数は同じになる。`LOCAL` ではすべて基数は $1000$ になる。

#### size

```cpp
I size()
```

文字列の長さを返す。

##### 計算量

- $O(1)$

#### hash

```cpp
mint hash(int l, int r)
```

$s[l, r)$ のハッシュ値を返す。

##### 制約

- $0 \leq l \leq r \leq \lvert s \rvert$

##### 計算量

- $O(1)$

#### push_back

```cpp
void push_back(V s)
```

文字列あるいは数列 $s$ を末尾に追加する。

##### 計算量

- $O(1)$

#### pop_back

```cpp
void pop_back()
```

文字列あるいは数列の末尾の要素を削除する。

##### 計算量

- $O(1)$

-----

#### calc_lcp_rh

```cpp
I calc_lcp_rh(
  RollingHash<mint, id> rh_s, int i,
  RollingHash<mint, id> rh_t, int j
)
```

$s[i, \dots)$ と $t[j, \dots)$ の LCP の長さを返す。

##### 制約

- $0 \leq i \leq \lvert s \rvert$
- $0 \leq j \leq \lvert t \rvert$

##### 計算量

- $O(\log(\lvert s \rvert + \lvert t \rvert))$

#### compare_substr_rh

```cpp
int compare_substr_rh(
  V s, RollingHash<mint, id> rh_s, int l1, int r1,
  V t, RollingHash<mint, id> rh_t, int l2, int r2
)
```

$s[l_1, r_1)$ と $t[l_2, r_2)$ の辞書順比較の結果を返す。`<`, `==`, `>` をそれぞれ $-1$, $0$, $1$ として返す。

##### 制約

- $0 \leq l_1 \leq r_1 \leq \lvert s \rvert$
- $0 \leq l_2 \leq r_2 \leq \lvert t \rvert$

##### 計算量

- $O(\log(\lvert s \rvert + \lvert t \rvert))$
