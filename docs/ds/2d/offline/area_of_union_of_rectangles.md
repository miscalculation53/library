## 概要

左端と右端をイベントとして $x$ 座標順に平面走査する。$y$ 軸上では「区間への被覆数加算」と「被覆数が $0$ でない部分の長さ」が必要になる。

被覆数の最小値と、その最小値を取る区間の長さを遅延セグ木で管理する。最小値が $0$ ならその長さを全体から引き、正なら全体が覆われている。

## 詳細なドキュメント

#### コンストラクタ

```cpp
AreaOfUnionOfRectangles<T=ll> area;
```

空のデータ構造を構築する。`T` は座標と面積の型。

##### 計算量

- $O(1)$

#### rectangle_add

```cpp
void rectangle_add(T lx, T rx, T ly, T ry)
```

半開区間 $[\mathrm{lx}, \mathrm{rx}) \times [\mathrm{ly}, \mathrm{ry})$ の長方形を追加する。

##### 制約

- $\mathrm{lx} \leq \mathrm{rx}$
- $\mathrm{ly} \leq \mathrm{ry}$
- 座標差と答えが `T` に収まる

##### 計算量

- 償却 $O(1)$

#### run

```cpp
T run()
```

追加済みの長方形の和集合の面積を返す。

##### 計算量

追加した長方形の個数を $q$ として、

- 時間：$O(q \log q)$
