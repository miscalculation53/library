## 概要

$((ai+b)\bmod m)_{0\le i<n}$ を $O(\sqrt n)$ 本の等差数列に分ける。各部分列について、値と元の添字の両方を等差数列として返す。

対象は高々 1 周期、すなわち $n\le m/\gcd(a,m)$。互いに素でない場合も直接扱える。部分列をまたぐ順序は並べ替わるため、全体の積や和には可換性が必要。

内部計算は `ll` で行う。剰余の列挙は加算と折り返しで進め、部分列の次の先頭は最後の有効な項を経由して求める。各中間値が `ll` に収まる順序で計算する。

先頭 $\lfloor\sqrt n\rfloor+1$ 個の剰余をソートし、値の差が最小の 2 点を取る。その添字差を $d$、符号付きの値の差を $h$ とすると、$d=O(\sqrt n)$、$|h|=O(m/\sqrt n)$。添字を $d$ ごとに進め、値が $[0,m)$ を出る直前で区切ると、部分列数は $O(d+n|h|/m)=O(\sqrt n)$ となる。[平方根分割の解説](https://codeforces.com/blog/entry/141889)。

複数周期を扱う場合は、$g=\gcd(a,m)$、$L=m/g$ とする。1 周期の値の多重集合は $b\bmod g+gj$ $(0\le j<L)$ なので、全周期をまとめ、残りの $n\bmod L$ 項をこの関数で分割できる。

## 使用例

```cpp
auto parts = split_linear_mod(4, 9, 4, 3);  // 3,7,2,6 を分割
for (auto p : parts) {
  for (ll j = 0; j < p.n; ++j) {
    ll index = p.origin_index(j);
    assert(p(j) == (4 * index + 3) % 9);
  }
}
```

## 詳細なドキュメント

### LinearModArithmetic

```cpp
ll n, a, b;
ll l, gap;
```

長さ `n`、値 `a*j+b`、元の添字 `l+gap*j` の部分列。`a` は負の場合もある。`split_linear_mod` が返す部分列では、値は $[0,m)$、元の添字は入力列の範囲内にあり、式の途中の積も `ll` に収まる。

#### operator()

```cpp
ll operator()(ll j) const
```

$j$ 番目の値を返す。

##### 制約

- $0\le j<n$。

##### 計算量

- $O(1)$。

#### origin_index

```cpp
ll origin_index(ll j) const
```

$j$ 番目の項の元の添字を返す。

##### 制約

- $0\le j<n$。

##### 計算量

- $O(1)$。

#### split_linear_mod

```cpp
vc<LinearModArithmetic> split_linear_mod(ll n, ll m, ll a, ll b)
```

元の各添字をちょうど 1 回ずつ含む部分列の一覧を返す。$n=0$ なら空の配列。負の $a,b$ も、剰余を $[0,m)$ に正規化して扱う。

##### 制約

- $m\ge1$。
- $0\le n\le m/\gcd(a,m)$。

##### 計算量

- $O(\log m+\sqrt n\log(n+1))$。
