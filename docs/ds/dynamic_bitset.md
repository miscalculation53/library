## 概要

長さを実行時に決められる bitset。

通常の `std::bitset` に近い一点操作、全体操作、論理演算、シフトに加え、次の操作を持つ。

- `resize`, `reserve`, `push_back`, `pop_back`
- 半開区間 $[l,r)$ の `set_range`, `reset_range`, `flip_range`, `count`, `any`, `none`, `all`
- 前後にある `1` または `0` の検索
- 一時オブジェクトを作らない shift-or

## 使用例

```cpp
DynamicBitset a(100);
a.set_range(10, 20);
a.flip(15);
int first = a.find_first();
int next = a.find_next(first);
a.or_shift_left(7);

DynamicBitset b("0010110");
cout << b << '\n';
```

## 詳細なドキュメント

### DynamicBitset

#### コンストラクタ

```cpp
DynamicBitset(int n, bool value = false)
DynamicBitset(const string &s, char zero = '0', char one = '1')
```

第1形式は長さ $n$、全ビットが `value` の bitset を作る。

第2形式は長さを `s.size()` とし、`s` の右端を0 bit目として初期化する。これは `std::bitset` の文字列との変換と同じ順序である。

##### 計算量

- 第1形式：$O(n/w)$
- 第2形式：$O(n)$

#### assign / to_string

```cpp
DynamicBitset &assign(const string &s, char zero = '0', char one = '1')
string to_string(char zero = '0', char one = '1')
```

`assign` は `s` の右端を0 bit目として代入し、長さを `s.size()` に変更する。`to_string` はその逆変換を行う。

##### 制約

- `zero != one`
- `s` の各文字は `zero` または `one`

##### 計算量

- $O(n)$

#### 入出力

```cpp
cin >> a;
cout << a;
```

入力は `0`, `1` からなる文字列として読み込み、その長さに変更する。出力は上位ビットから並べる。先頭の0も保持される。

##### 計算量

- $O(n)$

#### resize

```cpp
void resize(int n, bool value = false)
```

長さを $n$ に変更する。長さを増やした場合、新しいビットを `value` にする。

##### 計算量

- 償却 $O(1+|n-\operatorname{size()}|/w)$

#### set / reset / flip

```cpp
bool operator[](int i) const
Reference operator[](int i)
DynamicBitset &set(int i, bool value = true)
DynamicBitset &reset(int i)
DynamicBitset &flip(int i)
DynamicBitset &set_range(int l, int r, bool value = true)
DynamicBitset &reset_range(int l, int r)
DynamicBitset &flip_range(int l, int r)
```

`a[i]` は $i$ bit目を取得し、非 `const` の場合は代入や `a[i].flip()` も行える。

`set`, `reset`, `flip` は一点を変更する。`set_range`, `reset_range`, `flip_range` は半開区間 $[l,r)$ を変更する。引数なしの `set`, `reset`, `flip` は全体を変更する。

##### 計算量

- 一点：$O(1)$
- 区間または全体：$O(1+(r-l)/w)$

#### count / any / none / all

```cpp
int count()
int count(int l, int r)
bool any()
bool any(int l, int r)
bool none()
bool none(int l, int r)
bool all()
bool all(int l, int r)
```

全体または半開区間 $[l,r)$ に対して問い合わせる。

##### 計算量

- $O(1+(r-l)/w)$。引数なしの場合は $O(n/w)$

#### find_first / find_next / find_last / find_prev

```cpp
int find_first(bool value = true)
int find_next(int i, bool value = true)
int find_last(bool value = true)
int find_prev(int i, bool value = true)
```

`find_next(i, value)` は $i$ より右、`find_prev(i, value)` は $i$ より左にある最初の `value` の位置を返す。存在しない場合、右方向の検索は `size()`、左方向の検索は $-1$ を返す。

##### 計算量

- 調べたワード数に比例

#### 論理演算・シフト

```cpp
a &= b; a |= b; a ^= b;
a <<= k; a >>= k;
a & b; a | b; a ^ b; ~a; a << k; a >> k;
```

左右の長さは等しいものとする。

##### 計算量

- $O(n/w)$

#### or_shift_left / or_shift_right

```cpp
DynamicBitset &or_shift_left(int k)
DynamicBitset &or_shift_right(int k)
template <class F> DynamicBitset &or_shift_left(int k, const F &f)
template <class F> DynamicBitset &or_shift_right(int k, const F &f)
```

自身を $a$ として、それぞれ $a\mathrel{|}=a\ll k$、$a\mathrel{|}=a\gg k$ を一時オブジェクトなしで行う。

コールバックを渡す形式では、shift-or によって `0` から `1` に変わる各位置 $i$ について `f(i)` を1回呼ぶ。

##### 計算量

- $O(n/w)$
