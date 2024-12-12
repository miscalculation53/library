---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: math/modint/modint.hpp
    title: modint (32 bit)
  - icon: ':warning:'
    path: template/template.cpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5168\u4F53\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_algo.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\
      \u30E0\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  - icon: ':heavy_check_mark:'
    path: template/template_binsearch.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_bit.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\
      \uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_func.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\u30B8\
      \u30A7\u30AF\u30C8\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_inout.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_math.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_vector.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint.test.cpp
    title: verify/mytest/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_algo.test.cpp
    title: verify/mytest/template_algo.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_binsearch.test.cpp
    title: verify/mytest/template_binsearch.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_bit.test.cpp
    title: verify/mytest/template_bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_inout_top.test.cpp
    title: verify/mytest/template_inout_top.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_math_div.test.cpp
    title: verify/mytest/template_math_div.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_math_mulpow.test.cpp
    title: verify/mytest/template_math_mulpow.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_sgn.test.cpp.cpp
    title: verify/mytest/template_sgn.test.cpp.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_vector.test.cpp
    title: verify/mytest/template_vector.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/aplusb.test.cpp
    title: verify/yosupo/aplusb.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/kth_root_integer.test.cpp
    title: verify/yosupo/kth_root_integer.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb.test.cpp
    title: verify/yosupo/many_aplusb.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb_128bit.test.cpp
    title: verify/yosupo/many_aplusb_128bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb_tuple.test.cpp
    title: verify/yosupo/many_aplusb_tuple.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/yuki1550_dynamic_modint.test.cpp
    title: verify/yukicoder/yuki1550_dynamic_modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/yuki1550_static_modint.test.cpp
    title: verify/yukicoder/yuki1550_static_modint.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    _deprecated_at_docs: docs/template/template_rep.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
    links:
    - https://trap.jp/post/1224/
  bundledCode: "#line 2 \"template/template_rep.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    \ const auto\n#line 4 \"template/template_rep.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09\n * @docs docs/template/template_rep.md\n\
    \ */\n\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n*/\n\n#define\
    \ overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for (ll i = 0,\
    \ nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i = ll(l), rrrrr\
    \ = ll(r); i < rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i = ll(l), rrrrr\
    \ = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define\
    \ rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n#define repi1(i,\
    \ n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i, l, r) for\
    \ (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i, l, r, d)\
    \ for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i < rrrrr :\
    \ i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)\n\
    \n#define fe(...) for (auto __VA_ARGS__)\n#define fec(...) for (cauto &__VA_ARGS__)\n\
    #define fem(...) for (auto &__VA_ARGS__)\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09\n * @docs docs/template/template_rep.md\n\
    \ */\n\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n*/\n\n#define\
    \ overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for (ll i = 0,\
    \ nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i = ll(l), rrrrr\
    \ = ll(r); i < rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i = ll(l), rrrrr\
    \ = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define\
    \ rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n#define repi1(i,\
    \ n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i, l, r) for\
    \ (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i, l, r, d)\
    \ for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i < rrrrr :\
    \ i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)\n\
    \n#define fe(...) for (auto __VA_ARGS__)\n#define fec(...) for (cauto &__VA_ARGS__)\n\
    #define fem(...) for (auto &__VA_ARGS__)"
  dependsOn:
  - template/template_types.hpp
  isVerificationFile: false
  path: template/template_rep.hpp
  requiredBy:
  - math/modint/modint.hpp
  - template/template_bit.hpp
  - template/template_inout.hpp
  - template/template.cpp
  - template/template_all.hpp
  - template/template_algo.hpp
  - template/template_binsearch.hpp
  - template/template_math.hpp
  - template/template_vector.hpp
  - template/template_func.hpp
  timestamp: '2024-12-10 18:45:11+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/many_aplusb_128bit.test.cpp
  - verify/yosupo/kth_root_integer.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/mytest/modint.test.cpp
  - verify/mytest/template_algo.test.cpp
  - verify/mytest/template_vector.test.cpp
  - verify/mytest/template_inout_top.test.cpp
  - verify/mytest/template_binsearch.test.cpp
  - verify/mytest/template_bit.test.cpp
  - verify/mytest/template_math_mulpow.test.cpp
  - verify/mytest/template_sgn.test.cpp.cpp
  - verify/mytest/template_math_div.test.cpp
  - verify/yukicoder/yuki1550_static_modint.test.cpp
  - verify/yukicoder/yuki1550_dynamic_modint.test.cpp
documentation_of: template/template_rep.hpp
layout: document
redirect_from:
- /library/template/template_rep.hpp
- /library/template/template_rep.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
---
## テンプレート（rep）

### rep

指定する引数は Python の range と同じ。つまり、

- `rep(i, l, r, d)`： $l$ から始めて $d$ ずつ変えるのを $r$ まで（$r$ は含まない）
- `rep(i, l, r)`： `rep(i, l, r, 1)` と同じ
- `rep(i, r)`： `rep(i, 0, r, 1)` と同じ

引数の数が違う `rep1`, `rep2`, `rep3` をすべて `rep` で呼べるようにしてある（参考： https://trap.jp/post/1224/ ）。

また、`rep` では `ll` 型を、`repi` では `int` 型を用いる。

引数は**最初の $1$ 回だけ**評価される。

（滅多にないと思うが、）`lllll`, `rrrrr`, `ddddd`, `nnnnn` という名前の変数を `rep` や `repi` の内部で使わないこと。


### 範囲 for 

- `fec(e : v)`
  - `for (const auto &e : v)` と同じ
  - for each const
  - **だいたいはこれを使っておくべき**
- `fe(e : v)`
  - `for (auto e : v)` と同じ
  - for each
  - コピーを作って中で変更したいときのみ使う（`fec` で済むときにこれを使うと遅い）
- `fem(e : v)`
  - `for (auto &e : v)` と同じ
  - for each mutable
  - 本当に内部を変更したいときにしか使わない

`e` には `[a, b]` とかも入れられる。区切りが通常の範囲 for と同様の `:` であることに注意（マクロ化する際の都合）。