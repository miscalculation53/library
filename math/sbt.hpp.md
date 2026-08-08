---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':question:'
    path: template/template_algo.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\
      \u30E0\uFF09"
  - icon: ':question:'
    path: template/template_all_but_modint.hpp
    title: template/template_all_but_modint.hpp
  - icon: ':question:'
    path: template/template_binsearch.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09"
  - icon: ':question:'
    path: template/template_bit.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\
      \uFF09"
  - icon: ':question:'
    path: template/template_dump.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
  - icon: ':question:'
    path: template/template_inout.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
  - icon: ':question:'
    path: template/template_math.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
  - icon: ':question:'
    path: template/template_random.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\u751F\
      \u6210\uFF09"
  - icon: ':question:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':question:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  - icon: ':question:'
    path: template/template_vector.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
  - icon: ':question:'
    path: utils/is_integral_ext.hpp
    title: "$128$ \u30D3\u30C3\u30C8\u6574\u6570\u3092\u542B\u3081\u305F\u6574\u6570\
      \u5224\u5B9A"
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':x:'
    path: verify/yosupo/rational_approximation.test.cpp
    title: verify/yosupo/rational_approximation.test.cpp
  - icon: ':x:'
    path: verify/yosupo/stern_brocot_tree.test.cpp
    title: verify/yosupo/stern_brocot_tree.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':x:'
  attributes:
    _deprecated_at_docs: docs/math/sbt.md
    document_title: Stern Brocot Tree
    links:
    - https://atcoder.jp/contests/abc408/tasks/abc408_g/editorial
  bundledCode: "#line 2 \"math/sbt.hpp\"\n\n#line 2 \"template/template_all_but_modint.hpp\"\
    \n\n#line 2 \"template/template_types.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n * @docs docs/template/template_types.md\n\
    \ */\n\n#include <bits/stdc++.h>\nusing namespace std;\n\n#ifndef EPS\n#define\
    \ EPS 1e-11\n#endif\nusing ld = decltype(EPS);\n\nusing ll = long long;\nusing\
    \ uint = unsigned int;\nusing ull = unsigned long long;\nusing pll = pair<ll,\
    \ ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll = tuple<ll, ll, ll, ll>;\n\
    \n#define vc vector\ntemplate <class T>\nusing vvc = vc<vc<T>>;\ntemplate <class\
    \ T>\nusing vvvc = vc<vc<vc<T>>>;\n\nusing vb = vc<bool>;\nusing vl = vc<ll>;\n\
    using vpll = vc<pll>;\nusing vtlll = vc<tlll>;\nusing vtllll = vc<tllll>;\nusing\
    \ vstr = vc<string>;\nusing vvb = vvc<bool>;\nusing vvl = vvc<ll>;\n\ntemplate\
    \ <class T>\nusing pql = priority_queue<T, vc<T>, greater<T>>;\ntemplate <class\
    \ T>\nusing pqg = priority_queue<T>;\n\n#ifdef __SIZEOF_INT128__\nusing i128 =\
    \ __int128_t;\nusing u128 = __uint128_t;\ni128 stoi128(const string &s)\n{\n \
    \ const bool neg = s.front() == '-';\n  u128 res = 0;\n  for (int i = neg; i <\
    \ (int)s.size(); i++)\n    res = 10 * res + s[i] - '0';\n  if (neg)\n    return\
    \ -i128(res - 1) - 1;\n  return i128(res);\n}\nstring i128tos(i128 x)\n{\n  if\
    \ (x == 0) return \"0\";\n  string sign = \"\", res = \"\";\n  u128 ux;\n  if\
    \ (x < 0)\n    ux = u128(-(x + 1)) + 1, sign = \"-\";\n  else\n    ux = x;\n \
    \ while (ux > 0)\n  {\n    res += '0' + ux % 10;\n    ux /= 10;\n  }\n  reverse(res.begin(),\
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
    #line 2 \"template/template_math.hpp\"\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n\
    #endif\n#ifndef EPS\n#define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\
    \n\n#line 2 \"utils/is_integral_ext.hpp\"\n\n#line 4 \"utils/is_integral_ext.hpp\"\
    \n\n/**\n * @brief $128$ \u30D3\u30C3\u30C8\u6574\u6570\u3092\u542B\u3081\u305F\
    \u6574\u6570\u5224\u5B9A\n * @docs docs/utils/is_integral_ext.md\n */\n\ntemplate\
    \ <class T>\nconstexpr bool is_integral_ext = is_integral_v<T> || is_same_v<T,\
    \ i128> || is_same_v<T, u128>;\n\ntemplate <class T>\nconstexpr bool is_signed_ext\
    \ = is_signed_v<T> || is_same_v<T, i128>;\n\ntemplate <class T>\nconstexpr bool\
    \ is_unsigned_ext = is_signed_v<T> || is_same_v<T, u128>;\n#line 14 \"template/template_math.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\
    \n * @docs docs/template/template_math.md\n */\n\ntemplate <class T, class U>\n\
    inline bool chmin(T &a, U b) { return a > b ? a = b, true : false; }\ntemplate\
    \ <class T, class U>\ninline bool chmax(T &a, U b) { return a < b ? a = b, true\
    \ : false; }\n\ntemplate <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U>\
    \ && is_integral_ext<V>>>\ninline constexpr T divfloor(U a, V b) { return T(a)\
    \ / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate <class T = ll, class\
    \ U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>\n\
    inline constexpr T divceil(U a, V b) { return T(a) / T(b) + (T(a) % T(b) && (T(a)\
    \ ^ T(b)) >= 0); }\ntemplate <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U>\
    \ && is_integral_ext<V>>>\ninline constexpr T divround(U a, V b) { return divfloor<T>(2\
    \ * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll, class U, class V, typename\
    \ = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>\ninline constexpr T\
    \ safemod(U a, V b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class\
    \ T = ll, class U, class V>\nconstexpr T ipow(U a, V b)\n{\n  assert(b >= 0);\n\
    \  if (b == 0)\n    return 1;\n  if (a == 0 || a == 1)\n    return a;\n  if (a\
    \ < 0 && a == -1)\n    return b & 1 ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while\
    \ (true)\n  {\n    if (b & 1)\n      res *= tmp;\n    b >>= 1;\n    if (b == 0)\n\
    \      break;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T = ll,\
    \ class A, class B, class M>\nT mul_limited(A a, B b, M m)\n{\n  assert(a >= 0\
    \ && b >= 0 && m >= 0);\n  if (b == 0)\n    return 0;\n  return T(a) > T(m) /\
    \ T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T = ll, class A, class B>\nT\
    \ mul_limited(A a, B b) { return mul_limited<T>(a, b, INF); }\ntemplate <class\
    \ T = ll, class A, class B, class M>\nT pow_limited(A a, B b, M m)\n{\n  assert(a\
    \ >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a,\
    \ b), T(m));\n  \n  T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n\
    \    {\n      if (res > T(m) / tmp)\n        return m;\n      res *= tmp;\n  \
    \  }\n    b >>= 1;\n    if (b == 0)\n      break;\n    if (tmp > T(m) / tmp)\n\
    \      return m;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T =\
    \ ll, class A, class B>\nT pow_limited(A a, B b) { return pow_limited<T>(a, b,\
    \ INF); }\n\ntemplate <class T = ll, class A, class K>\nconstexpr T iroot(A a,\
    \ K k)\n{\n  assert(a >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n    return a;\n\
    \  if (k == 2)\n  {\n    const T aa = T(a);\n    T x = T(sqrtl((long double)a));\n\
    \    while (x > aa / x)\n      x--;\n    while (x < numeric_limits<T>::max())\n\
    \    {\n      const T y = x + 1;\n      if (y > aa / y)\n        break;\n    \
    \  x = y;\n    }\n    return x;\n  }\n\n  auto isok = [&](T x) -> bool\n  {\n\
    \    if (x == 0)\n      return true;\n    T res = 1, k2 = k;\n    while (true)\n\
    \    {\n      if (k2 & 1)\n      {\n        if (res > T(a) / x)\n          return\
    \ false;\n        res *= x;\n      }\n      k2 >>= 1;\n      if (k2 == 0)\n  \
    \      break;\n      if (x > T(a) / x)\n        return false;\n      x *= x;\n\
    \    }\n    return res <= T(a);\n  };\n\n  T x = pow(a, 1.0 / k);\n  bool up =\
    \ true;\n  while (!isok(x))\n    up = false, x--;\n  if (up)\n  {\n    while (x\
    \ < numeric_limits<T>::max() && isok(x + 1))\n      x++;\n  }\n  return x;\n}\n\
    template <class T = ll, class A, class K>\nconstexpr T iroot_ceil(A a, K k)\n\
    {\n  T x = iroot<T>(a, k);\n  return ipow<T>(x, k) == a ? x : x + 1;\n}\n\n//\
    \ https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS), class A>\nint SGN(A a, D eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }\n\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\
    \u540C\u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\n// 0\
    \ \u306B\u5BFE\u3057\u3066\u306F {0} \u304C\u8FD4\u308B\ntemplate <class T = ll,\
    \ class U, class V>\nvc<T> base_repr(U val, V base)\n{\n  assert(val >= 0);\n\
    \  assert(base >= 2);\n  if (val == 0)\n    return {0};\n  vc<T> a;\n  while (val\
    \ > 0)\n  {\n    a.emplace_back(val % base);\n    val /= base;\n  }\n  reverse(a.begin(),\
    \ a.end());\n  return a;\n}\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\u540C\
    \u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate <class\
    \ T = ll, class U, class V>\nvc<T> base_repr(U val, V base, int n)\n{\n  assert(val\
    \ >= 0);\n  assert(base >= 2);\n  assert(n >= 0);\n  vc<T> a(n);\n  repi(i, n)\n\
    \  {\n    a[i] = val % base;\n    val /= base;\n  }\n  reverse(a.begin(), a.end());\n\
    \  return a;\n}\ntemplate <const bool use_upper = true, class U>\nstring base_repr_str(U\
    \ val, int base)\n{\n  assert(val >= 0);\n  assert(2 <= base && base <= 36);\n\
    \  auto a = base_repr(val, base);\n  string s = \"\";\n  for (cauto &ai : a)\n\
    \    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return\
    \ s;\n}\ntemplate <const bool use_upper = true, class U>\nstring base_repr_str(U\
    \ val, int base, int n)\n{\n  assert(val >= 0);\n  assert(2 <= base && base <=\
    \ 36);\n  assert(n >= 0);\n  auto a = base_repr(val, base, n);\n  string s = \"\
    \";\n  for (cauto &ai : a)\n    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' :\
    \ 'a') + (ai - 10));\n  return s;\n}\n#line 2 \"template/template_vector.hpp\"\
    \n\n#line 6 \"template/template_vector.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08vector\uFF09\n * @docs docs/template/template_vector.md\n\
    \ */\n\n#define ALL(a) (a).begin(), (a).end()\ntemplate <class T = ll, class V>\n\
    inline T SZ(const V &x) { return x.size(); }\n#define eb emplace_back\n\n#define\
    \ LMD(x, fx) ([&](auto x) { return fx; })\ntemplate <class F>\nauto gen_vec(int\
    \ n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n  repi(i, n) res[i] = f(i);\n\
    \  return res;\n}\n#define GEN_VEC(n, i, fi) (gen_vec(n, LMD(i, fi)))\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
    template <class T, size_t d, size_t i = 0, class V>\nauto dvec(const V (&sz)[d],\
    \ const T &init)\n{\n  if constexpr (i < d)\n    return vc(sz[i], dvec<T, d, i\
    \ + 1>(sz, init));\n  else\n    return init;\n}\n\ntemplate <class T = ll>\nT\
    \ ctol(const char &c, const string &s)\n{\n  repi(i, SZ<int>(s)) if (s[i] == c)\
    \ return i;\n  return -1;\n}\ntemplate <class T = ll>\nvc<T> stov(const string\
    \ &s, char first)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n         \
    \        { return s[i] - first; });\n}\ntemplate <class T = ll>\nvc<T> stov(const\
    \ string &s, const string &t)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n\
    \                 { return ctol(s[i], t); });\n}\ntemplate <class T>\nstring vtos(const\
    \ vc<T> &v, char first)\n{\n  string res = \"\";\n  fe(vi : v) res += vi + first;\n\
    \  return res;\n}\ntemplate <class T>\nstring vtos(const vc<T> &v, const string\
    \ &t)\n{\n  string res = \"\";\n  fe(vi : v) res += t[vi];\n  return res;\n}\n\
    \ntemplate <class T>\nvc<T> concat(const vvc<T> &vs)\n{\n  vc<T> res;\n  for (cauto\
    \ &v : vs)\n    res.insert(res.end(), ALL(v));\n  return res;\n}\ntemplate <class\
    \ T>\nvc<T> concat(const vc<T> &v) { return v; }\ntemplate <class T, class...\
    \ Ts>\nvc<T> concat(vc<T> v, const vc<Ts> &...vs)\n{\n  (v.insert(v.end(), ALL(vs)),\
    \ ...);\n  return v;\n}\n\ntemplate <class T>\nvc<T> merged(const vc<T> &a, const\
    \ vc<T> &b)\n{\n  vc<T> res;\n  merge(ALL(a), ALL(b), back_inserter(res));\n \
    \ return res;\n}\n\ntemplate <class T, class I>\nT vecget(const vc<T> &v, I i,\
    \ const T &dflt_negative = -INF, const T &dflt_positive = INF)\n{\n  if (i < 0)\n\
    \    return dflt_negative;\n  if (i >= SZ<int>(v))\n    return dflt_positive;\n\
    \  return v[i];\n}\n#line 2 \"template/template_algo.hpp\"\n\n#ifndef INF\n#define\
    \ INF 4'000'000'000'000'000'037LL\n#endif\n\n#line 11 \"template/template_algo.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\
    \u30EA\u30BA\u30E0\uFF09\n * @docs docs/template/template_algo.md\n */\n\ntemplate\
    \ <class V>\nauto SUM(const V &v)\n{\n  typename V::value_type s{};\n  fec(vi\
    \ : v) s += vi;\n  return s;\n}\ntemplate <class T, class V>\nT SUM(const V &v)\n\
    {\n  T s{};\n  fec(vi : v) s += vi;\n  return s;\n}\ntemplate <class V>\nauto\
    \ MAX(const V &v) { return *max_element(ALL(v)); }\ntemplate <class V>\nauto MIN(const\
    \ V &v) { return *min_element(ALL(v)); }\ntemplate <class I = ll, class V>\nI\
    \ ARGMAX(const V &v) { return max_element(ALL(v)) - v.begin(); }\ntemplate <class\
    \ I = ll, class V>\nI ARGMIN(const V &v) { return min_element(ALL(v)) - v.begin();\
    \ }\n\ntemplate<class T = ll, class V>\nT mex(const V &a)\n{\n  int n = a.size();\n\
    \  vector<bool> exists(n, false);\n  repi(i, n) if (0 <= a[i] && a[i] < n) exists[a[i]]\
    \ = true;\n  repi(x, n) if (!exists[x]) return x;\n  return n;\n}\n\n// (0, 1.\
    \ ..., n-1) \u306E\u9806\u5217\u304B\u5224\u5B9A\ntemplate <class I>\nbool is_permutation(const\
    \ vc<I> &p)\n{\n  const int n = p.size();\n  vc<bool> b(n, false);\n  repi(i,\
    \ n)\n  {\n    if (!(0 <= p[i] && p[i] < n))\n      return false;\n    b[p[i]]\
    \ = true;\n  }\n  return all_of(ALL(b), [](bool bi)\n                { return\
    \ bi; });\n}\n\ntemplate <class T = ll>\nvc<T> permid(const int &n, const int\
    \ &base_index = 0)\n{\n  vc<T> p(n);\n  repi(i, n) p[i] = i + base_index;\n  return\
    \ p;\n}\ntemplate <class T>\nvc<T> perminv(const vc<T> &p)\n{\n  if (p.empty())\n\
    \    return {};\n  const int n = p.size();\n  vc<T> q(MAX(p) + 1);\n  repi(i,\
    \ n) if (p[i] >= 0) q[p[i]] = i;\n  return q;\n}\n// a[p[i]] for all i\ntemplate\
    \ <class T, class U>\nvc<T> permuted(const vc<T> &a, const vc<U> &p)\n{\n  const\
    \ int n = p.size();\n  vc<T> res(n);\n  repi(i, n)\n  {\n    assert(0 <= p[i]\
    \ && p[i] < U(a.size()));\n    res[i] = a[p[i]];\n  }\n  return res;\n}\n// p[q[r[i]]]\
    \ for all i \u306A\u3069\ntemplate <class T, class U, class... Ts>\nvc<T> permuted(const\
    \ vc<T> &p, const vc<U> &q, const vc<Ts> &...rs)\n{\n  return permuted(permuted(p,\
    \ q), rs...);\n}\n\ntemplate <class V>\nV reversed(const V &v) { return V(v.rbegin(),\
    \ v.rend()); }\n\n#if __cplusplus < 202002L\ntemplate <class V, class... Args>\n\
    V sorted(V v, Args&&... args)\n{\n  sort(ALL(v), forward<Args>(args)...);\n  return\
    \ v;\n}\n#else\ntemplate <class V, class... Args>\nV sorted(V v, Args&&... args)\n\
    {\n  ranges::sort(v, forward<Args>(args)...);\n  return v;\n}\n#endif\n\ntemplate\
    \ <class V>\nvoid unique(V &v) { v.erase(std::unique(ALL(v)), v.end()); }\ntemplate\
    \ <class V>\nV uniqued(V v) { unique(v); return v; }\n\ntemplate <class V>\nvoid\
    \ sortunique(V &v)\n{\n  sort(ALL(v));\n  unique(v);\n}\ntemplate <class V>\n\
    V sortuniqued(V v) { sortunique(v); return v; }\n\n// 01234 -> 12340\ntemplate\
    \ <class V, class U>\nvoid rotate(V &v, U k)\n{ \n  const U n = v.size();\n  if\
    \ (n == 0)\n    return;\n  k = (k % n + n) % n;\n  std::rotate(v.begin(), v.begin()\
    \ + k, v.end());\n}\n// 01234 -> 12340\ntemplate <class V, class U>\nV rotated(V\
    \ v, U k) { rotate(v, k); return v; }\n\ntemplate <class T>\nvvc<T> top(const\
    \ vvc<T> &a)\n{\n  if (a.empty())\n    return {};\n  const int n = a.size(), m\
    \ = a[0].size();\n  vvc<T> b(m, vc<T>(n));\n  repi(i, n)\n  {\n    assert(SZ<int>(a[i])\
    \ == m);\n    repi(j, m) b[j][i] = a[i][j];\n  }\n  return b;\n}\nvstr top(const\
    \ vstr &a)\n{\n  vvc<char> a_(a.size());\n  repi(i, SZ<int>(a)) a_[i] = {ALL(a[i])};\n\
    \  vvc<char> b_ = top(a_);\n  vstr b(b_.size());\n  repi(i, SZ<int>(b)) b[i] =\
    \ {ALL(b_[i])};\n  return b;\n}\n\ntemplate <class T, class = void>\nstruct has_e0\
    \ : false_type {};\ntemplate <class T>\nstruct has_e0<T, void_t<decltype(T::e0())>>\
    \ : true_type {};\ntemplate <class T>\ninline constexpr bool has_e0_v = has_e0<T>::value;\n\
    \ntemplate <class T>\nstruct MonoidAdd\n{\n  using S = T;\n  static constexpr\
    \ S op(S a, S b) { return a + b; }\n  static constexpr S e()\n  {\n    if constexpr\
    \ (has_e0_v<S>)\n      return S::e0();\n    else\n      return {};\n  }\n  template\
    \ <class I, class = decltype(declval<S>() * declval<I>())>\n  static constexpr\
    \ S pow(const S &a, I k) { return a * k; }\n};\ntemplate <class T, const T infty\
    \ = INF>\nstruct MonoidMin\n{\n  using S = T;\n  static constexpr S op(S a, S\
    \ b) { return min(a, b); }\n  static constexpr S e() { return infty; }\n  template\
    \ <class I>\n  static constexpr S pow(const S &a, I k) { return k == 0 ? e() :\
    \ a; }\n};\ntemplate <class T, const T infty = INF>\nstruct MonoidMax\n{\n  using\
    \ S = T;\n  static constexpr S op(S a, S b) { return max(a, b); }\n  static constexpr\
    \ S e() { return -infty; }\n  template <class I>\n  static constexpr S pow(const\
    \ S &a, I k) { return k == 0 ? e() : a; }\n};\n\nnamespace internal\n{\n  template\
    \ <class M, class I, class = void>\n  struct HasMonoidPow : false_type\n  {\n\
    \  };\n  template <class M, class I>\n  struct HasMonoidPow<M, I, void_t<decltype(M::pow(declval<const\
    \ typename M::S &>(), declval<I>()))>> : true_type\n  {\n  };\n}\n\ntemplate <class\
    \ M, class I>\ntypename M::S pow_monoid(typename M::S a, I k)\n{\n  if constexpr\
    \ (is_signed_ext<I>)\n    assert(k >= 0);\n  if constexpr (internal::HasMonoidPow<M,\
    \ I>::value)\n    return M::pow(a, k);\n  else\n  {\n    typename M::S c = M::e();\n\
    \    for (; k; k >>= 1)\n    {\n      if (k & 1)\n        c = M::op(c, a);\n \
    \     a = M::op(a, a);\n    }\n    return c;\n  }\n}\n\ntemplate <class G, class\
    \ I>\ntypename G::S pow_group(typename G::S a, I k)\n{\n  if constexpr (is_signed_ext<I>)\n\
    \  {\n    if (k < 0)\n    {\n      a = G::inv(a);\n      return G::op(pow_monoid<G>(a,\
    \ -(k + 1)), a);\n    }\n  }\n  return pow_monoid<G>(a, k);\n}\n\n// left_index\
    \ \u304C 0 \u306A\u3089\u3001\u9577\u3055 n+1 \u3067 a.front() \u304C e()\n//\
    \ left_index \u304C 1 \u306A\u3089\u3001\u9577\u3055 n \u3067 e() \u304C\u306A\
    \u3044\ntemplate <class M>\nvc<typename M::S> cuml(const vc<typename M::S> &v,\
    \ int left_index = 0)\n{\n  const int n = v.size();\n  vc<typename M::S> res(n\
    \ + 1);\n  res[0] = M::e();\n  repi(i, n) res[i + 1] = M::op(res[i], v[i]);\n\
    \  res.erase(res.begin(), res.begin() + left_index);\n  return res;\n}\n// right_index\
    \ \u304C 0 \u306A\u3089\u3001\u9577\u3055 n+1 \u3067 a.back() \u304C e()\n// right_index\
    \ \u304C 1 \u306A\u3089\u3001\u9577\u3055 n \u3067 e() \u304C\u306A\u3044\ntemplate\
    \ <class M>\nvc<typename M::S> cumr(const vc<typename M::S> &v, int right_index\
    \ = 0)\n{ return reversed(cuml<M>(reversed(v), right_index)); }\ntemplate <class\
    \ T>\nvc<T> cumlsum(const vc<T> &v, int left_index = 0)\n{ return cuml<MonoidAdd<T>>(v,\
    \ left_index); }\ntemplate <class T>\nvc<T> cumrsum(const vc<T> &v, int right_index\
    \ = 0)\n{ return cumr<MonoidAdd<T>>(v, right_index); }\ntemplate <class T>\nvc<T>\
    \ cumlmin(const vc<T> &v, int left_index = 0)\n{ return cuml<MonoidMin<T>>(v,\
    \ left_index); }\ntemplate <class T>\nvc<T> cumrmin(const vc<T> &v, int right_index\
    \ = 0)\n{ return cumr<MonoidMin<T>>(v, right_index); }\ntemplate <class T>\nvc<T>\
    \ cumlmax(const vc<T> &v, int left_index = 0)\n{ return cuml<MonoidMax<T>>(v,\
    \ left_index); }\ntemplate <class T>\nvc<T> cumrmax(const vc<T> &v, int right_index\
    \ = 0)\n{ return cumr<MonoidMax<T>>(v, right_index); }\n\n// \u30C7\u30D5\u30A9\
    \u30EB\u30C8\u3067\u306F\u9577\u3055 n+1\n// left_index, right_index \u3092\u305D\
    \u308C\u305E\u308C 1 \u306B\u3059\u308B\u3068\u3001\u5DE6\u53F3\u304C\u524A\u9664\
    \u3055\u308C\u308B\ntemplate <class T>\nvc<T> adjd(const vc<T> &v, int left_index\
    \ = 0, int right_index = 0)\n{\n  int n = v.size();\n  assert(0 <= left_index\
    \ && 0 <= right_index && left_index + right_index <= n + 1);\n  vc<T> res(n +\
    \ 1);\n  if (n == 0)\n  {\n    res[0] = T{};\n    res.erase(res.end() - right_index,\
    \ res.end());\n    res.erase(res.begin(), res.begin() + left_index);\n    return\
    \ res;\n  }\n  res[0] = v[0];\n  repi(i, 1, n) res[i] = v[i] - v[i - 1];\n  res[n]\
    \ = -v[n - 1];\n  res.erase(res.end() - right_index, res.end());\n  res.erase(res.begin(),\
    \ res.begin() + left_index);\n  return res;\n}\n\nconstexpr array<pll, 4> DRULgrid\
    \ = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};\nconstexpr array<pll, 4> DRULplane =\
    \ {{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};\n#line 2 \"template/template_binsearch.hpp\"\
    \n\n#line 6 \"template/template_binsearch.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09\n * @docs docs/template/template_binsearch.md\n\
    \ */\n\ntemplate <class T>\nstruct is_random_access_iterator\n{\n  static constexpr\
    \ bool value = is_same_v<\n    typename iterator_traits<T>::iterator_category,\n\
    \    random_access_iterator_tag\n  >;\n};\ntemplate <class T>\nconstexpr bool\
    \ is_random_access_iterator_v = is_random_access_iterator<T>::value;\n\n// ---\
    \ LB, UB ---\n\n#if __cplusplus < 202002L\nstruct identity\n{\n  template <class\
    \ T>\n  constexpr T &&operator()(T &&t) const noexcept\n  { return forward<T>(t);\
    \ }\n};\nnamespace internal\n{\n  template <class T = ll, class V, class Judge>\n\
    \  inline T bound_helper(const V &v, Judge judge)\n  {\n    int l = -1, r = v.size();\n\
    \    while (r - l > 1)\n    {\n      int m = (l + r) / 2;\n      if (judge(m))\n\
    \        l = m;\n      else\n        r = m;\n    }\n    return r;\n  }\n};\n//\
    \ val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u672A\u6E80\u306E\u5024\
    \u306E\u500B\u6570)\ntemplate <class T = ll, class V, class Value, class Comp\
    \ = less<>, class Proj = identity>\ninline T LB(const V &v, const Value &val,\
    \ Comp comp = {}, Proj proj = {})\n{\n  return internal::bound_helper(v, [&](int\
    \ i) -> bool\n                                { return comp(proj(*(v.begin() +\
    \ i)), val); });\n}\n// val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val\
    \ \u4EE5\u4E0B\u306E\u5024\u306E\u500B\u6570)\ntemplate <class T = ll, class V,\
    \ class Value, class Comp = less<>, class Proj = identity>\ninline T UB(const\
    \ V &v, const Value &val, Comp comp = {}, Proj proj = {})\n{\n  return internal::bound_helper(v,\
    \ [&](int i) -> bool\n                                { return !comp(val, proj(*(v.begin()\
    \ + i))); });\n}\n#define DEFAULT_COMP less<>\n#else\n// val <= v[i] \u3068\u306A\
    \u308B\u6700\u5C0F\u306E i (val \u672A\u6E80\u306E\u5024\u306E\u500B\u6570)\n\
    template <class T = ll, class V, class Value, class Comp = ranges::less, class\
    \ Proj = identity>\ninline T LB(const V &v, const Value &val, Comp comp = {},\
    \ Proj proj = {})\n{ return ranges::lower_bound(v, val, comp, proj) - v.begin();\
    \ }\n// val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\u4E0B\u306E\
    \u5024\u306E\u500B\u6570)\ntemplate <class T = ll, class V, class Value, class\
    \ Comp = ranges::less, class Proj = identity>\ninline T UB(const V &v, const Value\
    \ &val, Comp comp = {}, Proj proj = {})\n{ return ranges::upper_bound(v, val,\
    \ comp, proj) - v.begin(); }\n#define DEFAULT_COMP ranges::less\n#endif\n\n//\
    \ --- vector \u7B49\u306E lt, leq, gt, geq ---\n\n// v[i] < val \u3068\u306A\u308B\
    \u6700\u5927\u306E i (\u306A\u3051\u308C\u3070 -1)\ntemplate <class T = ll, class\
    \ V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto\
    \ lt_max(const V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return LB<T>(v, val, comp, proj) - 1; }\n// v[i] <= val\
    \ \u3068\u306A\u308B\u6700\u5927\u306E i (\u306A\u3051\u308C\u3070 -1)\ntemplate\
    \ <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj =\
    \ identity>\ninline auto leq_max(const V &v, const Value &val, Comp comp = {},\
    \ Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename V::iterator>,\
    \ T>\n{ return UB<T>(v, val, comp, proj) - 1; }\n// val < v[i] \u3068\u306A\u308B\
    \u6700\u5C0F\u306E i (\u306A\u3051\u308C\u3070 n)\ntemplate <class T = ll, class\
    \ V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto\
    \ gt_min(const V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return UB<T>(v, val, comp, proj); }\n// val <= v[i] \u3068\
    \u306A\u308B\u6700\u5C0F\u306E i (\u306A\u3051\u308C\u3070 n)\ntemplate <class\
    \ T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>\n\
    inline auto geq_min(const V &v, const Value &val, Comp comp = {}, Proj proj =\
    \ {})\n-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>\n\
    { return LB<T>(v, val, comp, proj); }\n// v[i] < val \u3068\u306A\u308B i \u306E\
    \u500B\u6570\ntemplate <class T = ll, class V, class Value, class Comp = DEFAULT_COMP,\
    \ class Proj = identity>\ninline auto lt_cnt(const V &v, const Value &val, Comp\
    \ comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return LB<T>(v, val, comp, proj); }\n// v[i] <= val \u3068\
    \u306A\u308B i \u306E\u500B\u6570\ntemplate <class T = ll, class V, class Value,\
    \ class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto leq_cnt(const\
    \ V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return UB<T>(v, val, comp, proj); }\n// val < v[i] \u3068\
    \u306A\u308B i \u306E\u500B\u6570\ntemplate <class T = ll, class V, class Value,\
    \ class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto gt_cnt(const\
    \ V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return SZ<T>(v) - UB<T>(v, val, comp, proj); }\n// val <=\
    \ v[i] \u3068\u306A\u308B i \u306E\u500B\u6570\ntemplate <class T = ll, class\
    \ V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto\
    \ geq_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return SZ<T>(v) - LB<T>(v, val, comp, proj); }\n// l <=\
    \ v[i] < r \u3068\u306A\u308B i \u306E\u500B\u6570\ntemplate <class T = ll, class\
    \ V, class L, class R, class Comp = DEFAULT_COMP, class Proj = identity>\ninline\
    \ auto in_cnt(const V &v, L l, R r, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{\n  if (l > r)\n    return 0;\n  return lt_cnt<T>(v, r, comp,\
    \ proj) - lt_cnt<T>(v, l, comp, proj);\n}\n\n// --- set \u7B49\u306E lt, leq,\
    \ gt, geq ---\n\n// *it < val \u3068\u306A\u308B\u6700\u5927\u306E it (\u306A\u3051\
    \u308C\u3070 end())\ntemplate <class V, class Value>\ninline auto lt_max(const\
    \ V &v, const Value &val)\n-> enable_if_t<!is_random_access_iterator_v<typename\
    \ V::iterator>, typename V::const_iterator>\n{\n  auto it = v.lower_bound(val);\n\
    \  return it == v.begin() ? v.end() : prev(it);\n}\n// *it <= val \u3068\u306A\
    \u308B\u6700\u5927\u306E it (\u306A\u3051\u308C\u3070 end())\ntemplate <class\
    \ V, class Value>\ninline auto leq_max(const V &v, const Value &val)\n-> enable_if_t<!is_random_access_iterator_v<typename\
    \ V::iterator>, typename V::const_iterator>\n{\n  auto it = v.upper_bound(val);\n\
    \  return it == v.begin() ? v.end() : prev(it);\n}\n// val < *it \u3068\u306A\u308B\
    \u6700\u5C0F\u306E it (\u306A\u3051\u308C\u3070 end())\ntemplate <class V, class\
    \ Value>\ninline auto gt_min(const V &v, const Value &val)\n-> enable_if_t<!is_random_access_iterator_v<typename\
    \ V::iterator>, typename V::const_iterator>\n{ return v.upper_bound(val); }\n\
    // val <= *it \u3068\u306A\u308B\u6700\u5C0F\u306E it (\u306A\u3051\u308C\u3070\
    \ end())\ntemplate <class V, class Value>\ninline auto geq_min(const V &v, const\
    \ Value &val)\n-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>,\
    \ typename V::const_iterator>\n{ return v.lower_bound(val); }\n\n// --- \u81EA\
    \u4F5C\u4E8C\u5206\u63A2\u7D22 ---\n\nnamespace internal\n{\ntemplate <class T>\n\
    bool binsearch_adjacent(T a, T b)\n{\n  if (a < b)\n    return a + 1 == b;\n \
    \ if (b < a)\n    return b + 1 == a;\n  return false;\n}\n};\n\n// (ok, ng)\n\
    template <class T = ll, class Judge, class InitOk, class InitNg>\npair<T, T> binsearch(const\
    \ Judge &judge, InitOk init_ok, InitNg init_ng, bool check_ok = true, bool check_ng\
    \ = true)\n{\n  T ok(init_ok), ng(init_ng);\n  if (check_ok)\n    assert(judge(ok));\n\
    \  if (check_ng)\n    assert(!judge(ng));\n  while (!internal::binsearch_adjacent(ok,\
    \ ng))\n  {\n    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid) ? ok :\
    \ ng) = mid;\n  }\n  return {ok, ng};\n}\ntemplate <class T = ld, class Judge,\
    \ class InitOk, class InitNg>\nT binsearch_real(const Judge &judge, InitOk init_ok,\
    \ InitNg init_ng, int iteration_count = 100, bool check_ok = true, bool check_ng\
    \ = true)\n{\n  T ok(init_ok), ng(init_ng);\n  if (check_ok)\n    assert(judge(ok));\n\
    \  if (check_ng)\n    assert(!judge(ng));\n  repi(_, iteration_count)\n  {\n \
    \   T mid = (ok + ng) / 2;\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return ok;\n\
    }\n// (ok, ng)\ntemplate <class T = ll, class Judge, class InitVal>\npair<T, T>\
    \ expsearch(const Judge &judge, InitVal init_val, bool positive = true)\n{\n \
    \ T cur(init_val), step = 1;\n  const bool cur_ok = judge(cur);\n  auto advance\
    \ = [&](T x, T d, bool pos) -> T\n  {\n    if (pos)\n      return x > numeric_limits<T>::max()\
    \ - d ? numeric_limits<T>::max() : x + d;\n    else\n      return x < numeric_limits<T>::lowest()\
    \ + d ? numeric_limits<T>::lowest() : x - d;\n  };\n  T prv = advance(cur, 1,\
    \ !positive);\n  if (prv != cur && judge(prv) != cur_ok)\n  {\n    if (cur_ok)\n\
    \      return {cur, prv};\n    else\n      return {prv, cur};\n  }\n  while (true)\n\
    \  {\n    T nxt = advance(cur, step, positive);\n    assert(nxt != cur && \"the\
    \ boundary must exist in the searched direction\");\n    if (nxt == cur || judge(nxt)\
    \ != cur_ok)\n    {\n      T ok = cur_ok ? cur : nxt;\n      T ng = cur_ok ? nxt\
    \ : cur;\n      return binsearch<T>(judge, ok, ng, false, false);\n    }\n   \
    \ cur = nxt;\n    if (step > numeric_limits<T>::max() / 2)\n      step = numeric_limits<T>::max();\n\
    \    else\n      step *= 2;\n  }\n}\n#line 2 \"template/template_bit.hpp\"\n\n\
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
    \ y) { return x & ~y; }\n#line 2 \"template/template_inout.hpp\"\n\n#line 2 \"\
    template/template_dump.hpp\"\n\n#line 4 \"template/template_dump.hpp\"\n\n/**\n\
    \ * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n\
    \ */\n\n#ifdef LOCAL\n#include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
    namespace cpp_dump::_detail\n{\n  inline string export_var(\n      const i128\
    \ &x, const string &indent, size_t last_line_length,\n      size_t current_depth,\
    \ bool fail_on_newline, const export_command &command\n  ) {\n    return export_var(i128tos(x),\
    \ indent, last_line_length, current_depth, fail_on_newline, command);\n  }\n}\
    \ // namespace cpp_dump::_detail\n#define dump(...) cpp_dump(__VA_ARGS__)\nnamespace\
    \ cp = cpp_dump;\nCPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\n\
    CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 100);\n#define local(...) __VA_ARGS__\n\
    #define oj(...)\n#define local_oj(a, b) (a)\nCPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(content());\n\
    #else\n#define dump(...)\n#define local(...)\n#define oj(...) __VA_ARGS__\n#define\
    \ local_oj(a, b) (b)\n#endif\n\ntemplate <class T, class Sequence>\nvc<T> content(queue<T,\
    \ Sequence> que)\n{\n  vc<T> res;\n  while (!que.empty())\n  {\n    res.eb(que.front());\n\
    \    que.pop();\n  }\n  return res;\n}\ntemplate <class T, class Sequence, class\
    \ Compare>\nvc<T> content(priority_queue<T, Sequence, Compare> pque)\n{\n  vc<T>\
    \ res;\n  while (!pque.empty())\n  {\n    res.eb(pque.top());\n    pque.pop();\n\
    \  }\n  return res;\n}\ntemplate <class T>\nauto content(const T &obj) { return\
    \ obj.content(); }\n#line 6 \"template/template_inout.hpp\"\n\n/**\n * @brief\
    \ \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09\n * @docs\
    \ docs/template/template_inout.md\n */\n\n// https://judge.yosupo.jp/submission/170706\
    \ (maspy \u3055\u3093)\n// https://judge.yosupo.jp/submission/21623  (Nyaan \u3055\
    \u3093)\n#if defined FAST_IO and not defined LOCAL\nnamespace fastio {\ntemplate\
    \ <class T>\nstruct unsigned_integer\n{\n  using type = make_unsigned_t<T>;\n\
    };\ntemplate <>\nstruct unsigned_integer<i128>\n{\n  using type = u128;\n};\n\
    template <>\nstruct unsigned_integer<u128>\n{\n  using type = u128;\n};\ntemplate\
    \ <class T>\nusing unsigned_integer_t = typename unsigned_integer<T>::type;\n\n\
    static constexpr uint32_t SIZ = 1 << 17;\nchar ibuf[SIZ];\nchar obuf[SIZ];\nchar\
    \ out[100];\n// pointer of ibuf, obuf\nuint32_t pil = 0, pir = 0, por = 0;\n\n\
    struct Pre {\n  char num[10000][4];\n  constexpr Pre() : num() {\n    for (int\
    \ i = 0; i < 10000; i++) {\n      int n = i;\n      for (int j = 3; j >= 0; j--)\
    \ {\n        num[i][j] = n % 10 | '0';\n        n /= 10;\n      }\n    }\n  }\n\
    } constexpr pre;\n\ninline void load() {\n  memcpy(ibuf, ibuf + pil, pir - pil);\n\
    \  pir = pir - pil + fread(ibuf + pir - pil, 1, SIZ - pir + pil, stdin);\n  pil\
    \ = 0;\n  if (pir < SIZ) ibuf[pir++] = '\\n';\n}\n\ninline void flush() {\n  fwrite(obuf,\
    \ 1, por, stdout);\n  por = 0;\n}\n\nvoid rd1(char &c) {\n  do {\n    if (pil\
    \ + 1 > pir) load();\n    c = ibuf[pil++];\n  } while (isspace(c));\n}\n\nvoid\
    \ rd1(string &x) {\n  x.clear();\n  char c;\n  do {\n    if (pil + 1 > pir) load();\n\
    \    c = ibuf[pil++];\n  } while (isspace(c));\n  do {\n    x += c;\n    if (pil\
    \ == pir) load();\n    c = ibuf[pil++];\n  } while (!isspace(c));\n}\n\ntemplate\
    \ <typename T>\nvoid rd1_real(T &x) {\n  string s;\n  rd1(s);\n  if constexpr\
    \ (is_same_v<T, long double>)\n    x = stold(s);\n  else\n    x = stod(s);\n}\n\
    \ntemplate <typename T>\nvoid rd1_integer(T &x) {\n  if (pil + 100 > pir) load();\n\
    \  char c;\n  do\n    c = ibuf[pil++];\n  while (c < '-');\n  bool minus = 0;\n\
    \  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {\n    if (c == '-')\
    \ { minus = 1, c = ibuf[pil++]; }\n  }\n  using U = unsigned_integer_t<T>;\n \
    \ U val = 0;\n  while ('0' <= c) { val = val * 10 + (c & 15), c = ibuf[pil++];\
    \ }\n  pil--;\n  if constexpr (is_signed<T>::value || is_same_v<T, i128>)\n  {\n\
    \    if (minus)\n    {\n      const U min_abs = U(numeric_limits<T>::max()) +\
    \ 1;\n      assert(val <= min_abs);\n      x = val == min_abs ? numeric_limits<T>::lowest()\
    \ : -T(val);\n    }\n    else\n    {\n      assert(val <= U(numeric_limits<T>::max()));\n\
    \      x = T(val);\n    }\n  }\n  else\n    x = T(val);\n}\n\nvoid rd1(int &x)\
    \ { rd1_integer(x); }\nvoid rd1(ll &x) { rd1_integer(x); }\nvoid rd1(i128 &x)\
    \ { rd1_integer(x); }\nvoid rd1(uint &x) { rd1_integer(x); }\nvoid rd1(ull &x)\
    \ { rd1_integer(x); }\nvoid rd1(u128 &x) { rd1_integer(x); }\nvoid rd1(double\
    \ &x) { rd1_real(x); }\nvoid rd1(long double &x) { rd1_real(x); }\n// void rd1(f128\
    \ &x) { rd1_real(x); }\n\ntemplate <class T, class U>\nvoid rd1(pair<T, U> &p)\
    \ {\n  return rd1(p.first), rd1(p.second);\n}\ntemplate <class... T>\nvoid rd1(tuple<T...>\
    \ &tpl) {\n  apply([](auto &...x) { (rd1(x), ...); }, tpl);\n}\n\ntemplate <size_t\
    \ N = 0, typename T>\nvoid rd1(array<T, N> &x) {\n  for (auto &d: x) rd1(d);\n\
    }\ntemplate <class T>\nvoid rd1(vc<T> &x) {\n  for (auto &d: x) rd1(d);\n}\n\n\
    template <class... T>\nvoid read(T &...x) {\n  (rd1(x), ...);\n}\n\nvoid wt1(const\
    \ char c) {\n  if (por == SIZ) flush();\n  obuf[por++] = c;\n}\nvoid wt1(const\
    \ string s) {\n  for (char c: s) wt1(c);\n}\nvoid wt1(const char *s) {\n  size_t\
    \ len = strlen(s);\n  for (size_t i = 0; i < len; i++) wt1(s[i]);\n}\n\ntemplate\
    \ <typename T>\nvoid wt1_integer(T x) {\n  if (por > SIZ - 100) flush();\n  using\
    \ U = unsigned_integer_t<T>;\n  U ux;\n  if constexpr (is_signed<T>::value ||\
    \ is_same_v<T, i128>)\n  {\n    if (x < 0)\n      obuf[por++] = '-', ux = U(0)\
    \ - U(x);\n    else\n      ux = U(x);\n  }\n  else\n    ux = x;\n  int outi;\n\
    \  for (outi = 96; ux >= 10000; outi -= 4) {\n    memcpy(out + outi, pre.num[ux\
    \ % 10000], 4);\n    ux /= 10000;\n  }\n  if (ux >= 1000) {\n    memcpy(obuf +\
    \ por, pre.num[ux], 4);\n    por += 4;\n  } else if (ux >= 100) {\n    memcpy(obuf\
    \ + por, pre.num[ux] + 1, 3);\n    por += 3;\n  } else if (ux >= 10) {\n    int\
    \ q = (ux * 103) >> 10;\n    obuf[por] = q | '0';\n    obuf[por + 1] = (ux - q\
    \ * 10) | '0';\n    por += 2;\n  } else\n    obuf[por++] = ux | '0';\n  memcpy(obuf\
    \ + por, out + outi + 4, 96 - outi);\n  por += 96 - outi;\n}\n\ntemplate <typename\
    \ T>\nvoid wt1_real(T x) {\n  ostringstream oss;\n  oss << fixed << setprecision(15)\
    \ << x;\n  string s = oss.str();\n  wt1(s);\n}\n\nvoid wt1(int x) { wt1_integer(x);\
    \ }\ntemplate <class T, enable_if_t<is_integral_v<T>, int> = 0>\nvoid wt1(T x)\
    \ { wt1_integer(x); }\nvoid wt1(i128 x) { wt1_integer(x); }\nvoid wt1(u128 x)\
    \ { wt1_integer(x); }\nvoid wt1(double x) { wt1_real(x); }\nvoid wt1(long double\
    \ x) { wt1_real(x); }\n// void wt1(f128 x) { wt1_real(x); }\n\ntemplate <class\
    \ T, class U>\nvoid wt1(const pair<T, U> &val) {\n  wt1(val.first);\n  wt1(' ');\n\
    \  wt1(val.second);\n}\ntemplate <class... T>\nvoid wt1(const tuple<T...> &tpl)\
    \ {\n  if constexpr (sizeof...(T))\n  {\n    int i = 0;\n    apply([&](const auto\
    \ &...x)\n          { ((i++ ? wt1(' ') : void(), wt1(x)), ...); }, tpl);\n  }\n\
    }\ntemplate <class T, size_t S>\nvoid wt1(const array<T, S> &val) {\n  auto n\
    \ = val.size();\n  for (size_t i = 0; i < n; i++) {\n    if (i) wt1(' ');\n  \
    \  wt1(val[i]);\n  }\n}\ntemplate <class T>\nvoid wt1(const vector<T> &val) {\n\
    \  auto n = val.size();\n  for (size_t i = 0; i < n; i++) {\n    if (i) wt1('\
    \ ');\n    wt1(val[i]);\n  }\n}\n\ntemplate <class... T>\nvoid write(T &&...x)\
    \ {\n  (wt1(std::forward<T>(x)), ...);\n}\n\ntemplate <class... T>\nvoid print(T\
    \ &&...x) {\n  if constexpr (sizeof...(T))\n  {\n    int i = 0;\n    ((i++ ? wt1('\
    \ ') : void(), wt1(std::forward<T>(x))), ...);\n  }\n  wt1('\\n');\n}\n\n} //\
    \ namespace fastio\n\n#endif\n\n#if defined FAST_IO and not defined LOCAL\nstruct\
    \ Dummy {\n  Dummy() { atexit(fastio::flush); }\n} dummy;\n#endif\n\n// https://trap.jp/post/1224/\n\
    \n// ---- \u5165\u529B ----\n#if defined LOCAL or not defined FAST_IO\ntemplate\
    \ <class T, class U>\nistream &operator>>(istream &is, pair<T, U> &p)\n{\n  is\
    \ >> p.first >> p.second;\n  return is;\n}\ntemplate <class... Ts>\nistream &operator>>(istream\
    \ &is, tuple<Ts...> &t)\n{\n  apply([&](auto &...a)\n        { (is >> ... >> a);\
    \ }, t);\n  return is;\n}\ntemplate <class T, size_t n>\nistream &operator>>(istream\
    \ &is, array<T, n> &a)\n{\n  for (size_t i = 0; i < n; i++)\n    is >> a[i];\n\
    \  return is;\n}\ntemplate <class T>\nistream &operator>>(istream &is, vc<T> &a)\n\
    {\n  const size_t n = a.size();\n  for (size_t i = 0; i < n; i++)\n    is >> a[i];\n\
    \  return is;\n}\n#endif\n\nnamespace internal\n{\n\n#if defined LOCAL or not\
    \ defined FAST_IO\ntemplate <class... Ts>\nvoid CIN(Ts &...a) { (cin >> ... >>\
    \ a); }\n#endif\n\n#if defined FAST_IO and not defined LOCAL\ntemplate <class...\
    \ Ts>\nvoid READnodump(Ts &...a) { fastio::read(a...); }\n#else\ntemplate <class...\
    \ Ts>\nvoid READnodump(Ts &...a) { CIN(a...); }\n#endif\n\ntemplate <class...\
    \ T>\nvoid READVECnodump(int n, vc<T> &...v)\n{\n  (v.resize(n), ...);\n  READnodump(v...);\n\
    }\n\ntemplate <class... T>\nvoid READVEC2nodump(int n, int m, vvc<T> &...v)\n\
    {\n  (v.assign(n, vc<T>(m)), ...);\n  READnodump(v...);\n}\n\ntemplate <class...\
    \ T>\nvoid READJAGnodump(int n, vvc<T> &...vs)\n{\n  auto read_one = [&](auto\
    \ &v)\n  {\n    v.resize(n);\n    for (auto &row : v)\n    {\n      int k;\n \
    \     READnodump(k);\n      row.resize(k);\n      READnodump(row);\n    }\n  };\n\
    \  (read_one(vs), ...);\n}\n\n}; // namespace internal\n\n#define READ(...) internal::READnodump(__VA_ARGS__);\
    \ dump(__VA_ARGS__)\n\n#define IN(T, ...) T __VA_ARGS__; READ(__VA_ARGS__)\n\n\
    #define CHAR(...) IN(char, __VA_ARGS__)\n#define INT(...) IN(int, __VA_ARGS__)\n\
    #define LL(...) IN(ll, __VA_ARGS__)\n#define STR(...) IN(string, __VA_ARGS__)\n\
    #define ARR(T, n, ...) array<T, n> __VA_ARGS__; READ(__VA_ARGS__)\n\n#define READVEC(...)\
    \ internal::READVECnodump(__VA_ARGS__); dump(__VA_ARGS__)\n#define READVEC2(...)\
    \ internal::READVEC2nodump(__VA_ARGS__); dump(__VA_ARGS__)\n\n#define VEC(T, n,\
    \ ...) vc<T> __VA_ARGS__; READVEC(n, __VA_ARGS__)\n#define VEC2(T, n, m, ...)\
    \ vvc<T> __VA_ARGS__; READVEC2(n, m, __VA_ARGS__)\n\n#define READJAG(...) internal::READJAGnodump(__VA_ARGS__);\
    \ dump(__VA_ARGS__)\n\n#define JAG(T, n, ...) vvc<T> __VA_ARGS__; READJAG(n, __VA_ARGS__)\n\
    \n// ----------\n\n// ----- \u51FA\u529B -----\n#ifdef INTERACTIVE\n#define ENDL\
    \ endl\n#else\n#define ENDL '\\n'\n#endif\n\n#if defined LOCAL or not defined\
    \ FAST_IO\ntemplate <class T, class U>\nostream &operator<<(ostream &os, const\
    \ pair<T, U> &p)\n{\n  os << p.first << ' ' << p.second;\n  return os;\n}\n\n\
    template <class... Ts>\nostream &operator<<(ostream &os, const tuple<Ts...> &t)\n\
    {\n  if constexpr (sizeof...(Ts))\n  {\n    apply([&](const auto &...x)\n    \
    \      {\n            int i = 0;\n            ((os << (i++ ? \" \" : \"\") <<\
    \ x), ...);\n          }, t);\n  }\n  return os;\n}\ntemplate <class T, size_t\
    \ n>\nostream &operator<<(ostream &os, const array<T, n> &a)\n{\n  for (size_t\
    \ i = 0; i < n; i++)\n  {\n    if (i)\n      os << ' ';\n    os << a[i];\n  }\n\
    \  return os;\n}\ntemplate <class T>\nostream &operator<<(ostream &os, const vc<T>\
    \ &v)\n{\n  const size_t n = v.size();\n  for (size_t i = 0; i < n; i++)\n  {\n\
    \    if (i)\n      os << ' ';\n    os << v[i];\n  }\n  return os;\n}\n\nnamespace\
    \ internal\n{\n\ntemplate <class... Ts>\nvoid COUTW(const Ts &...a)\n{\n  if constexpr\
    \ (sizeof...(Ts))\n    (cout << ... << a);\n}\n\ntemplate <class... Ts>\nvoid\
    \ COUTP(const Ts &...a)\n{\n  if constexpr (sizeof...(Ts))\n  {\n    int i = 0;\n\
    \    ((cout << (i++ ? \" \" : \"\") << a), ...);\n  }\n  cout << ENDL;\n}\n\n\
    }; // namespace internal\n#endif\n\n#if defined FAST_IO and not defined LOCAL\n\
    #define WRITE fastio::write\n#define PRINT fastio::print\n#else\n#define WRITE\
    \ internal::COUTW\n#define PRINT internal::COUTP\n#endif\n#define PRINTEXIT(...)\
    \ do { PRINT(__VA_ARGS__); exit(0); } while (false)\n#define PRINTRETURN(...)\
    \ do { PRINT(__VA_ARGS__); return; } while (false)\n\ntemplate <class T>\nvoid\
    \ PRINTV(const vc<T> &v) { for (auto &vi : v) PRINT(vi); }\n#define PRINTVEXIT(...)\
    \ do { PRINTV(__VA_ARGS__); exit(0); } while (false)\n#define PRINTVRETURN(...)\
    \ do { PRINTV(__VA_ARGS__); return; } while (false)\n// ----------\n\n// -----\
    \ \u57FA\u6E96\u305A\u3089\u3057 -----\ntemplate <class T, class U, class P>\n\
    pair<T, U> &operator+=(pair<T, U> &a, const P &b)\n{\n  a.first += b.first;\n\
    \  a.second += b.second;\n  return a;\n}\ntemplate <class T, class U, class P>\n\
    pair<T, U> operator+(pair<T, U> a, const P &b) { return a += b; }\n\ntemplate\
    \ <class T, size_t n, class A>\narray<T, n> &operator+=(array<T, n> &a, const\
    \ A &b)\n{\n  for (size_t i = 0; i < n; i++)\n    a[i] += b[i];\n  return a;\n\
    }\ntemplate <class T, size_t n, class A>\narray<T, n> operator+(array<T, n> a,\
    \ const A &b) { return a += b; }\n\nnamespace internal\n{\n\ntemplate <size_t...\
    \ I, class A, class B>\nauto &tuple_add_impl(A &a, const B &b, const index_sequence<I...>)\n\
    {\n  ((get<I>(a) += get<I>(b)), ...);\n  return a;\n}\n\n}; // namespace internal\n\
    \ntemplate <class... Ts, class Tp>\ntuple<Ts...> &operator+=(tuple<Ts...> &a,\
    \ const Tp &b)\n{ return internal::tuple_add_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{});\
    \ }\ntemplate <class... Ts, class Tp>\ntuple<Ts...> operator+(tuple<Ts...> a,\
    \ const Tp &b) { return a += b; }\n\ntemplate <class T, class Add>\nvoid offset(vc<T>\
    \ &v, const Add &add) { for (auto &vi : v) vi += add; }\ntemplate <class T, class\
    \ Add>\nvoid offset(vvc<T> &v, const Add &add) { for (auto &vi : v) for (auto\
    \ &vij : vi) vij += add; }\n// ----------\n\n// ----- \u8EE2\u7F6E -----\ntemplate\
    \ <class T, const size_t m>\narray<vc<T>, m> unzip(const vc<array<T, m>> &vt)\n\
    {\n  const size_t n = vt.size();\n  array<vc<T>, m> tv;\n  tv.fill(vc<T>(n));\n\
    \  for (size_t i = 0; i < n; i++)\n    for (size_t j = 0; j < m; j++)\n      tv[j][i]\
    \ = vt[i][j];\n  return tv;\n}\ntemplate <class T, const size_t m>\nvc<array<T,\
    \ m>> zip(const array<vc<T>, m> &tv)\n{\n  if (tv.empty()) return {};\n  const\
    \ size_t n = tv[0].size();\n  vc<array<T, m>> vt(n);\n  for (size_t j = 0; j <\
    \ m; j++)\n  {\n    assert(tv[j].size() == n);\n    for (size_t i = 0; i < n;\
    \ i++)\n      vt[i][j] = tv[j][i];\n  }\n  return vt;\n}\n\ntemplate <class T,\
    \ class U>\npair<vc<T>, vc<U>> unzip(const vc<pair<T, U>> &vt)\n{\n  const size_t\
    \ n = vt.size();\n  pair<vc<T>, vc<U>> tv;\n  tv.first.resize(n), tv.second.resize(n);\n\
    \  for (size_t i = 0; i < n; i++)\n    tie(tv.first[i], tv.second[i]) = vt[i];\n\
    \  return tv;\n}\ntemplate <class T, class U>\nvc<pair<T, U>> zip(const pair<vc<T>,\
    \ vc<U>> &tv)\n{\n  const size_t n = tv.first.size();\n  assert(n == tv.second.size());\n\
    \  vc<pair<T, U>> vt(n);\n  for (size_t i = 0; i < n; i++)\n    vt[i] = make_pair(tv.first[i],\
    \ tv.second[i]);\n  return vt;\n}\n\nnamespace internal\n{\n\ntemplate <size_t...\
    \ I, class V, class Tp>\nauto vt_to_tv_impl(V &tv, const Tp &t, index_sequence<I...>,\
    \ size_t index)\n{ ((get<I>(tv)[index] = get<I>(t)), ...); }\n\ntemplate <size_t...\
    \ I, class Tp>\nauto tv_to_vt_impl(const Tp &tv, index_sequence<I...>, size_t\
    \ index)\n{ return make_tuple(get<I>(tv)[index]...); }\n\n};\n\ntemplate <class...\
    \ Ts>\nauto unzip(const vc<tuple<Ts...>> &vt)\n{\n  const size_t n = vt.size();\n\
    \  tuple<vc<Ts>...> tv;\n  apply([&](auto &...v)\n        { ((v.resize(n)), ...);\
    \ }, tv);\n  for (size_t i = 0; i < n; i++)\n    internal::vt_to_tv_impl(tv, vt[i],\
    \ make_index_sequence<tuple_size_v<decltype(tv)>>{}, i);\n  return tv;\n}\n\n\
    template <class... Ts>\nauto zip(const tuple<vc<Ts>...> &tv)\n{\n  size_t n =\
    \ get<0>(tv).size();\n  apply([&](auto &...v)\n        { ((void(v), assert(v.size()\
    \ == n)), ...); }, tv);\n  vc<tuple<Ts...>> vt(n);\n  for (size_t i = 0; i < n;\
    \ i++)\n    vt[i] = internal::tv_to_vt_impl(tv, index_sequence_for<Ts...>{}, i);\n\
    \  return vt;\n}\n\n#define UNZIP(vt, ...) auto [__VA_ARGS__] = unzip(vt)\n#define\
    \ ZIP(vt, ...) auto vt = zip(tuple{__VA_ARGS__})\n// ----------\n#line 2 \"template/template_random.hpp\"\
    \n\n#line 5 \"template/template_random.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\u751F\u6210\uFF09\n * @docs docs/template/template_random.md\n\
    \ */\n\nmt19937_64 mt;\n\n// [l, r] \u304B\u3089\u7B49\u78BA\u7387\ntemplate <class\
    \ T = ll, class U1, class U2>\nT randint(U1 l, U2 r)\n{\n  assert(T(l) <= T(r));\n\
    \  return uniform_int_distribution<T>(T(l), T(r))(mt);\n}\n// [l, r) \u304B\u3089\
    \u7B49\u78BA\u7387\ntemplate <class T = ll, class U1, class U2>\nT randrange(U1\
    \ l, U2 r)\n{\n  assert(T(l) < T(r));\n  return uniform_int_distribution<T>(T(l),\
    \ T(r) - 1)(mt);\n}\n\n// [l, r) \u304B\u3089\u4E00\u69D8\u30E9\u30F3\u30C0\u30E0\
    \u306A\u5B9F\u6570\u3092\u8FD4\u3059\ntemplate <class T = double, class U1, class\
    \ U2>\nT randreal(U1 l, U2 r)\n{\n  assert(T(l) < T(r));\n  return uniform_real_distribution<T>(T(l),\
    \ T(r))(mt);\n}\n\n// \u78BA\u7387 p \u3067 true \u3092\u8FD4\u3059\nbool randbool(double\
    \ p)\n{\n  assert(0 <= p && p <= 1);\n  return bernoulli_distribution(p)(mt);\n\
    }\n\nnamespace internal\n{\ntemplate <bool does_sort, class V, class T>\nvoid\
    \ random_sample_range(V &res, T l, T r)\n{\n  int k = res.size();\n  T n = r -\
    \ l;\n  if (k <= 256)\n  {\n    repi(i, k)\n    {\n      T j = n - T(k) + T(i),\
    \ x = randint<T>(0, j);\n      if (find(res.begin(), res.begin() + i, x) != res.begin()\
    \ + i)\n        x = j;\n      res[i] = x;\n    }\n  }\n  else\n  {\n    unordered_set<T>\
    \ used;\n    used.reserve(2 * size_t(k));\n    repi(i, k)\n    {\n      T j =\
    \ n - T(k) + T(i), x = randint<T>(0, j);\n      if (!used.insert(x).second)\n\
    \        x = j, used.insert(x);\n      res[i] = x;\n    }\n  }\n  for (T &x :\
    \ res) x += l;\n  if constexpr (does_sort)\n    sort(res.begin(), res.end());\n\
    \  else\n    shuffle(res.begin(), res.end(), mt);\n}\n}; // namespace internal\n\
    \n// [l, r) \u304B\u3089\u76F8\u7570\u306A\u308B k \u500B\u3092\u9078\u3076\n\
    // does_sort: \u30BD\u30FC\u30C8\u3059\u308B\u304B\u3069\u3046\u304B\ntemplate\
    \ <int k, bool does_sort, class T = ll, class U1, class U2>\narray<T, k> random_sample_range_array(U1\
    \ l, U2 r)\n{\n  assert(T(r) - T(l) >= T(k));\n  array<T, k> res;\n  internal::random_sample_range<does_sort>(res,\
    \ T(l), T(r));\n  return res;\n}\n// [l, r) \u304B\u3089\u76F8\u7570\u306A\u308B\
    \ k \u500B\u3092\u9078\u3076\n// does_sort: \u30BD\u30FC\u30C8\u3059\u308B\u304B\
    \u3069\u3046\u304B\ntemplate <bool does_sort, class T = ll, class U1, class U2>\n\
    vc<T> random_sample_range_vector(U1 l, U2 r, int k)\n{\n  assert(k >= 0);\n  assert(T(r)\
    \ - T(l) >= T(k));\n  vc<T> res(k);\n  internal::random_sample_range<does_sort>(res,\
    \ T(l), T(r));\n  return res;\n}\n#line 2 \"math/extgcd.hpp\"\n\n#line 4 \"math/extgcd.hpp\"\
    \n\n/**\n * @brief \u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\
    \u6CD5 (extgcd)\n * @docs docs/math/extgcd.md\n */\n\n// g == gcd(x, y) >= 0,\
    \ ax + by == g \u3092\u6E80\u305F\u3059 (g, x, y)\n// max(|x|, |y|) <= max(|a|,\
    \ |b|)\ntemplate <class T = ll>\nconstexpr tuple<T, T, T> extgcd(T a, T b)\n{\n\
    \  if (a == 0 && b == 0)\n    return {0, 0, 0};\n  \n  // a*x1 + b*y1 == z1  ...(1)\n\
    \  // a*x2 + b*y2 == z2  ...(2)\n  T x1 = 1, y1 = 0, z1 = a;\n  T x2 = 0, y2 =\
    \ 1, z2 = b;\n  while (z2 != 0)\n  {\n    // (1)' = (2)\n    // (2)' = (1) - q*(2)\n\
    \    T q = z1 / z2;\n    tie(x1, x2) = make_pair(x2, x1 - q * x2);\n    tie(y1,\
    \ y2) = make_pair(y2, y1 - q * y2);\n    tie(z1, z2) = make_pair(z2, z1 - q *\
    \ z2);\n  }\n  if (z1 < 0)\n    x1 = -x1, y1 = -y1, z1 = -z1;\n  return {z1, x1,\
    \ y1};\n}\n#line 5 \"math/sbt.hpp\"\n\n/**\n * @brief Stern Brocot Tree\n * @docs\
    \ docs/math/sbt.md\n */\n\n// \u4FDD\u6301\u3059\u308B\u3082\u306E\n// - range:\
    \ p, q, r, s\n// \u6027\u8CEA\n// - num/den == (p+r)/(q+s)\n// - \u5DE6\u306E\u5B50\
    \u306F (p, q, p+r, q+s), \u53F3\u306E\u5B50\u306F (p+r, q+s, r, s)\n// - \u5B50\
    \u5B6B\u306E\u4E0B\u9650\u306F p/q, \u4E0A\u9650\u306F r/s\ntemplate <class T\
    \ = ll>\nstruct SBTNode\n{\n  using Path = vc<pair<char, T>>;\n\npublic:\n  T\
    \ p, q, r, s;\n  T num() const { return p + r; }\n  T den() const { return q +\
    \ s; }\n\n  SBTNode() : p(0), q(1), r(1), s(0) {}\n  SBTNode(T num_, T den_)\n\
    \  {\n    auto [g, _, __] = extgcd(num_, den_);\n    num_ /= g, den_ /= g;\n \
    \   p = num_, q = den_, r = 0, s = 0;\n    *this = SBTNode(encode_path());\n \
    \ }\n  SBTNode(T p, T q, T r, T s) : p(p), q(q), r(r), s(s) {}\n  SBTNode(const\
    \ Path &path) : p(0), q(1), r(1), s(0) { fec([ dir, d ] : path) descend(dir, d);\
    \ }\n\n  // dir \u65B9\u5411\u306B d \u500B\u6F5C\u308B (dir \u306F 'L' \u304B\
    \ 'R')\n  void descend(char dir, T d)\n  {\n    assert(dir == 'L' || dir == 'R');\n\
    \    assert(d >= 0);\n    if (dir == 'L')\n      r += d * p, s += d * q;\n   \
    \ else\n      p += d * r, q += d * s;\n  }\n  // \u89AA\u65B9\u5411\u306B d \u500B\
    \u4E0A\u304C\u308B\n  void ascend(T d)\n  {\n    fec([dir, c] : reversed(encode_path()))\n\
    \    {\n      T e = min(c, d);\n      if (dir == 'L')\n        r -= e * p, s -=\
    \ e * q;\n      else\n        p -= e * r, q -= e * s;\n      d -= e;\n    }\n\
    \    assert(d == 0 && \"failed: 0 <= d <= depth\");\n  }\n  T depth() const\n\
    \  {\n    T res = 0;\n    fec([ dir, d ] : encode_path()) res += d;\n    return\
    \ res;\n  }\n\n  Path encode_path() const\n  {\n    T f = num(), g = den();\n\
    \    vc<pair<char, T>> path;\n    char dir = 'R';\n    if (f < g)\n    {\n   \
    \   dir = 'L';\n      swap(f, g);\n    }\n    while (f != 1)\n    {\n      path.eb(dir,\
    \ f / g);\n      dir ^= 'L' ^ 'R';\n      f %= g;\n      swap(f, g);\n    }\n\
    \    if (!path.empty())\n    {\n      if (path.back().second == 1)\n        path.pop_back();\n\
    \      else\n        path.back().second--;\n    }\n    return path;\n  }\n\n \
    \ SBTNode<T> lca(const SBTNode<T> &rhs) const\n  {\n    SBTNode<T> node;\n   \
    \ auto path1 = encode_path(), path2 = rhs.encode_path();\n    repi(i, min(path1.size(),\
    \ path2.size()))\n    {\n      if (path1[i].first == path2[i].first)\n      {\n\
    \        char dir = path1[i].first;\n        T c = min(path1[i].second, path2[i].second);\n\
    \        node.descend(dir, c);\n      }\n      if (path1[i] != path2[i])\n   \
    \     break;\n    }\n    return node;\n  }\n};\n#ifdef LOCAL\nCPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(num(),\
    \ den(), p, q, r, s);\n#endif\n\n// judge \u306F bool(T num, T den)\n// judge:\
    \ [0, \u221E] \u2192 {true, false}\n// judge \u306F\u5358\u8ABF\u6027\u3092\u6301\
    \u3064\u3001\u3059\u306A\u308F\u3061\u3001\u3042\u308B\u5B9F\u6570 \u03B1 \u3092\
    \u5883\u306B true \u3068 false \u304C\u5207\u308A\u66FF\u308F\u308B\n// \u5206\
    \u6BCD\u30FB\u5206\u5B50\u304C max_value \u4EE5\u4E0B\u306E\u6709\u7406\u6570\u306E\
    \u3046\u3061\u3001\u03B1 \u306B\u6700\u3082\u8FD1\u3044\u3082\u306E (\u4E0B\u5074\
    : p/q, \u4E0A\u5074: r/s) \u3092\u8FD4\u3059\ntemplate <class T = ll, class F>\n\
    SBTNode<T> sbt_search(const F &judge, T max_value)\n{\n  SBTNode<T> node;\n  const\
    \ bool judge_01 = judge(0, 1), judge_10 = judge(1, 0);\n  assert(judge_01 != judge_10);\n\
    \  assert(max_value >= 1);\n  while (node.num() <= max_value && node.den() <=\
    \ max_value)\n  {\n    const bool judge_now = judge(node.num(), node.den());\n\
    \    const char dir = judge_now ^ judge_01 ? 'L' : 'R';\n    T max_d;\n    if\
    \ (dir == 'L')\n    {\n      if (node.p == 0)\n        max_d = (max_value - node.s)\
    \ / node.q;\n      else\n        max_d = min((max_value - node.r) / node.p, (max_value\
    \ - node.s) / node.q);\n    }\n    else\n    {\n      if (node.s == 0)\n     \
    \   max_d = (max_value - node.p) / node.r;\n      else\n        max_d = min((max_value\
    \ - node.q) / node.s, (max_value - node.p) / node.r);\n    }\n    auto judge_next\
    \ = [&](T d) -> bool\n    {\n      auto next_node = node;\n      next_node.descend(dir,\
    \ d);\n      return judge_now == judge(next_node.num(), next_node.den());\n  \
    \  };\n    T dl = 1, dr = 2;\n    while (dr <= max_d)\n    {\n      if (!judge_next(dr))\n\
    \        break;\n      dl = dr, dr = min(2 * dr, max_d + 1);\n    }\n    while\
    \ (dr - dl > 1)\n    {\n      T dm = dl + (dr - dl) / 2;\n      (judge_next(dm)\
    \ ? dl : dr) = dm;\n    }\n    node.descend(dir, dl);\n  }\n  return node;\n}\n\
    \n// a/b < p/q < c/d \u3092\u6E80\u305F\u3059 p/q \u306E\u3046\u3061 q \u304C\u6700\
    \u5C0F\u306E\u3082\u306E\n// q != 1 \u306A\u3089\u4E00\u610F\u3002q == 1 \u306A\
    \u3089 p \u304C\u6700\u5C0F\u3068\u306A\u308B\u3088\u3046\u306B\u3068\u308B\n\
    // https://atcoder.jp/contests/abc408/tasks/abc408_g/editorial\ntemplate <class\
    \ T>\npair<T, T> abc408g(T a, T b, T c, T d)\n{\n  T n = a / b;\n  a -= n * b,\
    \ c -= n * d;\n  if (c > d)\n    return {n + 1, 1};\n  auto [p, q] = abc408g(d,\
    \ c, b, a);\n  return {p * n + q, p};\n}\n"
  code: "#pragma once\n\n#include \"../template/template_all_but_modint.hpp\"\n#include\
    \ \"extgcd.hpp\"\n\n/**\n * @brief Stern Brocot Tree\n * @docs docs/math/sbt.md\n\
    \ */\n\n// \u4FDD\u6301\u3059\u308B\u3082\u306E\n// - range: p, q, r, s\n// \u6027\
    \u8CEA\n// - num/den == (p+r)/(q+s)\n// - \u5DE6\u306E\u5B50\u306F (p, q, p+r,\
    \ q+s), \u53F3\u306E\u5B50\u306F (p+r, q+s, r, s)\n// - \u5B50\u5B6B\u306E\u4E0B\
    \u9650\u306F p/q, \u4E0A\u9650\u306F r/s\ntemplate <class T = ll>\nstruct SBTNode\n\
    {\n  using Path = vc<pair<char, T>>;\n\npublic:\n  T p, q, r, s;\n  T num() const\
    \ { return p + r; }\n  T den() const { return q + s; }\n\n  SBTNode() : p(0),\
    \ q(1), r(1), s(0) {}\n  SBTNode(T num_, T den_)\n  {\n    auto [g, _, __] = extgcd(num_,\
    \ den_);\n    num_ /= g, den_ /= g;\n    p = num_, q = den_, r = 0, s = 0;\n \
    \   *this = SBTNode(encode_path());\n  }\n  SBTNode(T p, T q, T r, T s) : p(p),\
    \ q(q), r(r), s(s) {}\n  SBTNode(const Path &path) : p(0), q(1), r(1), s(0) {\
    \ fec([ dir, d ] : path) descend(dir, d); }\n\n  // dir \u65B9\u5411\u306B d \u500B\
    \u6F5C\u308B (dir \u306F 'L' \u304B 'R')\n  void descend(char dir, T d)\n  {\n\
    \    assert(dir == 'L' || dir == 'R');\n    assert(d >= 0);\n    if (dir == 'L')\n\
    \      r += d * p, s += d * q;\n    else\n      p += d * r, q += d * s;\n  }\n\
    \  // \u89AA\u65B9\u5411\u306B d \u500B\u4E0A\u304C\u308B\n  void ascend(T d)\n\
    \  {\n    fec([dir, c] : reversed(encode_path()))\n    {\n      T e = min(c, d);\n\
    \      if (dir == 'L')\n        r -= e * p, s -= e * q;\n      else\n        p\
    \ -= e * r, q -= e * s;\n      d -= e;\n    }\n    assert(d == 0 && \"failed:\
    \ 0 <= d <= depth\");\n  }\n  T depth() const\n  {\n    T res = 0;\n    fec([\
    \ dir, d ] : encode_path()) res += d;\n    return res;\n  }\n\n  Path encode_path()\
    \ const\n  {\n    T f = num(), g = den();\n    vc<pair<char, T>> path;\n    char\
    \ dir = 'R';\n    if (f < g)\n    {\n      dir = 'L';\n      swap(f, g);\n   \
    \ }\n    while (f != 1)\n    {\n      path.eb(dir, f / g);\n      dir ^= 'L' ^\
    \ 'R';\n      f %= g;\n      swap(f, g);\n    }\n    if (!path.empty())\n    {\n\
    \      if (path.back().second == 1)\n        path.pop_back();\n      else\n  \
    \      path.back().second--;\n    }\n    return path;\n  }\n\n  SBTNode<T> lca(const\
    \ SBTNode<T> &rhs) const\n  {\n    SBTNode<T> node;\n    auto path1 = encode_path(),\
    \ path2 = rhs.encode_path();\n    repi(i, min(path1.size(), path2.size()))\n \
    \   {\n      if (path1[i].first == path2[i].first)\n      {\n        char dir\
    \ = path1[i].first;\n        T c = min(path1[i].second, path2[i].second);\n  \
    \      node.descend(dir, c);\n      }\n      if (path1[i] != path2[i])\n     \
    \   break;\n    }\n    return node;\n  }\n};\n#ifdef LOCAL\nCPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(num(),\
    \ den(), p, q, r, s);\n#endif\n\n// judge \u306F bool(T num, T den)\n// judge:\
    \ [0, \u221E] \u2192 {true, false}\n// judge \u306F\u5358\u8ABF\u6027\u3092\u6301\
    \u3064\u3001\u3059\u306A\u308F\u3061\u3001\u3042\u308B\u5B9F\u6570 \u03B1 \u3092\
    \u5883\u306B true \u3068 false \u304C\u5207\u308A\u66FF\u308F\u308B\n// \u5206\
    \u6BCD\u30FB\u5206\u5B50\u304C max_value \u4EE5\u4E0B\u306E\u6709\u7406\u6570\u306E\
    \u3046\u3061\u3001\u03B1 \u306B\u6700\u3082\u8FD1\u3044\u3082\u306E (\u4E0B\u5074\
    : p/q, \u4E0A\u5074: r/s) \u3092\u8FD4\u3059\ntemplate <class T = ll, class F>\n\
    SBTNode<T> sbt_search(const F &judge, T max_value)\n{\n  SBTNode<T> node;\n  const\
    \ bool judge_01 = judge(0, 1), judge_10 = judge(1, 0);\n  assert(judge_01 != judge_10);\n\
    \  assert(max_value >= 1);\n  while (node.num() <= max_value && node.den() <=\
    \ max_value)\n  {\n    const bool judge_now = judge(node.num(), node.den());\n\
    \    const char dir = judge_now ^ judge_01 ? 'L' : 'R';\n    T max_d;\n    if\
    \ (dir == 'L')\n    {\n      if (node.p == 0)\n        max_d = (max_value - node.s)\
    \ / node.q;\n      else\n        max_d = min((max_value - node.r) / node.p, (max_value\
    \ - node.s) / node.q);\n    }\n    else\n    {\n      if (node.s == 0)\n     \
    \   max_d = (max_value - node.p) / node.r;\n      else\n        max_d = min((max_value\
    \ - node.q) / node.s, (max_value - node.p) / node.r);\n    }\n    auto judge_next\
    \ = [&](T d) -> bool\n    {\n      auto next_node = node;\n      next_node.descend(dir,\
    \ d);\n      return judge_now == judge(next_node.num(), next_node.den());\n  \
    \  };\n    T dl = 1, dr = 2;\n    while (dr <= max_d)\n    {\n      if (!judge_next(dr))\n\
    \        break;\n      dl = dr, dr = min(2 * dr, max_d + 1);\n    }\n    while\
    \ (dr - dl > 1)\n    {\n      T dm = dl + (dr - dl) / 2;\n      (judge_next(dm)\
    \ ? dl : dr) = dm;\n    }\n    node.descend(dir, dl);\n  }\n  return node;\n}\n\
    \n// a/b < p/q < c/d \u3092\u6E80\u305F\u3059 p/q \u306E\u3046\u3061 q \u304C\u6700\
    \u5C0F\u306E\u3082\u306E\n// q != 1 \u306A\u3089\u4E00\u610F\u3002q == 1 \u306A\
    \u3089 p \u304C\u6700\u5C0F\u3068\u306A\u308B\u3088\u3046\u306B\u3068\u308B\n\
    // https://atcoder.jp/contests/abc408/tasks/abc408_g/editorial\ntemplate <class\
    \ T>\npair<T, T> abc408g(T a, T b, T c, T d)\n{\n  T n = a / b;\n  a -= n * b,\
    \ c -= n * d;\n  if (c > d)\n    return {n + 1, 1};\n  auto [p, q] = abc408g(d,\
    \ c, b, a);\n  return {p * n + q, p};\n}\n"
  dependsOn:
  - template/template_all_but_modint.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_math.hpp
  - utils/is_integral_ext.hpp
  - template/template_vector.hpp
  - template/template_algo.hpp
  - template/template_binsearch.hpp
  - template/template_bit.hpp
  - template/template_inout.hpp
  - template/template_dump.hpp
  - template/template_random.hpp
  - math/extgcd.hpp
  isVerificationFile: false
  path: math/sbt.hpp
  requiredBy: []
  timestamp: '2026-08-08 20:51:19+09:00'
  verificationStatus: LIBRARY_ALL_WA
  verifiedWith:
  - verify/yosupo/stern_brocot_tree.test.cpp
  - verify/yosupo/rational_approximation.test.cpp
documentation_of: math/sbt.hpp
layout: document
redirect_from:
- /library/math/sbt.hpp
- /library/math/sbt.hpp.html
title: Stern Brocot Tree
---
## Stern–Brocot Tree

各頂点に正の有理数 $\dfrac{p+r}{q+s}$ が一対一対応する二分探索木。性質：

- $\dfrac{p+r}{q+s}$ の左の子は $\dfrac{p+(r+p)}{q+(s+q)}$、右の子は $\dfrac{(p+r)+r}{(q+s)+s}$
- $\dfrac{p+r}{q+s}$ の子孫として現れる有理数の範囲は $\left(\dfrac{p}{q}, \dfrac{r}{s} \right)$
- パスを RLE したものの長さは $O(\log 値)$

### コンストラクタ

```cpp
(1) SBTNode<T>(T num, T den)
(2) SBTNode<T>(T p, T q, T r, T s)
(3) SBTNode<T>(Path path)
```

- (1)：$\dfrac{\mathrm{num}}{\mathrm{den}}$ の頂点を作る。
- (2)：$\dfrac{p+r}{q+s}$ の頂点を作る。
- (3)：`path` のパスをたどって頂点を作る。`Path` は `vc<pair<char, T>>` で、方向（`L`, `R`）と進む個数の組の列。

### メンバ変数

```cpp
T p, q, r, s
```

### メンバ関数

#### num, den

```cpp
(1) T num()
(2) T den()
```

##### 計算量

- $O(1)$

#### descend

```cpp
void descend(char dir, T d)
```

`dir` 方向に $d$ だけ潜る（頂点自体が変更される）。

##### 制約

- `dir` は `L` または `R`
- $d \geq 0$

##### 計算量

- $O(1)$

#### ascend

```cpp
void ascend(T d)
```

親方向に $d$ だけ上がる（頂点自体が変更される）。

##### 制約

- $0 \leq d \leq (頂点の深さ)$

##### 計算量

- $O(\log (p+q+r+s))$

#### depth

```cpp
T depth()
```

現在の頂点の深さを返す。

##### 計算量

- $O(\log (p+q+r+s))$

#### encode_path

```cpp
Path encode_path()
```

根から現在の頂点までのパスを `Path` として返す。

##### 計算量

- $O(\log (p+q+r+s))$

#### lca

```cpp
SBTNode<T> lca(SBTNode<T> rhs)
```

現在の頂点と `rhs` の頂点の LCA を求める。

なお、$\dfrac{a}{b}$ と $\dfrac{c}{d}$ の LCA は、$\dfrac{a}{b} \leq \dfrac{p}{q} \leq \dfrac{c}{d}$ を満たす $\dfrac{p}{q}$ のうち分母 $q$ が最小のもの。

##### 計算量

$v$ を現在の頂点と `rhs` の頂点の分子・分母の最大値として

- $O(\log v)$

### 外側の関数

#### sbt_search

```cpp
SBTNode<T> sbt_search(auto judge, T max_value)
```

`judge(T num, T den)` は $[0/1, 1/0]$ から `bool` への関数で、単調性を持つ、すなわちある実数 $\alpha$ が存在して、$\alpha$ を境に `true` と `false` が切り替わるとする（$\alpha$ で `true` か `false` かは問わない）。このとき、分母・分子がともに $\mathrm{max\_value}$ 以下の有理数のうち、下側・上側それぞれで $\alpha$ に最も近いものを求める。

##### 制約

- `judge` は単調性を持つ
- $\mathrm{max\_value} \geq 1$

##### 計算量

- $O(\log \mathrm{max\_value})$ 回の `judge` の呼び出し
