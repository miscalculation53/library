## 概要

rollback 可能な Mo's algorithm

## 詳細なドキュメント

#### mo_rollback

```cpp
(1) void mo_rollback(
      int n, const vc<pair<I, I>>& lrs,
      add_l, add_r, snapshot, rollback, reset, rem)
(2) void mo_rollback(
      int n, const vc<pair<I, I>>& lrs,
      add, snapshot, rollback, reset, rem)
```

削除が難しい静的列の区間クエリを、snapshot と rollback を使ってオフライン処理する。

- `add_l(l, r)`：現在の区間が $[l+1, r)$ のとき、添字 $l$ を左側に追加する。
- `add_r(l, r)`：現在の区間が $[l, r)$ のとき、添字 $r$ を右側に追加する。
- `snapshot()`：現在の状態を保存する。
- `rollback()`：直前の snapshot の状態へ戻す。
- `reset()`：空区間に対応する初期状態へ戻す。
- `rem(qid)`：現在の状態から `lrs[qid]` の答えを記録する。

(2) では `add(i, is_left)` を渡す。左からの追加では `is_left == true`、右からの追加では `false` になる。

##### 制約

- $n \geq 0$
- 各クエリ $[l, r)$ について $0 \leq l \leq r \leq n$

##### 計算量

$q = \lvert \mathrm{lrs} \rvert$ として

- $O((n+q)\sqrt{q})$ 回の `add` 呼び出し
- $O(q)$ 回の `snapshot`, `rollback`, `reset`, `rem` 呼び出し
