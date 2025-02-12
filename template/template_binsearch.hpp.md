---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_math.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_vector.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
  _extendedRequiredBy:
  - icon: ':warning:'
    path: algo/merge_sort.hpp
    title: "\u30DE\u30FC\u30B8\u30BD\u30FC\u30C8"
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
    path: itertools/bit.hpp
    title: "\u76F4\u7A4D"
  - icon: ':heavy_check_mark:'
    path: itertools/direct_product.hpp
    title: "\u76F4\u7A4D"
  - icon: ':heavy_check_mark:'
    path: math/algebra/algebra_base.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\u306E struct\uFF08\u57FA\u672C\uFF09"
  - icon: ':heavy_check_mark:'
    path: math/algebra/algebra_basic_ops.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\uFF08\u56DB\u5247\u6F14\u7B97\u3068 min,\
      \ max\uFF09"
  - icon: ':heavy_check_mark:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':heavy_check_mark:'
    path: math/modint/binomial.hpp
    title: "\u4E8C\u9805\u4FC2\u6570"
  - icon: ':heavy_check_mark:'
    path: math/modint/modint.hpp
    title: modint (32 bit)
  - icon: ':heavy_check_mark:'
    path: math/modint/modint64.hpp
    title: modint (64 bit)
  - icon: ':heavy_check_mark:'
    path: math/modint/modint_internal.hpp
    title: math/modint/modint_internal.hpp
  - icon: ':heavy_check_mark:'
    path: math/prime/euler_phi_carmichael.hpp
    title: "\u30AA\u30A4\u30E9\u30FC\u306E\u30D5\u30A1\u30A4\u95A2\u6570\u30FB\u30AB\
      \u30FC\u30DE\u30A4\u30B1\u30EB\u95A2\u6570"
  - icon: ':heavy_check_mark:'
    path: math/prime/factorize.hpp
    title: "\u7D20\u56E0\u6570\u5206\u89E3"
  - icon: ':heavy_check_mark:'
    path: math/prime/order_primitive_root.hpp
    title: "\u5143\u306E\u4F4D\u6570\u3068\u539F\u59CB\u6839"
  - icon: ':heavy_check_mark:'
    path: math/prime/primality_test.hpp
    title: "\u7D20\u6570\u5224\u5B9A"
  - icon: ':heavy_check_mark:'
    path: math/prime/prime_power.hpp
    title: "\u7D20\u3079\u304D\u69CB\u9020\u4F53"
  - icon: ':heavy_check_mark:'
    path: math/prime/zeta_mobius_divisor_multiple_large.hpp
    title: "\u7D04\u6570\u30FB\u500D\u6570 \u30BC\u30FC\u30BF\u30FB\u30E1\u30D3\u30A6\
      \u30B9\u5909\u63DB\uFF08\u5927\u304D\u3044 $m$ \u306E\u7D04\u6570\uFF09"
  - icon: ':heavy_check_mark:'
    path: math/quotients.hpp
    title: "\u5546\u5217\u6319"
  - icon: ':heavy_check_mark:'
    path: math/set/and_or_convolution.hpp
    title: "and/or \u7573\u307F\u8FBC\u307F"
  - icon: ':heavy_check_mark:'
    path: math/set/zeta_mobius.hpp
    title: "\u30BC\u30FC\u30BF\u30FB\u30E1\u30D3\u30A6\u30B9\u5909\u63DB"
  - icon: ':warning:'
    path: template/template.cpp
    title: template/template.cpp
  - icon: ':heavy_check_mark:'
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
    path: verify/mytest/itertools_bit.test.cpp
    title: verify/mytest/itertools_bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/itertools_direct_product.test.cpp
    title: verify/mytest/itertools_direct_product.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint.test.cpp
    title: verify/mytest/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint64.test.cpp
    title: verify/mytest/modint64.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_binsearch.test.cpp
    title: verify/mytest/template_binsearch.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/and_or_convolution.test.cpp
    title: verify/yosupo/and_or_convolution.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/aplusb.test.cpp
    title: verify/yosupo/aplusb.test.cpp
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
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/quotients.test.cpp
    title: verify/yosupo/quotients.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/static_range_frequency.test.cpp
    title: verify/yosupo/static_range_frequency.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/base_repr.test.cpp
    title: verify/yukicoder/base_repr.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/binomial.test.cpp
    title: verify/yukicoder/binomial.test.cpp
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
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    _deprecated_at_docs: docs/template/template_binsearch.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\
      \uFF09"
    links: []
  bundledCode: "#line 2 \"template/template_binsearch.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    #line 2 \"template/template_vector.hpp\"\n\n#line 2 \"template/template_math.hpp\"\
    \n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n#ifndef EPS\n\
    #define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\n\n/**\n\
    \ * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs\
    \ docs/template/template_math.md\n */\n\ninline bool chmin(auto &a, cauto &b)\
    \ { return a > b ? a = b, true : false; }\ninline bool chmax(auto &a, cauto &b)\
    \ { return a < b ? a = b, true : false; }\n\ntemplate <class T = ll>\ninline constexpr\
    \ T divfloor(cauto &a, cauto &b) { return T(a) / T(b) - (T(a) % T(b) && (T(a)\
    \ ^ T(b)) < 0); }\ntemplate <class T = ll>\ninline constexpr T divceil(cauto &a,\
    \ cauto &b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate\
    \ <class T = ll>\ninline constexpr T divround(cauto &a, cauto &b) { return divfloor<T>(2\
    \ * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll>\ninline constexpr T safemod(cauto\
    \ &a, cauto &b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class\
    \ T = ll>\nconstexpr T ipow(cauto &a, auto b)\n{\n  assert(b >= 0);\n  if (b ==\
    \ 0)\n    return 1;\n  if (a == 0 || a == 1)\n    return a;\n  if (a < 0 && a\
    \ == -1)\n    return b & 1 ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while (true)\n\
    \  {\n    if (b & 1)\n      res *= tmp;\n    b >>= 1;\n    if (b == 0)\n     \
    \ break;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T = ll>\nT mul_limited(cauto\
    \ &a, cauto &b, cauto &m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if\
    \ (b == 0)\n    return 0;\n  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);\n\
    }\ntemplate <class T = ll>\nT pow_limited(cauto &a, auto b, cauto &m = INF)\n\
    {\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return\
    \ min(ipow<T>(a, b), T(m));\n  \n  T res = 1, tmp = a;\n  while (true)\n  {\n\
    \    if (b & 1)\n    {\n      if (res > T(m) / tmp)\n        return m;\n     \
    \ res *= tmp;\n    }\n    b >>= 1;\n    if (b == 0)\n      break;\n    if (tmp\
    \ > T(m) / tmp)\n      return m;\n    tmp *= tmp;\n  }\n  return res;\n}\n\ntemplate\
    \ <class T = ll>\nconstexpr T iroot(cauto &a, cauto &k)\n{\n  assert(a >= 0 &&\
    \ k >= 1);\n  if (a <= 1 || k == 1)\n    return a;\n  if (k == 2 && a <= ULLONG_MAX)\n\
    \    return sqrtl(a);\n\n  auto isok = [&](T x) -> bool\n  {\n    if (x == 0)\n\
    \      return true;\n    T res = 1, k2 = k;\n    while (true)\n    {\n      if\
    \ (k2 & 1)\n      {\n        if (res > T(a) / x)\n          return false;\n  \
    \      res *= x;\n      }\n      k2 >>= 1;\n      if (k2 == 0)\n        break;\n\
    \      if (x > T(a) / x)\n        return false;\n      x *= x;\n    }\n    return\
    \ res <= T(a);\n  };\n\n  T x = pow(a, 1.0 / k);\n  bool up = true;\n  while (!isok(x))\n\
    \    up = false, x--;\n  if (up)\n  {\n    while (x < numeric_limits<T>::max()\
    \ && isok(x + 1))\n      x++;\n  }\n  return x;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS)>\nint SGN(cauto &a, const D &eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }\n\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\
    \u540C\u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\n// 0\
    \ \u306B\u5BFE\u3057\u3066\u306F {0} \u304C\u8FD4\u308B\ntemplate <class T = ll>\n\
    vc<T> base_repr(auto val, auto base)\n{\n  assert(val >= 0);\n  assert(base >=\
    \ 2);\n  if (val == 0)\n    return {0};\n  vc<T> a;\n  while (val > 0)\n  {\n\
    \    a.emplace_back(val % base);\n    val /= base;\n  }\n  reverse(a.begin(),\
    \ a.end());\n  return a;\n}\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\u540C\
    \u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate <class\
    \ T = ll>\nvc<T> base_repr(auto val, auto base, int n)\n{\n  assert(val >= 0);\n\
    \  assert(base >= 2);\n  assert(n >= 0);\n  vc<T> a(n);\n  repi(i, n)\n  {\n \
    \   a[i] = val % base;\n    val /= base;\n  }\n  reverse(a.begin(), a.end());\n\
    \  return a;\n}\nstring base_repr_str(auto val, int base, bool use_upper = true)\n\
    {\n  assert(val >= 0);\n  assert(2 <= base && base <= 36);\n  auto a = base_repr(val,\
    \ base);\n  string s = \"\";\n  for (cauto &ai : a)\n    s += (ai < 10 ? '0' +\
    \ ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return s;\n}\nstring base_repr_str(auto\
    \ val, int base, int n, bool use_upper = true)\n{\n  assert(val >= 0);\n  assert(2\
    \ <= base && base <= 36);\n  assert(n >= 0);\n  auto a = base_repr(val, base,\
    \ n);\n  string s = \"\";\n  for (cauto &ai : a)\n    s += (ai < 10 ? '0' + ai\
    \ : (use_upper ? 'A' : 'a') + (ai - 10));\n  return s;\n}\n#line 6 \"template/template_vector.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09\n *\
    \ @docs docs/template/template_vector.md\n */\n\n#define ALL(a) (a).begin(), (a).end()\n\
    template <class T = ll>\ninline T SZ(cauto &x) { return x.size(); }\n\ntemplate\
    \ <class F>\nauto gen_vec(const int &n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n\
    \  repi(i, n) res[i] = f(i);\n  return res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
    template <class T, size_t d, size_t i = 0>\nauto dvec(cauto (&sz)[d], const T\
    \ &init)\n{\n  if constexpr (i < d)\n    return vc(sz[i], dvec<T, d, i + 1>(sz,\
    \ init));\n  else\n    return init;\n}\n\ntemplate <class T = ll>\nT ctol(const\
    \ char &c, const string &s)\n{\n  repi(i, SZ<int>(s)) if (s[i] == c) return i;\n\
    \  return -1;\n}\ntemplate <class T = ll>\nvc<T> stov(const string &s, const char\
    \ &first)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n                 {\
    \ return s[i] - first; });\n}\ntemplate <class T = ll>\nvc<T> stov(const string\
    \ &s, const string &t)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n    \
    \             { return ctol(s[i], t); });\n}\n\ntemplate <class T>\nvc<T> concat(const\
    \ vvc<T> &vs)\n{\n  vc<T> res;\n  for (cauto &v : vs)\n    res.insert(res.end(),\
    \ ALL(v));\n  return res;\n}\ntemplate <class T>\nvc<T> concat(const vc<T> &v)\
    \ { return v; }\ntemplate <class T, class... Ts>\nvc<T> concat(vc<T> v, const\
    \ vc<Ts> &...vs)\n{\n  (v.insert(v.end(), ALL(vs)), ...);\n  return v;\n}\n\n\
    template <class T>\nT vecget(const vc<T> &v, cauto &i, const T &dflt_negative\
    \ = -INF, const T &dflt_positive = INF)\n{\n  if (i < 0)\n    return dflt_negative;\n\
    \  if (i >= SZ<int>(v))\n    return dflt_positive;\n  return v[i];\n}\n#line 6\
    \ \"template/template_binsearch.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09\n * @docs docs/template/template_binsearch.md\n\
    \ */\n\n#if __cplusplus < 202002L\n// val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\
    \u306E i (val \u672A\u6E80\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\u6570: comp\n\
    template <class T = ll, class V, class... Args>\ninline T LB(const V &v, Args&&...\
    \ args)\n{ return lower_bound(ALL(v), forward<Args>(args)...) - v.begin(); }\n\
    // val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\u4E0B\u306E\u5024\
    \u306E\u500B\u6570)\n// \u5F15\u6570: comp\ntemplate <class T = ll, class V, class...\
    \ Args>\ninline T UB(const V &v, Args&&... args)\n{ return upper_bound(ALL(v),\
    \ forward<Args>(args)...) - v.begin(); }\n#else\n// val <= v[i] \u3068\u306A\u308B\
    \u6700\u5C0F\u306E i (val \u672A\u6E80\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\
    \u6570: comp, proj\ntemplate <class T = ll, class V, class... Args>\ninline T\
    \ LB(const V &v, Args&&... args)\n{ return ranges::lower_bound(v, forward<Args>(args)...)\
    \ - v.begin(); }\n// val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\
    \u4E0B\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\u6570: comp, proj\ntemplate <class\
    \ T = ll, class V, class... Args>\ninline T UB(const V &v, Args&&... args)\n{\
    \ return ranges::upper_bound(v, forward<Args>(args)...) - v.begin(); }\n#endif\n\
    \ntemplate <class T>\nstruct is_random_access_iterator\n{\n  static constexpr\
    \ bool value = is_same_v<\n    typename iterator_traits<T>::iterator_category,\n\
    \    random_access_iterator_tag\n  >;\n};\ntemplate <class T>\nconstexpr bool\
    \ is_random_access_iterator_v = is_random_access_iterator<T>::value;\n\ntemplate\
    \ <class T = ll, class V, class... Args>\ninline auto lt_max(const V &v, Args&&...\
    \ args)\n{\n  if constexpr (is_random_access_iterator_v<typename V::iterator>)\n\
    \    return LB<T>(v, forward<Args>(args)...) - 1;\n  else\n  {\n    auto it =\
    \ v.lower_bound(forward<Args>(args)...);\n    if (it == v.begin())\n      return\
    \ v.end();\n    else\n      return prev(it);\n  }\n}\ntemplate <class T = ll,\
    \ class V, class... Args>\ninline auto leq_max(const V &v, Args&&... args)\n{\n\
    \  if constexpr (is_random_access_iterator_v<typename V::iterator>)\n    return\
    \ UB<T>(v, forward<Args>(args)...) - 1;\n  else\n  {\n    auto it = v.upper_bound(forward<Args>(args)...);\n\
    \    if (it == v.begin())\n      return v.end();\n    else\n      return prev(it);\n\
    \  }\n}\ntemplate <class T = ll, class V, class... Args>\ninline auto gt_min(const\
    \ V &v, Args&&... args)\n{\n  if constexpr (is_random_access_iterator_v<typename\
    \ V::iterator>)\n    return UB<T>(v, forward<Args>(args)...);\n  else\n    return\
    \ v.upper_bound(forward<Args>(args)...);\n}\ntemplate <class T = ll, class V,\
    \ class... Args>\ninline auto geq_min(const V &v, Args&&... args)\n{\n  if constexpr\
    \ (is_random_access_iterator_v<typename V::iterator>)\n    return LB<T>(v, forward<Args>(args)...);\n\
    \  else\n    return v.lower_bound(forward<Args>(args)...);\n}\n\ntemplate <class\
    \ T = ll, class V, class... Args>\ninline T lt_cnt(const V &v, Args&&... args)\n\
    { return LB<T>(v, forward<Args>(args)...); }\ntemplate <class T = ll, class V,\
    \ class... Args>\ninline T leq_cnt(const V &v, Args&&... args)\n{ return UB<T>(v,\
    \ forward<Args>(args)...); }\ntemplate <class T = ll, class V, class... Args>\n\
    inline T gt_cnt(const V &v, Args&&... args)\n{ return SZ<T>(v) - UB<T>(v, forward<Args>(args)...);\
    \ }\ntemplate <class T = ll, class V, class... Args>\ninline T geq_cnt(const V\
    \ &v, Args&&... args)\n{ return SZ<T>(v) - LB<T>(v, forward<Args>(args)...); }\n\
    \ntemplate <class T = ll, class V, class... Args>\ninline T in_cnt(const V &v,\
    \ auto l, auto r, Args&&... args)\n{\n  if (l > r)\n    return 0;\n  return lt_cnt<T>(v,\
    \ r, forward<Args>(args)...) - lt_cnt<T>(v, l, forward<Args>(args)...);\n}\n\n\
    template <class T = ll>\npair<T, T> binsearch(cauto &judge, cauto &init_ok, cauto\
    \ &init_ng)\n{\n  T ok(init_ok), ng(init_ng);\n  assert(judge(ok));\n  assert(!judge(ng));\n\
    \  while (ok - ng != 1 && ng - ok != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng)\
    \ >> 1);\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return make_pair(ok, ng);\n\
    }\ntemplate <class T = ld>\nT binsearch_real(cauto &judge, cauto &init_ok, cauto\
    \ &init_ng, const int &iteration_count = 100)\n{\n  T ok(init_ok), ng(init_ng);\n\
    \  assert(judge(ok));\n  assert(!judge(ng));\n  repi(_, iteration_count)\n  {\n\
    \    T mid = (ok + ng) / 2;\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return\
    \ ok;\n}\ntemplate <class T = ll>\npair<T, T> expsearch(cauto &judge, cauto &init_val,\
    \ const bool &positive = true)\n{\n  T ok, ng;\n  if (judge(init_val))\n  {\n\
    \    ok = init_val, ng = init_val + (positive ? 1 : -1);\n    for (int i = 1;\
    \ judge(ng); i++)\n      ok = ng, ng = init_val + (positive ? 1 : -1) * (T(1)\
    \ << i);\n  }\n  else\n  {\n    ng = init_val, ok = init_val + (positive ? 1 :\
    \ -1);\n    for (int i = 1; !judge(ok); i++)\n      ng = ok, ok = init_val + (positive\
    \ ? 1 : -1) * (T(1) << i);\n  }\n  while (ok - ng != 1 && ng - ok != 1)\n  {\n\
    \    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid) ? ok : ng) = mid;\n\
    \  }\n  return make_pair(ok, ng);\n}\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n#include \"template_vector.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\
    \u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09\n * @docs docs/template/template_binsearch.md\n\
    \ */\n\n#if __cplusplus < 202002L\n// val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\
    \u306E i (val \u672A\u6E80\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\u6570: comp\n\
    template <class T = ll, class V, class... Args>\ninline T LB(const V &v, Args&&...\
    \ args)\n{ return lower_bound(ALL(v), forward<Args>(args)...) - v.begin(); }\n\
    // val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\u4E0B\u306E\u5024\
    \u306E\u500B\u6570)\n// \u5F15\u6570: comp\ntemplate <class T = ll, class V, class...\
    \ Args>\ninline T UB(const V &v, Args&&... args)\n{ return upper_bound(ALL(v),\
    \ forward<Args>(args)...) - v.begin(); }\n#else\n// val <= v[i] \u3068\u306A\u308B\
    \u6700\u5C0F\u306E i (val \u672A\u6E80\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\
    \u6570: comp, proj\ntemplate <class T = ll, class V, class... Args>\ninline T\
    \ LB(const V &v, Args&&... args)\n{ return ranges::lower_bound(v, forward<Args>(args)...)\
    \ - v.begin(); }\n// val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\
    \u4E0B\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\u6570: comp, proj\ntemplate <class\
    \ T = ll, class V, class... Args>\ninline T UB(const V &v, Args&&... args)\n{\
    \ return ranges::upper_bound(v, forward<Args>(args)...) - v.begin(); }\n#endif\n\
    \ntemplate <class T>\nstruct is_random_access_iterator\n{\n  static constexpr\
    \ bool value = is_same_v<\n    typename iterator_traits<T>::iterator_category,\n\
    \    random_access_iterator_tag\n  >;\n};\ntemplate <class T>\nconstexpr bool\
    \ is_random_access_iterator_v = is_random_access_iterator<T>::value;\n\ntemplate\
    \ <class T = ll, class V, class... Args>\ninline auto lt_max(const V &v, Args&&...\
    \ args)\n{\n  if constexpr (is_random_access_iterator_v<typename V::iterator>)\n\
    \    return LB<T>(v, forward<Args>(args)...) - 1;\n  else\n  {\n    auto it =\
    \ v.lower_bound(forward<Args>(args)...);\n    if (it == v.begin())\n      return\
    \ v.end();\n    else\n      return prev(it);\n  }\n}\ntemplate <class T = ll,\
    \ class V, class... Args>\ninline auto leq_max(const V &v, Args&&... args)\n{\n\
    \  if constexpr (is_random_access_iterator_v<typename V::iterator>)\n    return\
    \ UB<T>(v, forward<Args>(args)...) - 1;\n  else\n  {\n    auto it = v.upper_bound(forward<Args>(args)...);\n\
    \    if (it == v.begin())\n      return v.end();\n    else\n      return prev(it);\n\
    \  }\n}\ntemplate <class T = ll, class V, class... Args>\ninline auto gt_min(const\
    \ V &v, Args&&... args)\n{\n  if constexpr (is_random_access_iterator_v<typename\
    \ V::iterator>)\n    return UB<T>(v, forward<Args>(args)...);\n  else\n    return\
    \ v.upper_bound(forward<Args>(args)...);\n}\ntemplate <class T = ll, class V,\
    \ class... Args>\ninline auto geq_min(const V &v, Args&&... args)\n{\n  if constexpr\
    \ (is_random_access_iterator_v<typename V::iterator>)\n    return LB<T>(v, forward<Args>(args)...);\n\
    \  else\n    return v.lower_bound(forward<Args>(args)...);\n}\n\ntemplate <class\
    \ T = ll, class V, class... Args>\ninline T lt_cnt(const V &v, Args&&... args)\n\
    { return LB<T>(v, forward<Args>(args)...); }\ntemplate <class T = ll, class V,\
    \ class... Args>\ninline T leq_cnt(const V &v, Args&&... args)\n{ return UB<T>(v,\
    \ forward<Args>(args)...); }\ntemplate <class T = ll, class V, class... Args>\n\
    inline T gt_cnt(const V &v, Args&&... args)\n{ return SZ<T>(v) - UB<T>(v, forward<Args>(args)...);\
    \ }\ntemplate <class T = ll, class V, class... Args>\ninline T geq_cnt(const V\
    \ &v, Args&&... args)\n{ return SZ<T>(v) - LB<T>(v, forward<Args>(args)...); }\n\
    \ntemplate <class T = ll, class V, class... Args>\ninline T in_cnt(const V &v,\
    \ auto l, auto r, Args&&... args)\n{\n  if (l > r)\n    return 0;\n  return lt_cnt<T>(v,\
    \ r, forward<Args>(args)...) - lt_cnt<T>(v, l, forward<Args>(args)...);\n}\n\n\
    template <class T = ll>\npair<T, T> binsearch(cauto &judge, cauto &init_ok, cauto\
    \ &init_ng)\n{\n  T ok(init_ok), ng(init_ng);\n  assert(judge(ok));\n  assert(!judge(ng));\n\
    \  while (ok - ng != 1 && ng - ok != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng)\
    \ >> 1);\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return make_pair(ok, ng);\n\
    }\ntemplate <class T = ld>\nT binsearch_real(cauto &judge, cauto &init_ok, cauto\
    \ &init_ng, const int &iteration_count = 100)\n{\n  T ok(init_ok), ng(init_ng);\n\
    \  assert(judge(ok));\n  assert(!judge(ng));\n  repi(_, iteration_count)\n  {\n\
    \    T mid = (ok + ng) / 2;\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return\
    \ ok;\n}\ntemplate <class T = ll>\npair<T, T> expsearch(cauto &judge, cauto &init_val,\
    \ const bool &positive = true)\n{\n  T ok, ng;\n  if (judge(init_val))\n  {\n\
    \    ok = init_val, ng = init_val + (positive ? 1 : -1);\n    for (int i = 1;\
    \ judge(ng); i++)\n      ok = ng, ng = init_val + (positive ? 1 : -1) * (T(1)\
    \ << i);\n  }\n  else\n  {\n    ng = init_val, ok = init_val + (positive ? 1 :\
    \ -1);\n    for (int i = 1; !judge(ok); i++)\n      ng = ok, ok = init_val + (positive\
    \ ? 1 : -1) * (T(1) << i);\n  }\n  while (ok - ng != 1 && ng - ok != 1)\n  {\n\
    \    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid) ? ok : ng) = mid;\n\
    \  }\n  return make_pair(ok, ng);\n}"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_vector.hpp
  - template/template_math.hpp
  isVerificationFile: false
  path: template/template_binsearch.hpp
  requiredBy:
  - math/modint/binomial.hpp
  - math/modint/modint_internal.hpp
  - math/modint/modint64.hpp
  - math/modint/modint.hpp
  - math/quotients.hpp
  - math/extgcd.hpp
  - math/algebra/algebra_basic_ops.hpp
  - math/algebra/algebra_base.hpp
  - math/set/zeta_mobius.hpp
  - math/set/and_or_convolution.hpp
  - math/prime/factorize.hpp
  - math/prime/primality_test.hpp
  - math/prime/prime_power.hpp
  - math/prime/euler_phi_carmichael.hpp
  - math/prime/zeta_mobius_divisor_multiple_large.hpp
  - math/prime/order_primitive_root.hpp
  - template/template.cpp
  - template/template_all.hpp
  - itertools/bit.hpp
  - itertools/direct_product.hpp
  - ds/group_index.hpp
  - ds/csr.hpp
  - ds/coordinate_compression.hpp
  - algo/merge_sort.hpp
  timestamp: '2025-02-12 07:45:54+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/mytest/modint.test.cpp
  - verify/mytest/itertools_direct_product.test.cpp
  - verify/mytest/itertools_bit.test.cpp
  - verify/mytest/template_binsearch.test.cpp
  - verify/mytest/modint64.test.cpp
  - verify/aoj/csr.test.cpp
  - verify/aoj/extgcd.test.cpp
  - verify/yukicoder/dynamic_modint.test.cpp
  - verify/yukicoder/zeta_mobius_divisor_large.test.cpp
  - verify/yukicoder/base_repr.test.cpp
  - verify/yukicoder/static_modint.test.cpp
  - verify/yukicoder/order_mod_carmichael.test.cpp
  - verify/yukicoder/binomial.test.cpp
  - verify/yukicoder/zeta_mobius_multiple_large.test.cpp
  - verify/yukicoder/order_mod_euler_phi.test.cpp
  - verify/yukicoder/divisors.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/quotients.test.cpp
  - verify/yosupo/kth_root_integer.test.cpp
  - verify/yosupo/and_or_convolution.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
  - verify/yosupo/predecessor_problem.test.cpp
  - verify/yosupo/static_range_frequency.test.cpp
  - verify/yosupo/factorize.test.cpp
  - verify/yosupo/primality_test.test.cpp
  - verify/yosupo/primitive_root.test.cpp
  - verify/yosupo/associative_array_lower_bound.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/yosupo/primitive_root_min.test.cpp
  - verify/yosupo/binomial.test.cpp
  - verify/yosupo/many_aplusb_128bit.test.cpp
