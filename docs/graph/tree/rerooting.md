## 全方位木 DP

https://trap.jp/post/1702/

可換モノイドの演算 $\oplus$ と、関数 $\mathrm{putv}_v(\mathrm{val}), \mathrm{pute}_e(\mathrm{val})$ があるとする。根を $r$ として、各頂点 $v$ について、次の $\mathrm{res}_r[\cdot]$ を計算する木 DP を考える：

$\displaystyle \mathrm{res}_r[v] = \mathrm{putv}_v\left(\bigoplus_{c\in \mathrm{children}(v)} \mathrm{pute}_{(v, c)}(\mathrm{res}_r[c]) \right)$

このとき、すべての $r$ について $\mathrm{res}_r[r]$ を計算する。

### 中身

まず、ある根について普通の木 DP を計算しておいて

$\begin{aligned}
f[v] &= \bigoplus_{c \in \mathrm{children}(v)} \mathrm{pute}_{(v,c)}(g[c]) \\\\  
g[v] &= \mathrm{putv}_v(f[v])
\end{aligned}$

としておく。$f$ は部分木全体について pute までしたもので、$g$ はそれを putv したもの。

```
   q2 
    ↓
   q1
    ↓
    p
 ↙︎  ↓ ↘︎
d1  v d2
 ↙︎  ↓ ↘︎
c1 c2 c3
```

上のような図をイメージする。

$\begin{aligned}
h[v] &= \mathrm{pute}_{(p,v)}\left(\mathrm{putv}_p\left(\left(\bigoplus_{\substack{d \in \mathrm{children}(p) \\\\  d \neq v}} \mathrm{pute}_{(p,d)}(g[d]) \right) \oplus h[p]\right)\right) \\  
\mathrm{res}_v[v] &= \mathrm{putv}_v(f[v] \oplus h[v])
\end{aligned}$

となっていることがわかる。$h$ は親方向全体について pute までしたもの。

$\displaystyle \bigoplus_{\substack{d \in \mathrm{children}(p) \\\\  d \neq v}} \mathrm{pute}_{(p,d)}(g[d])$ の部分は、左右からの累積和があれば計算できる。

### ライブラリ

#### rerooting

```cpp
vc<M::S> rerooting<M>(int n, vc<pair<I, I>> es, auto pute, auto putv, bool debug = true)
```

- 木の頂点数 `n`
- 辺を格納した vector `es`
- 関数 `M::S pute(M::S val, int eid)`
- 関数 `M::S putv(M::S val, int v)`

を渡すと、長さ $n$ の vector を返す。$v$ 番目には $\mathrm{res}_v[v]$ が格納される。

##### 制約

- `M` は可換モノイド

##### 計算量

- $O(n)$
