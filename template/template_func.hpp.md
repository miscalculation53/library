---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':question:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: ds/coordinate_compression.hpp
    title: "\u5EA7\u6A19\u5727\u7E2E"
  - icon: ':heavy_check_mark:'
    path: ds/csr.hpp
    title: CSR
  - icon: ':heavy_check_mark:'
    path: ds/group_index.hpp
    title: "\u6DFB\u5B57\u3092\u5024\u3067\u5206\u985E"
  - icon: ':heavy_check_mark:'
    path: math/algebraic_struct.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\u306E struct"
  - icon: ':question:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':question:'
    path: math/modint/modint.hpp
    title: modint (32 bit)
  - icon: ':question:'
    path: math/modint/modint64.hpp
    title: modint (64 bit)
  - icon: ':question:'
    path: math/modint/modint_internal.hpp
    title: math/modint/modint_internal.hpp
  - icon: ':heavy_check_mark:'
    path: math/prime/euler_phi_carmichael.hpp
    title: "\u30AA\u30A4\u30E9\u30FC\u306E\u30D5\u30A1\u30A4\u95A2\u6570\u30FB\u30AB\
      \u30FC\u30DE\u30A4\u30B1\u30EB\u95A2\u6570"
  - icon: ':question:'
    path: math/prime/factorize.hpp
    title: "\u7D20\u56E0\u6570\u5206\u89E3"
  - icon: ':heavy_check_mark:'
    path: math/prime/order_primitive_root.hpp
    title: "\u5143\u306E\u4F4D\u6570\u3068\u539F\u59CB\u6839"
  - icon: ':question:'
    path: math/prime/primality_test.hpp
    title: "\u7D20\u6570\u5224\u5B9A"
  - icon: ':question:'
    path: math/prime/prime_power.hpp
    title: "\u7D20\u3079\u304D\u69CB\u9020\u4F53"
  - icon: ':heavy_check_mark:'
    path: math/prime/zeta_mobius_divisor_multiple_large.hpp
    title: "\u7D04\u6570\u30FB\u500D\u6570 \u30BC\u30FC\u30BF\u30FB\u30E1\u30D3\u30A6\
      \u30B9\u5909\u63DB\uFF08\u5927\u304D\u3044 $m$ \u306E\u7D04\u6570\uFF09"
  - icon: ':x:'
    path: math/quotients.hpp
    title: "\u5546\u5217\u6319"
  - icon: ':warning:'
    path: template/template.cpp
    title: template/template.cpp
  - icon: ':question:'
    path: template/template_algo.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\
      \u30E0\uFF09"
  - icon: ':question:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/aoj/csr.test.cpp
    title: verify/aoj/csr.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/aoj/extgcd.test.cpp
    title: verify/aoj/extgcd.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint.test.cpp
    title: verify/mytest/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint64.test.cpp
    title: verify/mytest/modint64.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_algo.test.cpp
    title: verify/mytest/template_algo.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_bit.test.cpp
    title: verify/mytest/template_bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/aplusb.test.cpp
    title: verify/yosupo/aplusb.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/associative_array_lower_bound.test.cpp
    title: verify/yosupo/associative_array_lower_bound.test.cpp
  - icon: ':x:'
    path: verify/yosupo/factorize.test.cpp
    title: verify/yosupo/factorize.test.cpp
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
    path: verify/yosupo/predecessor_problem.test.cpp
    title: verify/yosupo/predecessor_problem.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/primality_test.test.cpp
    title: verify/yosupo/primality_test.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/primitive_root.test.cpp
    title: verify/yosupo/primitive_root.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/primitive_root_min.test.cpp
    title: verify/yosupo/primitive_root_min.test.cpp
  - icon: ':x:'
    path: verify/yosupo/quotients.test.cpp
    title: verify/yosupo/quotients.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/static_range_frequency.test.cpp
    title: verify/yosupo/static_range_frequency.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/base_repr.test.cpp
    title: verify/yukicoder/base_repr.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/divisors.test.cpp
    title: verify/yukicoder/divisors.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/dynamic_modint.test.cpp
    title: verify/yukicoder/dynamic_modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/order_mod_carmichael.test.cpp
    title: verify/yukicoder/order_mod_carmichael.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/order_mod_euler_phi.test.cpp
    title: verify/yukicoder/order_mod_euler_phi.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/static_modint.test.cpp
    title: verify/yukicoder/static_modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/zeta_mobius_divisor_large.test.cpp
    title: verify/yukicoder/zeta_mobius_divisor_large.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/zeta_mobius_multiple_large.test.cpp
    title: verify/yukicoder/zeta_mobius_multiple_large.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':question:'
  attributes:
    _deprecated_at_docs: docs/template/template_func.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\
      \u30B8\u30A7\u30AF\u30C8\uFF09"
    links: []
  bundledCode: "#line 2 \"template/template_func.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    using i128 = __int128_t;\nusing u128 = __uint128_t;\ni128 stoi128(const string\
    \ &s)\n{\n  i128 res = 0;\n  if (s.front() == '-')\n  {\n    for (int i = 1; i\
    \ < (int)s.size(); i++)\n      res = 10 * res + s[i] - '0';\n    res = -res;\n\
    \  }\n  else\n  {\n    for (auto &&c : s)\n      res = 10 * res + c - '0';\n \
    \ }\n  return res;\n}\nstring i128tos(i128 x)\n{\n  if (x == 0) return \"0\";\n\
    \  string sign = \"\", res = \"\";\n  if (x < 0)\n    x = -x, sign = \"-\";\n\
    \  while (x > 0)\n  {\n    res += '0' + x % 10;\n    x /= 10;\n  }\n  reverse(res.begin(),\
    \ res.end());\n  return sign + res;\n}\nistream &operator>>(istream &is, i128\
    \ &a)\n{\n  string s;\n  is >> s;\n  a = stoi128(s);\n  return is;\n}\nostream\
    \ &operator<<(ostream &os, const i128 &a)\n{\n  os << i128tos(a);\n  return os;\n\
    }\n#endif\n\n#define cauto const auto\n#line 2 \"template/template_rep.hpp\"\n\
    \n#line 4 \"template/template_rep.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08rep\uFF09\n * @docs docs/template/template_rep.md\n */\n\n/**\n\
    \ * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n*/\n\n#define overload4(_1,\
    \ _2, _3, _4, name, ...) name\n#define rep1(i, n) for (ll i = 0, nnnnn = ll(n);\
    \ i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i = ll(l), rrrrr = ll(r); i <\
    \ rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i = ll(l), rrrrr = ll(r), ddddd\
    \ = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define rep(...) overload4(__VA_ARGS__,\
    \ rep3, rep2, rep1)(__VA_ARGS__)\n#define repi1(i, n) for (int i = 0, nnnnn =\
    \ int(n); i < nnnnn; i++)\n#define repi2(i, l, r) for (int i = int(l), rrrrr =\
    \ int(r); i < rrrrr; i++)\n#define repi3(i, l, r, d) for (int i = int(l), rrrrr\
    \ = int(r), ddddd = int(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define\
    \ repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)\n\n#define\
    \ fe(...) for (auto __VA_ARGS__)\n#define fec(...) for (cauto &__VA_ARGS__)\n\
    #define fem(...) for (auto &__VA_ARGS__)\n#line 5 \"template/template_func.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\
    \u30D6\u30B8\u30A7\u30AF\u30C8\uFF09\n * @docs docs/template/template_func.md\n\
    \ */\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\ntemplate\
    \ <class T = ll>\nstruct max_op\n{\n  T operator()(const T &a, const T &b) const\
    \ { return max(a, b); }\n};\ntemplate <class T = ll>\nstruct min_op\n{\n  T operator()(const\
    \ T &a, const T &b) const { return min(a, b); }\n};\n\ntemplate <class T, const\
    \ T val>\nstruct const_fn\n{\n  T operator()() const { return val; }\n};\nusing\
    \ max_e = const_fn<ll, -INF>;\nusing min_e = const_fn<ll, INF>;\nusing zero_fn\
    \ = const_fn<ll, 0LL>;\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\
    \u30D6\u30B8\u30A7\u30AF\u30C8\uFF09\n * @docs docs/template/template_func.md\n\
    \ */\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\ntemplate\
    \ <class T = ll>\nstruct max_op\n{\n  T operator()(const T &a, const T &b) const\
    \ { return max(a, b); }\n};\ntemplate <class T = ll>\nstruct min_op\n{\n  T operator()(const\
    \ T &a, const T &b) const { return min(a, b); }\n};\n\ntemplate <class T, const\
    \ T val>\nstruct const_fn\n{\n  T operator()() const { return val; }\n};\nusing\
    \ max_e = const_fn<ll, -INF>;\nusing min_e = const_fn<ll, INF>;\nusing zero_fn\
    \ = const_fn<ll, 0LL>;"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  isVerificationFile: false
  path: template/template_func.hpp
  requiredBy:
  - math/modint/modint_internal.hpp
  - math/modint/modint64.hpp
  - math/modint/modint.hpp
  - math/quotients.hpp
  - math/extgcd.hpp
  - math/algebraic_struct.hpp
  - math/prime/factorize.hpp
  - math/prime/primality_test.hpp
  - math/prime/prime_power.hpp
  - math/prime/euler_phi_carmichael.hpp
  - math/prime/zeta_mobius_divisor_multiple_large.hpp
  - math/prime/order_primitive_root.hpp
  - template/template.cpp
  - template/template_algo.hpp
  - template/template_all.hpp
  - ds/group_index.hpp
  - ds/csr.hpp
  - ds/coordinate_compression.hpp
  timestamp: '2024-12-20 09:47:18+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - verify/mytest/template_bit.test.cpp
  - verify/mytest/modint.test.cpp
  - verify/mytest/template_algo.test.cpp
  - verify/mytest/modint64.test.cpp
  - verify/aoj/csr.test.cpp
  - verify/aoj/extgcd.test.cpp
  - verify/yukicoder/dynamic_modint.test.cpp
  - verify/yukicoder/zeta_mobius_divisor_large.test.cpp
  - verify/yukicoder/base_repr.test.cpp
  - verify/yukicoder/static_modint.test.cpp
  - verify/yukicoder/order_mod_carmichael.test.cpp
  - verify/yukicoder/zeta_mobius_multiple_large.test.cpp
  - verify/yukicoder/order_mod_euler_phi.test.cpp
  - verify/yukicoder/divisors.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/quotients.test.cpp
  - verify/yosupo/kth_root_integer.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
  - verify/yosupo/predecessor_problem.test.cpp
  - verify/yosupo/static_range_frequency.test.cpp
  - verify/yosupo/factorize.test.cpp
  - verify/yosupo/primality_test.test.cpp
  - verify/yosupo/primitive_root.test.cpp
  - verify/yosupo/associative_array_lower_bound.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/yosupo/primitive_root_min.test.cpp
  - verify/yosupo/many_aplusb_128bit.test.cpp
documentation_of: template/template_func.hpp
layout: document
redirect_from:
- /library/template/template_func.hpp
- /library/template/template_func.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\u30B8\u30A7\
  \u30AF\u30C8\uFF09"
---
## テンプレート（関数オブジェクト）

- `max_op`：$\max$ の関数オブジェクト
- `min_op`：$\min$ の関数オブジェクト
- `max_e`：定数 `INF` の関数オブジェクト
- `min_e`：定数 `-INF` の関数オブジェクト
- `zero_fn`：定数 $0$ の関数オブジェクト

たとえば、`segtree` 等の宣言で使う：

`segtree<ll, max_op{}, max_e{}> seg(vec);`