documentation_of: template/template_binsearch.hpp
layout: document
redirect_from:
- /library/template/template_binsearch.hpp
- /library/template/template_binsearch.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09"
---
## テンプレート（二分探索）

### LB, UB

```cpp
(1) U=ll LB(V v, T val, ...)
(2) U=ll UB(V v, T val, ...)
```

`ranges::lower_bound` や `ranges::upper_bound` で返るイテレータの `begin` からの距離を整数で返す。

$v$ がソート済みで、後ろの引数がデフォルトの場合、

- (1)：$\mathrm{val} \leq v_i$ となる最小の $i$
  - これは $\mathrm{val}$ 未満の値の個数に一致

- (2)：$\mathrm{val} < v_i$ となる最小の $i$
  - これは $\mathrm{val}$ 以下の値の個数に一致

を返す。

後ろには C++17 以前なら `std::lower_bound`, C++20 以降なら `std::ranges::lower_bound` の引数をとれる。

##### 制約

- `V` のイテレータはランダムアクセスイテレータ
  - OK 例：vector など
  - NG 例：set など
- $v$ はソート済みであることを想定（実際はもう少し緩い）

##### 計算量

- $O(\log \lvert v \rvert)$

### lt, leq, gt, geq

```cpp
(1) lt_max(V v, T val, ...)
(2) leq_max(V v, T val, ...)
(3) gt_min(V v, T val, ...)
(4) geq_min(V v, T val, ...)
(5) U=ll lt_cnt(V v, T val, ...)
(6) U=ll leq_cnt(V v, T val, ...)
(7) U=ll gt_cnt(V v, T val, ...)
(8) U=ll geq_cnt(V v, T val, ...)
(9) U=ll in_cnt(V v, T l, T r, ...)
```

