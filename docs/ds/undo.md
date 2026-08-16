## 概要

undo 可能な vector など

## 詳細なドキュメント

### ValRollback

値と snapshot を一つだけ保持する。

#### コンストラクタ

```cpp
(1) ValRollback<T>()
(2) ValRollback<T>(T x)
```

- (1)：値と snapshot を `T{}` で初期化する。
- (2)：値と snapshot を `x` で初期化する。

##### 計算量

- $O(1)$

#### get

```cpp
T get()
```

現在の値を返す。

##### 計算量

- $O(1)$

#### set

```cpp
void set(T x)
```

現在の値を `x` に変更する。

##### 計算量

- $O(1)$

#### snapshot

```cpp
void snapshot()
```

現在の値を snapshot として保存する。以前の snapshot は上書きされる。

##### 計算量

- $O(1)$

#### rollback

```cpp
void rollback()
```

現在の値を最後に `snapshot()` した値へ戻す。一度も `snapshot()` していない場合は、コンストラクタで保存された値へ戻す。

##### 計算量

- $O(1)$

### ValUndo

値の変更履歴をすべて保持する。

#### コンストラクタ

```cpp
(1) ValUndo<T>()
(2) ValUndo<T>(T x)
```

- (1)：初期値を `T{}` とする。
- (2)：初期値を `x` とする。

##### 計算量

- $O(1)$

#### get

```cpp
T get()
```

現在の値を返す。

##### 計算量

- $O(1)$

#### set

```cpp
void set(T x)
```

`x` を新しい値として履歴に追加する。

##### 計算量

- 償却 $O(1)$

#### undo

```cpp
void undo(int times = 1)
```

直近 `times` 回の `set` を取り消す。

##### 制約

- $0 \leq \mathrm{times} < $ 現在の履歴数

##### 計算量

- $O(\mathrm{times})$

#### reset

```cpp
void reset()
```

初期値まで戻し、変更履歴と snapshot を破棄する。

##### 計算量

- $O(h)$

ここで $h$ は破棄する履歴数。

#### snapshot

```cpp
void snapshot()
```

現在の履歴位置を snapshot として保存する。以前の snapshot は上書きされる。

##### 計算量

- $O(1)$

#### rollback

```cpp
void rollback()
```

最後に `snapshot()` した履歴位置まで変更を取り消す。

##### 計算量

- $O(h)$

ここで $h$ は snapshot より後の変更回数。

### VectorUndo

`set` による要素の変更履歴を保持する vector。

#### コンストラクタ

```cpp
(1) VectorUndo<T>()
(2) VectorUndo<T>(int n)
(3) VectorUndo<T>(Iter first, Iter last)
(4) VectorUndo<T>(const vc<T>& a)
```

- (1)：空の vector で初期化する。
- (2)：`T{}` を $n$ 個並べた vector で初期化する。
- (3), (4)：与えられた列で初期化する。

##### 制約

- (2)：$n \geq 0$

##### 計算量

- (1)：$O(1)$
- (2)--(4)：$O(n)$

#### get

```cpp
T get(int i)
```

`i` 番目の値を返す。

##### 制約

- $0 \leq i < n$

##### 計算量

- $O(1)$

#### set

```cpp
void set(int i, T x)
```

`i` 番目の値を `x` に変更し、変更前の値を履歴に追加する。

##### 制約

- $0 \leq i < n$

##### 計算量

- 償却 $O(1)$

#### undo

```cpp
void undo(int times = 1)
```

直近 `times` 回の `set` を取り消す。

##### 制約

- $0 \leq \mathrm{times} \leq $ 現在の履歴数

##### 計算量

- $O(\mathrm{times})$

#### reset

```cpp
void reset()
```

すべての変更を取り消し、構築直後の vector に戻す。

##### 計算量

- $O(h)$

ここで $h$ は現在の履歴数。

#### snapshot

```cpp
void snapshot()
```

現在の履歴位置を snapshot として保存する。以前の snapshot は上書きされる。

##### 計算量

- $O(1)$

#### rollback

```cpp
void rollback()
```

最後に `snapshot()` した履歴位置まで変更を取り消す。

##### 計算量

- $O(h)$

ここで $h$ は snapshot より後の変更回数。

#### content

```cpp
vc<T> content()
```

現在の vector のコピーを返す。

##### 計算量

- 時間：$O(n)$
