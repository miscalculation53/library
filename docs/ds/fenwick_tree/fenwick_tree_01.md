## 01 列に対する Fenwick Tree

01 列に対する Fenwick Tree は、長さ $B = 64$ の 01 列を `ull` で持つことで、長さ $n/64$ の Fenwick Tree に対する処理で行え、多少の高速化になる。（$\log_2 n$ が $\log_2(n/64) = \log_2 n - 6$ になる。）

使用場面の例：

- 転倒数
- 集合を管理する

### コンストラクタ

```cpp
(1) FenwickTree01<T = ll, Word = ull>(int n)
(2) FenwickTree01<T = ll, Word = ull>(vc<U> v)
```

`T` は和を管理する整数型。`int` で足りるが、使いやすさのためデフォルトでは `ll` にしている。

- (1) 長さ $n$、全要素単位元で初期化する。
- (2) vector $v$ で初期化する。


#### 計算量

- (1)：$O(n)$
- (2)：$O\left(n + \dfrac{n}{B}\log \dfrac{n}{B}\right)$


### メンバ関数

#### size

```cpp
I size<I=ll>()
```

$n$ を返す。

##### 計算量

- $O(1)$


#### sum

```cpp
(1) T sum(int r)
(2) T sum(int l, int r)
```

- (1)：$[0, r)$ の和を返す。
- (2)：$[l, r)$ の和を返す。

##### 制約

- (1)：$0 \leq r \leq n$
- (2)：$0 \leq l \leq r \leq n$

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$


#### get

```cpp
bool get(int i)
```

$i$ 番目の値を返す。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(1)$



#### set

```cpp
void set(int i, bool b)
```

$i$ 番目の要素を $b$ にする。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$


#### leq_max, lt_max, gt_min, geq_min

```cpp
(1) I leq_max<I=ll>(T w)
(2) I lt_max<I=ll>(T w)
(3) I gt_max<I=ll>(T w)
(4) I geq_max<I=ll>(T w)
```

$[0, r)$ の要素の和に関して二分探索する：

- (1)：$\mathrm{sum}[0, r) \leq w$ となる $r$ の最大値（なければ $-1$）を返す。
- (2)：$\mathrm{sum}[0, r) \lt w$ となる $r$ の最大値（なければ $-1$）を返す。
- (3)：$\mathrm{sum}[0, r) \gt w$ となる $r$ の最大値（なければ $n$）を返す。
- (4)：$\mathrm{sum}[0, r) \geq w$ となる $r$ の最大値（なければ $n$）を返す。

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$


---

$[0, n)$ の要素からなる集合を管理するのにも使える： 要素 $i$ が存在するかどうかをデータ構造の $i$ 番目に持てばよい。（クエリ先読み + 座標圧縮と組み合わせることもしばしば）

以下は、この使い方をしたとき用のメソッドである。

（`sum` を使うと「$x$ 以上の値の個数」などがわかるがこれは別のメソッドにはしていない。）

#### size_of_set

```cpp
S size_of_set()
```

集合のサイズを返す。`sum(n)` に一致。

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$

#### order_in_set

```cpp
pair<I, I> order_in_set<I=ll>(int x)
```

$x$ が集合に存在するとき、$x$ が何番目かを返す。存在しないときは（も）、「$x$ 以上で最小の要素」が何番目かを返す。

##### 制約

- $0 \leq x \lt n$

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$

#### kth_in_set

```cpp
I kth_in_set<I=ll>(G::S k)
```

「集合の $k$ 番目に小さい要素」を返す。ただし、$k \lt 0$ なら $-1$ を、$k \geq (集合のサイズ)$ なら $n$ を返す。

なおこれは（$k \geq 0$ のとき）`leq_max` と一致する。

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$

#### lt_max_in_multiset など

```cpp
(1) I lt_max_in_multiset<I=ll>(T x)
(2) I leq_max_in_multiset<I=ll>(T x)
(3) I geq_max_in_multiset<I=ll>(T x)
(4) I gt_max_in_multiset<I=ll>(T x)
```

- (1)：$x$ 未満で最大の要素が**小さい方から何番目か**を返す。なければ $-1$ を返す。
- (2)：$x$ 以下で最大の要素が**小さい方から何番目か**を返す。なければ $-1$ を返す。
- (3)：$x$ 以上で最小の要素が**小さい方から何番目か**を返す。なければ集合の要素数を返す。
- (4)：$x$ 超過で最小の要素が**小さい方から何番目か**を返す。なければ集合の要素数を返す。

##### 計算量

- $O\left(1 + \log \dfrac{n}{B}\right)$
