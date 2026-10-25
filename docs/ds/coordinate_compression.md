このドキュメントには AI による作成・変更が含まれています。

## 概要

値の大小関係を保って、異なる値を連続した非負整数に対応させる。

## 使用例

```cpp
#include "ds/coordinate_compression.hpp"

vc<ll> a{30, 10, 30};
CoordinateCompression<ll> cc(a);
assert(cc.get_id(30) == 1);
assert(cc.get_val(0) == 10);
assert(compressed(a) == (vc<ll>{1, 0, 1}));
```

## 詳細なドキュメント

### CoordinateCompression

座標圧縮を行うクラス。

一括変換の `compressed` はソート順に順位を付ける。固定幅整数には基数ソートを使い、各要素の二分探索を省く。

同じ列の要素を繰り返し参照する場合は、`compressed(A)` の結果を保持すると、その後の探索を省ける。

#### コンストラクタ

```cpp
CoordinateCompression(vc<T> a)
```

$a$ の重複を除いた値たち（$n$ 個あるとする）に対して、小さいほうから $0, 1, \dots, n-1$ の添字に対応させて管理する。つまり、大小関係を保ったまま番号を振る。

`vals = sortuniqued(a)` と同じこと。

##### 制約

- 値の比較と等値比較が整合する全順序を持つこと。

##### 計算量

$N=\lvert a\rvert$ として、

- 固定幅整数型（`bool` を除き、`i128`, `u128` を含む）：$O(N)$
- その他：$O(N\log N)$

#### メンバ変数

- `vals`：$i$ 番目には、$a$ のうち（重複を除いて）$i$ 番目に小さい値、つまり添字 $i$ に対応する値が入る。

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
optional<I> get_id(T v)
```

値 $v$ に対応する添字を返す。$v$ が存在しない場合 `nullopt` を返す。

$v$ が存在する場合は `LB(vals, v)` と同じこと。

##### 計算量

- $O(\log n)$

#### size

```cpp
I=ll size()
```

値の個数 $n$ を返す。`vals.size()` と同じこと。

##### 計算量

- $O(1)$

#### content

```cpp
vc<T> content() const
```

圧縮対象の値を重複除去した昇順の列で返す。列の添字は圧縮後の添字に一致する。

##### 計算量

- $O(n)$。$n$ は異なる値の個数

#### compressed

```cpp
vc<I> compressed(vc<T> a)
```

長さ $\lvert a \rvert$ で、$i$ 番目が `*get_id(a[i])` であるような vector を返す。

言い換えると、次の条件を満たす「最小の」非負整数列 $b$ を返す。

- $a_i < a_j \iff b_i < b_j$
- $a_i = a_j \iff b_i = b_j$
- $a_i > a_j \iff b_i > b_j$

##### 計算量

- 固定幅整数型（`bool` を除き、`i128`, `u128` を含む）：$O(\lvert a \rvert)$
- その他：$O(\lvert a \rvert\log\lvert a \rvert)$

#### compressed_unordered

```cpp
vc<I> compressed_unordered(vc<T> a)
```

次の条件を満たす辞書順最小の非負整数列 $b$ を返す。

- $a_i = a_j \iff b_i = b_j$
- $a_i \neq a_j \iff b_i \neq b_j$

つまり、同じ値には同じ番号、違う値には違う番号を振るようにしつつ、先に登場した値ほど小さい番号を振るようにする。

使用例のひとつに、DP の状態の圧縮がある（例題：[TDPC S - マス目](https://atcoder.jp/contests/tdpc/tasks/tdpc_grid)）。

##### 計算量

- 固定幅整数型（`bool` を除き、`i128`, `u128` を含む）：$O(\lvert a \rvert)$
- その他：$O(\lvert a \rvert\log\lvert a \rvert)$
