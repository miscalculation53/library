## SWAG

stack, queue, deque について、「現在格納されているすべての要素のモノイド積」が取得できるようにしたもの。

「一般の $2$ 元の積は時間がかかるが、push する元と一般の元の積は高速」なときにも使える。

アイデア：stack は簡単。queue, deque は、stack 2 つで作るテクを使う。

### コンストラクタ

```cpp
(1) FoldableStack<M>
(2) FoldableQueue<M>
(3) FoldableDeque<M>
```

`M` はモノイド。`M::S` がデータ構造に格納される。

### メンバ関数

普通の stack, queue, deque でもできる操作は、それにおおむね準拠。計算量はどれも償却 $O(1)$。

#### all_prod

```cpp
(1) M::S all_prod()
(2) pair<M::S> all_prod_left_right()
```

- (1)：現在格納されている要素の積を取得する。
- (2)：`FoldableQueue`, `FoldableDeque` のみ。$x \bullet y$ が現在格納されている要素の積に一致するような $(x, y)$ を一つ返す。
  - 使用場面：最終的な答えとしては一般の元の積がいらない場合（例： https://yukicoder.me/problems/no/2215 ）。

##### 計算量

- $O(1)$


#### content

```cpp
vc<M::S> content()
```

現在格納されている要素をすべて格納した vector を返す。デバッグ用想定。

##### 計算量

- $O(要素数)$
