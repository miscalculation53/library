## テンプレート（入出力）

参考： https://trap.jp/post/1224/

### 入力

- `cin >>` で tuple-like な型（`pair`, `tuple`, `array`）にも入力できる
- 変数を宣言と同時に入力するマクロ
  - `INT(...)`：`int` 型の値（引数は複数）
  - `LL(...)`：`ll` 型の値（引数は複数）
  - `STR(...)`：`string` 型の値（引数は複数）
  - `ARR(T, n, ...)`：`array<T, n>` 型の値（第 $3$ 引数以降は複数）
  - `VEC(T, n, ...)`：`vector<T>` 型の値（サイズは $n$）（第 $3$ 引数以降は複数）
  - `VEC2(T, n, m, ...)`：`vector<T>` 型の値（サイズは $n \times m$）（第 $4$ 引数以降は複数）
- （すでにある変数に入力するマクロ（`INPUT` から始まる諸々）も副産物としてあるが、あまり使わないと思う）

マクロに型を入れる場合、カンマが入っている型（`pair<int, int>` など）でうまく動かないので注意（`pll` 的なものを `using` するとよい）。

### 出力

- `PRINT(...)`：出力（引数は複数、空白区切り、最後に改行）
- `PRINTVEC(vc<T> v)`：`v` を空白区切りで出力し、最後に改行
- `PRINTVECT(vc<T> v)`：`v` を改行区切りで出力
  - `vstr` にも使える
- `PRINTVEC2(vvc<T> v)`：$2$ 次元配列 `v` をいい感じに出力
- `PRINTEXIT`：`PRINT` して `exit(0)` する
  - あまり使わない（そもそも単一テストケースでないと使えない）、再帰関数の中身で使うくらいか？
- `PRINTRETURN`：`PRINT` して `return` する
  - `main` の外に `main2` を作っておけば、複数テストケースの場合も `PRINTRETURN` でよい

改行の際の flush は、`FAST_IO` が define されていれば flush しない、define されていなければ flush する。

このあたりの思想は [テンプレート全体](../../verify/yosupo/many_aplusb.test.cpp) も見るとよいかも。

### その他

#### 0-indexed に直す

`offset(vc<T> v, T add)` や `offset(vvc<T> v, T add)` で `v` の各要素に `add` が足される。

`T` は tuple-like な型でも可。

副産物として、tuple-like な型どうしが `+` で足し算できる。

#### 転置

`vc<tuple-like<T1, T2, ...>>` と `tuple-like<vc<T1>, vc<T2>, ...>` を相互変換するには `top()` を用いる。名前の由来は $\LaTeX$ の転置記号 $\top$ を出力するコマンド `\top`。

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

をうまく扱いたいというところからきている（前者が vector 2 つ、後者が pair の vector で、相互に変換できると便利じゃね？）。

---

### 使用例（入力）

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
auto [A, B] = top(AB);
```
  
これで `AB` は `vc<pll>` に、`A`, `B` はそれぞれ `vc<ll>` になる。
  
ソートしてから `top` を噛ませるような使い方もできる。vector 2 つで与えられたものを、pair の vector にして、ソートして、vector 2 つに戻す、というようなこともできる。
  
### 例 5：グラフ
```
3
1 2 100
2 3 200
```

```cpp
LL(N);
VEC(tlll, N - 1, ABC);
offset(N, tlll{-1, -1, 0});
// これで ABC = {{0, 1, 100}, {1, 2, 200}} となる
```

一旦 `vc<pll>` なり `vc<tlll>` なりで受け取って、その配列をグラフライブラリに渡す、という設計を考えている。

####  例 6：二次元配列
  
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

#### 例 7：二次元の文字

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
