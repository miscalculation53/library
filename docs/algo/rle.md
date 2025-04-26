## ランレングス圧縮

#### rle

```cpp
(1) vc<rle_info<T>> rle(vc<T> a)
(2) vc<rle_info<char>> rle(string s)
```

ランレングス圧縮し、連ごとに

- 値 $\mathrm{val}$
- 長さ $\mathrm{len}$
- もとの列での区間 $[\mathrm{left}, \mathrm{right})$

を持った構造体を格納した vector を返す。

##### 計算量

vector や文字列の長さを $n$ として

- $O(n)$
