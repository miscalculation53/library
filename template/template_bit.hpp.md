---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':question:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':x:'
    path: verify/mytest/template_bit.test.cpp
    title: verify/mytest/template_bit.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':x:'
  attributes:
    _deprecated_at_docs: docs/template/template_bit.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\
      \u7B97\uFF09"
    links: []
  bundledCode: "#line 2 \"template/template_bit.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n *\
    \ @docs docs/template/template_types.md\n */\n\n#include <bits/stdc++.h>\nusing\
    \ namespace std;\n\n#ifndef EPS\n#define EPS 1e-11\n#endif\nusing ld = decltype(EPS);\n\
    \nusing ll = long long;\nusing uint = unsigned int;\nusing ull = unsigned long\
    \ long;\nusing pll = pair<ll, ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll\
    \ = tuple<ll, ll, ll, ll>;\n\n#define vc vector\ntemplate <class T>\nusing vvc\
    \ = vc<vc<T>>;\ntemplate <class T>\nusing vvvc = vc<vc<vc<T>>>;\n\nusing vb =\
    \ vc<bool>;\nusing vl = vc<ll>;\nusing vpll = vc<pll>;\nusing vtlll = vc<tlll>;\n\
    using vtllll = vc<tllll>;\nusing vstr = vc<string>;\nusing vvb = vvc<bool>;\n\
    using vvl = vvc<ll>;\n\ntemplate <class T>\nusing pql = priority_queue<T, vc<T>,\
    \ greater<T>>;\ntemplate <class T>\nusing pqg = priority_queue<T>;\n\n#ifdef __SIZEOF_INT128__\n\
    using i128 = __int128_t;\ni128 stoi128(const string &s)\n{\n  i128 res = 0;\n\
    \  if (s.front() == '-')\n  {\n    for (int i = 1; i < (int)s.size(); i++)\n \
    \     res = 10 * res + s[i] - '0';\n    res = -res;\n  }\n  else\n  {\n    for\
    \ (auto &&c : s)\n      res = 10 * res + c - '0';\n  }\n  return res;\n}\nstring\
    \ i128tos(i128 x)\n{\n  if (x == 0) return \"0\";\n  string sign = \"\", res =\
    \ \"\";\n  if (x < 0)\n    x = -x, sign = \"-\";\n  while (x > 0)\n  {\n    res\
    \ += '0' + x % 10;\n    x /= 10;\n  }\n  reverse(res.begin(), res.end());\n  return\
    \ sign + res;\n}\nistream &operator>>(istream &is, i128 &a)\n{\n  string s;\n\
    \  is >> s;\n  a = stoi128(s);\n  return is;\n}\nostream &operator<<(ostream &os,\
    \ const i128 &a)\n{\n  os << i128tos(a);\n  return os;\n}\n#endif\n\n#define cauto\
    \ const auto\n#line 2 \"template/template_rep.hpp\"\n\n#line 4 \"template/template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09\n * @docs\
    \ docs/template/template_rep.md\n */\n\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n\
    */\n\n#define overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for\
    \ (ll i = 0, nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i =\
    \ ll(l), rrrrr = ll(r); i < rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i =\
    \ ll(l), rrrrr = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i +=\
    \ d)\n#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n\
    #define repi1(i, n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i,\
    \ l, r) for (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i,\
    \ l, r, d) for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i\
    \ < rrrrr : i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3,\
    \ repi2, repi1)(__VA_ARGS__)\n\n#define fe(...) for (auto __VA_ARGS__)\n#define\
    \ fec(...) for (cauto &__VA_ARGS__)\n#define fem(...) for (auto &__VA_ARGS__)\n\
    #line 5 \"template/template_bit.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\uFF09\n * @docs docs/template/template_bit.md\n\
    \ */\n\ninline ull pow2(auto k) { return 1ULL << k; }\ninline ull MASK(auto k)\
    \ { return (1ULL << k) - 1; }\n\n#if __cplusplus < 202002L\n// x == 0 \u306A\u3089\
    \u3070 0\u3001\u305D\u3046\u3067\u306A\u3051\u308C\u3070 1 + floor(log2(x))\n\
    // 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... \ninline ull bit_width(ull x) { return flsll(x);\
    \ }\n// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline ull bit_floor(ull x) { return\
    \ x == 0 ? 0ULL : 1ULL << (flsll(x) - 1); }\n// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16,\
    \ ...\ninline ull bit_ceil(ull x) { return x == 0 ? 1ULL : 1ULL << flsll(x - 1);\
    \ }\ninline ull countr_zero(ull x) { assert(x != 0); return __builtin_ctzll(x);\
    \ }\ninline ull popcount(ull x) { return __builtin_popcountll(x); }\ninline bool\
    \ has_single_bit(ull x) { return popcount(x) == 1; }\n#else\n// 0, 1, 2, 2, 3,\
    \ 3, 3, 3, 4, 4, ... \ninline ll bit_width(ll x) { return bit_width((ull)x); }\n\
    // 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline ll bit_floor(ll x) { return bit_floor((ull)x);\
    \ }\n// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...\ninline ll bit_ceil(ll x) { return\
    \ bit_ceil((ull)x); }\ninline ll countr_zero(ll x) { assert(x != 0); return countr_zero((ull)x);\
    \ }\ninline ll popcount(ll x) { return popcount((ull)x); }\ninline bool has_single_bit(ll\
    \ x) { return has_single_bit((ull)x); }\n#endif\n\ninline ull lsb_pos(ull x) {\
    \ assert(x != 0); return countr_zero(x); }\ninline ull msb_pos(ull x) { assert(x\
    \ != 0); return bit_width(x) - 1; }\ninline ull lsb_mask(ull x) { assert(x !=\
    \ 0); return x & -x; }\ninline ull msb_mask(ull x) { assert(x != 0); return bit_floor(x);\
    \ }\n\ninline bool btest(ull x, uint k) { return (x >> k) & 1; }\ntemplate <class\
    \ T>\ninline void bset(T &x, uint k, bool b = 1) { b ? x |= (1ULL << k) : x &=\
    \ ~(1ULL << k); }\ntemplate <class T>\ninline void bflip(T &x, uint k) { x ^=\
    \ (1ULL << k); }\ninline bool bsubset(ull x, ull y) { return (x & y) == x; }\n\
    inline bool bsupset(ull x, ull y) { return (x & y) == y; }\ninline ull bsetminus(ull\
    \ x, ull y) { return x & ~y; }\n\ntemplate <class T>\nstruct bsubsets\n{\nprivate:\n\
    \  T x;\npublic:\n  bsubsets(T x) : x(x) {}\n  struct Iterator\n  {\n  private:\n\
    \    T y;\n    bool is_end;\n    const bsubsets &bs;\n  public:\n    Iterator(T\
    \ y, bool is_end, const bsubsets &bs) : y(y), is_end(is_end), bs(bs) {}\n    T\
    \ operator*() const { return y; }\n    Iterator& operator++()\n    {\n      if\
    \ (y == 0)\n        is_end = true;\n      y = (y - 1) & bs.x;\n      return *this;\n\
    \    }\n    bool operator!=(const Iterator &other) const { return y != other.y\
    \ || is_end != other.is_end; }\n  };\n  Iterator begin() const { return Iterator(x,\
    \ false, *this); }\n  Iterator end() const { return Iterator(x, true, *this);\
    \ }\n};\ntemplate <class T>\nstruct bsupsets\n{\nprivate:\n  int n;\n  T x;\n\
    public:\n  bsupsets(int n, T x) : n(n), x(x) {}\n  struct Iterator\n  {\n  private:\n\
    \    T y;\n    const bsupsets &bs;\n  public:\n    Iterator(T y, const bsupsets\
    \ &bs) : y(y), bs(bs) {}\n    T operator*() const { return y; }\n    Iterator&\
    \ operator++()\n    {\n      y = (y + 1) | bs.x;\n      return *this;\n    }\n\
    \    bool operator!=(const Iterator &other) const { return y != other.y; }\n \
    \ };\n  Iterator begin() const { return Iterator(x, *this); }\n  Iterator end()\
    \ const { return Iterator((T(1) << n) | x, *this); }\n};\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\
    \u6F14\u7B97\uFF09\n * @docs docs/template/template_bit.md\n */\n\ninline ull\
    \ pow2(auto k) { return 1ULL << k; }\ninline ull MASK(auto k) { return (1ULL <<\
    \ k) - 1; }\n\n#if __cplusplus < 202002L\n// x == 0 \u306A\u3089\u3070 0\u3001\
    \u305D\u3046\u3067\u306A\u3051\u308C\u3070 1 + floor(log2(x))\n// 0, 1, 2, 2,\
    \ 3, 3, 3, 3, 4, 4, ... \ninline ull bit_width(ull x) { return flsll(x); }\n//\
    \ 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline ull bit_floor(ull x) { return x ==\
    \ 0 ? 0ULL : 1ULL << (flsll(x) - 1); }\n// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...\n\
    inline ull bit_ceil(ull x) { return x == 0 ? 1ULL : 1ULL << flsll(x - 1); }\n\
    inline ull countr_zero(ull x) { assert(x != 0); return __builtin_ctzll(x); }\n\
    inline ull popcount(ull x) { return __builtin_popcountll(x); }\ninline bool has_single_bit(ull\
    \ x) { return popcount(x) == 1; }\n#else\n// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ...\
    \ \ninline ll bit_width(ll x) { return bit_width((ull)x); }\n// 0, 1, 2, 2, 4,\
    \ 4, 4, 4, 8, 8, ...\ninline ll bit_floor(ll x) { return bit_floor((ull)x); }\n\
    // 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...\ninline ll bit_ceil(ll x) { return bit_ceil((ull)x);\
    \ }\ninline ll countr_zero(ll x) { assert(x != 0); return countr_zero((ull)x);\
    \ }\ninline ll popcount(ll x) { return popcount((ull)x); }\ninline bool has_single_bit(ll\
    \ x) { return has_single_bit((ull)x); }\n#endif\n\ninline ull lsb_pos(ull x) {\
    \ assert(x != 0); return countr_zero(x); }\ninline ull msb_pos(ull x) { assert(x\
    \ != 0); return bit_width(x) - 1; }\ninline ull lsb_mask(ull x) { assert(x !=\
    \ 0); return x & -x; }\ninline ull msb_mask(ull x) { assert(x != 0); return bit_floor(x);\
    \ }\n\ninline bool btest(ull x, uint k) { return (x >> k) & 1; }\ntemplate <class\
    \ T>\ninline void bset(T &x, uint k, bool b = 1) { b ? x |= (1ULL << k) : x &=\
    \ ~(1ULL << k); }\ntemplate <class T>\ninline void bflip(T &x, uint k) { x ^=\
    \ (1ULL << k); }\ninline bool bsubset(ull x, ull y) { return (x & y) == x; }\n\
    inline bool bsupset(ull x, ull y) { return (x & y) == y; }\ninline ull bsetminus(ull\
    \ x, ull y) { return x & ~y; }\n\ntemplate <class T>\nstruct bsubsets\n{\nprivate:\n\
    \  T x;\npublic:\n  bsubsets(T x) : x(x) {}\n  struct Iterator\n  {\n  private:\n\
    \    T y;\n    bool is_end;\n    const bsubsets &bs;\n  public:\n    Iterator(T\
    \ y, bool is_end, const bsubsets &bs) : y(y), is_end(is_end), bs(bs) {}\n    T\
    \ operator*() const { return y; }\n    Iterator& operator++()\n    {\n      if\
    \ (y == 0)\n        is_end = true;\n      y = (y - 1) & bs.x;\n      return *this;\n\
    \    }\n    bool operator!=(const Iterator &other) const { return y != other.y\
    \ || is_end != other.is_end; }\n  };\n  Iterator begin() const { return Iterator(x,\
    \ false, *this); }\n  Iterator end() const { return Iterator(x, true, *this);\
    \ }\n};\ntemplate <class T>\nstruct bsupsets\n{\nprivate:\n  int n;\n  T x;\n\
    public:\n  bsupsets(int n, T x) : n(n), x(x) {}\n  struct Iterator\n  {\n  private:\n\
    \    T y;\n    const bsupsets &bs;\n  public:\n    Iterator(T y, const bsupsets\
    \ &bs) : y(y), bs(bs) {}\n    T operator*() const { return y; }\n    Iterator&\
    \ operator++()\n    {\n      y = (y + 1) | bs.x;\n      return *this;\n    }\n\
    \    bool operator!=(const Iterator &other) const { return y != other.y; }\n \
    \ };\n  Iterator begin() const { return Iterator(x, *this); }\n  Iterator end()\
    \ const { return Iterator((T(1) << n) | x, *this); }\n};"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  isVerificationFile: false
  path: template/template_bit.hpp
  requiredBy: []
  timestamp: '2024-12-10 18:45:11+09:00'
  verificationStatus: LIBRARY_ALL_WA
  verifiedWith:
  - verify/mytest/template_bit.test.cpp
