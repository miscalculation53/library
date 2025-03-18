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

### 体

- `FieldAddSubMulDiv<T>`

---

また、モノイドを渡すと累乗を計算してくれる関数 `pow_monoid` を用意した。

```cpp
M::S pow_monoid(M::S a, ll k)
```