- (1), (2), (3), (4)：次の条件を満たす $v$ の要素を探し、`V` が vector など（ランダムアクセスイテレータ）なら添字を、set など（ランダムアクセス不可で、メンバ関数に `lower_bound` や `upper_bound` を持つ）ならイテレータを返す。前者のときは $v$ がソート済みであることを想定。
  - (1)：$\mathrm{val}$ 未満で最大
  - (2)：$\mathrm{val}$ 以下で最大
  - (3)：$\mathrm{val}$ 超過で最小
  - (4)：$\mathrm{val}$ 以上で最小
  
  そのような要素が存在しない場合は、
  - 添字が返るときは、配列が無限に伸びている（負方向には $-\infty$ で正方向には $\infty$）とみなして計算し、返す（つまり、$-1$ か $n$ が返る）。
  - イテレータが返るときは、前か後ろかにかかわらず `end()` を返す。

- (5), (6), (7), (8), (9)：次の条件を満たす $v$ の要素の個数を返す。$v$ はソート済みであることを想定。
  - (5)：$\mathrm{val}$ 未満
  - (6)：$\mathrm{val}$ 以下
  - (7)：$\mathrm{val}$ 超過
  - (8)：$\mathrm{val}$ 以上
  - (9)：$l$ 以上 $r$ 未満

