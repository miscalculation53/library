このドキュメントには AI による作成・変更が含まれている可能性があります。

## 概要

費用行列から最小費用の割当を求める。`n <= m` の長方形に対応し、各行を異なる列へ割り当てる。負の費用も扱う。

行列上で最短増加路を探すハンガリアン法を使う。正方形では Jonker–Volgenant 型の列簡約・余裕の移動・2巡の行簡約で初期割当を作る。行簡約は追い出した行をすぐ再処理し、各巡回の行走査回数を $n$ に制限する。長方形では競合しない行最小値から割当を始める。同距離の列をまとめて処理し、双対変数は探索後に更新する。作業領域は $O(n+m)$。入力行列は参照して使う。

正方形では隣接 $2\times2$ を調べ、Monge／逆 Monge の場合は直接解く。計算用の型に既定値や自動拡張はなく、`Hungarian<ll>` などと指定する。C++17 の型推論では入力の要素型を使う。

初期化と最短増加路探索は、BSD-2-Clause の [gatagat/lap の実装](https://github.com/gatagat/lap/blob/e29341d9e0ec8c3a1f11f239eb65a31973f4a0ab/_lapjv_cpp/lapjv.cpp)を基に書き換えた。整数型、長方形、探索領域の再利用、双対解の返却をこのライブラリに合わせている。

上流の[ライセンス全文・著作権表示](https://github.com/gatagat/lap/blob/e29341d9e0ec8c3a1f11f239eb65a31973f4a0ab/LICENSE)をヘッダに保持する。ソースを再配布する際はこの表示を保持し、バイナリを再配布する際は添付資料等に同じ表示を含める。

## 使用例

```cpp
vvc<int> a{{4, 3, 5}, {3, 5, 9}, {4, 1, 4}};
Hungarian<ll> h(a);
assert(h.cost == 9);
for (int i = 0; i < 3; i++) {
  int j = h.row_to_col[i];
  assert(h.row_potential[i] + h.col_potential[j] == a[i][j]);
}

vvc<i128> large{{i128(1) << 90, 0}, {0, i128(1) << 90}};
Hungarian<i128> h128(large);
```

## 詳細なドキュメント

### Hungarian

#### コンストラクタ

```cpp
template <class Cost>
struct Hungarian;

template <class Matrix>
explicit Hungarian(const Matrix& a);

template <class Input>
Hungarian(int n, int m, const Input* a, int stride = -1);
```

最小費用割当を計算する。`Matrix` は `vector<vector<T>>`、ライブラリの `Matrix`、`array<array<T, m>, n>` など、各行が連続配置された二次元配列。空の二次元配列は $0\times0$ として扱う。

ポインタ版は `a[i * stride + j]` を費用として使う。`stride = -1` は `stride = m` を表す。$0\times m$ も指定できる。

`Cost` は費用・距離・双対変数・総費用の型。入力の要素型より広い型を明示できる。たとえば Library Checker の制約では、入力は `int`、計算は `ll` とする。

##### 制約

- $0\le n\le m$。二次元配列の行の長さは等しい。
- ポインタ版は `stride >= m` で、参照する要素が存在する。
- 全ての行・列の組に有限の費用がある。
- `Cost` は符号付き整数型。`i128` に対応する。
- 入力の費用、計算中の距離・双対変数・余裕、最終的な総費用が `Cost` に収まる。和の相殺に伴う一時的なはみ出しは同じ幅の符号なし演算で処理する。

##### 計算量

- 一般の場合 $O(n^2m)$。正方形では $O(n^3)$。
- 正方形の Monge／逆 Monge 行列では判定を含め $O(n^2)$。

#### 結果

```cpp
Cost cost;
vector<int> row_to_col, col_to_row;
vector<Cost> row_potential, col_potential;
```

- `cost`：最小総費用。
- `row_to_col[i]`：行 `i` に割り当てた列。
- `col_to_row[j]`：列 `j` に割り当てた行。未割当は `-1`。
- `row_potential[i]`, `col_potential[j]`：双対解。常に $u_i+v_j\le a_{ij}$、割当辺では等号となる。双対目的値 $\sum_i u_i+\sum_j v_j$ は `cost` と一致する。

長方形で $n<m$ の場合、$v_j\le0$、未割当列では $v_j=0$。同じ最小総費用を達成する割当が複数ある場合、そのうち1つを返す。

## Monge の場合

行と列の順序が決まっていて、

$$
a_{ij}+a_{kl}\le a_{il}+a_{kj}\quad(i<k,\ j<l)
$$

を満たす行列を Monge 行列と呼ぶ。交差する2組の割当を順序通りに入れ替えると、費用が増えない。このため正方形の完全割当では `row_to_col[i] = i` が最適となる。不等号を逆にした逆 Monge では `row_to_col[i] = n-1-i` が最適となる。[Burkard ほかの解説](https://pure.tue.nl/ws/files/2373438/Metis148543.pdf)

Monge 性が保証されていて、費用を $O(1)$ で評価できる場合、割当の構成と費用の合計は $O(n)$。この実装は一般の行列を受け取るため、隣接 $2\times2$ の条件を $O(n^2)$ で確認する。

たとえば昇順の列 $x,y$ に対する $a_{ij}=\lvert x_i-y_j\rvert$ や $(x_i-y_j)^2$ は Monge。行・列ごとの定数を加えても Monge 性は保たれる。

長方形では、列を飛ばせる順序付き割当に帰着できる。単純な DP でも $O(nm)$ となる。SMAWK が高速に求めるのは行ごとの最小値であり、割当で必要な列の重複回避も合わせて扱う必要がある。現在のコンストラクタで直接解く高速化の対象は正方形。
