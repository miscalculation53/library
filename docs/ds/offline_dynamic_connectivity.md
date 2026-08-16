## 概要

オフラインダイコネ以外にも使える。

使用場面（ざっくり）：「追加」「削除」は難しいが「追加」「undo」ならできるとき。

## 詳細なドキュメント

#### offline_dynamic_connectivity

```cpp
void offline_dynamic_connectivity(int tmax, vc<tuple<I, I, Obj>> span_obj, auto add, auto undo, auto run)
```

- $t_{\max}$：時系列が $[0, t_{\max})$ であることを表す
- `span_obj`：生存期間 $[l, r)$ のオブジェクト $\mathrm{obj}$ について $(l, r, \mathrm{obj})$ を格納したもの
- `aad(Obj obj)`：オブジェクトの追加処理（**可換**）
- `undo()`：オブジェクトの追加を $1$ 回分戻す処理

このとき、$t = 0, 1, \dots, t_{\max}-1$ の順に `run(t)` が実行される（`run(t)` は時刻 $t$ に存在するオブジェクトが追加された状態で実行される）。

undo 可能 UF を使ったオフラインダイコネであれば、オブジェクトは辺に相当する。

##### 計算量

- $O(t_{\max} + \lvert \mathrm{span\_obj} \rvert \log t_{\max})$

#### add_del_to_span

```cpp
vc<tuple<I, I, Obj>> add_del_to_span(int tmax, vc<pair<Time, Obj>> time_obj)
```

追加・削除クエリとして与えられるときで、「存在するときに追加」「存在しないときに削除」が飛んでこないとする。これは、時刻とオブジェクトのペアが「存在しないなら追加、存在するなら削除」という形で与えられることに相当する。この (時刻, オブジェクト) を `time_obj` に並べてこの関数を適用すると、`offline_dynamic_connectivity` に渡すべき `span_obj` の形に変換してくれる。

##### 制約

- `Obj` は比較可能

特に、**同じオブジェクトが同じとみなされるように十分注意すること**。たとえば UF での使用例での $(u, v)$ と $(v, u)$ など。

##### 計算量

- $O(\lvert \mathrm{time\_obj} \rvert \log \lvert \mathrm{time\_obj} \rvert)$
