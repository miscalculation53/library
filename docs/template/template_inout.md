## 概要

テンプレート（入出力）

### 設計について

（このファイルに限らないが）関数とマクロで大文字小文字が結構混在しているの、まずい気もするけど（現時点では）競技で使う用として大きな問題はないと判断

## 使用例

#### 例 1：順列

```
5
1 3 4 2 5
```

```cpp
LL(N);
VEC(ll, N, P);
offset(P, -1);
// これで P = {0, 2, 3, 1, 4} となる
```

#### 例 2：vector 2 つ
  
```
N
A_1 ... A_N
B_1 ... B_N
```
```cpp
LL(N);
VEC(ll, N, A, B);
```

`A` と `B` をいっぺんに受け取れる。

#### 例 3：サイズの異なる vector 2 つ

```
N M
A_1 ... A_N
B_1 ... B_M
```

```cpp
LL(N, M);
VEC(ll, N, A);
VEC(ll, M, B);
```

サイズが異なるときはいっぺんに受け取れないので注意。

#### 例 4：pair の vector
  
```
N
A_1 B_1
:
A_N B_N
```

```cpp
LL(N);
VEC(pll, N, AB);
auto [A, B] = unzip(AB);
// 上記はマクロを使って UNZIP(AB, A, B) と書ける
```
  
これで `AB` は `vc<pll>` に、`A`, `B` はそれぞれ `vc<ll>` になる。

#### 例 5：vector 2 つを片方の値でソート

```
N
A_1 ... A_N
B_1 ... B_N
```

```cpp
LL(N);
VEC(ll, N, A, B);
auto AB = zip(pair{A, B});
// 上記はマクロを使って ZIP(AB, A, B) と書ける (ただしこのマクロではすべて tuple になる)
ranges::sort(AB);
tie(A, B) = unzip(AB);
```
  
このように、vector 2 つで与えられたものを、pair の vector にして、ソートして、vector 2 つに戻す、ということもできる。
  
#### 例 6：グラフ
```
3
1 2 100
2 3 200
```

```cpp
LL(N);
VEC(tlll, N - 1, ABC);
offset(N, tlll{-1, -1, 0});
// これで ABC = { {0, 1, 100}, {1, 2, 200} } となる
```

一旦 `vc<pll>` なり `vc<tlll>` なりで受け取って、その配列をグラフライブラリに渡す、という設計を考えている。

####  例 7：二次元配列
  
```
N M
A[1][1] ... A[1][M]
:
A[N][1] ... A[N][M]
B[1][1] ... B[1][M]
:
B[N][1] ... B[N][M]
```

```cpp
LL(N, M);
VEC2(ll, N, M, A, B);
```

#### 例 8：二次元の文字

```
3 4
...#
..#.
#...
```

```cpp
LL(N, M);
VEC(string, N, S);
```

#### 例 9：ジャグ配列 (1)

```
3
2 100 200
3 300 400 500
1 600
```

```cpp
LL(N);
JAG(ll, N, A);
```

各行の列数が行の最初に与えられるときは `JAG` を使えばよい。

#### 例 10：ジャグ配列 (2)

```
N
A[1][1]
A[2][1] A[2][2]
:
A[N][1] A[N][2] ... A[N][N]
```

```cpp
LL(N);
vl A(N);
rep(i, N) A.at(i).resize(i + 1);
READ(A);
```

各行の列数があらかじめわかっているときは、そのサイズの vector を作っておいてから `READ` すればよい。


## 詳細なドキュメント

### 入出力のタイプ

`FAST_IO`, `FAST_CIO`, `INTERACTIVE` のうちどれか $1$ つまでを `#define` する。$2$ つ以上を define してはならない。

### pair, array, tuple の加減算

`pair`, `array`, `tuple` に対し、成分ごとの `+=`, `+`, `-=`, 二項 `-`, 単項 `-` を定義する。
たとえば `GroupAddSub<pll>` の演算と逆元にも利用できる。

#### 制約

- 各成分について、使用する演算が定義されている。
- 二項演算では左右の要素数が等しい。

#### 計算量

- `pair`：各成分の演算が $O(1)$ なら $O(1)$
- `array<T, N>`, `tuple<Ts...>`：各成分の演算が $O(1)$ なら $O(N)$

- `FAST_IO`
  - （人から借りた）爆速入出力
    - 参考： https://maspypy.com/library-checker-many-a-b
  - cin, cout と併用してはならない
  - cin でたまにやる、`12+345` に対して `ll x, y; char c; cin >> x >> c >> y;` とするようなやつは多分未対応？（TLE するので注意）
    - 実装してないだけなのか原理的に難しいのかを知らない
- `FAST_CIO`
  - cin, cout を使う
  - `cin.tie(0); ios::sync_with_stdio(false);` をする
  - `ENDL` を `"\n"` にする
- `INTERACTIVE`
  - cin, cout を使う
  - `cin.tie(0); ios::sync_with_stdio(false);` をする
  - `ENDL` を `endl` にする
