## 線形 RMQ

https://qiita.com/okateim/items/e2f4a734db4e5f90e410

ブロック分割と sparse table を組み合わせることで、ブロック幅 $B$ に対して前計算 $O\left(\dfrac{N}{B} \log \dfrac{N}{B}\right)$ 時間・空間（$B = \log N$ ととると $O(N)$）、異なるブロック間のクエリ $O(1)$ 時間になるのだった。モノイドが $\min, \max$ などであるときに限り、同じブロック内のクエリも適切な前計算のもと $O(1)$ 時間でできるようにする。（アイデアはスライド最小値。）

実用上どちらが速いかはちゃんと測定してない（$N, Q$ の大きさにもよるかも。$N = Q$ なら雑に LC に投げた感じ同じくらい？ LC の fastest はブロック分割までらしい）。

### コンストラクタ

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

- $O\left(\dfrac{n}{B}\log \dfrac{n}{B}\right)$


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
