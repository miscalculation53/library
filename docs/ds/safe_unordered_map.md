## 概要

`unordered_map` への衝突攻撃を防ぐための、実行時ランダムを使ったハッシュ関数。

整数、128 bit 整数、`std::hash` が定義された型に加え、`pair`、`tuple`、`array`、`vector`、`basic_string`、`basic_string_view` を再帰的にハッシュ化できる。ライブラリの型では `mint`、`Rational`、`RollingHash`、`ModintDiv`、`BigInteger` に対応している。

整数と複合型の各要素は splitmix64 で秘密 seed と混合する。文字列は秘密 base を使って内容を走査するため、入力だけから衝突する bucket を構成できない。

任意の `std::hash<T>` にフォールバックする場合、その `std::hash<T>` 自体が同じ値を返す二つの入力までは区別できない。整数・文字列およびそれらからなる対応済み複合型では、この制限を受けない。

## 使用例

```cpp
unordered_map<pair<ll, ll>, int, safe_hash> mp;
unordered_map<tuple<int, string, vc<int>>, int, safe_hash> nested;
```

## 詳細なドキュメント

### safe_hash

#### operator()

```cpp
size_t operator()(const T& x) const
```

`x` を実行ごとの秘密 seed でハッシュ化する。同じ実行中の等しい値には同じ値を返す。

##### 制約

- `T` が対応済みの型、または `std::hash<T>` が定義された型
- 複合型の各要素も同じ制約を満たす

##### 計算量

- スカラー：$O(1)$
- `pair`, `tuple`, `array`：要素数に比例
- `vector`, `basic_string`, `basic_string_view`：長さに比例
- 整数型の `Rational`：gcd の計算量
- `Rational<mint>`：`mint` の除算の計算量
- `BigInteger`：内部の桁数に比例

#### splitmix64

```cpp
ull splitmix64(ull x)
```

`x` を splitmix64 の出力関数で攪拌した値を返す。

##### 計算量

- $O(1)$