##### 制約

- (1), (2), (3), (4)：`V` は次のいずれかを満たす。
  - `V` のイテレータはランダムアクセスイテレータ
  - `V` はメンバ関数に `lower_bound`, `upper_bound` を持つ
- (5), (6), (7), (8), (9)：`V` のイテレータはランダムアクセスイテレータ
- (1), (2), (3), (4) の前者および (5), (6), (7), (8), (9) では、$v$ がソート済みであることを想定（実際はもう少し緩い）


##### 計算量

- $O(\log \lvert v \rvert)$


### binsearch

```cpp
(1) T binsearch(F judge, T ok, T ng)
(2) T binsearch_real(F judge, T ok, T ng, int iteration_count)
```

- (1)：整数の二分探索。整数 `T` から `bool` への関数 $\mathrm{judge}$ が $\mathrm{judge}(\mathrm{ok}) = \mathrm{true}, \mathrm{judge}(\mathrm{ng}) = \mathrm{false}$ を満たすとする。このとき $\mathrm{ok}$ と $\mathrm{ng}$ の間にある整数 $\alpha, \beta$ であって $\mathrm{judge}(\alpha) = \mathrm{true}, \mathrm{judge}(\beta) = \mathrm{false}, \lvert \alpha - \beta \rvert = 1$ を満たすものが存在する（特に $\mathrm{judge}$ が単調な場合は唯一である）。そのような $\alpha$ を一つ返す。
- (2)：実数の二分探索。実数 `T` から `bool` への関数 $\mathrm{judge}$ が $\mathrm{judge}(\mathrm{ok}) = \mathrm{true}, \mathrm{judge}(\mathrm{ng}) = \mathrm{false}$ を満たすとする。$1$ 回の反復で、$\mathrm{judge}(\mathrm{ok}) = \mathrm{true}, \mathrm{judge}(\mathrm{ng}) = \mathrm{false}$ を満たしたまま $\lvert \mathrm{ok} - \mathrm{ng} \rvert$ を半分にできる。この反復を $\mathrm{iteration\_count}$ 回繰り返す。

