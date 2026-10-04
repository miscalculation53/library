## 概要

列の転倒数を求める `inversion_number` と、区間を伸縮しながら転倒数を管理する `InversionSlider`。

どちらも比較可能な値の列を直接受け取り、重複、負数、大きい整数、文字列などを扱える。`InversionSlider` は構築時に値の順序を前計算し、[Mo](../mo/mo.md) に渡せる。

## 使用例

```cpp
vl a = {3, -1, 3, 2};
InversionSlider slider(a); // 値の型を推論。初期区間は [0, 0)
slider.set(0, 4);
ll all = slider.inversion_num; // 3
slider.set(1, 4);
ll part = slider.inversion_num; // 1
```

## 詳細なドキュメント

#### inversion_number

```cpp
ll inversion_number(V v)
```

$0 \leq i \lt j \lt \lvert v \rvert$ かつ $v_i \gt v_j$ を満たす $(i, j)$ の個数を返す。

##### 制約

- 要素の `<` が狭義弱順序を定めること。

##### 計算量

- $O(\lvert v \rvert \log \lvert v \rvert)$

### InversionSlider

#### コンストラクタ

```cpp
(1) InversionSlider<T = ll>()
(2) InversionSlider<T>(const vc<T>& a)
```

- (1)：空列について初期化する。
- (2)：列 $a$ について初期化する。`InversionSlider slider(a)` と書くと `T` を引数から推論する。

初期区間は $[0,0)$、転倒数は $0$。内部では値の昇順に順位を付け、同値の要素は元の添字順に並べる。この順列に対する転倒数は、どの部分区間でも元の列と一致する。

##### 制約

- (2)：`T` の `<` が狭義弱順序を定めること。

##### 計算量

$n = |a|$、値の比較を $O(1)$ として、

- (1)：$O(1)$
- (2)：$O(n \log(n+1))$

#### メンバ変数

```cpp
int n, l, r;
ll inversion_num;
```

`n` は元の列の長さ、`[l, r)` は現在の区間。`inversion_num` は $l \leq i < j < r$ かつ $a_j < a_i$ を満たす組 $(i,j)$ の個数。

#### set

```cpp
void set(int nl, int nr)
```

現在の区間を $[\mathrm{nl},\mathrm{nr})$ に移し、`inversion_num` を更新する。

##### 制約

- $0 \leq \mathrm{nl} \leq \mathrm{nr} \leq n$

##### 計算量

呼び出し前の区間を $[l,r)$ とし、$D = |\mathrm{nl}-l|+|\mathrm{nr}-r|$ として、

- $O(1+D\log(n+1))$

#### lpp, rpp, lmm, rmm

```cpp
void lpp()
void rpp()
void lmm()
void rmm()
```

区間の端点を $1$ つ動かし、`inversion_num` を更新する。

| 関数 | 操作 | 制約 |
| --- | --- | --- |
| `lpp()` | 左端を削除し、`l` を $1$ 増やす | $l < r$ |
| `rpp()` | 右端に追加し、`r` を $1$ 増やす | $r < n$ |
| `lmm()` | 左端に追加し、`l` を $1$ 減らす | $0 < l$ |
| `rmm()` | 右端を削除し、`r` を $1$ 減らす | $l < r$ |

##### 計算量

- $O(\log(n+1))$
