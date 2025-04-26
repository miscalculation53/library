## ブロック分割 + (disjoint) sparse table

静的列に対するモノイドの区間積クエリを、列を幅 $B$ のブロックに分割した上で sparse table または disjoint sparse table と組み合わせることで、前計算 $O\left(\dfrac{N}{B} \log \dfrac{N}{B}\right)$ 時間・空間、クエリ最悪 $O(B)$ 時間で処理する。$B = O(\log N)$ ととると、前計算の時空間は線形となる。

クエリに関しては幅 $B$ 以上のときは $O(1)$ 時間で、幅 $B$ 未満のときに愚直 $O(B)$ 時間ということなので、（モノイド積の軽さにもよるが）額面の印象より軽いと思う。

### コンストラクタ

```cpp
(1) BlockSparseTable<M, int B = 16>(vc<M::S> v)
(2) BlockDisjointSparseTable<M, int B = 16>(vc<M::S> v)
```

##### 制約

- `M` はモノイド
  - (1) においては `M` は冪等である必要がある

##### 計算量

`vec` の長さを $n$ として

- $O\left(\dfrac{n}{B} \log \dfrac{n}{B} \right)$

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

- $r - l \lt B$ のとき、$O(B)$
- $r - l \geq B$ のとき、$O(1)$
