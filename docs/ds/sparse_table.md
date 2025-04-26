## sparse table

静的列に対するモノイドの区間積クエリを、モノイドが冪等（$\max, \min, \gcd, \mathrm{lcm}, \mathrm{and}, \mathrm{or}$ など）のときに限って、前計算 $O(N\log N)$ 時間・空間、クエリ $O(1)$ 時間で処理する。

### コンストラクタ

```cpp
SparseTable<M>(vc<M::S> v)
```

##### 制約

- `M` は**冪等**なモノイド

##### 計算量

`vec` の長さを $n$ として

- $O(n \log n)$

### メンバ関数

#### get

```cpp
M::S get(int p)
```

$v_p$ の値を返す。

##### 制約

- $0 \leq p \lt n$

##### 計算量

- $O(1)$


#### prod

```cpp
M::S prod(int l, int r)
```

$v_l \bullet \cdots \bullet v_{r-1}$ の値を返す。

##### 制約

- $0 \leq l \leq r \leq n$

##### 計算量

- $O(1)$
  - モノイド演算は $1$ 回

----

### 中身

前計算：各 $i, j \ (0 \leq i + 2^j \lt n)$ に対して $[i, i+2^j)$ の答えを計算しておく（これはダブリング風にできる）。

クエリ：$[l, r)$ の答えは、$l \leq r-2^j \leq l+2^j \leq r$ なる $j$ をとってきて、$[l, l+2^j)$ の答えと $[r-2^j, r)$ の答えの積として計算する（冪等なのでこれでよい）。