このドキュメントには AI による作成・変更が含まれています。

## 概要

入力の昇順に並ぶ添字列を返す。同じ値・キーでは元の添字順を保つ。

`vector` の固定幅整数キーには基数ソートを使い、小さい列とその他の型には比較ソートを使う。要素自体のコピーや移動を省いてソート順を求められる。

昇順の整数列は、そのままの添字順を返す。

## 使用例

```cpp
vc<ll> a = {5, -2, 5, 0};
auto ord = stable_argsort(a); // {1, 3, 0, 2}

vc<pair<int, string>> records = {{2, "A"}, {-1, "B"}, {2, "C"}};
auto by_key = stable_argsort(records, [](const auto& x) { return x.first; });
// {1, 0, 2}
```

## 詳細なドキュメント

#### stable_argsort

```cpp
(1) template <class V>
    vc<int> stable_argsort(const V& a)
(2) template <class T, class Key>
    vc<int> stable_argsort(const vc<T>& a, const Key& key)
```

- (1)：`a[ord[i]]` が昇順になる添字列 `ord` を返す。
- (2)：`key(a[ord[i]])` が昇順になる添字列を返す。

入力の要素を保持する。`vector` 以外でも、(1) は `size()` と添字による参照を使える列に対応する。

##### 制約

- 値またはキーの `<` が狭義弱順序を定めること。
- (2) の `key` は `const T&` を受け取り、呼び出し中に各要素に対して同じキーを返すこと。

##### 計算量

$n=|a|$ とし、比較・キー取得を $O(1)$ とする。

- `vector` の固定幅整数キー（`bool` を除き、`i128`, `u128` を含む）：$O(n)$
- その他：$O(n\log n)$
