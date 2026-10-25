このドキュメントには AI による作成・変更が含まれています。

## 概要

trie 木を管理するライブラリ。

思想としては、このライブラリは木自体の管理に集中し、ノードに値を持ったりするのはその場で書く。また、文字列は最初から $[0, \sigma)$ の整数に変換して考える（テンプレの `stov`, `vtos` を使う）。

## 使用例

```cpp
#include "string/trie.hpp"

Trie<26> trie;
assert(!trie.parent(0) && !trie.child(0, 0));
auto path = trie.insert(vc<int>{0, 1, 2});  // "abc"
assert(path.front() == 0 && path.size() == 4);
assert(trie.to_string<int>(path.back()) == vc<int>({0, 1, 2}));
assert(trie.insert_last(vc<int>{0, 1}) == path[2]);
```

## 詳細なドキュメント

#### コンストラクタ

```cpp
Trie<int CHAR_NUM>()
```

文字の種類数 `CHAR_NUM` $=\sigma$ を指定する。

#### num_of_vertices

```cpp
I num_of_vertices<I=ll>()
```

現在の頂点数を返す。

##### 計算量

- $O(1)$

#### parent

```cpp
optional<I> parent<I = ll>(int v) const
```

頂点 $v$ の親の頂点番号を返す。$v$ が根なら `nullopt` を返す。

##### 制約

- $0 \leq v < 頂点数$

##### 計算量

- $O(1)$

#### child

```cpp
optional<I> child<I = ll>(int v, int c) const
```

頂点 $v$ の文字 $c$ 方向への子を返す。対応する子がなければ `nullopt`。

##### 制約

- $0 \leq v < 頂点数$
- $0 \leq c < \sigma$

##### 計算量

- $O(1)$

#### insert

```cpp
vc<I> insert(vc<C> s, int v = 0)
```

頂点 $v$（デフォルトでは根）から始めて文字列 $s$ を挿入する。そして、途中で現れた頂点の列を返す（この列は最初が $v$ で長さ $\lvert s \rvert + 1$）。

##### 制約

- $0 \leq v < 頂点数$
- $0 \leq s_i < \sigma$

##### 計算量

- $O(\lvert s \rvert \sigma)$

#### insert_last

```cpp
template <class I = ll, class C>
I insert_last(const vc<C>& s, int v = 0)
```

`insert` と同じ文字列を挿入し、終点の頂点番号を返す。終点だけが必要な場合に、経路配列の確保と書き込みを省ける。

##### 制約

- $0 \leq v < 頂点数$
- $0 \leq s_i < \sigma$

##### 計算量

- $O(\lvert s \rvert\sigma)$

#### to_string

```cpp
vc<C> to_string(int v)
```

頂点 $v$ が表す文字列を vector として返す。

##### 制約

- $0 \leq v < 頂点数$

##### 計算量

- $O(\lvert v \rvert \sigma)$
