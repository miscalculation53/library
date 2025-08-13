## trie

trie 木を管理するライブラリ。

思想としては、このライブラリは木自体の管理に集中し、ノードに値を持ったりするのはその場で書く。また、文字列は最初から $[0, \sigma)$ の整数に変換して考える（テンプレの `stov`, `vtos` を使う）。

### コンストラクタ

```cpp
Trie<int CHAR_NUM>()
```

文字の種類数 `CHAR_NUM` $=\sigma$ を指定する。

### メンバ関数

#### num_of_vertices

```cpp
I num_of_vertices<I=ll>()
```

現在の頂点数を返す。

##### 計算量

- $O(1)$

#### parent

```cpp
I parent(int v)
```

頂点 $v$ の親の頂点番号を返す。$v$ が根のときは $-1$ を返す。

##### 制約

- $0 \leq v < 頂点数$

##### 計算量

- $O(1)$

#### child

```cpp
I child(int v, int c)
```

頂点 $v$ の文字 $c$ 方向への子（なければ $-1$）を返す。

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


#### to_string

```cpp
vc<C> to_string(int v)
```

頂点 $v$ が表す文字列を vector として返す。

##### 制約

- $0 \leq v < 頂点数$

##### 計算量

- $O(\lvert v \rvert \sigma)$
