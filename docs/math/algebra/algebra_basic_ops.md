## 代数的構造（四則演算と min, max）

`+`, `-`, `*`, `/`, `min`, `max`, `0`, `1` を使って定義できるやつ

### モノイド

- `MonoidAdd<T>`
- `MonoidMul<T>`
- `MonoidMin<T, infty>`
- `MonoidMax<T, infty>`

### 群

- `GroupAddSub<T>`
- `GroupMulDiv<T>`

### 半環

- `SemiRingMinPlus<T>`
- `SemiRingMaxPlus<T>`

### 環

- `RingAddSubMul<T>`