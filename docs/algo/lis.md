## 概要

https://miscalc.hatenablog.com/entry/2024/07/25/212618

## 詳細なドキュメント

#### コンストラクタ

```cpp
(1) LIS()
(2) LIS(const vc<T>& a, Compare comp = Compare())
```

- (1)：デフォルトコンストラクタ。
- (2)：列 `a` について、`comp(a[i], a[j])` を増加関係とした最長増加部分列を前計算する。`Compare` のデフォルトは `less<T>`。

##### 制約

- `comp` は `less{}`, `greater{}`, `less_equal{}`, `greater_equal{}` のいずれか

##### 計算量

`a` の長さを $n$ として、

- $O(n \log n)$

#### len_of_lis

```cpp
I len_of_lis<I = ll>()
```

LIS の長さを返す。

##### 計算量

- $O(1)$

#### pos_in_lis

```cpp
I pos_in_lis<I = ll>(int i)
```

`a[i]` がいずれかの LIS に使われ得る場合、その LIS 内での位置を $0$ 始まりで返す。使われ得ない場合は $-1$ を返す。

##### 制約

- $0 \leq i < n$

##### 計算量

- $O(1)$

#### lds_id

```cpp
I lds_id<I = ll>(int i)
```

列を `comp` に関する広義減少部分列へ分解したものの一つについて、`a[i]` が属する列の番号を返す。

##### 制約

- $0 \leq i < n$

##### 計算量

- $O(1)$

#### candidates

```cpp
auto candidates(int j)
```

いずれかの LIS の $j$ 番目として使われ得る添字を、昇順に走査できる範囲として返す。

##### 制約

- $0 \leq j < \mathrm{len\_of\_lis}()$

##### 計算量

- $O(1)$
