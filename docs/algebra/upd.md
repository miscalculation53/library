このドキュメントには AI による作成・変更が含まれています。

## 概要

値を更新する関数を合成で扱うモノイド。

## 使用例

```cpp
using M = MonoidUpdFlag<ll>;
auto f = M::S(3);
auto g = M::S(5);
auto h = M::op(f, g); // 先に g、次に f
```

## 詳細なドキュメント

### MonoidUpd

```cpp
MonoidUpd<T, idnty>
```

`idnty` を恒等作用、それ以外の値をその値への更新作用として扱う。要素型 `S` は `T`。

#### op

```cpp
S op(S f, S g)
```

先に `g`、次に `f` を適用する作用を返す。`f == idnty` なら `g`、そうでなければ `f` を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

恒等作用 `idnty` を返す。

##### 計算量

- $O(1)$

### MonoidUpdFlag

```cpp
MonoidUpdFlag<T>
```

要素型 `S` は、恒等作用かを表す `is_id` と更新値 `val` を持つ。

#### S のコンストラクタ

```cpp
(1) S()
(2) S(T val)
(3) S(bool is_id, T val)
```

(1) は恒等作用、(2) は `val` への更新作用、(3) は両メンバを直接指定した作用を構築する。

##### 計算量

- $O(1)$

#### op

```cpp
S op(const S& f, const S& g)
```

先に `g`、次に `f` を適用する作用を返す。`f.is_id` なら `g`、そうでなければ `f` を返す。

##### 計算量

- $O(1)$

#### e

```cpp
S e()
```

恒等作用 `{true, T{}}` を返す。

##### 計算量

- $O(1)$

### 各型の pow

```cpp
template <class I>
static constexpr S pow(const S& x, I k);
```

`MonoidUpd`・`MonoidUpdFlag` に共通する。正の指数では `x` を返す。指数 $k$ は非負整数とし、`k = 0` なら `e()` を返す。型と整数倍の条件は [pow の共通仕様](monoid_power.md) を参照。

##### 計算量

- $O(1)$。整数倍を使う場合は、その演算を $O(1)$ とする。
