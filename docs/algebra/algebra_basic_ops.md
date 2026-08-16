## 概要

`+`, `-`, `*`, `/`, `min`, `max`, `0`, `1` を使って定義できるやつ

## 詳細なドキュメント

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

### 累乗

`MonoidAdd<T>`, `MonoidMin<T, infty>`, `MonoidMax<T, infty>`, `GroupAddSub<T>` は、反復演算を直接計算できる場合に `pow` を持つ。`MonoidAdd<T>` と `GroupAddSub<T>` では `T * I` が定義されている場合に限り使用できる。

#### pow_monoid

```cpp
M::S pow_monoid<M>(M::S a, I k)
```

`a` をモノイド `M` の演算で `k` 回まとめた値を返す。`M::pow(a, k)` が使用できる場合はそれを呼び、使用できない場合は二分累乗を行う。したがって、各モノイドに二分累乗を呼ぶだけの `pow` を定義する必要はない。

##### 制約

- $k \geq 0$

##### 計算量

- `M::pow(a, k)` が使用できる場合：その計算量
- それ以外：`M::op` を $O(\log k)$ 回

#### pow_group

```cpp
G::S pow_group<G>(G::S a, I k)
```

`a` を群 `G` の演算で `k` 回まとめた値を返す。`k < 0` の場合は `G::inv(a)` を $-k$ 回まとめる。非負部分の計算には `pow_monoid<G>` を使う。

##### 計算量

- `pow_monoid<G>` と同じ
