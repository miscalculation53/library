このドキュメントには AI による作成・変更が含まれています。

## 概要

https://qiita.com/okateim/items/e2f4a734db4e5f90e410

ブロック分割と sparse table を組み合わせることで、ブロック幅 $B$ に対して前計算 $O\left(N+\dfrac{N}{B} \log \dfrac{N}{B}\right)$ 時間・空間（$B = \Theta(\log N)$ ととると $O(N)$）、異なるブロック間のクエリ $O(1)$ 時間となる。モノイドが $\min, \max$ などの場合は、単調スタックの状態をビット列として前計算し、同じブロック内のクエリも $O(1)$ 時間で求める。

ブロック全体の積は、共通のブロック分割実装で求めた suffix の値を再利用する。

## 使用例

```cpp
#include "algebra/algebra_basic_ops.hpp"
#include "ds/static_range/linear_rmq.hpp"

LinearRMQ<MonoidMin<ll>> rmq(vc<ll>{5, 2, 7, 1});
assert(rmq.prod(0, 3) == 2);
assert(rmq.get(3) == 1);
```

## 詳細なドキュメント

### LinearRMQ

#### コンストラクタ

```cpp
LinearRMQ<M, Word = uint64_t>(vc<M::S> v)
```

モノイドを渡すようにしているが、これはインターフェースを類似のデータ構造（sparse table 等）と揃えるためである。

##### 制約

- `M` は $\min, \max$ などのモノイド
  - 厳密には、$\mathrm{op}(a, b)$ が $a$ または $b$ で、かつこれが順序関係になっている
- `Word` は符号なし整数

##### 計算量

`Word` のビット数を $B$ として

- $O\left(n+\dfrac{n}{B}\log \dfrac{n}{B}\right)$

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

#### content

```cpp
vc<M::S> content() const
```

構築時の全要素を添字順に並べた配列のコピーを返す。空列では空の配列を返す。

##### 計算量

- $O(n)$
