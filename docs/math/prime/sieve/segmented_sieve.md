## 概要

$l, r$ が与えられたとき、各 $l \leq x \leq r$ の素因数を列挙することが、$m = \max(r-l, \sqrt{r})$ として $O(m \log \log m)$ でできる。

方法：$\sqrt{r}$ 以下の素数で各 $x$ を割り、最後に残った $1$ より大きい因数も列挙する。

素因数を `vvc` とかで持っておく実装も考えられたが、場合によっては定数倍が気になるので $f(p, x)$ の呼び出しという形になった。（イテレータみたいにしてもいいかも）

## 詳細なドキュメント

#### segmented_sieve

```cpp
void segmented_sieve(ll l, ll r, auto f)
```

次の条件を満たす整数組 $(p, x)$ すべてに対して $f(p, x)$ を呼び出す。

- $l \leq x \leq r$
- $p$ は $x$ の素因数
- $p \neq x$

##### 制約

- $1 \leq l \leq r$

##### 計算量

$m = \max(r-l, \sqrt{r})$ として

- $O(m \log \log m)$ 回の $f$ の呼び出し
