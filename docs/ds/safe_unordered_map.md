## unordered_map の安全なハッシュ関数

unordered_map のハッシュは衝突させることができるので、その対策。実行時ランダムを使っている。

```cpp
unordered_map<Key, Value, safe_hash> mp;
```

のように使う。

----

map と unordered_map の速度差は意外とある場合もあるので、どちらでもよい場合は unordered_map を使うのがよいと思う。

TODO：pair, tuple, array, vector なども載せられるようにしたい。
