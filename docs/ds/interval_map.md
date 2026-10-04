## 概要

値つきの半開区間 $[l, r)$ を `set` で管理する。区間代入・削除、点の値の取得、点を含む区間や次の区間の検索ができる。
保持する区間は互いに重ならず、接する同値の区間は自動的に併合する。

未登録の点の値は `dflt`。`dflt` を明示的に代入した区間も登録区間として保持する。
値なしの区間集合として使うときは、一定の値を `set` で登録し、`erase` で削除する。

更新時に同値の隣接区間まで範囲を広げ、重なる区間を取り除いて、左右に残る部分と新しい区間を挿入する。
端点には比較だけを使うため、負の座標や整数型の最小値・最大値も扱える。

## 使用例

```cpp
#include "ds/interval_map.hpp"

IntervalMap<int> mp(-1);  // 値は int、座標は既定で ll、未登録の値は -1
mp.set(2, 5, 7);
mp.set(5, 8, 7);       // [2, 8): 7 に併合
assert(mp[3] == 7 && mp[8] == -1);

mp.erase(4, 6);          // [2, 4): 7 と [6, 8): 7 が残る
auto it = mp.get_it(3);
assert(it != mp.end() && it->l == 2 && it->r == 4 && it->x == 7);
it = mp.next_it(4);
assert(it->l == 6);
dump(mp.content());      // {(2, 4, 7), (6, 8, 7)}

for (const auto &[l, r, x] : mp)
  cout << l << ' ' << r << ' ' << x << '\n';

IntervalMap<int, int> from_array(vc<int>{1, 1, 2, 2, 1});
// [0, 2): 1, [2, 4): 2, [4, 5): 1
```

コールバックで値ごとの総延長と区間数を管理する例。

```cpp
IntervalMap<int> mp;
map<int, i128> lengths;
ll segments = 0;
auto add = [&](ll l, ll r, int x) {
  lengths[x] += i128(r) - l;
  ++segments;
};
auto del = [&](ll l, ll r, int x) {
  lengths[x] -= i128(r) - l;
  --segments;
};
mp.set(0, 10, 3, add, del);
mp.set(4, 6, 5, add, del);
assert(lengths[3] == 8 && lengths[5] == 2 && segments == 3);
mp.erase(4, 6, add, del);
assert(lengths[5] == 0 && segments == 2);
```

## 詳細なドキュメント

### IntervalMap

`T` は値の型、`I` は端点の型。以降、$m$ は保持する区間数とする。
計算量は端点・値の比較とコピー、およびコールバック1回を $O(1)$ として記す。

#### コンストラクタ

```cpp
template <class T, class I = ll>
explicit IntervalMap(const T& dflt = T())
IntervalMap(const vc<T>& a, const T& dflt = T())
```

- 第1形式：空の区間集合を作る。
- 第2形式：各 $[i, i+1)$ に `a[i]` を登録し、連続する同値の区間を併合する。登録範囲は $[0, |a|)$。

`dflt` は未登録の点を `get` / `operator[]` で取得した際の値。

##### 制約

- `I` はコピー・代入ができ、`operator<` が端点に全順序を与える。
- `T` はコピーができ、`operator==` が同値関係を与える。
- `dflt` を省略するときは `T()` が有効である。
- 配列から作るときは、`I` への変換で $0, \ldots, |a|$ の添字を正確に表現できる。

##### 計算量

- 第1形式：$O(1)$
- 第2形式：$O(|a|)$

#### get / operator[]

```cpp
T get(const I p) const
T operator[](const I p) const
```

点 `p` の値をコピーして返す。未登録なら `dflt` を返す。

##### 計算量

- $O(\log(m + 1))$

#### get_it / next_it

```cpp
iterator get_it(const I p) const
iterator next_it(const I p) const
```

- `get_it(p)`：`p` を含む区間のイテレータ。未登録なら `end()`。
- `next_it(p)`：`p` を含む区間、または `p` より右側で最初の区間のイテレータ。該当する区間がなければ `end()`。

要素は公開メンバ `I l, r; T x;` を持つ `Segment`。
`iterator` は `set` の `const_iterator` で、`it->l`, `it->r`, `it->x` から区間と値を取得できる。

##### 計算量

- $O(\log(m + 1))$

#### set

```cpp
void set(I l, I r, T x)
template <class Add, class Del>
void set(I l, I r, T x, Add&& add, Del&& del)
```

$[l, r)$ の値を `x` にし、左右で接する同値の区間と併合する。
`x == dflt` の場合も区間を登録する。`l == r` の場合は状態を保ち、コールバックも呼ばない。
`x` は値渡しなので、`mp.set(l, r, it->x)` の形でも使える。

コールバック付きの形式は、削除する区間全体に `del(l, r, x)`、追加する区間全体に `add(l, r, x)` を呼ぶ。
部分的な上書きでは元の区間全体を削除扱いにし、左右に残る部分を追加扱いにする。併合相手も削除扱いにする。
たとえば `[0, 10): A` のうち `[3, 7)` を `B` に変えると、`del(0, 10, A)` と、`add(0, 3, A)`, `add(7, 10, A)`, `add(3, 7, B)` を呼ぶ。

これにより、更新完了後には任意の区間ごとの寄与 $\sum f(l, r, x)$ を維持できる。
同値の再代入でも追加・削除を通知することがある。通知順序への依存を避け、各通知を差分として処理する。
構築済みのオブジェクトに外部集計を付けるときは、最初に全区間を走査して集計を初期化する。

##### 制約

- $l \leq r$
- `add`, `del` は `(const I&, const I&, const T&)` で呼び出せる。引数の参照は呼び出し中に使用する。
- コールバックは正常に戻り、その中で同じ `IntervalMap` を変更しない。

##### 計算量

$k$ を削除扱いにする既存区間数として、

- 償却 $O(\log(m + 1) + k)$

1回の操作で作る区間は高々3個。空の状態から $q$ 回の `set` / `erase` を行う合計時間は $O(q \log(q + 1))$。

#### erase

```cpp
void erase(I l, I r)
template <class Add, class Del>
void erase(I l, I r, Add&& add, Del&& del)
```

$[l, r)$ を未登録に戻す。外側の部分は元の値のまま保持する。
部分的な削除で残る区間があるため、コールバック付きの形式は `add` と `del` の両方を受け取る。
コールバックの規約と空区間の扱いは `set` と同じ。

##### 制約

- `set` と同じ。

##### 計算量

- 削除扱いにする既存区間数を $k$ として、償却 $O(\log(m + 1) + k)$

#### empty / begin / end

```cpp
bool empty() const
iterator begin() const
iterator end() const
```

`empty` は区間集合が空かを返す。`begin` / `end` で左端の昇順に列挙できる。
更新時に削除扱いになった区間のイテレータ・参照は無効になる。それ以外の区間を指すものは有効。

##### 計算量

- 各操作 $O(1)$。全区間の列挙は $O(m)$。

#### content

```cpp
vc<tuple<I, I, T>> content() const
```

登録区間を `(l, r, x)` の列にして、左端の昇順に返す。デバッグ出力用。
返り値はコピーなので、取得後も保持できる。空の区間集合には空の列を返す。

##### 計算量

- $O(m)$
