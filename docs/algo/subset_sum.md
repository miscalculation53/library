## 概要

部分和問題を前計算し、判定と復元を行う。

- `SubsetSumFromFrequency`：非負整数 $v$ の個数を `freq[v]` として受け取る。復元結果は（値、使用個数）の列。
- `SubsetSum`：負要素を含んでもよい元配列を受け取る。復元結果は元配列の各添字を使用するかを表す `vc<bool>`。

`SubsetSumFromFrequency` は、同じ値を個数分並べた配列を内部で作らない。個数を二進分割した後、同じ重みの要素をまとめて `DynamicBitset` による DP を行う。

`SubsetSum` は、負要素 $a_i$ を $-a_i$ に置き換え、目標値を

$$
x-\sum_{a_i<0}a_i
$$

にずらして `SubsetSumFromFrequency` を利用する。復元時には、負要素を使用するかどうかを反転する。

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

#### exists

```cpp
bool exists(int x)
```

和 $x$ を作れるか返す。

#### answer

```cpp
pair<bool, vc<pair<int, int>>> answer(int x)
```

和 $x$ を作れない場合は `{false, {}}` を返す。作れる場合は、使用する（値、個数）の列とともに `true` を返す。個数が $0$ の値は含まれず、列の順序は未規定。

### SubsetSum

#### コンストラクタ

```cpp
SubsetSum(vc<T> a, int smax)
```

元配列 $a$ に対し、$smax$ 以下の部分和を前計算する。$a$ は負要素を含んでもよい。

##### 制約

- `T` は整数型
- $smax-\sum_{a_i<0}a_i$ は `int` の範囲内

#### exists

```cpp
bool exists(int x)
```

和 $x$ を作れるか返す。$x>smax$ の場合は `false` を返す。

#### answer

```cpp
pair<bool, vc<bool>> answer(int x)
```

和 $x$ を作れない場合は `{false, {}}` を返す。作れる場合は、元配列の各要素を使用するかを表す長さ $|a|$ の `vc<bool>` とともに `true` を返す。
