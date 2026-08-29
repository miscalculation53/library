## 概要

部分和問題を前計算し、判定と復元を行う。

- `SubsetSumFromFrequency`：非負整数 $v$ の個数を `freq[v]` として受け取る。復元結果は（値、使用個数）の列。
- `SubsetSum`：負要素を含んでもよい元配列を受け取る。復元結果は元配列の各添字を使用するかを表す `vc<bool>`。

`SubsetSumFromFrequency` は、同じ値を個数分並べた配列を内部で作らない。値 $v$ の個数を二進分割し、各（値、個数）の塊を重さ $v\times cnt$ の要素として `DynamicBitset` による DP を行う。

`SubsetSum` は、負要素 $a_i$ を $-a_i$ に置き換え、目標値を

$$
x-\sum_{a_i<0}a_i
$$

にずらして `SubsetSumFromFrequency` を利用する。復元時には、負要素を使用するかどうかを反転する。

## アルゴリズムの説明

長さ $n$ の非負整数列 $a$ に対し、$s$ 以下の部分和を判定するには bitset DP を使えばよい。復元のためにはいくつか方法が考えられる。

- bitset の履歴をすべて保持する。前計算が $O(ns/w)$ 時間、空間が $O(ns/w)$、復元が $O(n)$ 時間。
- bitset の履歴を $\sqrt n$ 個ごとに保持する。前計算が $O(ns/w)$ 時間、空間が $O(\sqrt n s/w)$、復元が $O(ns/w)$ 時間。
- $\mathrm{time}[j]\ (0\leq j\leq s)$ に、$a[0,i)$ から $j$ が作れるようになる最小の $i$ を記録する。DP の更新ごとに新しく立ったビットを列挙すればよく、前計算が $O(ns/w+s)$ 時間、空間が $O(s)$、復元が $O(n)$ 時間。

なお、分割統治による方法もある。左半分と右半分でそれぞれ DP して復元する部分和を決め、再帰する。全体で $O(ns\log n/w)$ 時間となり、bitset を使い回せば空間は $O(s/w)$ となる。$s=\sum a_i$ のような場合は、動的 bitset を使うと深さ $d$ の処理量が $O\left(n\sum a_i/(2^d w)\right)$ となるため、$\log n$ を取り除ける。

ここで $w$ は `DynamicBitset` のワードサイズである。本ライブラリでは `time` を使う方法を採用している。`or_slice` のコールバックによって新しく立ったビットを列挙するため、その列挙回数の合計は $O(s)$ となる。

さらに、DP の前に要素数を減らせる。同じ値 $j$ が3個あるとき、$j,j,j$ を $j,2j$ に置き換える[操作](https://x.com/noshi91/status/1404719619097567236)を繰り返す方法がある。別の方法として、値 $v$ の個数 $c_v$ を $1,2,4,\ldots$ 個の塊へ二進分割してもよい。

本ライブラリでは後者を採用している。$s$ 以下の部分和に関係する個数を

$$
c_v=\min\left(\mathrm{freq}[v],\left\lfloor\frac{s}{v}\right\rfloor\right)
$$

とし、各塊を重さ $v\times cnt$ の 0-1 要素として扱う。生成される塊の総数を

$$
m=\sum_{v=1}^{s}\left\lceil\log_2(c_v+1)\right\rceil
$$

とすると、$m=O\left(\min\left(n,s,\sqrt{\sum a_i}\right)\right)$ である。復元時には選んだ塊の（値、個数）をそのまま足し合わせればよい。

負要素を含む場合は、$a_i<0$ を $-a_i$ に置き換え、目標値 $x$ を

$$
x-\sum_{a_i<0}a_i
$$

へ移す。復元時には、元が負だった要素を使うかどうかを反転する。このとき $s$ は $s-\sum_{a_i<0}a_i$ に、$\sum a_i$ は $\sum|a_i|$ に対応する。

## 使用例

```cpp
vc<int> freq = {0, 2, 1};
SubsetSumFromFrequency ss1(freq, 4);
auto [ok1, cnt] = ss1.answer(4);

vc<int> a = {-3, 1, 2, 4};
SubsetSum ss2(a, 5);
auto [ok2, use] = ss2.answer(3);
```

## 詳細なドキュメント

### SubsetSumFromFrequency

#### コンストラクタ

```cpp
SubsetSumFromFrequency(vc<T> freq, int smax)
```

`freq[v]` を値 $v$ の個数として、$smax$ 以下の部分和を前計算する。値 $0$ の要素は復元結果に含めない。

##### 制約

- `T` は整数型
- $smax\geq 0$
- $freq[v]\geq 0$

##### 計算量

$s=smax$、上で定義した塊の総数を $m$ とする。

- $O\left(s+m\left\lceil s/w\right\rceil\right)$

#### exists

```cpp
bool exists(int x)
```

和 $x$ を作れるか返す。

##### 計算量

- $O(1)$

#### answer

```cpp
pair<bool, vc<pair<int, int>>> answer(int x)
```

和 $x$ を作れない場合は `{false, {}}` を返す。作れる場合は、使用する（値、個数）の列とともに `true` を返す。個数が $0$ の値は含まれず、列の順序は未規定。

##### 計算量

- $O(m)$

### SubsetSum

#### コンストラクタ

```cpp
SubsetSum(vc<T> a, int smax)
```

元配列 $a$ に対し、$smax$ 以下の部分和を前計算する。$a$ は負要素を含んでもよい。

##### 制約

- `T` は整数型
- $smax-\sum_{a_i<0}a_i$ は `int` の範囲内

##### 計算量

$n=|a|$、$s=\max\left(0,smax-\sum_{a_i<0}a_i\right)$ とし、$|a_i|$ の頻度から生成される塊の総数を $m$ とする。

- $O\left(n+s+m\left\lceil s/w\right\rceil\right)$

#### exists

```cpp
bool exists(int x)
```

和 $x$ を作れるか返す。$x>smax$ の場合は `false` を返す。

##### 計算量

- $O(1)$

#### answer

```cpp
pair<bool, vc<bool>> answer(int x)
```

和 $x$ を作れない場合は `{false, {}}` を返す。作れる場合は、元配列の各要素を使用するかを表す長さ $|a|$ の `vc<bool>` とともに `true` を返す。

##### 計算量

- $O(n)$
