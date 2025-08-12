## 逆元をまとめて求める

$a_0, \dots, a_{n-1}$ の mod 逆元を $O(n + \log p)$ で求める。

方法：$(a_i)_i$ の累積積と $(a_i^{-1})_i$ の累積積がわかっていればできるが、後者は全体の積の逆元を $O(\log p)$ で求めてからかけていけばよい。

#### inv_many

```cpp
vc<mint> inv_many(vc<mint> a)
```

$a_0^{-1}, \dots, a_{n-1}^{-1}$ を格納した vector を返す。

##### 計算量

- $O(n + \log p)$