documentation_of: template/template_bit.hpp
layout: document
redirect_from:
- /library/template/template_bit.hpp
- /library/template/template_bit.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\uFF09"
---
## テンプレート（ビット演算）

### pow2, MASK

```cpp
pow2(k) == (1ULL << k)
MASK(k) == (1ULL << k) - 1
```

### C++20 にある機能

C++17 でも使えるようにしてある。また、C++20 では引数が unsigned でないと使えないが、signed でも使えるようにしてある。（あまりないと思うが、型の上限付近では注意。）

#### bit_width

```cpp
ull bit_width(ull x)
```

$x = 0$ ならば $0$、そうでなければ $1 + \lfloor \log_2 x \rfloor$ を返す。

|0|1|2|3|4|5|6|7|8|9|
|-|-|-|-|-|-|-|-|-|-|
|0|1|2|2|3|3|3|3|4|4|

#### bit_floor

```cpp
ull bit_floor(ull x)
```

$x = 0$ ならば $0$、そうでなければ $x$ 以下の最大の $2$ べきを返す。

|0|1|2|3|4|5|6|7|8|9|
|-|-|-|-|-|-|-|-|-|-|
|0|1|2|2|4|4|4|4|8|8|

#### bit_ceil

```cpp
ull bit_ceil(ull x)
```

