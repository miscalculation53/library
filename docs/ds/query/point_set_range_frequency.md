## 概要

中身：値ごとに添字集合を管理する。先読みした添字で座圧して Fenwick Tree を用いる（この Fenwick Tree は 01 にできる）。

オンラインでの処理が必要な場合は、Fenwick Tree の代わりに平衡二分探索木などを用いる（特に本ライブラリで整備した `pbds_set` を使うと便利で、かなりすぐ実装できる）。いかにも遅そうだが Library Checker に提出してみた感じ意外と速かった（？）

## 詳細なドキュメント

#### コンストラクタ

```cpp
PointSetRangeFrequency(vc<T> a)
```

列を $a$ で初期化する。

##### 制約

- `T` は比較可能

##### 計算量

- $O(\lvert a \rvert)$

#### point_set

```cpp
void point_set(int p, T v)
```

「添字 $p$ の値を $v$ に更新する」というクエリを追加する。

##### 制約

- $0 \leq p < \lvert a \rvert$

##### 計算量

- $O(1)$

#### range_frequency

```cpp
void range_frequency(int l, int r, T v)
```

「区間 $[l, r)$ の中で値が $v$ である添字の個数を求める」というクエリを追加する。

##### 制約

- $0 \leq l \leq r \leq \lvert a \rvert$

##### 計算量

- $O(1)$

#### run

```cpp
vc<I=ll> run()
```

すべてのクエリにまとめて答え、結果を返す。結果は呼び出し順に格納される。

##### 計算量

$n = \lvert a \rvert$、$q$ をクエリの個数として

- $O((n + q) \log (n + q))$
