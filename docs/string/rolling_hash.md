## 概要

基数 $B$ を固定し、列 $s$ のハッシュを

$$
s_0 B^{n-1} + s_1 B^{n-2} + \dots + s_{n-1}
$$

で定義する。ハッシュの一致による判定は確率的である。

用途に応じて次の型を使う。

- `RollingHashValue`：ある列のハッシュと長さを表す値。値どうしを $O(1)$ で連結できる。
- `RollingHash`：一つの元列を前計算し、任意の連続部分列のハッシュを $O(1)$ で求める。
- `RollingHashConcat`：複数の `RollingHash` の断片を順に追加し、断片を連結した仮想的な列を扱う。

`RollingHash`、`RollingHashConcat` と、それらの `substr` が返す view では、部分列のハッシュ、LCP、LCS、辞書順比較を行える。また、二つの view に対する `x + y` は、要素をコピーせずに $xy$ を表す view を返す。二つの view の元列は異なってもよい。

`a + b + c + ...` は断片を入れ子にした軽量な非所有 view を作る。少数の断片を一時的に比較する用途に向く。`RollingHashConcat` は断片を vector で管理し、断片を繰り返し追加する場合や、断片数が多い場合に向く。

`RollingHashValue += RollingHashValue` は常に $O(1)$、`RollingHashConcat += RollingHash` または `RollingHashConcat += RollingHashRange` は償却 $O(1)$ である。断片数に比例する処理が隠れないよう、`RollingHashConcat += RollingHashConcat` は定義していない。

追加した各断片は元の `RollingHash` への参照を保持する。利用中は、追加元のすべての `RollingHash` とその元列を破棄・移動・変更してはならない。異なる元列でも `mint` と `id` が同じなら同じ基数を使うため、一つの `RollingHashConcat` に混在させられる。

## 使用例

```cpp
string s = "abracadabra";
RollingHash rh(s);

auto x = rh.substr(0, 4);
auto y = rh.substr(7, 11);
bool same = (x == y);

RollingHashConcat t;
t += rh.substr(3, 7);
t += rh.substr(0, 4);

RollingHashConcat best = rh.substr();
auto candidate = rh.substr(0, 3) + rh.substr(4, 7) + rh.substr(8);
chmin(best, candidate);

auto a = t.substr(1, 6);
auto b = rh.substr(2, 7);
int cmp = a.compare(b);  // a < b, a == b, a > b のとき -1, 0, 1

auto ab = a + b;
auto ba = b + a;
bool less = (ab < ba);

string restored = t.content();
dump(t);  // LOCAL では仮想列の実際の内容を表示する
```

## 詳細なドキュメント

### RollingHashValue

#### コンストラクタ

```cpp
(1) RollingHashValue<mint=modint61, int id=INT_MIN>()
(2) RollingHashValue<mint=modint61, int id=INT_MIN>(V s)
```

- (1)：空列のハッシュを作る。
- (2)：列 `s` のハッシュを作る。

`mint` と `id` が同じ型は同じ基数を使用する。`LOCAL` では基数は $1000$ になる。

##### 計算量

- (1)：$O(1)$
- (2)：$O(\lvert s \rvert)$

#### value

```cpp
mint value()
```

ハッシュ値を返す。

##### 計算量

- $O(1)$

#### size

```cpp
ll size()
```

列の長さを返す。

##### 計算量

- $O(1)$

#### 演算子

- `+=`, `+`：列を連結したハッシュを求める。
- `==`, `!=`：長さとハッシュ値を比較する。

`RollingHashValue` は $B^{\lvert s \rvert}$ も値の一部として保持するため、論理的な列の長さが大きくても連結の計算量は変わらない。

##### 計算量

- $O(1)$

#### remove_prefix

```cpp
RollingHashValue remove_prefix(RollingHashValue prefix)
```

管理する列から `prefix` と同じ長さの接頭辞を取り除いた列のハッシュを返す。`prefix` が実際の接頭辞であるかは検査しない。

##### 制約

- `prefix.size() <= size()`

##### 計算量

- $O(1)$

### RollingHash

#### コンストラクタ

```cpp
RollingHash<V=string, mint=modint61, int id=INT_MIN>(const V& s)
```

元列 `s` を前計算する。`RollingHash` は `s` への参照を保持するため、利用中は `s` を破棄・変更してはならない。

##### 計算量

- $O(\lvert s \rvert)$

#### hash

```cpp
(1) RollingHashValue<mint, id> hash()
(2) RollingHashValue<mint, id> hash(ll l, ll r)
```

- (1)：列全体のハッシュを返す。
- (2)：$s[l, r)$ のハッシュを返す。

##### 制約

- $0 \leq l \leq r \leq \lvert s \rvert$

##### 計算量

- $O(1)$

#### size

```cpp
ll size()
```

元列の長さを返す。

##### 計算量

- $O(1)$

#### get

```cpp
decltype(auto) get(ll i)
```

$s_i$ を返す。

##### 制約

- $0 \leq i < \lvert s \rvert$

##### 計算量

- $O(1)$

### RollingHashConcat

#### コンストラクタ

