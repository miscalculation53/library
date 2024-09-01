## テンプレート（関数オブジェクト）

- `max_op`：$\max$ の関数オブジェクト
- `min_op`：$\min$ の関数オブジェクト
- `max_e`：定数 `INF` の関数オブジェクト
- `min_e`：定数 `-INF` の関数オブジェクト
- `zero_fn`：定数 $0$ の関数オブジェクト

たとえば、`segtree` 等の宣言で使う：

`segtree<ll, max_op{}, max_e{}> seg(vec);`