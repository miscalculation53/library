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
