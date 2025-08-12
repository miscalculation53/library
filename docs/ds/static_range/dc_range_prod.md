## 分割統治による静的列の区間積クエリ

https://maspypy.com/%e5%88%86%e5%89%b2%e7%b5%b1%e6%b2%bb%e3%81%ab%e3%82%88%e3%82%8b%e9%9d%99%e7%9a%84%e5%88%97%e3%81%ae%e5%8c%ba%e9%96%93%e7%a9%8d%e3%82%af%e3%82%a8%e3%83%aa

静的列に対するモノイドの区間積クエリ $Q$ 個を、$O(N \log N + Q)$ 時間、$O(N + Q)$ 空間でまとめて処理する。

- 似たようなアイデアでオンラインでできるようにしたのが disjoint sparse table とみなせる。
- 「一般の $2$ 元の積は時間がかかるが、列にもとある元と一般の元の積は高速」なときにも使える。具体的には、列にもとある元と一般の元の積を $O(N \log N)$ 回、一般の元の積を $O(Q)$ 回。
  - 他にこの特徴を持つ手法：
    - disjoint sparse table
    - SWAG
      - これはクエリが queue や deque のようになっている場合だけ
- その他、クエリの区間幅が常に小さいときなどに速いらしい（冒頭の maspy さんの記事参照）。

#### dc_range_prod

```cpp
(1) vc<M::S> dc_range_prod<M>(vc<M::S> v, vc<pair<I, I>> lrs)
(2) vc<pair<M::S, M::S>> dc_range_prod_left_right<M>(vc<M::S> v, vc<pair<I, I>> lrs)
```

$\mathrm{lrs}$ の長さを $q$ とし、$i$ 番目の要素を $(l_i, r_i)$ と書く。

- (1)：長さ $q$ の vector を返す。$i$ 番目の要素は、$v_{l_i} \bullet \cdots \bullet v_{r_i-1}$ である。
- (2)：長さ $q$ の vector を返す。$i$ 番目の要素は、$v_{l_i} \bullet \cdots \bullet v_{r_i-1} = w_l \bullet w_r$ を満たす $(w_l, w_r)$ のひとつである。
  - 使用場面：最終的な答えとしては一般の元の積がいらない場合（例： https://yukicoder.me/problems/no/2215 ）。

##### 制約

$v$ の長さを $n$ として

- $0 \leq l_i \leq r_i \leq n$

##### 計算量

- $O(n \log n + q)$