```cpp
(1) RollingHashConcat<V=string, mint=modint61, int id=INT_MIN>()
(2) RollingHashConcat<V=string, mint=modint61, int id=INT_MIN>(RollingHashRange<RollingHash<V, mint, id>> range)
(3) RollingHashConcat<V=string, mint=modint61, int id=INT_MIN>(RollingHashConcatView view)
```

- (1)：断片を連結する空の列を作る。
- (2)：`range` を最初の断片として持つ列を作る。`RollingHashConcat cat = rh.substr(l, r);` と書ける。
- (3)：`+` で連結した view の各断片を保持する列を作る。

##### 計算量

- (1)：$O(1)$
- (2)：$O(1)$
- (3)：断片数を $b$ として $O(b)$

#### reserve

```cpp
void reserve(int n)
```

断片を $n$ 個追加できる領域を確保する。

##### 計算量

- $O(n)$

#### size

```cpp
ll size()
```

仮想列の長さを返す。

##### 計算量

- $O(1)$

#### operator+=

```cpp
(1) RollingHashConcat& operator+=(const RollingHash<V, mint, id>& source)
(2) RollingHashConcat& operator+=(const RollingHashRange<RollingHash<V, mint, id>>& range)
```

- (1)：`source` 全体を末尾に追加する。
- (2)：`range` が参照する元列の連続部分列を末尾に追加する。

同じ元列の隣接する断片は自動的に一つにまとめる。異なる元列から得た断片を混在させてもよい。`RollingHashConcat` どうしを追加する演算は定義していない。

##### 計算量

- 償却 $O(1)$

#### operator=

```cpp
RollingHashConcat& operator=(RollingHashConcatView view)
```

`+` で連結した view の各断片を保持する。これにより、形の異なる view を比較しながら一つの `RollingHashConcat` に最良の列を保存できる。

##### 計算量

- view の断片数を $b$ として $O(b)$

#### hash

```cpp
(1) RollingHashValue<mint, id> hash()
(2) RollingHashValue<mint, id> hash(ll l, ll r)
```

- (1)：仮想列全体のハッシュを返す。
- (2)：仮想列の $[l, r)$ のハッシュを返す。

$b$ を断片数とする。

##### 制約

- $0 \leq l \leq r \leq \mathrm{size}()$

##### 計算量

- (1)：$O(1)$
- (2)：$O(\log b)$

#### get

```cpp
decltype(auto) get(ll i)
```

仮想列の $i$ 番目の要素を返す。

$b$ を断片数とする。

##### 制約

- $0 \leq i < \mathrm{size}()$

##### 計算量

- $O(\log b)$

### 共通の列操作

#### content

```cpp
auto content()
```

列の実際の内容を返す。要素型が `char` なら `string`、それ以外なら `vector<要素型>` を返す。`LOCAL` で `dump` したときもこの内容が表示される。

`RollingHashValue` は元列を保持していないため、このメソッドを持たない。

$n = \mathrm{size}()$、`get` 一回の計算量を $G$ とする。

##### 計算量

- $O(nG)$

#### substr

```cpp
(1) auto substr(ll l, ll r)
(2) auto substr(ll l=0)
```

- (1)：$[l, r)$ を表す非所有 view を返す。
- (2)：$[l, \mathrm{size}())$ を表す非所有 view を返す。

返された view より先に元の `RollingHash`、`RollingHashConcat`、または view を破棄してはならない。

##### 計算量

- $O(1)$

#### hash

```cpp
(1) RollingHashValue hash()
(2) RollingHashValue hash(ll l, ll r)
```

- (1)：view 全体のハッシュを返す。
- (2)：view の $[l, r)$ のハッシュを返す。

##### 制約

- $0 \leq l \leq r \leq \mathrm{size}()$

##### 計算量

- `RollingHash` の view：$O(1)$
- 断片数 $b$ の `RollingHashConcat` の view：$O(\log b)$

#### lcp / lcs

```cpp
ll lcp(const Other& rhs)
ll lcs(const Other& rhs)
```

二つの列の最長共通接頭辞、最長共通接尾辞の長さを返す。

$L$ を短い方の列の長さ、$H$ を一回の部分ハッシュ取得の計算量とする。

##### 計算量

- $O(H \log (L + 1))$

#### compare

```cpp
int compare(const Other& rhs)
```

辞書順で自身が `rhs` より小さい、等しい、大きいとき、それぞれ $-1, 0, 1$ を返す。`<`, `<=`, `>`, `>=` も使用できる。

$L$ を短い方の列の長さ、$H$ を一回の部分ハッシュ取得の計算量とする。

##### 計算量

- `RollingHash` の view どうし：$O(\log (L + 1))$
- 断片数 $b$ の `RollingHashConcat` の view を含む場合：$O(\log b \log (L + 1))$

#### operator+

```cpp
auto operator+(View lhs, View rhs)
```

`lhs` と `rhs` を連結した非所有 view を返す。`View` は `RollingHashRange` またはこの演算で作った連結 view であり、3個以上の view も `a + b + c + ...` と結合できる。元の列や断片をコピーしない。各 view の元列は異なってもよい。主に $xy$ と $yx$ の比較に用いる。

$k$ 個の view を左結合または右結合で連結した場合、型も同じ形で入れ子になる。各要素へのアクセスや全体のハッシュ取得は最悪 $O(k)$ になるため、断片が多い場合は `RollingHashConcat` を用いる。

##### 計算量

- $O(1)$