$x$ 以上の最小の $2$ べきを返す。

|0|1|2|3|4|5|6|7|8|9|
|-|-|-|-|-|-|-|-|-|-|
|1|1|2|4|4|8|8|8|8|16|

#### countr_zero

```cpp
ull countr_zero(ull x)
```

下の桁に $0$ が続く個数を返す。

$x \neq 0$ の場合、$x$ が $2$ で割り切れる回数や `lsb_pos(x)` と一致。

$x = 0$ の場合に使うことは想定していない。使った場合以下の挙動を示す：
- C++17 以前・C++20 の signed で使えるようにこちらで実装したものを使った場合、エラー
- C++20 にある関数を使った場合（引数が unsigned の場合が該当する）、符号なし整数のビット数が返る

#### popcount

```cpp
ull popcount(ull x)
```

立っているビットの個数を返す。

#### has_single_bit

```cpp
bool has_single_bit(ull x)
```

$x$ が $2$ べきかどうかを返す。

### lsb, msb

```cpp
(1) ull lsb_pos(ull x)
(2) ull msb_pos(ull x)
(3) ull lsb_mask(ull x)
(4) ull msb_mask(ull x)
```

- (1)：LSB（立っている最小のビット）の位置
- (2)：MSB（立っている最大のビット）の位置
- (3)：LSB だけを立てた値
- (4)：MSB だけを立てた値

