## 概要

$[0,n)$ の整数集合を64分木で管理する。64個のキーの有無を1ワードにまとめ、そのワードが空かを上の階層のビットで表す。

存在判定は $O(1)$、挿入・削除・隣接要素の検索は $O(\log_{64} n)$。値域全体のビット配列を確保するため、単独の集合で更新と隣接検索を繰り返す用途に向く。複数の疎な集合を併合する用途には [MeldableIntegerSetPool](meldable_integer_set.md) を使える。

以下では $H$ を階層数（$n=0$ のときは1）、$C$ を全階層のワード数とする。$H=O(1+\log_{64}(n+1))$、$C=O(\lceil n/64\rceil)$。使用メモリは $O(C+1)$。空の値域の構築・操作は定数時間で扱う。

## 使用例

```cpp
FastIntegerSet st(1000);
st.insert(3);
st.insert(70);
assert(st.contains(3));
assert(st.size() == 2);
assert(st.next(4) == 70);
assert(st.prev(69) == 3);
assert(st.next(71) == st.universe_size());

for (int key : st) cout << key << '\n';
st.enumerate(0, 64, [&](int key) { cout << key << '\n'; });
```

## 詳細なドキュメント

### FastIntegerSet

#### コンストラクタ

```cpp
FastIntegerSet()
explicit FastIntegerSet(int n)
template <class F> FastIntegerSet(int n, const F& f)
```

デフォルト構築では値域が空になる。`FastIntegerSet(n)` は $[0,n)$ の空集合を作る。`FastIntegerSet(n,f)` は `f(i)` が真であるキーを登録する。

コピーは独立した集合を作る。ムーブ後の元の集合は空の値域になる。

##### 制約

- $0\leq n$
- `f(i)` は `bool` に変換可能な値を返す

##### 計算量

- デフォルト構築：$O(1)$
- 空集合の構築：$O(C+1)$
- `f` による構築：$O(n+C+1)$（`f` の評価を $O(1)$ とする）
- コピー構築：$O(C+1)$
- ムーブ構築：$O(1)$

#### universe_size / size / empty

```cpp
int universe_size() const
int size() const
bool empty() const
```

値域の大きさ `n`、登録したキーの個数、集合が空かを返す。

##### 計算量

- $O(1)$

#### contains / operator[]

```cpp
bool contains(int x) const
bool operator[](int x) const
```

`x` が登録されていれば `true` を返す。

##### 制約

- $0\leq x<n$

##### 計算量

- $O(1)$

#### insert / erase

```cpp
bool insert(int x)
bool erase(int x)
```

`insert` は `x` を登録し、新規登録なら `true` を返す。`erase` は `x` を削除し、登録されていた場合に `true` を返す。

##### 制約

- $0\leq x<n$

##### 計算量

- $O(H)$

#### next / prev / 隣接検索

```cpp
int next(int x) const
int prev(int x) const
int geq_min(int x) const
int gt_min(int x) const
int leq_max(int x) const
int lt_max(int x) const
int min_element() const
int max_element() const
```

| 関数 | 返すキー | 該当するキーがない場合 |
|---|---|---|
| `next(x)`, `geq_min(x)` | `x` 以上で最小 | `n` |
| `gt_min(x)` | `x` より大きいキーの最小 | `n` |
| `prev(x)`, `leq_max(x)` | `x` 以下で最大 | `-1` |
| `lt_max(x)` | `x` より小さいキーの最大 | `-1` |
| `min_element()` | 全体の最小 | `n` |
| `max_element()` | 全体の最大 | `-1` |

`x` は値域の外でもよい。

##### 計算量

- $O(H)$

#### enumerate

```cpp
template <class F> void enumerate(int l, int r, const F& f) const
```

$[l,r)$ の登録済みキーを昇順に `f(key)` へ渡す。コールバックの実行中は集合の内容を維持する。

##### 制約

- $0\leq l\leq r\leq n$

##### 計算量

- $O((k+1)H)$。$k$ は列挙するキーの個数（`f` の実行時間を除く）

#### begin / end / cbegin / cend

```cpp
iterator begin() const
iterator end() const
const_iterator cbegin() const
const_iterator cend() const
```

キーを昇順に走査する入力イテレータを返す。`iterator` と `const_iterator` は同じ型で、参照すると `int` の値を返す。前置・後置 `++` と等値比較に対応する。

更新・`clear`・代入・`swap` はイテレータを無効化する。

##### 計算量

- `begin` / `cbegin` / `++`：$O(H)$
- `end` / `cend` / 参照 / 比較：$O(1)$
- 全要素の走査：$O((s+1)H)$。$s$ は要素数

#### content

```cpp
vc<int> content() const
```

全キーを昇順の配列で返す。

##### 計算量

- $O((s+1)H)$。$s$ は要素数

#### clear

```cpp
void clear()
```

全キーを削除する。値域と確保した容量は維持する。

##### 計算量

- $O(C+1)$。既に空なら $O(1)$

#### swap

```cpp
void swap(FastIntegerSet& other)
void swap(FastIntegerSet& a, FastIntegerSet& b)
```

値域、要素、確保した容量を交換する。

##### 計算量

- $O(1)$
