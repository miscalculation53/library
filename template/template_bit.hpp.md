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
  - icon: ':warning:'
    path: algo/merge_sort.hpp
    title: "\u30DE\u30FC\u30B8\u30BD\u30FC\u30C8"
  - icon: ':x:'
    path: algo/parallel_binsearch.hpp
    title: "\u4E26\u5217\u4E8C\u5206\u63A2\u7D22"
  - icon: ':heavy_check_mark:'
    path: ds/coordinate_compression.hpp
    title: "\u5EA7\u6A19\u5727\u7E2E"
  - icon: ':heavy_check_mark:'
    path: ds/csr.hpp
    title: CSR
  - icon: ':heavy_check_mark:'
    path: ds/flat_map.hpp
    title: "\u30AD\u30FC\u304C\u3059\u3079\u3066\u5148\u306B\u308F\u304B\u308B\u5834\
      \u5408\u306E map"
  - icon: ':heavy_check_mark:'
    path: ds/group_index.hpp
    title: "\u6DFB\u5B57\u3092\u5024\u3067\u5206\u985E"
  - icon: ':heavy_check_mark:'
    path: ds/offline_dynamic_connectivity.hpp
    title: "\u30AA\u30D5\u30E9\u30A4\u30F3\u30C0\u30A4\u30B3\u30CD\u306E\u30C6\u30AF\
      \u30CB\u30C3\u30AF"
  - icon: ':question:'
    path: ds/uf/uf.hpp
    title: UnionFind
  - icon: ':x:'
    path: ds/uf/uf_partially_persistent.hpp
    title: "\u90E8\u5206\u6C38\u7D9A UnionFind"
  - icon: ':x:'
    path: ds/uf/uf_potential.hpp
    title: "\u30DD\u30C6\u30F3\u30B7\u30E3\u30EB\u3064\u304D UnionFind"
  - icon: ':heavy_check_mark:'
    path: ds/uf/uf_undo.hpp
    title: "undo \u53EF\u80FD UnionFind"
  - icon: ':warning:'
    path: graph/graph.hpp
    title: "\u30B0\u30E9\u30D5\uFF08\u57FA\u5E95\u30AF\u30E9\u30B9\uFF09"
  - icon: ':heavy_check_mark:'
    path: itertools/bit.hpp
    title: "\u90E8\u5206\u96C6\u5408\u30FB\u4E0A\u4F4D\u96C6\u5408\u5168\u63A2\u7D22"
  - icon: ':heavy_check_mark:'
    path: itertools/direct_product.hpp
    title: "\u76F4\u7A4D\u5168\u63A2\u7D22"
  - icon: ':question:'
    path: math/algebra/algebra_base.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\u306E struct\uFF08\u57FA\u672C\uFF09"
  - icon: ':question:'
    path: math/algebra/algebra_basic_ops.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\uFF08\u56DB\u5247\u6F14\u7B97\u3068 min,\
      \ max\uFF09"
  - icon: ':x:'
    path: math/algebra/matmul22.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\uFF08$2 \\times 2$ \u884C\u5217\u306E\u7A4D\
      \uFF09"
  - icon: ':warning:'
    path: math/algebra/polynomial_ring.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\uFF08\u591A\u9805\u5F0F\u74B0\uFF09"
  - icon: ':question:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':question:'
    path: math/modint/binomial.hpp
    title: "\u4E8C\u9805\u4FC2\u6570"
  - icon: ':question:'
    path: math/modint/modint.hpp
    title: modint (32 bit)
  - icon: ':question:'
    path: math/modint/modint32_internal.hpp
    title: math/modint/modint32_internal.hpp
  - icon: ':question:'
    path: math/modint/modint64.hpp
    title: modint (64 bit)
  - icon: ':question:'
    path: math/modint/modint64_internal.hpp
    title: math/modint/modint64_internal.hpp
  - icon: ':question:'
    path: math/modint/modint_base.hpp
    title: math/modint/modint_base.hpp
  - icon: ':heavy_check_mark:'
    path: math/modint/power_table.hpp
    title: "\u7D2F\u4E57\u30C6\u30FC\u30D6\u30EB"
  - icon: ':question:'
    path: math/prime/euler_phi_carmichael.hpp
    title: "\u30AA\u30A4\u30E9\u30FC\u306E\u30D5\u30A1\u30A4\u95A2\u6570\u30FB\u30AB\
      \u30FC\u30DE\u30A4\u30B1\u30EB\u95A2\u6570"
  - icon: ':question:'
    path: math/prime/factorize.hpp
    title: "\u7D20\u56E0\u6570\u5206\u89E3"
  - icon: ':heavy_check_mark:'
    path: math/prime/lcm_gcd_convolution.hpp
    title: "LCM/GCD \u7573\u307F\u8FBC\u307F"
  - icon: ':heavy_check_mark:'
    path: math/prime/linear_sieve.hpp
    title: "\u7DDA\u5F62\u7BE9"
  - icon: ':question:'
    path: math/prime/order_primitive_root.hpp
    title: "\u5143\u306E\u4F4D\u6570\u3068\u539F\u59CB\u6839"
  - icon: ':question:'
    path: math/prime/primality_test.hpp
    title: "\u7D20\u6570\u5224\u5B9A"
  - icon: ':question:'
    path: math/prime/prime_power.hpp
    title: "\u7D20\u3079\u304D\u69CB\u9020\u4F53"
  - icon: ':heavy_check_mark:'
    path: math/prime/zeta_mobius_divisor_multiple.hpp
    title: "\u7D04\u6570\u30FB\u500D\u6570 \u30BC\u30FC\u30BF\u30FB\u30E1\u30D3\u30A6\
      \u30B9\u5909\u63DB\uFF08$1$ \u4EE5\u4E0A $n$ \u4EE5\u4E0B\uFF09"
  - icon: ':x:'
    path: math/prime/zeta_mobius_divisor_multiple_large.hpp
    title: "\u7D04\u6570\u30FB\u500D\u6570 \u30BC\u30FC\u30BF\u30FB\u30E1\u30D3\u30A6\
      \u30B9\u5909\u63DB\uFF08\u5927\u304D\u3044 $m$ \u306E\u7D04\u6570\uFF09"
  - icon: ':x:'
    path: math/quadratic_equation_integer.hpp
    title: "\u4E8C\u6B21\u65B9\u7A0B\u5F0F\u306E\u6574\u6570\u89E3"
  - icon: ':heavy_check_mark:'
    path: math/quotients.hpp
    title: "\u5546\u5217\u6319"
  - icon: ':heavy_check_mark:'
    path: math/set/and_or_convolution.hpp
    title: "and/or \u7573\u307F\u8FBC\u307F"
  - icon: ':heavy_check_mark:'
    path: math/set/kronecker_power.hpp
    title: "\u30AF\u30ED\u30CD\u30C3\u30AB\u30FC\u51AA\u306E\u4F5C\u7528"
  - icon: ':heavy_check_mark:'
    path: math/set/subset_convolution.hpp
    title: subset convolution
  - icon: ':x:'
    path: math/set/xor_convolution.hpp
    title: "\u30A2\u30C0\u30DE\u30FC\u30EB\u5909\u63DB\u30FBxor \u7573\u307F\u8FBC\
      \u307F"
  - icon: ':heavy_check_mark:'
    path: math/set/zeta_mobius.hpp
    title: "\u30BC\u30FC\u30BF\u30FB\u30E1\u30D3\u30A6\u30B9\u5909\u63DB"
  - icon: ':warning:'
    path: template/template.cpp
    title: template/template.cpp
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
    path: verify/aoj/modpow.test.cpp
    title: verify/aoj/modpow.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/itertools_bit.test.cpp
    title: verify/mytest/itertools_bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/itertools_direct_product.test.cpp
    title: verify/mytest/itertools_direct_product.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/linear_sieve.test.cpp
    title: verify/mytest/linear_sieve.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint.test.cpp
    title: verify/mytest/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint64.test.cpp
    title: verify/mytest/modint64.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/power_table.test.cpp
    title: verify/mytest/power_table.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_bit.test.cpp
    title: verify/mytest/template_bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/and_or_convolution.test.cpp
    title: verify/yosupo/and_or_convolution.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/aplusb.test.cpp
    title: verify/yosupo/aplusb.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/associative_array_flat_map.test.cpp
    title: verify/yosupo/associative_array_flat_map.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/associative_array_lower_bound.test.cpp
    title: verify/yosupo/associative_array_lower_bound.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/binomial.test.cpp
    title: verify/yosupo/binomial.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/factorize.test.cpp
    title: verify/yosupo/factorize.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/gcd_convolution.test.cpp
    title: verify/yosupo/gcd_convolution.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/kronecker_power_hadamard.test.cpp
    title: verify/yosupo/kronecker_power_hadamard.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/kronecker_power_zeta_mobius.test.cpp
    title: verify/yosupo/kronecker_power_zeta_mobius.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/kth_root_integer.test.cpp
    title: verify/yosupo/kth_root_integer.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/lcm_convolution.test.cpp
    title: verify/yosupo/lcm_convolution.test.cpp
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
    path: verify/yosupo/offline_dynamic_connectivity.test.cpp
    title: verify/yosupo/offline_dynamic_connectivity.test.cpp
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
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/quotients.test.cpp
    title: verify/yosupo/quotients.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/static_range_frequency.test.cpp
    title: verify/yosupo/static_range_frequency.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/subset_convolution.test.cpp
    title: verify/yosupo/subset_convolution.test.cpp
  - icon: ':x:'
    path: verify/yosupo/unionfind.test.cpp
    title: verify/yosupo/unionfind.test.cpp
  - icon: ':x:'
    path: verify/yosupo/unionfind_potential.test.cpp
    title: verify/yosupo/unionfind_potential.test.cpp
  - icon: ':x:'
    path: verify/yosupo/unionfind_potential_non_commutative.test.cpp
    title: verify/yosupo/unionfind_potential_non_commutative.test.cpp
  - icon: ':x:'
    path: verify/yosupo/xor_convolution.test.cpp
    title: verify/yosupo/xor_convolution.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/base_repr.test.cpp
    title: verify/yukicoder/base_repr.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/binomial.test.cpp
    title: verify/yukicoder/binomial.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/divisors.test.cpp
    title: verify/yukicoder/divisors.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/dynamic_modint.test.cpp
    title: verify/yukicoder/dynamic_modint.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/order_mod_carmichael.test.cpp
    title: verify/yukicoder/order_mod_carmichael.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/order_mod_euler_phi.test.cpp
    title: verify/yukicoder/order_mod_euler_phi.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/parallel_binsearch.test.cpp
    title: verify/yukicoder/parallel_binsearch.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/quadratic_equation.test.cpp
    title: verify/yukicoder/quadratic_equation.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/static_modint.test.cpp
    title: verify/yukicoder/static_modint.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/uf_min_max_leader.test.cpp
    title: verify/yukicoder/uf_min_max_leader.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/uf_partially_persistent.test.cpp
    title: verify/yukicoder/uf_partially_persistent.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/zeta_mobius_divisor_large.test.cpp
    title: verify/yukicoder/zeta_mobius_divisor_large.test.cpp
  - icon: ':x:'
    path: verify/yukicoder/zeta_mobius_multiple_large.test.cpp
    title: verify/yukicoder/zeta_mobius_multiple_large.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':question:'
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
    \u30FC\u30C8\uFF08rep\uFF09\n * @docs docs/template/template_rep.md\n */\n\n//\
    \ https://trap.jp/post/1224/\n\n#define overload4(_1, _2, _3, _4, name, ...) name\n\
    #define rep1(i, n) for (ll i = 0, nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i,\
    \ l, r) for (ll i = ll(l), rrrrr = ll(r); i < rrrrr; i++)\n#define rep3(i, l,\
    \ r, d) for (ll i = ll(l), rrrrr = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr\
    \ : i > rrrrr; i += d)\n#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n\
    #define repi1(i, n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i,\
    \ l, r) for (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i,\
    \ l, r, d) for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i\
    \ < rrrrr : i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3,\
    \ repi2, repi1)(__VA_ARGS__)\n\n#define fe(...) for (auto __VA_ARGS__)\n#define\
    \ fec(...) for (cauto &__VA_ARGS__)\n#define fem(...) for (auto &__VA_ARGS__)\n\
    #line 5 \"template/template_bit.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\uFF09\n * @docs docs/template/template_bit.md\n\
    \ */\n\ntemplate <class T>\ninline constexpr ull pow2(T k) { return 1ULL << k;\
    \ }\ntemplate <class T>\ninline constexpr ull MASK(T k) { return (1ULL << k) -\
    \ 1ULL; }\n\n#if __cplusplus < 202002L\n// x == 0 \u306A\u3089\u3070 0\u3001\u305D\
    \u3046\u3067\u306A\u3051\u308C\u3070 1 + floor(log2(x))\n// 0, 1, 2, 2, 3, 3,\
    \ 3, 3, 4, 4, ... \ninline constexpr ull bit_width(ull x) { return x == 0 ? 0\
    \ : 64 - __builtin_clzll(x); }\n// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline constexpr\
    \ ull bit_floor(ull x) { return x == 0 ? 0ULL : 1ULL << (bit_width(x) - 1); }\n\
    // 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...\ninline constexpr ull bit_ceil(ull x) {\
    \ return x == 0 ? 1ULL : 1ULL << bit_width(x - 1); }\ninline constexpr ull countr_zero(ull\
    \ x) { assert(x != 0); return __builtin_ctzll(x); }\ninline constexpr ull popcount(ull\
    \ x) { return __builtin_popcountll(x); }\ninline constexpr bool has_single_bit(ull\
    \ x) { return popcount(x) == 1; }\n#else\n// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ...\
    \ \ninline constexpr ll bit_width(ll x) { return std::bit_width((ull)x); }\n//\
    \ 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline constexpr ll bit_floor(ll x) { return\
    \ std::bit_floor((ull)x); }\n// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...\ninline constexpr\
    \ ll bit_ceil(ll x) { return std::bit_ceil((ull)x); }\ninline constexpr ll countr_zero(ll\
    \ x) { assert(x != 0); return std::countr_zero((ull)x); }\ninline constexpr ll\
    \ popcount(ll x) { return std::popcount((ull)x); }\ninline constexpr bool has_single_bit(ll\
    \ x) { return std::has_single_bit((ull)x); }\n#endif\n\ninline constexpr ull lsb_pos(ull\
    \ x) { assert(x != 0); return countr_zero(x); }\ninline constexpr ull msb_pos(ull\
    \ x) { assert(x != 0); return bit_width(x) - 1; }\ninline constexpr ull lsb_mask(ull\
    \ x) { assert(x != 0); return x & -x; }\ninline constexpr ull msb_mask(ull x)\
    \ { assert(x != 0); return bit_floor(x); }\n\ninline constexpr bool btest(ull\
    \ x, uint k) { return (x >> k) & 1; }\ntemplate <class T>\ninline void bset(T\
    \ &x, uint k, bool b = 1) { b ? x |= (1ULL << k) : x &= ~(1ULL << k); }\ntemplate\
    \ <class T>\ninline void bflip(T &x, uint k) { x ^= (1ULL << k); }\ninline constexpr\
    \ bool bsubset(ull x, ull y) { return (x & y) == x; }\ninline constexpr bool bsupset(ull\
    \ x, ull y) { return (x & y) == y; }\ninline constexpr ull bsetminus(ull x, ull\
    \ y) { return x & ~y; }\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\
    \u6F14\u7B97\uFF09\n * @docs docs/template/template_bit.md\n */\n\ntemplate <class\
    \ T>\ninline constexpr ull pow2(T k) { return 1ULL << k; }\ntemplate <class T>\n\
    inline constexpr ull MASK(T k) { return (1ULL << k) - 1ULL; }\n\n#if __cplusplus\
    \ < 202002L\n// x == 0 \u306A\u3089\u3070 0\u3001\u305D\u3046\u3067\u306A\u3051\
    \u308C\u3070 1 + floor(log2(x))\n// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... \ninline\
    \ constexpr ull bit_width(ull x) { return x == 0 ? 0 : 64 - __builtin_clzll(x);\
    \ }\n// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline constexpr ull bit_floor(ull\
    \ x) { return x == 0 ? 0ULL : 1ULL << (bit_width(x) - 1); }\n// 1, 1, 2, 4, 4,\
    \ 8, 8, 8, 8, 16, ...\ninline constexpr ull bit_ceil(ull x) { return x == 0 ?\
    \ 1ULL : 1ULL << bit_width(x - 1); }\ninline constexpr ull countr_zero(ull x)\
    \ { assert(x != 0); return __builtin_ctzll(x); }\ninline constexpr ull popcount(ull\
    \ x) { return __builtin_popcountll(x); }\ninline constexpr bool has_single_bit(ull\
    \ x) { return popcount(x) == 1; }\n#else\n// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ...\
    \ \ninline constexpr ll bit_width(ll x) { return std::bit_width((ull)x); }\n//\
    \ 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline constexpr ll bit_floor(ll x) { return\
    \ std::bit_floor((ull)x); }\n// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...\ninline constexpr\
    \ ll bit_ceil(ll x) { return std::bit_ceil((ull)x); }\ninline constexpr ll countr_zero(ll\
    \ x) { assert(x != 0); return std::countr_zero((ull)x); }\ninline constexpr ll\
    \ popcount(ll x) { return std::popcount((ull)x); }\ninline constexpr bool has_single_bit(ll\
    \ x) { return std::has_single_bit((ull)x); }\n#endif\n\ninline constexpr ull lsb_pos(ull\
    \ x) { assert(x != 0); return countr_zero(x); }\ninline constexpr ull msb_pos(ull\
    \ x) { assert(x != 0); return bit_width(x) - 1; }\ninline constexpr ull lsb_mask(ull\
    \ x) { assert(x != 0); return x & -x; }\ninline constexpr ull msb_mask(ull x)\
    \ { assert(x != 0); return bit_floor(x); }\n\ninline constexpr bool btest(ull\
    \ x, uint k) { return (x >> k) & 1; }\ntemplate <class T>\ninline void bset(T\
    \ &x, uint k, bool b = 1) { b ? x |= (1ULL << k) : x &= ~(1ULL << k); }\ntemplate\
    \ <class T>\ninline void bflip(T &x, uint k) { x ^= (1ULL << k); }\ninline constexpr\
    \ bool bsubset(ull x, ull y) { return (x & y) == x; }\ninline constexpr bool bsupset(ull\
    \ x, ull y) { return (x & y) == y; }\ninline constexpr ull bsetminus(ull x, ull\
    \ y) { return x & ~y; }"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  isVerificationFile: false
  path: template/template_bit.hpp
  requiredBy:
  - math/extgcd.hpp
  - math/set/zeta_mobius.hpp
  - math/set/kronecker_power.hpp
  - math/set/subset_convolution.hpp
  - math/set/and_or_convolution.hpp
  - math/set/xor_convolution.hpp
  - math/prime/order_primitive_root.hpp
  - math/prime/zeta_mobius_divisor_multiple_large.hpp
  - math/prime/euler_phi_carmichael.hpp
  - math/prime/prime_power.hpp
  - math/prime/factorize.hpp
  - math/prime/lcm_gcd_convolution.hpp
  - math/prime/primality_test.hpp
  - math/prime/zeta_mobius_divisor_multiple.hpp
  - math/prime/linear_sieve.hpp
  - math/modint/modint64_internal.hpp
  - math/modint/modint32_internal.hpp
  - math/modint/modint.hpp
  - math/modint/binomial.hpp
  - math/modint/modint_base.hpp
  - math/modint/modint64.hpp
  - math/modint/power_table.hpp
  - math/quotients.hpp
  - math/algebra/matmul22.hpp
  - math/algebra/algebra_basic_ops.hpp
  - math/algebra/polynomial_ring.hpp
  - math/algebra/algebra_base.hpp
  - math/quadratic_equation_integer.hpp
  - template/template_all.hpp
  - template/template.cpp
  - algo/merge_sort.hpp
  - algo/parallel_binsearch.hpp
  - graph/graph.hpp
  - itertools/direct_product.hpp
  - itertools/bit.hpp
  - ds/csr.hpp
  - ds/flat_map.hpp
  - ds/coordinate_compression.hpp
  - ds/offline_dynamic_connectivity.hpp
  - ds/uf/uf_partially_persistent.hpp
  - ds/uf/uf.hpp
  - ds/uf/uf_potential.hpp
  - ds/uf/uf_undo.hpp
  - ds/group_index.hpp
  timestamp: '2025-03-12 06:54:29+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - verify/aoj/modpow.test.cpp
  - verify/aoj/csr.test.cpp
  - verify/aoj/extgcd.test.cpp
  - verify/yukicoder/parallel_binsearch.test.cpp
  - verify/yukicoder/order_mod_euler_phi.test.cpp
  - verify/yukicoder/zeta_mobius_multiple_large.test.cpp
  - verify/yukicoder/quadratic_equation.test.cpp
  - verify/yukicoder/uf_partially_persistent.test.cpp
  - verify/yukicoder/order_mod_carmichael.test.cpp
  - verify/yukicoder/divisors.test.cpp
  - verify/yukicoder/base_repr.test.cpp
  - verify/yukicoder/zeta_mobius_divisor_large.test.cpp
  - verify/yukicoder/static_modint.test.cpp
  - verify/yukicoder/uf_min_max_leader.test.cpp
  - verify/yukicoder/dynamic_modint.test.cpp
  - verify/yukicoder/binomial.test.cpp
  - verify/yosupo/quotients.test.cpp
  - verify/yosupo/kronecker_power_hadamard.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
  - verify/yosupo/static_range_frequency.test.cpp
  - verify/yosupo/and_or_convolution.test.cpp
  - verify/yosupo/primitive_root_min.test.cpp
  - verify/yosupo/factorize.test.cpp
  - verify/yosupo/many_aplusb_128bit.test.cpp
  - verify/yosupo/gcd_convolution.test.cpp
  - verify/yosupo/primality_test.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/kth_root_integer.test.cpp
  - verify/yosupo/xor_convolution.test.cpp
  - verify/yosupo/offline_dynamic_connectivity.test.cpp
  - verify/yosupo/primitive_root.test.cpp
  - verify/yosupo/unionfind.test.cpp
  - verify/yosupo/subset_convolution.test.cpp
  - verify/yosupo/predecessor_problem.test.cpp
  - verify/yosupo/unionfind_potential_non_commutative.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/yosupo/associative_array_lower_bound.test.cpp
  - verify/yosupo/associative_array_flat_map.test.cpp
  - verify/yosupo/lcm_convolution.test.cpp
  - verify/yosupo/unionfind_potential.test.cpp
  - verify/yosupo/kronecker_power_zeta_mobius.test.cpp
  - verify/yosupo/binomial.test.cpp
  - verify/mytest/modint64.test.cpp
  - verify/mytest/template_bit.test.cpp
  - verify/mytest/power_table.test.cpp
  - verify/mytest/itertools_direct_product.test.cpp
  - verify/mytest/modint.test.cpp
  - verify/mytest/linear_sieve.test.cpp
  - verify/mytest/itertools_bit.test.cpp
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

- $x \neq 0$

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
