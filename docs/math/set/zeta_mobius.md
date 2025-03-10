## ゼータ・メビウス変換

ゼータ変換には可換モノイドが、メビウス変換には可換群が載る。以下に述べる定義は、演算を $+$ とした場合のもの。

### 定義

#### ゼータ変換（部分集合）

$\displaystyle \zeta a(s) = \sum_{t \subseteq s} a(t)$

#### メビウス変換（部分集合）

$\mu \zeta a(s) = a(s)$

$\displaystyle \mu a(s) = \sum_{t \subseteq s} (-1)^{\lvert s \setminus t \rvert} a(t)$

#### ゼータ変換（上位集合）

$\displaystyle \zeta' a(s) = \sum_{s \subseteq t} a(t)$

#### メビウス変換（上位集合）

$\mu' \zeta' a (s) = a(s)$

$\displaystyle \mu' a(s) = \sum_{s \subseteq t} (-1)^{\lvert t \setminus s \rvert} a(t)$


### ライブラリ

#### zeta_subset

```cpp
vc<M::S> zeta_subset(vc<M::S> a)
```

##### 制約

- `M` は**可換**モノイド
- $\lvert a \rvert$ は $0$ または $2$ べき


##### 計算量

$\lvert a \rvert = n$ として

- $O(n 2^n)$

#### mobius_subset

```cpp
vc<G::S> mobius_subset(vc<G::S> a)
```

##### 制約

- `G` は**可換**群
- $\lvert a \rvert$ は $0$ または $2$ べき


##### 計算量

$\lvert a \rvert = n$ として

- $O(n 2^n)$

---

zeta_supset, mobius_supset も同様（省略）