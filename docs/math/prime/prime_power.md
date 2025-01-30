## 素べき構造体

主に素因数分解を扱うための構造体。


### メンバ変数

- `P p`：素数
- `int e`：指数
- `P pe`：素べきの値（$p^e$）


### コンストラクタ

```cpp
(1) PrimePower<P>()
(2) PrimePower<P>(P p, int e = 1)
(3) PrimePower<P>(P p, int e, P pe)
```

- (2) `p`, `e` を設定し、`pe` は自動で計算する。
- (3) `p`, `e`, `pe` を設定する。

##### 制約

- `P` は `int` か `ll`
- $p$ は素数
- $e \geq 0$
- (3) において、`pe` は $p^e$ に等しい

##### 計算量

- (2) のみ $O(\log e)$、他は $O(1)$


### メンバ関数

#### mul_p

```cpp
void mul_p()
```

素べきに $p$ を掛ける。$p^e$ が `P` に収まらないときは、$\bmod \ 2^{32}$ や $\bmod \ 2^{64}$ で等しい値にする。

#### div_p

```cpp
void div_p()
```

素べきを $p$ で割る。

##### 制約

- $e > 0$（すなわち $p^e > p$）


### 外側の関数

#### factorized_mul

整数の素因数分解形（`p` についてソート済み）を複数受け取り、その積の素因数分解形（`p` についてソート済み）を返す。`pe` がオーバーフローする場合は $\bmod \ 2^{32}$ や $\bmod \ 2^{64}$ で等しい値を返す。

```cpp
vc<PrimePower<P>> factorized_mul(vc<PrimePower<P>> fac1, vc<PrimePower<P>> fac2)
```

##### 制約

- `fac1`, `fac2` は `p` についてソート済み

##### 計算量

- $O(\lvert \mathrm{fac1} \rvert + \lvert \mathrm{fac2} \rvert)$