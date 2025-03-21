## 並列二分探索

#### parallel_binsearch

```cpp
pair<vc<T>, vc<T>> parallel_binsearch(int q, auto judge, T init_ok, T init_ng)
```

$q$ 個の関数 $\mathrm{judge}_i\colon$ `T` $\to$ `bool` があり、いずれも $\mathrm{judge}_i(\textrm{init\_ok}) = \mathrm{true}$, $\mathrm{judge}_i(\textrm{init\_ng}) = \mathrm{false}$ を満たしているとする。このとき各 $i$ について $\mathrm{judge}_i(\mathrm{ok}_i) = \mathrm{true}$, $\mathrm{judge}_i(\mathrm{ng}_i) = \mathrm{false}$, $\lvert \mathrm{ok}_i - \mathrm{ng}_i \rvert = 1$ を満たす $\mathrm{ok}_i, \mathrm{ng}_i$ を求めたい。

これは、$q$ 個の $\mathrm{judge}_i(x_i)$ たちをまとめて求めることが高速にできればよい。（$1$ 個ずつ求めることが可能な場合はそもそも普通の二分探索でよいので、このパートの解法は自然と クエリ先読み + イベントソート のようになる。）

引数に渡す `judge` は、$x_i$ たちを受け取り、 $\mathrm{judge}_i(x_i)$ たちをまとめて求めて返す関数。

返り値は、first が $\mathrm{ok}_i$ を並べたもの、second が $\mathrm{ng}_i$ を並べたもの。

使う際の注意：**`judge` の中で長い vector を宣言しない（外で宣言する）**。典型定数倍高速化テクではあるのだが、思ったより変わるので注意。

##### 計算量

`judge` の計算量が $O(q + T)$ であるとき

- $O((q + T)\log \lvert \textrm{init\_ok} - \textrm{init\_ng} \rvert)$


#### parallel_binsearch_real

実数にしただけ（説明は省略）