##### 制約

$x \neq 0$

### ビット操作

#### btest

```cpp
bool btest(ull x, uint k)
```

$x$ の下から $k$ ビット目が立っているかどうかを返す。

#### bset

```cpp
void bset(ull x, uint k, bool b = 1)
```

$x$ の下から $k$ ビット目を $b$ にする。

#### bflip

```cpp
void bflip(ull x, uint k)
```

$x$ の下から $k$ ビット目を反転させる。

#### bsubset

```cpp
bool bsubset(ull x, ull y)
```

$x \subseteq y$ かどうかを返す。

#### bsupset

```cpp
bool bsupset(ull x, ull y)
```

$x \supseteq y$ かどうかを返す。

#### bsetminus

```cpp
ull bsetminus(ull x, ull y)
```

$x \setminus y$ を返す。

### 列挙

#### bsubsets

次のように書くと、$y \subseteq x$ なる $y$ を値の大きい方から列挙できる。

```cpp
fec(y : bsubsets(x))
```

##### 計算量

- $1$ 回のイテレーションに $O(1)$

$y \subseteq x \subseteq [n]$ なる $(x, y)$ の個数は $3^n$ であることは有名。この列挙を $O(3^n)$ で行うのが主な用途。

#### bsupsets

次のように書くと、$y \subseteq x \subseteq [n]$ なる $x$ を値の小さい方から列挙できる。

```cpp
fec(x : bsupsets(n, y))
```

##### 計算量

- $1$ 回のイテレーションに $O(1)$

$y \subseteq x \subseteq [n]$ なる $(x, y)$ の個数は $3^n$ であることは有名。この列挙を $O(3^n)$ で行うのが主な用途。