##### 制約

- $\mathrm{judge}(\mathrm{ok}) = \mathrm{true}$
- $\mathrm{judge}(\mathrm{ng}) = \mathrm{false}$
- (1)：型 `T` は整数
- (1)：$\mathrm{ok}, \mathrm{ng}$ は型 `T` の上限・下限付近ではない
- (2)：型 `T` は実数

##### 計算量

$\mathrm{judge}(x)$ の実行にかかる時間を $T$ として

- (1)：$O(T \log \lvert \mathrm{ok} - \mathrm{ng} \rvert)$
- (2)：$O(T \cdot \mathrm{iteration\_count})$

#### expsearch

`T expsearch(F judge, T first = 0, bool positive = true)`

整数の指数探索。整数 `T` から `bool` への関数 $\mathrm{judge}$ は単調であり、$\mathrm{judge}$ の返り値が切り替わる境界（$\mathrm{judge}(\alpha) = \mathrm{true}, \mathrm{judge}(\beta) = \mathrm{false}, \lvert \alpha - \beta \rvert = 1$ を満たす唯一の整数組 $\alpha, \beta$）が

- $\mathrm{positive} = \mathrm{true}$ なら、$[\mathrm{first}, \infty)$ の範囲にある
- $\mathrm{positive} = \mathrm{false}$ なら、$(-\infty, \mathrm{first}]$ の範囲にある

とする。このとき、その $\alpha$ を返す。

最悪計算量のオーダーは二分探索と変わらない。二分探索と比べると、

- $\mathrm{ok}, \mathrm{ng}$ を詳細に見積もって指定する必要がない
- 答えが小さい場合に高速

である点が優れている。

ただし、判定関数内でオーバーフローしないように注意する必要はある。

##### 制約

- 型 `T` は整数
- $\mathrm{judge}$ は単調
- $\mathrm{positive} = \mathrm{true}$ なら、答えは $[\mathrm{first}, \infty)$ の範囲にある
- $\mathrm{positive} = \mathrm{false}$ なら、答えは $(-\infty, \mathrm{first}]$ の範囲にある
- 答えは `T` の範囲に収まる

##### 計算量

$\mathrm{judge}(x)$ の実行にかかる時間を $T$ として

- $O(T \log \lvert \mathrm{first} - \alpha \rvert)$