- 上記のどれも define されていないとき、または `LOCAL` が define されているとき
  - cin, cout を使う
  - `cin.tie(0); ios::sync_with_stdio(false);` をする
  - `ENDL` を `"\n"` にする

どの場合も、`READ`, `WRITE`, `IN`, `PRINT` が使える。これらの使い方はこの後詳しく述べるが、大雑把には次の通り：

- `READ`：定義済み変数に入力（cin でできることに相当）
- `WRITE`：空白や改行を自動で入れずに出力（cout でできることに相当）
- `IN`：変数を定義すると同時に入力
- `PRINT`：空白や改行を自動で入れて出力

ほとんどの場合 `IN` 系（`LL(...)` なども含む）と `PRINT` 系で十分だと思う。

### 入力

#### READ

`READ(...)` で、定義された変数（複数可）に入力する。

対応している型：

- 整数型
  - i128 含む
- modint 系（modint 側に実装）
- pair, tuple, array
- vector
  - 確保された size に応じて入力

他にも

- `READVEC(n, v1, v2, ...)`：すでにある vector `v1`, `v2`, $\dots$ について、サイズ $n$ にしてから入力
- `READVEC2(n, m, v1, v2, ...)`：すでにある vector `v1`, `v2`, $\dots$ について、サイズ $n\times m$ にしてから入力

#### IN

変数を宣言と同時に入力するマクロ。

- `IN(T, ...)`：`T` 型の値（複数可）
  - `READ(...)` が対応している型なら OK、ただし注意点：
    - カンマが入っている型に注意（`pair<int, int>` など。`using pii = pair<int, int> IN(pii, p)` のようにする）
    - vector は宣言後に resize する必要があるため `IN` を使ってもうまくいかない。`VEC` や `VEC2` を使う
　
- `CHAR(...)`
- `INT(...)`
- `LL(...)`
- `STR(...)`

　
- `ARR(T, n, ...)`：`array<T, n>` 型の値
- `VEC(T, n, ...)`：`vc<T>` 型の値、宣言してサイズを $n$ にして入力
- `VEC2(T, n, m, ...)`：`vc<vc<T>>` 型の値、宣言してサイズを $n \times m$ にして入力
- `JAG(T, n, ...)`：次のような入力形式（各行の列数が行の先頭にくる）の際に `vc<vc<T>>` 型の値を入力。$n$ は $1$ 次元目のサイズ。
  ```
  n
  k[1] a[1][1] ... a[1][k[1]]
  :
  k[n] a[n][1] ... a[n][k[n]]
  ```

---

`READ`, `IN` のどちらでも、手元では入力するたびに `dump` するようにしている。木の入力を $N$ 個にして無限ループか？と勘違いするなどはよくあるが、これをすると気づきやすくなると思う。

### 出力

#### WRITE と PRINT

- `WRITE(...)`：出力（引数は複数、区切りなし、改行なし）
- `PRINT(...)`：出力（引数は複数、空白区切り、最後に改行）

対応している型は `READ` と同じ。tuple-like や vector は要素を空白区切り。

#### それ以外

- `PRINTV(...)`：改行区切りで出力
  - `vpll`, `vstr`, `vvc<T>` などに
- `PRINTRETURN(...)`：`PRINT` して `return` する
  - `main` の外に `main2` を作っておけば、複数テストケースの場合も `PRINTRETURN` でよい
- `PRINTVRETURN(...)`：`PRINTV` して `return` する
- `PRINTEXIT(...)`：`PRINT` して `exit(0)` する
  - あまり使わない（そもそも単一テストケースでないと使えない）。再帰関数の中身で使うくらいか？（手元でサンプルを複数試せなくなるが）
- `PRINTVEXIT(...)`：`PRINTV` して `exit(0)` する

### その他

#### 0-indexed に直す

`offset(vc<T> v, T add)` や `offset(vvc<T> v, T add)` で `v` の各要素に `add` が足される。

`T` は tuple-like な型でも可。

副産物として、tuple-like な型どうしが `+` で足し算できる。

#### zip, unzip

`vc<tuple-like<T1, T2, ...>>` (vt) と `tuple-like<vc<T1>, vc<T2>, ...>` (tv) を相互変換する。vt → tv が `unzip` で、tv → vt が `zip`。

思想としては

```
A_1 A_2 ... A_N
B_1 B_2 ... B_N
```

と

```
A_1 B_1
A_2 B_2
:
A_N B_N
```

をうまく扱いたいというところからきている（前者が vector 2 つ、後者が pair の vector で、これを相互変換したい）。

このような入力は、for 文で手癖で受け取ると入力形式ミスが発生しがちだが、このテンプレを使い始めてからその手のミスが非常に少なくなったと感じている。

また入力以外にも、vector 2 つを pair の vector にしてソートして vector 2 つに戻す、といった場面でも活躍する。

---
