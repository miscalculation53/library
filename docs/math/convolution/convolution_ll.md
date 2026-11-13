このドキュメントには AI による作成・変更が含まれています。

## 概要

畳み込み（long long）

## 使用例

```cpp
auto c = convolution_4e18(vc<ll>{1, -2}, vc<ll>{3, 4});
assert((c == vc<ll>{3, -2, -8}));
```

## 詳細なドキュメント

#### convolution_4e18

```cpp
template<class I = ll, class J = ll>
vc<ll> convolution_4e18(const vc<I>& a, const vc<J>& b)
```

畳み込みの最終結果が $4.2 \times 10^{18}$ 程度に収まる場合に、結果を `ll` で計算する。

ACL の `convolution_ll` は `ll` の範囲を全部できるようにしている代わりに mod が $3$ つになっている。欲しくなったらこれも作るかも。

入力の整数型はそれぞれ独立に選べ、`i128`・`u128` にも対応する。入力値は `ll` に収まるものとし、`ll` に変換して計算する。

##### 制約

- $n+m-1 \leq 2^{25}$
  - ただし、ここまで大きいとそもそも TLE する

##### 計算量

- $O((n+m)\log(n+m))$
  - 定数倍は NTT の $2$ 倍程度つく。
  - サイズが小さい場合や $a$ と $b$ のどちらかが疎な場合、$a, b$ の非零要素の個数を $n^{\prime}, m^{\prime}$ として $O(n + m + \min(n^{\prime}m, nm^{\prime}))$ の実装に切り替える。
