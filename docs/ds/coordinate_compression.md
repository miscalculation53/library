## 座標圧縮

### CoordinateCompression

座標圧縮を行うクラス。

~~（別に sortunique して lower_bound をそのまま書けばよくない？）~~

使う際の注意点：同じ $i$ に対して `get_id(A[i])` を毎回呼び出すのは実行時間の観点から効率的でない。これで TLE することもある。このような場面では、前計算した値（`compressed(A)` にあたる）を使うようにする。

#### コンストラクタ

```cpp
CoordinateCompression(vc<T> a)
```

$a$ の重複を除いた値たち（$n$ 個あるとする）に対して、小さいほうから $0, 1, \dots, n-1$ の添字に対応させて管理する。つまり、大小関係を保ったまま番号を振る。

`vals = sortuniqued(a)` と同じこと。

#### メンバ変数

- `val`：$i$ 番目には、$a$ のうち（重複を除いて）$i$ 番目に小さい値、つまり添字 $i$ に対応する値が入る。

#### get_val

```cpp
T get_val(int i)
```

添字 $i$ に対応する値を返す。

`vals[i]` と同じこと。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(1)$

#### get_id

```cpp
I get_id(T v)
```

値 $v$ に対応する添字を返す。$v$ が存在しない場合 $-1$ を返す。

$v$ が存在する場合は `LB(vals, v)` と同じこと。

##### 計算量

- $O(\log n)$

#### size

```cpp
I size()
```

値の個数 $n$ を返す。`vals.size()` と同じこと。

##### 計算量

- $O(1)$


### compressed

```cpp
vc<I> compressed(vc<T> a)
```

長さ $\lvert a \rvert$ で、$i$ 番目が `get_id(a[i])` であるような vector を返す。

言い換えると、次の条件を満たす「最小の」非負整数列 $b$ を返す。

- $a_i < a_j \iff b_i < b_j$
- $a_i = a_j \iff b_i = b_j$
- $a_i > a_j \iff b_i > b_j$

##### 計算量

- $O(\lvert a \rvert \log \lvert a \rvert)$


### compressed_unordered

```cpp
vc<I> compressed_unordered(vc<T> a)
```

次の条件を満たす辞書順最小の非負整数列 $b$ を返す。

- $a_i = a_j \iff b_i = b_j$
- $a_i \neq a_j \iff b_i \neq b_j$

つまり、同じ値には同じ番号、違う値には違う番号を振るようにしつつ、先に登場した値ほど小さい番号を振るようにする。

使用例のひとつに、DP の状態の圧縮がある（例題：[TDPC S - マス目](https://atcoder.jp/contests/tdpc/tasks/tdpc_grid)）。

##### 計算量

- $O(\lvert a \rvert \log \lvert a \rvert)$