## 概要

trie に、failure link と呼ばれる逆辺を追加する。failure link の行き先 $\mathrm{fail}[s]$ は、$s$ の最長接頭辞（注：ノードと文字列を同一視する）。加えて、$\mathrm{nxt}[s][c]$ という配列も持っている。これは $s$ から始めて、$c$ 番目の子がない間は failure link をたどっていき、$c$ 番目の子がある頂点が見つかったらその子を返すものである。

Wikipedia の図が参考になりそう https://ja.wikipedia.org/wiki/%E3%82%A8%E3%82%A4%E3%83%9B%E2%80%93%E3%82%B3%E3%83%A9%E3%82%B7%E3%83%83%E3%82%AF%E6%B3%95

## 詳細なドキュメント

#### 求め方

文字列の長さ順（BFS 順）に求める。$\mathrm{fail}[s + c]$ を求めたいときは、$\mathrm{fail}[s], \mathrm{fail}[\mathrm{fail}[s]], \mathrm{fail}[\mathrm{fail}[\mathrm{fail}[s]]], \dots$ のうち $c$ 番目の子が存在する最初のものを $t$ として、$\mathrm{fail}[s + c] = t + c$ となる。$t$ を探すのに while ループを用いても実は間に合っているようなのだが、より計算量解析が自明なのは $\mathrm{nxt}$ も同時に計算する方法である。$s + c$ が存在するときは $\mathrm{nxt}[s][c] = s + c, \mathrm{fail}[s + c] = \mathrm{nxt}[\mathrm{fail}[s]][c]$ であり、存在しないときは $\mathrm{nxt}[s][c] = \mathrm{nxt}[\mathrm{fail}[s]][c]$ となる（ただし根は適当に場合分けする）。

#### 使い方

たとえば次の問題が解ける：

> https://atcoder.jp/contests/abc362/tasks/abc362_g
>
> 文字列 $S$ および $T_1, \dots, T_Q$ が与えられる。各 $T_i$ が $S$ の中に連続部分文字列として現れる回数を求めよ。

$T_1, \dots, T_Q$ で trie と failure link を構築しておく。$S$ に従ってノードをたどり、ノードを訪れた回数を記録しておく。すると、$t$ の出現回数は、$t$ を suffix に持つノードに訪れた回数の合計、つまり failure link だけからなる木で $t$ に到達可能なノードに訪れた回数の合計となる。これはボトムアップに累積和をとる木 DP でできる。

> https://atcoder.jp/contests/abc268/tasks/abc268_h
>
> 文字列 $S$ および $T_1, \dots, T_Q$ が与えられる。$S$ の $1$ 文字を選んで `*` に書き換える操作を行って、どの $T_i$ も $S$ の中に現れないようにしたい。操作回数の最小値を求めよ。

同じように trie と failure link を構築する。$S$ に従ってノードをたどる。このとき訪れてはいけないノードに訪れたタイミングで `*` に書き換えることになる（このとき根に戻ることになる）。訪れてはいけないノードというのはどれかの $T_i$ を suffix に持つノードであり、これはトップダウンな木 DP で計算しておける。

---

#### コンストラクタ

```cpp
AhoCorasick<int CHAR_NUM>(const vvc<C> &strs)
```

文字列集合 `strs` から構築する。文字の種類数 `CHAR_NUM` $=\sigma$ を指定する。

##### 制約

- $0 \leq \mathrm{strs}[i][j] < \sigma$

##### 計算量

文字列長の総和を $n$ として

- $O(n \sigma)$

#### メンバ変数

`Trie<CHAR_NUM> trie` を持つ。

#### failure

```cpp
I failure(int v)
```

頂点 $v$ の failure link の行き先、つまり $v$ の表す文字列の最長 suffix を表すノードを返す。根のときは $-1$ を返す。

なお、`RootedTree` と組み合わせて

```cpp
RootedTree<bool, false> G(GEN_VEC(aho.trie.num_of_vertices(), i, failure(i)));
```
のようにすると failure link のみからなる木が作れる。failure link は子から親への辺として描かれることが多いが、`RootedTree` の辺は親から子への辺として管理されることに注意。

##### 制約

- $0 \leq v < 頂点数$

##### 計算量

- $O(1)$

#### next

```cpp
I next(int v, int c)
```

頂点 $v$ から始めて、$c$ 番目の子がない間は failure link をたどっていき、$c$ 番目の子がある頂点が現れたらその子を返す。存在しないときは $0$ を返す。

##### 制約

- $0 \leq v < 頂点数$
- $0 \leq c < \sigma$

##### 計算量

- $O(1)$

