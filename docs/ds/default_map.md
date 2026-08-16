## 概要

デフォルト値が `{}` に限らない（最初に指定できる）map。Python の defaultdict 的な。

## 詳細なドキュメント

#### コンストラクタ

```cpp
DefaultMap<Key, Value, default_value>()
```

`default_value` は引数なし、返り値の型 `Value` な関数。

C++20 以降であればラムダ式を渡せる：

```cpp
DefaultMap<ll, ll, []() { return INF; }> mp;
```

普通の `map` とだいたい同様に使える。
