## 概要

Cartesian Tree

## 詳細なドキュメント

### CartesianTree

列の Cartesian Tree を構築する。各頂点は元の列の添字に対応し、中間順巡回で添字が昇順になる。デフォルトでは値が小さい頂点ほど根に近い。

#### コンストラクタ

```cpp
(1) CartesianTree()
(2) CartesianTree(const vc<T>& a, Compare comp = Compare{})
```

- (1)：デフォルトコンストラクタ。
- (2)：列 `a` に対する Cartesian Tree を構築する。`Compare` のデフォルトは `less<T>`。

##### 制約

- `a` は空でない
- `comp` は `T` 上の strict weak ordering

##### 計算量

`a` の長さを $n$ として、

- 時間：$O(n)$

#### root

```cpp
I root<I = ll>()
```

根の頂点番号を返す。

##### 計算量

- $O(1)$

#### parent

```cpp
I parent<I = ll>(int i)
```

頂点 `i` の親を返す。`i` が根の場合は $-1$。

##### 制約

- $0 \leq i < n$

##### 計算量

- $O(1)$

#### left_child, right_child

```cpp
(1) I left_child<I = ll>(int i)
(2) I right_child<I = ll>(int i)
```

頂点 `i` の左の子、右の子を返す。対応する子が存在しない場合は $-1$。

##### 制約

- $0 \leq i < n$

##### 計算量

- $O(1)$

#### children

```cpp
auto children<I = ll>(int i)
```

頂点 `i` の存在する子を、左の子、右の子の順に走査できる範囲として返す。

##### 制約

- $0 \leq i < n$

##### 計算量

- $O(1)$

#### range

```cpp
pair<I, I> range<I = ll>(int i)
```

$(L_i, R_i)$ を返す。`i` が `comp` に関する区間最小となる区間 $[l,r)$ は、次の条件を満たす：

$$
L_i \leq l \leq i < r \leq R_i.
$$

##### 制約

- $0 \leq i < n$

##### 計算量

- $O(1)$

#### content

```cpp
auto content()
```

各頂点の親、左右の子、`range` の情報を持つ内部配列のコピーを返す。主にデバッグ用。

##### 計算量

- 時間：$O(n)$
