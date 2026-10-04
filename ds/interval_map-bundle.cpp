
// https://github.com/miscalculation53/library/tree/wip/template/template_all_but_modint.hpp

// https://github.com/miscalculation53/library/tree/wip/template/template_types.hpp

/**
 * @brief テンプレート（型）
 * @docs docs/template/template_types.md
 */

#include <bits/stdc++.h>
using namespace std;

#ifndef EPS
#define EPS 1e-11
#endif
using ld = decltype(EPS);

using ll = long long;
using uint = unsigned int;
using ull = unsigned long long;
using pll = pair<ll, ll>;
using tlll = tuple<ll, ll, ll>;
using tllll = tuple<ll, ll, ll, ll>;

#define vc vector
template <class T>
using vvc = vc<vc<T>>;
template <class T>
using vvvc = vc<vc<vc<T>>>;

using vb = vc<bool>;
using vl = vc<ll>;
using vpll = vc<pll>;
using vtlll = vc<tlll>;
using vtllll = vc<tllll>;
using vstr = vc<string>;
using vvb = vvc<bool>;
using vvl = vvc<ll>;

template <class T>
using pql = priority_queue<T, vc<T>, greater<T>>;
template <class T>
using pqg = priority_queue<T>;

#ifdef __SIZEOF_INT128__
using i128 = __int128_t;
using u128 = __uint128_t;
i128 stoi128(const string &s)
{
  const bool neg = s.front() == '-';
  u128 res = 0;
  for (int i = neg; i < (int)s.size(); i++)
    res = 10 * res + s[i] - '0';
  if (neg)
    return -i128(res - 1) - 1;
  return i128(res);
}
string i128tos(i128 x)
{
  if (x == 0) return "0";
  string sign = "", res = "";
  u128 ux;
  if (x < 0)
    ux = u128(-(x + 1)) + 1, sign = "-";
  else
    ux = x;
  while (ux > 0)
  {
    res += '0' + ux % 10;
    ux /= 10;
  }
  reverse(res.begin(), res.end());
  return sign + res;
}
istream &operator>>(istream &is, i128 &a)
{
  string s;
  is >> s;
  a = stoi128(s);
  return is;
}
ostream &operator<<(ostream &os, const i128 &a)
{
  os << i128tos(a);
  return os;
}
#endif

#define cauto const auto
// https://github.com/miscalculation53/library/tree/wip/template/template_rep.hpp


/**
 * @brief テンプレート（rep）
 * @docs docs/template/template_rep.md
 */

// https://trap.jp/post/1224/

#define overload4(_1, _2, _3, _4, name, ...) name
#define rep1(i, n) for (ll i = 0, nnnnn = ll(n); i < nnnnn; i++)
#define rep2(i, l, r) for (ll i = ll(l), rrrrr = ll(r); i < rrrrr; i++)
#define rep3(i, l, r, d) for (ll i = ll(l), rrrrr = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)
#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)
#define repi1(i, n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)
#define repi2(i, l, r) for (int i = int(l), rrrrr = int(r); i < rrrrr; i++)
#define repi3(i, l, r, d) for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)
#define repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)

#define fe(...) for (auto __VA_ARGS__)
#define fec(...) for (cauto &__VA_ARGS__)
#define fem(...) for (auto &__VA_ARGS__)
// https://github.com/miscalculation53/library/tree/wip/template/template_math.hpp

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif
#ifndef EPS
#define EPS 1e-11
#endif


// https://github.com/miscalculation53/library/tree/wip/utils/is_integral_ext.hpp


/**
 * @brief $128$ ビット整数を含めた整数判定
 * @docs docs/utils/is_integral_ext.md
 */

template <class T>
constexpr bool is_integral_ext = is_integral_v<T> || is_same_v<T, i128> || is_same_v<T, u128>;

template <class T>
constexpr bool is_signed_ext = is_signed_v<T> || is_same_v<T, i128>;

template <class T>
constexpr bool is_unsigned_ext = is_unsigned_v<T> || is_same_v<T, u128>;
// https://github.com/miscalculation53/library/tree/wip/utils/default_infty.hpp


#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif

/**
 * @brief 型ごとの既定の無限大
 * @docs docs/utils/default_infty.md
 */

namespace default_infty_detail
{
  template <class T, class = void>
  struct has_infty : false_type {};

  template <class T>
  struct has_infty<T, void_t<decltype(T::infty())>> : true_type {};

  template <class T>
  const T &custom_value()
  {
    static const T value = T::infty();
    return value;
  }

  template <class T>
  inline constexpr bool unsupported = false;
}

template <class T>
constexpr decltype(auto) default_infty()
{
  if constexpr (default_infty_detail::has_infty<T>::value)
    return default_infty_detail::custom_value<T>();
  else if constexpr (is_same_v<T, i128> || is_same_v<T, u128>)
    return T(INF) * T(INF);
  else if constexpr (is_integral_ext<T> && !is_same_v<T, bool>)
  {
    if constexpr (sizeof(T) >= sizeof(ll))
      return T(INF);
    else if constexpr (numeric_limits<T>::digits >= 31)
      return (T(1) << 30) - 1;
    else
      return T(numeric_limits<T>::max() / 2);
  }
  else if constexpr (numeric_limits<T>::has_infinity)
    return numeric_limits<T>::infinity();
  else
    static_assert(default_infty_detail::unsupported<T>, "No default infinity for this type; specify infty explicitly or define T::infty().");
}

/**
 * @brief テンプレート（演算）
 * @docs docs/template/template_math.md
 */

template <class T, class U>
inline bool chmin(T &a, U b) { return a > b ? a = b, true : false; }
template <class T, class U>
inline bool chmax(T &a, U b) { return a < b ? a = b, true : false; }

template <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>
inline constexpr T divfloor(U a, V b) { return T(a) / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }
template <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>
inline constexpr T divceil(U a, V b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }
template <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>
inline constexpr T divround(U a, V b) { return divfloor<T>(2 * T(a) + T(b), 2 * T(b)); }
template <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>
inline constexpr T safemod(U a, V b) { return T(a) - T(b) * divfloor<T>(a, b); }

template <class T = ll, class U, class V>
constexpr T ipow(U a, V b)
{
  assert(b >= 0);
  if (b == 0)
    return 1;
  if (a == 0 || a == 1)
    return a;
  if (a < 0 && a == -1)
    return b & 1 ? -1 : 1;

  T res = 1, tmp = a;
  while (true)
  {
    if (b & 1)
      res *= tmp;
    b >>= 1;
    if (b == 0)
      break;
    tmp *= tmp;
  }
  return res;
}
template <class T = ll, class A, class B, class M>
T mul_limited(A a, B b, M m)
{
  assert(a >= 0 && b >= 0 && m >= 0);
  if (b == 0)
    return 0;
  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);
}
template <class T = ll, class A, class B>
T mul_limited(A a, B b) { return mul_limited<T>(a, b, default_infty<T>()); }
template <class T = ll, class A, class B, class M>
T pow_limited(A a, B b, M m)
{
  assert(a >= 0 && b >= 0 && m >= 0);
  if (a <= 1 || b == 0)
    return min(ipow<T>(a, b), T(m));
  
  T res = 1, tmp = a;
  while (true)
  {
    if (b & 1)
    {
      if (res > T(m) / tmp)
        return m;
      res *= tmp;
    }
    b >>= 1;
    if (b == 0)
      break;
    if (tmp > T(m) / tmp)
      return m;
    tmp *= tmp;
  }
  return res;
}
template <class T = ll, class A, class B>
T pow_limited(A a, B b) { return pow_limited<T>(a, b, default_infty<T>()); }

template <class T = ll, class A, class K>
constexpr T iroot(A a, K k)
{
  assert(a >= 0 && k >= 1);
  if (a <= 1 || k == 1)
    return a;
  if (k == 2)
  {
    const T aa = T(a);
    T x = T(sqrtl((long double)a));
    while (x > aa / x)
      x--;
    while (x < numeric_limits<T>::max())
    {
      const T y = x + 1;
      if (y > aa / y)
        break;
      x = y;
    }
    return x;
  }

  auto isok = [&](T x) -> bool
  {
    if (x == 0)
      return true;
    T res = 1, k2 = k;
    while (true)
    {
      if (k2 & 1)
      {
        if (res > T(a) / x)
          return false;
        res *= x;
      }
      k2 >>= 1;
      if (k2 == 0)
        break;
      if (x > T(a) / x)
        return false;
      x *= x;
    }
    return res <= T(a);
  };

  T x = pow(a, 1.0 / k);
  bool up = true;
  while (!isok(x))
    up = false, x--;
  if (up)
  {
    while (x < numeric_limits<T>::max() && isok(x + 1))
      x++;
  }
  return x;
}
template <class T = ll, class A, class K>
constexpr T iroot_ceil(A a, K k)
{
  T x = iroot<T>(a, k);
  return ipow<T>(x, k) == a ? x : x + 1;
}

// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html
template <class D = decltype(EPS), class A>
int SGN(A a, D eps = EPS) { return int(a > eps) - int(a < -eps); }

// 位取り記数法と同じ順番（下位桁が後ろ）
// 0 に対しては {0} が返る
template <class T = ll, class U, class V>
vc<T> base_repr(U val, V base)
{
  assert(val >= 0);
  assert(base >= 2);
  if (val == 0)
    return {0};
  vc<T> a;
  while (val > 0)
  {
    a.emplace_back(val % base);
    val /= base;
  }
  reverse(a.begin(), a.end());
  return a;
}
// 位取り記数法と同じ順番（下位桁が後ろ）
template <class T = ll, class U, class V>
vc<T> base_repr(U val, V base, int n)
{
  assert(val >= 0);
  assert(base >= 2);
  assert(n >= 0);
  vc<T> a(n);
  repi(i, n)
  {
    a[i] = val % base;
    val /= base;
  }
  reverse(a.begin(), a.end());
  return a;
}
template <const bool use_upper = true, class U>
string base_repr_str(U val, int base)
{
  assert(val >= 0);
  assert(2 <= base && base <= 36);
  auto a = base_repr(val, base);
  string s = "";
  for (cauto &ai : a)
    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));
  return s;
}
template <const bool use_upper = true, class U>
string base_repr_str(U val, int base, int n)
{
  assert(val >= 0);
  assert(2 <= base && base <= 36);
  assert(n >= 0);
  auto a = base_repr(val, base, n);
  string s = "";
  for (cauto &ai : a)
    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));
  return s;
}
// https://github.com/miscalculation53/library/tree/wip/template/template_vector.hpp


/**
 * @brief テンプレート（vector）
 * @docs docs/template/template_vector.md
 */

#define ALL(a) (a).begin(), (a).end()
template <class T = ll, class V>
inline T SZ(const V &x) { return x.size(); }
#define eb emplace_back

#define LMD(x, fx) ([&](const auto &x) { return fx; })
template <class F>
auto gen_vec(int n, const F &f)
{
  vc<decltype(f(0))> res(n);
  repi(i, n) res[i] = f(i);
  return res;
}
#define GEN_VEC(n, i, fi) (gen_vec(n, LMD(i, fi)))

// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0
template <class T, size_t d, size_t i = 0, class V>
auto dvec(const V (&sz)[d], const T &init)
{
  if constexpr (i < d)
    return vc(sz[i], dvec<T, d, i + 1>(sz, init));
  else
    return init;
}

template <class T = ll>
T ctol(const char &c, const string &s)
{
  repi(i, SZ<int>(s)) if (s[i] == c) return i;
  return -1;
}
template <class T = ll>
vc<T> stov(const string &s, char first)
{
  return gen_vec(SZ<int>(s), [&](int i) -> T
                 { return s[i] - first; });
}
template <class T = ll>
vc<T> stov(const string &s, const string &t)
{
  return gen_vec(SZ<int>(s), [&](int i) -> T
                 { return ctol(s[i], t); });
}
template <class T>
string vtos(const vc<T> &v, char first)
{
  string res = "";
  fe(vi : v) res += vi + first;
  return res;
}
template <class T>
string vtos(const vc<T> &v, const string &t)
{
  string res = "";
  fe(vi : v) res += t[vi];
  return res;
}

template <class T>
vc<T> concat(const vvc<T> &vs)
{
  vc<T> res;
  for (cauto &v : vs)
    res.insert(res.end(), ALL(v));
  return res;
}
template <class T>
vc<T> concat(const vc<T> &v) { return v; }
template <class T, class... Ts>
vc<T> concat(vc<T> v, const vc<Ts> &...vs)
{
  (v.insert(v.end(), ALL(vs)), ...);
  return v;
}

template <class T>
vc<T> merged(const vc<T> &a, const vc<T> &b)
{
  vc<T> res;
  merge(ALL(a), ALL(b), back_inserter(res));
  return res;
}

template <class T, class I>
T vecget(const vc<T> &v, I i, const T &dflt_negative = -default_infty<T>(), const T &dflt_positive = default_infty<T>())
{
  if (i < 0)
    return dflt_negative;
  if (i >= SZ<int>(v))
    return dflt_positive;
  return v[i];
}
// https://github.com/miscalculation53/library/tree/wip/template/template_algo.hpp

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif

// https://github.com/miscalculation53/library/tree/wip/utils/resolved_infty.hpp

// https://github.com/miscalculation53/library/tree/wip/utils/resolved_value.hpp


/**
 * @brief 値または関数から値を取得
 * @docs docs/utils/resolved_value.md
 */

template <class T, auto x, enable_if_t<!is_invocable_v<decltype(x)>, int> = 0>
constexpr T resolved_value()
{
  return T(x);
}

template <class T, auto x, enable_if_t<is_invocable_v<decltype(x)>, long> = 0>
const T &resolved_value()
{
  static const T value = T(x());
  return value;
}

/**
 * @brief 既定値・値・関数から無限大を取得
 * @docs docs/utils/resolved_infty.md
 */

template <class T, auto x = nullptr>
constexpr decltype(auto) resolved_infty()
{
  if constexpr (is_same_v<decltype(x), nullptr_t>)
    return default_infty<T>();
  else
    return resolved_value<T, x>();
}

/**
 * @brief テンプレート（アルゴリズム）
 * @docs docs/template/template_algo.md
 */

template <class V>
auto SUM(const V &v)
{
  typename V::value_type s{};
  fec(vi : v) s += vi;
  return s;
}
template <class T, class V>
T SUM(const V &v)
{
  T s{};
  fec(vi : v) s += vi;
  return s;
}
template <class V>
auto MAX(const V &v) { return *max_element(ALL(v)); }
template <class V>
auto MIN(const V &v) { return *min_element(ALL(v)); }
template <class I = ll, class V>
I ARGMAX(const V &v) { return max_element(ALL(v)) - v.begin(); }
template <class I = ll, class V>
I ARGMIN(const V &v) { return min_element(ALL(v)) - v.begin(); }

template<class T = ll, class V>
T mex(const V &a)
{
  int n = a.size();
  vector<bool> exists(n, false);
  repi(i, n) if (0 <= a[i] && a[i] < n) exists[a[i]] = true;
  repi(x, n) if (!exists[x]) return x;
  return n;
}

// (0, 1. ..., n-1) の順列か判定
template <class I>
bool is_permutation(const vc<I> &p)
{
  const int n = p.size();
  vc<bool> b(n, false);
  repi(i, n)
  {
    if (!(0 <= p[i] && p[i] < n))
      return false;
    b[p[i]] = true;
  }
  return all_of(ALL(b), [](bool bi)
                { return bi; });
}

template <class T = ll>
vc<T> permid(const int &n, const int &base_index = 0)
{
  vc<T> p(n);
  repi(i, n) p[i] = i + base_index;
  return p;
}
template <class T>
vc<T> perminv(const vc<T> &p)
{
  if (p.empty())
    return {};
  const int n = p.size();
  vc<T> q(MAX(p) + 1);
  repi(i, n) if (p[i] >= 0) q[p[i]] = i;
  return q;
}
// a[p[i]] for all i
template <class T, class U>
vc<T> permuted(const vc<T> &a, const vc<U> &p)
{
  const int n = p.size();
  vc<T> res(n);
  repi(i, n)
  {
    assert(0 <= p[i] && p[i] < U(a.size()));
    res[i] = a[p[i]];
  }
  return res;
}
// p[q[r[i]]] for all i など
template <class T, class U, class... Ts>
vc<T> permuted(const vc<T> &p, const vc<U> &q, const vc<Ts> &...rs)
{
  return permuted(permuted(p, q), rs...);
}

template <class V>
V reversed(const V &v) { return V(v.rbegin(), v.rend()); }

#if __cplusplus < 202002L
template <class V, class... Args>
V sorted(V v, Args&&... args)
{
  sort(ALL(v), forward<Args>(args)...);
  return v;
}
#else
template <class V, class... Args>
V sorted(V v, Args&&... args)
{
  ranges::sort(v, forward<Args>(args)...);
  return v;
}
#endif

template <class V, class Equal = equal_to<>>
void unique(V &v, Equal equal = {}) { v.erase(std::unique(ALL(v), equal), v.end()); }
template <class V, class Equal = equal_to<>>
V uniqued(V v, Equal equal = {}) { unique(v, equal); return v; }

template <class V, class Compare = less<>, class Equal = equal_to<>>
void sortunique(V &v, Compare comp = {}, Equal equal = {})
{
  sort(ALL(v), comp);
  unique(v, equal);
}
template <class V, class Compare = less<>, class Equal = equal_to<>>
V sortuniqued(V v, Compare comp = {}, Equal equal = {})
{ sortunique(v, comp, equal); return v; }

// 01234 -> 12340
template <class V, class U>
void rotate(V &v, U k)
{ 
  const U n = v.size();
  if (n == 0)
    return;
  k = (k % n + n) % n;
  std::rotate(v.begin(), v.begin() + k, v.end());
}
// 01234 -> 12340
template <class V, class U>
V rotated(V v, U k) { rotate(v, k); return v; }

template <class T>
vvc<T> top(const vvc<T> &a)
{
  if (a.empty())
    return {};
  const int n = a.size(), m = a[0].size();
  vvc<T> b(m, vc<T>(n));
  repi(i, n)
  {
    assert(SZ<int>(a[i]) == m);
    repi(j, m) b[j][i] = a[i][j];
  }
  return b;
}
vstr top(const vstr &a)
{
  vvc<char> a_(a.size());
  repi(i, SZ<int>(a)) a_[i] = {ALL(a[i])};
  vvc<char> b_ = top(a_);
  vstr b(b_.size());
  repi(i, SZ<int>(b)) b[i] = {ALL(b_[i])};
  return b;
}

template <class T, class = void>
struct has_e0 : false_type {};
template <class T>
struct has_e0<T, void_t<decltype(T::e0())>> : true_type {};
template <class T>
inline constexpr bool has_e0_v = has_e0<T>::value;

template <class T>
struct MonoidAdd
{
  using S = T;
  static constexpr S op(S a, S b) { return a + b; }
  static constexpr S e()
  {
    if constexpr (has_e0_v<S>)
      return S::e0();
    else
      return {};
  }
  template <class I, class = decltype(declval<S>() * declval<I>())>
  static constexpr S pow(const S &a, I k) { return a * k; }
};
template <class T, auto infty = nullptr>
struct MonoidMin
{
  using S = T;
  static constexpr S op(S a, S b) { return min(a, b); }
  static constexpr decltype(auto) e() { return resolved_infty<T, infty>(); }
  template <class I>
  static constexpr S pow(const S &a, I k) { return k == 0 ? e() : a; }
};
template <class T, auto infty = nullptr>
struct MonoidMax
{
  using S = T;
  static constexpr S op(S a, S b) { return max(a, b); }
  static constexpr S e() { return -resolved_infty<T, infty>(); }
  template <class I>
  static constexpr S pow(const S &a, I k) { return k == 0 ? e() : a; }
};

namespace internal
{
  template <class M, class I, class = void>
  struct HasMonoidPow : false_type
  {
  };
  template <class M, class I>
  struct HasMonoidPow<M, I, void_t<decltype(M::pow(declval<const typename M::S &>(), declval<I>()))>> : true_type
  {
  };
}

template <class M, class I>
typename M::S pow_monoid(typename M::S a, I k)
{
  if constexpr (is_signed_ext<I>)
    assert(k >= 0);
  if constexpr (internal::HasMonoidPow<M, I>::value)
    return M::pow(a, k);
  else
  {
    typename M::S c = M::e();
    for (; k; k >>= 1)
    {
      if (k & 1)
        c = M::op(c, a);
      a = M::op(a, a);
    }
    return c;
  }
}

template <class G, class I>
typename G::S pow_group(typename G::S a, I k)
{
  if constexpr (is_signed_ext<I>)
  {
    if (k < 0)
    {
      a = G::inv(a);
      return G::op(pow_monoid<G>(a, -(k + 1)), a);
    }
  }
  return pow_monoid<G>(a, k);
}

// left_index が 0 なら、長さ n+1 で a.front() が e()
// left_index が 1 なら、長さ n で e() がない
template <class M>
vc<typename M::S> cuml(const vc<typename M::S> &v, int left_index = 0)
{
  const int n = v.size();
  vc<typename M::S> res(n + 1);
  res[0] = M::e();
  repi(i, n) res[i + 1] = M::op(res[i], v[i]);
  res.erase(res.begin(), res.begin() + left_index);
  return res;
}
// right_index が 0 なら、長さ n+1 で a.back() が e()
// right_index が 1 なら、長さ n で e() がない
template <class M>
vc<typename M::S> cumr(const vc<typename M::S> &v, int right_index = 0)
{ return reversed(cuml<M>(reversed(v), right_index)); }
template <class T>
vc<T> cumlsum(const vc<T> &v, int left_index = 0)
{ return cuml<MonoidAdd<T>>(v, left_index); }
template <class T>
vc<T> cumrsum(const vc<T> &v, int right_index = 0)
{ return cumr<MonoidAdd<T>>(v, right_index); }
template <class T>
vc<T> cumlmin(const vc<T> &v, int left_index = 0)
{ return cuml<MonoidMin<T>>(v, left_index); }
template <class T>
vc<T> cumrmin(const vc<T> &v, int right_index = 0)
{ return cumr<MonoidMin<T>>(v, right_index); }
template <class T>
vc<T> cumlmax(const vc<T> &v, int left_index = 0)
{ return cuml<MonoidMax<T>>(v, left_index); }
template <class T>
vc<T> cumrmax(const vc<T> &v, int right_index = 0)
{ return cumr<MonoidMax<T>>(v, right_index); }

// デフォルトでは長さ n+1
// left_index, right_index をそれぞれ 1 にすると、左右が削除される
template <class T>
vc<T> adjd(const vc<T> &v, int left_index = 0, int right_index = 0)
{
  int n = v.size();
  assert(0 <= left_index && 0 <= right_index && left_index + right_index <= n + 1);
  vc<T> res(n + 1);
  if (n == 0)
  {
    res[0] = T{};
    res.erase(res.end() - right_index, res.end());
    res.erase(res.begin(), res.begin() + left_index);
    return res;
  }
  res[0] = v[0];
  repi(i, 1, n) res[i] = v[i] - v[i - 1];
  res[n] = -v[n - 1];
  res.erase(res.end() - right_index, res.end());
  res.erase(res.begin(), res.begin() + left_index);
  return res;
}

constexpr array<pll, 4> DRULgrid = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};
constexpr array<pll, 4> DRULplane = {{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};
// https://github.com/miscalculation53/library/tree/wip/template/template_binsearch.hpp


/**
 * @brief テンプレート（二分探索）
 * @docs docs/template/template_binsearch.md
 */

template <class T>
struct is_random_access_iterator
{
  static constexpr bool value = is_same_v<
    typename iterator_traits<T>::iterator_category,
    random_access_iterator_tag
  >;
};
template <class T>
constexpr bool is_random_access_iterator_v = is_random_access_iterator<T>::value;

// --- LB, UB ---

#if __cplusplus < 202002L
struct identity
{
  template <class T>
  constexpr T &&operator()(T &&t) const noexcept
  { return forward<T>(t); }
};
namespace internal
{
  template <class T = ll, class V, class Judge>
  inline T bound_helper(const V &v, Judge judge)
  {
    int l = -1, r = v.size();
    while (r - l > 1)
    {
      int m = (l + r) / 2;
      if (judge(m))
        l = m;
      else
        r = m;
    }
    return r;
  }
};
// val <= v[i] となる最小の i (val 未満の値の個数)
template <class T = ll, class V, class Value, class Comp = less<>, class Proj = identity>
inline T LB(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
{
  return internal::bound_helper(v, [&](int i) -> bool
                                { return comp(proj(*(v.begin() + i)), val); });
}
// val < v[i] となる最小の i (val 以下の値の個数)
template <class T = ll, class V, class Value, class Comp = less<>, class Proj = identity>
inline T UB(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
{
  return internal::bound_helper(v, [&](int i) -> bool
                                { return !comp(val, proj(*(v.begin() + i))); });
}
#define DEFAULT_COMP less<>
#else
// val <= v[i] となる最小の i (val 未満の値の個数)
template <class T = ll, class V, class Value, class Comp = ranges::less, class Proj = identity>
inline T LB(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
{ return ranges::lower_bound(v, val, comp, proj) - v.begin(); }
// val < v[i] となる最小の i (val 以下の値の個数)
template <class T = ll, class V, class Value, class Comp = ranges::less, class Proj = identity>
inline T UB(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
{ return ranges::upper_bound(v, val, comp, proj) - v.begin(); }
#define DEFAULT_COMP ranges::less
#endif

// --- vector 等の lt, leq, gt, geq ---

// v[i] < val となる最大の i (なければ -1)
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto lt_max(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return LB<T>(v, val, comp, proj) - 1; }
// v[i] <= val となる最大の i (なければ -1)
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto leq_max(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return UB<T>(v, val, comp, proj) - 1; }
// val < v[i] となる最小の i (なければ n)
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto gt_min(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return UB<T>(v, val, comp, proj); }
// val <= v[i] となる最小の i (なければ n)
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto geq_min(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return LB<T>(v, val, comp, proj); }
// v[i] < val となる i の個数
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto lt_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return LB<T>(v, val, comp, proj); }
// v[i] <= val となる i の個数
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto leq_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return UB<T>(v, val, comp, proj); }
// val < v[i] となる i の個数
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto gt_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return SZ<T>(v) - UB<T>(v, val, comp, proj); }
// val <= v[i] となる i の個数
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto geq_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return SZ<T>(v) - LB<T>(v, val, comp, proj); }
// l <= v[i] < r となる i の個数
template <class T = ll, class V, class L, class R, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto in_cnt(const V &v, L l, R r, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{
  if (l > r)
    return 0;
  return lt_cnt<T>(v, r, comp, proj) - lt_cnt<T>(v, l, comp, proj);
}

// --- set 等の lt, leq, gt, geq ---

// *it < val となる最大の it (なければ end())
template <class V, class Value>
inline auto lt_max(const V &v, const Value &val)
-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>, typename V::const_iterator>
{
  auto it = v.lower_bound(val);
  return it == v.begin() ? v.end() : prev(it);
}
// *it <= val となる最大の it (なければ end())
template <class V, class Value>
inline auto leq_max(const V &v, const Value &val)
-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>, typename V::const_iterator>
{
  auto it = v.upper_bound(val);
  return it == v.begin() ? v.end() : prev(it);
}
// val < *it となる最小の it (なければ end())
template <class V, class Value>
inline auto gt_min(const V &v, const Value &val)
-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>, typename V::const_iterator>
{ return v.upper_bound(val); }
// val <= *it となる最小の it (なければ end())
template <class V, class Value>
inline auto geq_min(const V &v, const Value &val)
-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>, typename V::const_iterator>
{ return v.lower_bound(val); }

// --- 自作二分探索 ---

namespace internal
{
template <class T>
bool binsearch_adjacent(T a, T b)
{
  if (a < b)
    return a + 1 == b;
  if (b < a)
    return b + 1 == a;
  return false;
}
};

// (ok, ng)
template <class T = ll, class Judge, class InitOk, class InitNg>
pair<T, T> binsearch(const Judge &judge, InitOk init_ok, InitNg init_ng, bool check_ok = true, bool check_ng = true)
{
  T ok(init_ok), ng(init_ng);
  if (check_ok)
    assert(judge(ok));
  if (check_ng)
    assert(!judge(ng));
  while (!internal::binsearch_adjacent(ok, ng))
  {
    T mid = (ok & ng) + ((ok ^ ng) >> 1);
    (judge(mid) ? ok : ng) = mid;
  }
  return {ok, ng};
}
template <class T = ld, class Judge, class InitOk, class InitNg>
T binsearch_real(const Judge &judge, InitOk init_ok, InitNg init_ng, int iteration_count = 100, bool check_ok = true, bool check_ng = true)
{
  T ok(init_ok), ng(init_ng);
  if (check_ok)
    assert(judge(ok));
  if (check_ng)
    assert(!judge(ng));
  repi(_, iteration_count)
  {
    T mid = (ok + ng) / 2;
    (judge(mid) ? ok : ng) = mid;
  }
  return ok;
}
// (ok, ng)
template <class T = ll, class Judge, class InitVal>
pair<T, T> expsearch(const Judge &judge, InitVal init_val, bool positive = true)
{
  T cur(init_val), step = 1;
  const bool cur_ok = judge(cur);
  auto advance = [&](T x, T d, bool pos) -> T
  {
    if (pos)
      return x > numeric_limits<T>::max() - d ? numeric_limits<T>::max() : x + d;
    else
      return x < numeric_limits<T>::lowest() + d ? numeric_limits<T>::lowest() : x - d;
  };
  T prv = advance(cur, 1, !positive);
  if (prv != cur && judge(prv) != cur_ok)
  {
    if (cur_ok)
      return {cur, prv};
    else
      return {prv, cur};
  }
  while (true)
  {
    T nxt = advance(cur, step, positive);
    assert(nxt != cur && "the boundary must exist in the searched direction");
    if (nxt == cur || judge(nxt) != cur_ok)
    {
      T ok = cur_ok ? cur : nxt;
      T ng = cur_ok ? nxt : cur;
      return binsearch<T>(judge, ok, ng, false, false);
    }
    cur = nxt;
    if (step > numeric_limits<T>::max() / 2)
      step = numeric_limits<T>::max();
    else
      step *= 2;
  }
}
// https://github.com/miscalculation53/library/tree/wip/template/template_bit.hpp


/**
 * @brief テンプレート（ビット演算）
 * @docs docs/template/template_bit.md
 */

template <class T>
inline constexpr ull pow2(T k) { return 1ULL << k; }
template <class T>
inline constexpr ull MASK(T k) { return (1ULL << k) - 1ULL; }

#if __cplusplus < 202002L
// x == 0 ならば 0、そうでなければ 1 + floor(log2(x))
// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... 
inline constexpr ull bit_width(ull x) { return x == 0 ? 0 : 64 - __builtin_clzll(x); }
// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...
inline constexpr ull bit_floor(ull x) { return x == 0 ? 0ULL : 1ULL << (bit_width(x) - 1); }
// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...
inline constexpr ull bit_ceil(ull x) { return x == 0 ? 1ULL : 1ULL << bit_width(x - 1); }
inline constexpr ull countr_zero(ull x) { assert(x != 0); return __builtin_ctzll(x); }
inline constexpr ull popcount(ull x) { return __builtin_popcountll(x); }
inline constexpr bool has_single_bit(ull x) { return popcount(x) == 1; }
#else
// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... 
inline constexpr ll bit_width(ll x) { return std::bit_width((ull)x); }
// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...
inline constexpr ll bit_floor(ll x) { return std::bit_floor((ull)x); }
// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...
inline constexpr ll bit_ceil(ll x) { return std::bit_ceil((ull)x); }
inline constexpr ll countr_zero(ll x) { assert(x != 0); return std::countr_zero((ull)x); }
inline constexpr ll popcount(ll x) { return std::popcount((ull)x); }
inline constexpr bool has_single_bit(ll x) { return std::has_single_bit((ull)x); }
#endif

inline constexpr ull lsb_pos(ull x) { assert(x != 0); return countr_zero(x); }
inline constexpr ull msb_pos(ull x) { assert(x != 0); return bit_width(x) - 1; }
inline constexpr ull lsb_mask(ull x) { assert(x != 0); return x & -x; }
inline constexpr ull msb_mask(ull x) { assert(x != 0); return bit_floor(x); }

inline constexpr bool btest(ull x, uint k) { return (x >> k) & 1; }
template <class T>
inline void bset(T &x, uint k, bool b = 1) { b ? x |= (1ULL << k) : x &= ~(1ULL << k); }
template <class T>
inline void bflip(T &x, uint k) { x ^= (1ULL << k); }
inline constexpr bool bsubset(ull x, ull y) { return (x & y) == x; }
inline constexpr bool bsupset(ull x, ull y) { return (x & y) == y; }
inline constexpr ull bsetminus(ull x, ull y) { return x & ~y; }
// https://github.com/miscalculation53/library/tree/wip/template/template_inout.hpp

// https://github.com/miscalculation53/library/tree/wip/template/template_dump.hpp

// https://github.com/miscalculation53/library/tree/wip/template/template_dump_map.hpp


#ifdef LOCAL
// cpp-dump 内の再帰的な出力からも、map 用のオーバーロードを参照できるようにする。
namespace cpp_dump::_detail
{
  struct export_command;
#define _p_LIBRARY_DECLARE_MAP_EXPORT(Map)                                  \
  template <class... Args>                                                 \
  string export_var(const std::Map<Args...> &, const string &, size_t,       \
                    size_t, bool, const export_command &);
  _p_LIBRARY_DECLARE_MAP_EXPORT(map)
  _p_LIBRARY_DECLARE_MAP_EXPORT(multimap)
  _p_LIBRARY_DECLARE_MAP_EXPORT(unordered_map)
  _p_LIBRARY_DECLARE_MAP_EXPORT(unordered_multimap)
#undef _p_LIBRARY_DECLARE_MAP_EXPORT
}

#include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump

namespace cpp_dump::options
{
  inline string map_key_color = "\x1b[36m";
}

namespace cpp_dump::_detail
{
  template <class It, class Format>
  struct map_values_for_dump
  {
    It first, last;
    const Format &format;
    map_values_for_dump(It first, It last, const Format &format) : first(first), last(last), format(format) {}
    struct iterator
    {
      It it;
      const Format &format;
      decltype(auto) operator*() const { return format(it->second); }
      iterator &operator++() { ++it; return *this; }
      bool operator!=(const iterator &other) const { return it != other.it; }
    };
    iterator begin() const { return {first, format}; }
    iterator end() const { return {last, format}; }
  };

  inline string color_map_key(const string &text)
  {
    if (!use_es() || options::map_key_color.empty()) return text;
    string plain;
    for (size_t i = 0; i < text.size();)
    {
      if (text[i] == '\x1b' && i + 1 < text.size() && text[i + 1] == '[')
      {
        size_t end = i + 2;
        while (end < text.size() && (isdigit(static_cast<unsigned char>(text[end])) || text[end] == ';')) ++end;
        if (end < text.size() && text[end] == 'm')
        {
          i = end + 1;
          continue;
        }
      }
      plain += text[i++];
    }
    return es::apply(options::map_key_color, plain);
  }

  template <class Map, class Format>
  string export_library_map(
      const Map &map, const string &indent, size_t last_line_length,
      size_t current_depth, bool fail_on_newline, const export_command &command,
      const Format &format)
  {
    if (map.empty()) return es::bracket("{ }", current_depth);
    if (current_depth >= options::max_depth)
      return es::bracket("{ ", current_depth) + es::op("...") + es::bracket(" }", current_depth);

    const auto &key_command = command.next_for_map_key();
    const auto &value_command = command.next_for_map_value();
    const size_t next_depth = current_depth + 1;
    auto map_wrapper = [&]()
    {
      if constexpr (is_multimap<Map>) return _multimap_wrapper(map);
      else return _map_dummy_wrapper(map);
    }();

    bool multiline = options::cont_indent_style == types::cont_indent_style_t::always;
    if (options::cont_indent_style == types::cont_indent_style_t::when_nested)
      multiline = is_multimap<Map> || is_iterable_like<typename Map::key_type> || is_iterable_like<typename Map::mapped_type>;
    if (options::cont_indent_style == types::cont_indent_style_t::when_non_tuples_nested)
      multiline = is_multimap<Map>
          || (is_iterable_like<typename Map::key_type> && !is_tuple<typename Map::key_type>)
          || (is_iterable_like<typename Map::mapped_type> && !is_tuple<typename Map::mapped_type>);

    for (;; multiline = true)
    {
      if (multiline && fail_on_newline) return "\n";
      const string element_indent = multiline ? indent + "  " : indent;
      string output = es::bracket(multiline ? "{" : "{ ", current_depth);
      bool first = true, retry = false;
      auto skipped_map = command.create_skip_container(map_wrapper);
      for (const auto &[ellipsis, it, index] : skipped_map)
      {
        if (!first) output += es::op(multiline ? "," : ", ");
        first = false;
        if (multiline) output += "\n" + element_indent;
        if (ellipsis) output += es::op("...");
        else
        {
          const auto &[key, value] = *it;
          auto render = [&](const auto &x, const export_command &child_command)
          {
            return export_var(format(x), element_indent,
                              get_last_line_length(output, last_line_length),
                              next_depth, !multiline, child_command);
          };
          output += color_map_key(render(key, key_command));
          if constexpr (is_multimap<Map>)
            output += es::member(" (" + to_string(map.count(key)) + ")");
          output += es::op(": ");
          if constexpr (is_multimap<Map>)
          {
            auto [begin, end] = map.equal_range(key);
            // この view は所有しない。描画中だけ元の map を参照する。
            auto values = map_values_for_dump(begin, end, format);
            output += export_var(values, element_indent,
                                 get_last_line_length(output, last_line_length),
                                 next_depth, !multiline, value_command);
          }
          else output += render(value, value_command);
        }
        if (!multiline && (has_newline(output) || last_line_length + get_length(output) + 2 > options::max_line_width))
        {
          retry = true;
          break;
        }
      }
      if (retry) continue;
      return output + (multiline ? "\n" + indent : " ") + es::bracket("}", current_depth);
    }
  }

#define _p_LIBRARY_DEFINE_MAP_EXPORT(Map)                                                \
  template <class... Args>                                                             \
  string export_var(const std::Map<Args...> &value, const string &indent,                \
                    size_t last_line_length, size_t current_depth,                     \
                    bool fail_on_newline, const export_command &command)              \
  {                                                                                   \
    return export_library_map(value, indent, last_line_length, current_depth,           \
                              fail_on_newline, command,                               \
                              [](const auto &x) -> const auto & { return x; });        \
  }
  _p_LIBRARY_DEFINE_MAP_EXPORT(map)
  _p_LIBRARY_DEFINE_MAP_EXPORT(multimap)
  _p_LIBRARY_DEFINE_MAP_EXPORT(unordered_map)
  _p_LIBRARY_DEFINE_MAP_EXPORT(unordered_multimap)
#undef _p_LIBRARY_DEFINE_MAP_EXPORT
}
#endif

/**
 * @brief テンプレート（dump）
 * @docs docs/template/template_dump.md
 */

#ifdef LOCAL
namespace cpp_dump::_detail
{
  inline string export_var(
      const i128 &x, const string &, size_t,
      size_t, bool, const export_command &
  ) {
    return es::signed_number(i128tos(x));
  }

  template <class T>
  inline auto export_object_generic(
      const T &value, const string &indent, size_t last_line_length,
      size_t current_depth, bool fail_on_newline, const export_command &command
  ) -> decltype(value.dump_data(), string())
  {
    string class_name = es::class_name(get_typename<T>());
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;
    apply(
        [&](const auto &...member)
        { (append_output(member.first, member.second.get()), ...); },
        value.dump_data()
    );
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;
  }
} // namespace cpp_dump::_detail
#define _p_LIBRARY_CPP_DUMP_MEMBER(member) pair{string_view(#member), cref(member)}
#define CPP_DUMP_DEFINE_DATA(...)                                                      \
  auto dump_data() const                                                               \
  {                                                                                    \
    return make_tuple(_p_CPP_DUMP_EXPAND_VA(_p_LIBRARY_CPP_DUMP_MEMBER, __VA_ARGS__)); \
  }
#define dump(...) cpp_dump(__VA_ARGS__)
namespace cp = cpp_dump;
CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());
CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 100);
#define local(...) __VA_ARGS__
#define oj(...)
#define local_oj(a, b) (a)
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(content());
#else
#define CPP_DUMP_DEFINE_DATA(...)
#define dump(...) ((void)0)
#define local(...)
#define oj(...) __VA_ARGS__
#define local_oj(a, b) (b)
#endif

template <class T, class Sequence>
vc<T> content(queue<T, Sequence> que)
{
  vc<T> res;
  while (!que.empty())
  {
    res.eb(que.front());
    que.pop();
  }
  return res;
}
template <class T, class Sequence, class Compare>
vc<T> content(priority_queue<T, Sequence, Compare> pque)
{
  vc<T> res;
  while (!pque.empty())
  {
    res.eb(pque.top());
    pque.pop();
  }
  return res;
}
template <class T>
auto content(const T &obj) { return obj.content(); }

/**
 * @brief テンプレート（入出力）
 * @docs docs/template/template_inout.md
 */

// https://judge.yosupo.jp/submission/170706 (maspy さん)
// https://judge.yosupo.jp/submission/21623  (Nyaan さん)
#if defined FAST_IO and not defined LOCAL
namespace fastio {
template <class T>
struct unsigned_integer
{
  using type = make_unsigned_t<T>;
};
template <>
struct unsigned_integer<i128>
{
  using type = u128;
};
template <>
struct unsigned_integer<u128>
{
  using type = u128;
};
template <class T>
using unsigned_integer_t = typename unsigned_integer<T>::type;

static constexpr uint32_t SIZ = 1 << 17;
char ibuf[SIZ];
char obuf[SIZ];
char out[100];
// pointer of ibuf, obuf
uint32_t pil = 0, pir = 0, por = 0;

struct Pre {
  char num[10000][4];
  constexpr Pre() : num() {
    for (int i = 0; i < 10000; i++) {
      int n = i;
      for (int j = 3; j >= 0; j--) {
        num[i][j] = n % 10 | '0';
        n /= 10;
      }
    }
  }
} constexpr pre;

inline void load() {
  memcpy(ibuf, ibuf + pil, pir - pil);
  pir = pir - pil + fread(ibuf + pir - pil, 1, SIZ - pir + pil, stdin);
  pil = 0;
  if (pir < SIZ) ibuf[pir++] = '\n';
}

inline void flush() {
  fwrite(obuf, 1, por, stdout);
  por = 0;
}

void rd1(char &c) {
  do {
    if (pil + 1 > pir) load();
    c = ibuf[pil++];
  } while (c <= ' ');
}

void rd1(string &x) {
  x.clear();
  while (true) {
    if (pil == pir) load();
    while (pil < pir && ibuf[pil] <= ' ') ++pil;
    if (pil < pir) break;
  }
  while (true) {
    uint32_t p = pil;
    while (pil < pir && ibuf[pil] > ' ') ++pil;
    x.append(ibuf + p, pil - p);
    if (pil < pir) {
      ++pil;
      return;
    }
    load();
  }
}

template <typename T>
void rd1_real(T &x) {
  string s;
  rd1(s);
#if __cplusplus >= 202002L
  if constexpr (!is_same_v<T, long double>)
  {
    auto [p, ec] = from_chars(s.data(), s.data() + s.size(), x);
    if (ec == errc{} && p == s.data() + s.size()) return;
  }
#endif
  if constexpr (is_same_v<T, long double>)
    x = stold(s);
  else
    x = stod(s);
}

template <bool check_buffer = true, typename T>
void rd1_integer(T &x) {
  using U = unsigned_integer_t<T>;
  bool minus = false;
  U val = 0;
  if constexpr (check_buffer)
    if (pil + 100 > pir) load();
  uint32_t p = pil;
  while (ibuf[p] < '-') ++p;
  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {
    if (ibuf[p] == '-') minus = true, ++p;
  }
  while ('0' <= ibuf[p]) val = val * 10 + (ibuf[p++] & 15);
  pil = p;
  if constexpr (is_signed<T>::value || is_same_v<T, i128>)
  {
    if (minus)
    {
      const U min_abs = U(numeric_limits<T>::max()) + 1;
      x = val == min_abs ? numeric_limits<T>::lowest() : -T(val);
    }
    else x = T(val);
  }
  else
    x = T(val);
}

void rd1(int &x) { rd1_integer(x); }
void rd1(ll &x) { rd1_integer(x); }
void rd1(i128 &x) { rd1_integer(x); }
void rd1(uint &x) { rd1_integer(x); }
void rd1(ull &x) { rd1_integer(x); }
void rd1(u128 &x) { rd1_integer(x); }
void rd1(double &x) { rd1_real(x); }
void rd1(long double &x) { rd1_real(x); }
// void rd1(f128 &x) { rd1_real(x); }

template <class T, class U>
void rd1(pair<T, U> &p) {
  return rd1(p.first), rd1(p.second);
}
template <class... T>
void rd1(tuple<T...> &tpl) {
  apply([](auto &...x) { (rd1(x), ...); }, tpl);
}

template <size_t N = 0, typename T>
void rd1(array<T, N> &x) {
  for (auto &d: x) rd1(d);
}
template <class T>
void rd1(vc<T> &x) {
  for (auto &d: x) rd1(d);
}

template <class... T>
void read(T &...x) {
  if constexpr (sizeof...(T) <= SIZ / 100 &&
                ((!is_same_v<T, char> &&
                  (is_integral_v<T> || is_same_v<T, i128> || is_same_v<T, u128>)) && ...)) {
    if (pil + 100 * sizeof...(T) > pir) load();
    (rd1_integer<false>(x), ...);
  }
  else
    (rd1(x), ...);
}

void wt1(const char c) {
  if (por == SIZ) flush();
  obuf[por++] = c;
}
void wt1(string_view s) {
  while (!s.empty()) {
    if (por == SIZ) flush();
    size_t n = min<size_t>(s.size(), SIZ - por);
    memcpy(obuf + por, s.data(), n);
    por += n;
    s.remove_prefix(n);
  }
}

template <typename T>
void wt1_integer(T x) {
  if (por > SIZ - 100) flush();
  using U = unsigned_integer_t<T>;
  U ux;
  if constexpr (is_signed<T>::value || is_same_v<T, i128>)
  {
    if (x < 0)
      obuf[por++] = '-', ux = U(0) - U(x);
    else
      ux = U(x);
  }
  else
    ux = x;
  int outi;
  for (outi = 96; ux >= 10000; outi -= 4) {
    memcpy(out + outi, pre.num[ux % 10000], 4);
    ux /= 10000;
  }
  if (ux >= 1000) {
    memcpy(obuf + por, pre.num[ux], 4);
    por += 4;
  } else if (ux >= 100) {
    memcpy(obuf + por, pre.num[ux] + 1, 3);
    por += 3;
  } else if (ux >= 10) {
    int q = (ux * 103) >> 10;
    obuf[por] = q | '0';
    obuf[por + 1] = (ux - q * 10) | '0';
    por += 2;
  } else
    obuf[por++] = ux | '0';
  memcpy(obuf + por, out + outi + 4, 96 - outi);
  por += 96 - outi;
}

template <typename T>
void wt1_real(T x) {
#if __cplusplus >= 202002L
  if constexpr (!is_same_v<T, long double>)
  {
    auto [p, ec] = to_chars(out, out + sizeof(out), x, chars_format::fixed, 15);
    if (ec == errc{}) {
      wt1(string_view(out, p));
      return;
    }
  }
#endif
  ostringstream oss;
  oss << fixed << setprecision(15) << x;
  wt1(oss.str());
}

void wt1(int x) { wt1_integer(x); }
template <class T, enable_if_t<is_integral_v<T>, int> = 0>
void wt1(T x) { wt1_integer(x); }
void wt1(i128 x) { wt1_integer(x); }
void wt1(u128 x) { wt1_integer(x); }
void wt1(double x) { wt1_real(x); }
void wt1(long double x) { wt1_real(x); }
// void wt1(f128 x) { wt1_real(x); }

template <class T, class U>
void wt1(const pair<T, U> &val) {
  wt1(val.first);
  wt1(' ');
  wt1(val.second);
}
template <class... T>
void wt1(const tuple<T...> &tpl) {
  if constexpr (sizeof...(T))
  {
    int i = 0;
    apply([&](const auto &...x)
          { ((i++ ? wt1(' ') : void(), wt1(x)), ...); }, tpl);
  }
}
template <class T, size_t S>
void wt1(const array<T, S> &val) {
  auto n = val.size();
  for (size_t i = 0; i < n; i++) {
    if (i) wt1(' ');
    wt1(val[i]);
  }
}
template <class T>
void wt1(const vector<T> &val) {
  auto n = val.size();
  for (size_t i = 0; i < n; i++) {
    if (i) wt1(' ');
    wt1(val[i]);
  }
}

template <class... T>
void write(T &&...x) {
  (wt1(std::forward<T>(x)), ...);
}

template <class... T>
void print(T &&...x) {
  if constexpr (sizeof...(T))
  {
    int i = 0;
    ((i++ ? wt1(' ') : void(), wt1(std::forward<T>(x))), ...);
  }
  wt1('\n');
}

} // namespace fastio

#endif

#if defined FAST_IO and not defined LOCAL
struct Dummy {
  Dummy() { atexit(fastio::flush); }
} dummy;
#endif

// https://trap.jp/post/1224/

// ---- 入力 ----
#if defined LOCAL or not defined FAST_IO
template <class T, class U>
istream &operator>>(istream &is, pair<T, U> &p)
{
  is >> p.first >> p.second;
  return is;
}
template <class... Ts>
istream &operator>>(istream &is, tuple<Ts...> &t)
{
  apply([&](auto &...a)
        { (is >> ... >> a); }, t);
  return is;
}
template <class T, size_t n>
istream &operator>>(istream &is, array<T, n> &a)
{
  for (size_t i = 0; i < n; i++)
    is >> a[i];
  return is;
}
template <class T>
istream &operator>>(istream &is, vc<T> &a)
{
  const size_t n = a.size();
  for (size_t i = 0; i < n; i++)
    is >> a[i];
  return is;
}
#endif

namespace internal
{

#if defined LOCAL or not defined FAST_IO
template <class... Ts>
void CIN(Ts &...a) { (cin >> ... >> a); }
#endif

#if defined FAST_IO and not defined LOCAL
template <class... Ts>
void READnodump(Ts &...a) { fastio::read(a...); }
#else
template <class... Ts>
void READnodump(Ts &...a) { CIN(a...); }
#endif

template <class... T>
void READVECnodump(int n, vc<T> &...v)
{
  (v.resize(n), ...);
  READnodump(v...);
}

template <class... T>
void READVEC2nodump(int n, int m, vvc<T> &...v)
{
  (v.assign(n, vc<T>(m)), ...);
  READnodump(v...);
}

template <class... T>
void READJAGnodump(int n, vvc<T> &...vs)
{
  auto read_one = [&](auto &v)
  {
    v.resize(n);
    for (auto &row : v)
    {
      int k;
      READnodump(k);
      row.resize(k);
      READnodump(row);
    }
  };
  (read_one(vs), ...);
}

}; // namespace internal

#define READ(...) internal::READnodump(__VA_ARGS__); dump(__VA_ARGS__)

#define IN(T, ...) T __VA_ARGS__; READ(__VA_ARGS__)

#define CHAR(...) IN(char, __VA_ARGS__)
#define INT(...) IN(int, __VA_ARGS__)
#define LL(...) IN(ll, __VA_ARGS__)
#define STR(...) IN(string, __VA_ARGS__)
#define ARR(T, n, ...) array<T, n> __VA_ARGS__; READ(__VA_ARGS__)

#define READVEC(...) internal::READVECnodump(__VA_ARGS__); dump(__VA_ARGS__)
#define READVEC2(...) internal::READVEC2nodump(__VA_ARGS__); dump(__VA_ARGS__)

#define VEC(T, n, ...) vc<T> __VA_ARGS__; READVEC(n, __VA_ARGS__)
#define VEC2(T, n, m, ...) vvc<T> __VA_ARGS__; READVEC2(n, m, __VA_ARGS__)

#define READJAG(...) internal::READJAGnodump(__VA_ARGS__); dump(__VA_ARGS__)

#define JAG(T, n, ...) vvc<T> __VA_ARGS__; READJAG(n, __VA_ARGS__)

// ----------

// ----- 出力 -----
#ifdef INTERACTIVE
#define ENDL endl
#else
#define ENDL '\n'
#endif

#if defined LOCAL or not defined FAST_IO
template <class T, class U>
ostream &operator<<(ostream &os, const pair<T, U> &p)
{
  os << p.first << ' ' << p.second;
  return os;
}

template <class... Ts>
ostream &operator<<(ostream &os, const tuple<Ts...> &t)
{
  if constexpr (sizeof...(Ts))
  {
    apply([&](const auto &...x)
          {
            int i = 0;
            ((os << (i++ ? " " : "") << x), ...);
          }, t);
  }
  return os;
}
template <class T, size_t n>
ostream &operator<<(ostream &os, const array<T, n> &a)
{
  for (size_t i = 0; i < n; i++)
  {
    if (i)
      os << ' ';
    os << a[i];
  }
  return os;
}
template <class T>
ostream &operator<<(ostream &os, const vc<T> &v)
{
  const size_t n = v.size();
  for (size_t i = 0; i < n; i++)
  {
    if (i)
      os << ' ';
    os << v[i];
  }
  return os;
}

namespace internal
{

template <class... Ts>
void COUTW(const Ts &...a)
{
  if constexpr (sizeof...(Ts))
    (cout << ... << a);
}

template <class... Ts>
void COUTP(const Ts &...a)
{
  if constexpr (sizeof...(Ts))
  {
    int i = 0;
    ((cout << (i++ ? " " : "") << a), ...);
  }
  cout << ENDL;
}

}; // namespace internal
#endif

#if defined FAST_IO and not defined LOCAL
#define WRITE fastio::write
#define PRINT fastio::print
#else
#define WRITE internal::COUTW
#define PRINT internal::COUTP
#endif
#define PRINTEXIT(...) do { PRINT(__VA_ARGS__); exit(0); } while (false)
#define PRINTRETURN(...) do { PRINT(__VA_ARGS__); return; } while (false)

template <class T>
void PRINTV(const vc<T> &v) { for (auto &vi : v) PRINT(vi); }
#define PRINTVEXIT(...) do { PRINTV(__VA_ARGS__); exit(0); } while (false)
#define PRINTVRETURN(...) do { PRINTV(__VA_ARGS__); return; } while (false)
// ----------

// ----- 基準ずらし -----
template <class T, class U, class P>
pair<T, U> &operator+=(pair<T, U> &a, const P &b)
{
  a.first += b.first;
  a.second += b.second;
  return a;
}
template <class T, class U, class P>
pair<T, U> operator+(pair<T, U> a, const P &b) { return a += b; }
template <class T, class U, class P>
pair<T, U> &operator-=(pair<T, U> &a, const P &b)
{
  a.first -= b.first;
  a.second -= b.second;
  return a;
}
template <class T, class U, class P>
pair<T, U> operator-(pair<T, U> a, const P &b) { return a -= b; }
template <class T, class U>
pair<T, U> operator-(pair<T, U> a)
{
  a.first = -a.first;
  a.second = -a.second;
  return a;
}

template <class T, size_t n, class A>
array<T, n> &operator+=(array<T, n> &a, const A &b)
{
  for (size_t i = 0; i < n; i++)
    a[i] += b[i];
  return a;
}
template <class T, size_t n, class A>
array<T, n> operator+(array<T, n> a, const A &b) { return a += b; }
template <class T, size_t n, class A>
array<T, n> &operator-=(array<T, n> &a, const A &b)
{
  for (size_t i = 0; i < n; i++)
    a[i] -= b[i];
  return a;
}
template <class T, size_t n, class A>
array<T, n> operator-(array<T, n> a, const A &b) { return a -= b; }
template <class T, size_t n>
array<T, n> operator-(array<T, n> a)
{
  for (auto &ai : a)
    ai = -ai;
  return a;
}

namespace internal
{

template <size_t... I, class A, class B>
auto &tuple_add_impl(A &a, const B &b, const index_sequence<I...>)
{
  ((get<I>(a) += get<I>(b)), ...);
  return a;
}
template <size_t... I, class A, class B>
auto &tuple_sub_impl(A &a, const B &b, const index_sequence<I...>)
{
  ((get<I>(a) -= get<I>(b)), ...);
  return a;
}
template <size_t... I, class A>
auto &tuple_neg_impl(A &a, const index_sequence<I...>)
{
  ((get<I>(a) = -get<I>(a)), ...);
  return a;
}

}; // namespace internal

template <class... Ts, class Tp>
tuple<Ts...> &operator+=(tuple<Ts...> &a, const Tp &b)
{ return internal::tuple_add_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{}); }
template <class... Ts, class Tp>
tuple<Ts...> operator+(tuple<Ts...> a, const Tp &b) { return a += b; }
template <class... Ts, class Tp>
tuple<Ts...> &operator-=(tuple<Ts...> &a, const Tp &b)
{ return internal::tuple_sub_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{}); }
template <class... Ts, class Tp>
tuple<Ts...> operator-(tuple<Ts...> a, const Tp &b) { return a -= b; }
template <class... Ts>
tuple<Ts...> operator-(tuple<Ts...> a)
{
  internal::tuple_neg_impl(a, make_index_sequence<sizeof...(Ts)>{});
  return a;
}

template <class T, class Add>
void offset(vc<T> &v, const Add &add) { for (auto &vi : v) vi += add; }
template <class T, class Add>
void offset(vvc<T> &v, const Add &add) { for (auto &vi : v) for (auto &vij : vi) vij += add; }
// ----------

// ----- 転置 -----
template <class T, const size_t m>
array<vc<T>, m> unzip(const vc<array<T, m>> &vt)
{
  const size_t n = vt.size();
  array<vc<T>, m> tv;
  tv.fill(vc<T>(n));
  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < m; j++)
      tv[j][i] = vt[i][j];
  return tv;
}
template <class T, const size_t m>
vc<array<T, m>> zip(const array<vc<T>, m> &tv)
{
  if (tv.empty()) return {};
  const size_t n = tv[0].size();
  vc<array<T, m>> vt(n);
  for (size_t j = 0; j < m; j++)
  {
    assert(tv[j].size() == n);
    for (size_t i = 0; i < n; i++)
      vt[i][j] = tv[j][i];
  }
  return vt;
}

template <class T, class U>
pair<vc<T>, vc<U>> unzip(const vc<pair<T, U>> &vt)
{
  const size_t n = vt.size();
  pair<vc<T>, vc<U>> tv;
  tv.first.resize(n), tv.second.resize(n);
  for (size_t i = 0; i < n; i++)
    tie(tv.first[i], tv.second[i]) = vt[i];
  return tv;
}
template <class T, class U>
vc<pair<T, U>> zip(const pair<vc<T>, vc<U>> &tv)
{
  const size_t n = tv.first.size();
  assert(n == tv.second.size());
  vc<pair<T, U>> vt(n);
  for (size_t i = 0; i < n; i++)
    vt[i] = make_pair(tv.first[i], tv.second[i]);
  return vt;
}

namespace internal
{

template <size_t... I, class V, class Tp>
auto vt_to_tv_impl(V &tv, const Tp &t, index_sequence<I...>, size_t index)
{ ((get<I>(tv)[index] = get<I>(t)), ...); }

template <size_t... I, class Tp>
auto tv_to_vt_impl(const Tp &tv, index_sequence<I...>, size_t index)
{ return make_tuple(get<I>(tv)[index]...); }

};

template <class... Ts>
auto unzip(const vc<tuple<Ts...>> &vt)
{
  const size_t n = vt.size();
  tuple<vc<Ts>...> tv;
  apply([&](auto &...v)
        { ((v.resize(n)), ...); }, tv);
  for (size_t i = 0; i < n; i++)
    internal::vt_to_tv_impl(tv, vt[i], make_index_sequence<tuple_size_v<decltype(tv)>>{}, i);
  return tv;
}

template <class... Ts>
auto zip(const tuple<vc<Ts>...> &tv)
{
  size_t n = get<0>(tv).size();
  apply([&](auto &...v)
        { ((void(v), assert(v.size() == n)), ...); }, tv);
  vc<tuple<Ts...>> vt(n);
  for (size_t i = 0; i < n; i++)
    vt[i] = internal::tv_to_vt_impl(tv, index_sequence_for<Ts...>{}, i);
  return vt;
}

namespace internal
{

template <class... Ts>
auto zip_vectors(const vc<Ts> &...v)
{
  if constexpr (sizeof...(Ts) == 2)
    return zip(pair{v...});
  else
    return zip(tuple{v...});
}

};

#define UNZIP(vt, ...) auto [__VA_ARGS__] = unzip(vt)
#define ZIP(vt, ...) auto vt = internal::zip_vectors(__VA_ARGS__)
// ----------
// https://github.com/miscalculation53/library/tree/wip/template/template_random.hpp


/**
 * @brief テンプレート（ランダム生成）
 * @docs docs/template/template_random.md
 */

mt19937_64 mt;

// [l, r] から等確率
template <class T = ll, class U1, class U2>
T randint(U1 l, U2 r)
{
  assert(T(l) <= T(r));
  return uniform_int_distribution<T>(T(l), T(r))(mt);
}
// [l, r) から等確率
template <class T = ll, class U1, class U2>
T randrange(U1 l, U2 r)
{
  assert(T(l) < T(r));
  return uniform_int_distribution<T>(T(l), T(r) - 1)(mt);
}

// [l, r) から一様ランダムな実数を返す
template <class T = double, class U1, class U2>
T randreal(U1 l, U2 r)
{
  assert(T(l) < T(r));
  return uniform_real_distribution<T>(T(l), T(r))(mt);
}

// 確率 p で true を返す
bool randbool(double p)
{
  assert(0 <= p && p <= 1);
  return bernoulli_distribution(p)(mt);
}

namespace internal
{
template <bool does_sort, class V, class T>
void random_sample_range(V &res, T l, T r)
{
  int k = res.size();
  T n = r - l;
  if (k <= 256)
  {
    repi(i, k)
    {
      T j = n - T(k) + T(i), x = randint<T>(0, j);
      if (find(res.begin(), res.begin() + i, x) != res.begin() + i)
        x = j;
      res[i] = x;
    }
  }
  else
  {
    unordered_set<T> used;
    used.reserve(2 * size_t(k));
    repi(i, k)
    {
      T j = n - T(k) + T(i), x = randint<T>(0, j);
      if (!used.insert(x).second)
        x = j, used.insert(x);
      res[i] = x;
    }
  }
  for (T &x : res) x += l;
  if constexpr (does_sort)
    sort(res.begin(), res.end());
  else
    shuffle(res.begin(), res.end(), mt);
}
}; // namespace internal

// [l, r) から相異なる k 個を選ぶ
// does_sort: ソートするかどうか
template <int k, bool does_sort, class T = ll, class U1, class U2>
array<T, k> random_sample_range_array(U1 l, U2 r)
{
  assert(T(r) - T(l) >= T(k));
  array<T, k> res;
  internal::random_sample_range<does_sort>(res, T(l), T(r));
  return res;
}
// [l, r) から相異なる k 個を選ぶ
// does_sort: ソートするかどうか
template <bool does_sort, class T = ll, class U1, class U2>
vc<T> random_sample_range_vector(U1 l, U2 r, int k)
{
  assert(k >= 0);
  assert(T(r) - T(l) >= T(k));
  vc<T> res(k);
  internal::random_sample_range<does_sort>(res, T(l), T(r));
  return res;
}

/**
 * @brief 値つき区間の管理（区間代入・削除）
 * @docs docs/ds/interval_map.md
 */

template <class T, class I = ll>
struct IntervalMap
{
  struct Segment
  {
    I l, r;
    T x;
  };

private:
  struct Compare
  {
    using is_transparent = void;
    bool operator()(const Segment &a, const Segment &b) const { return a.l < b.l; }
    bool operator()(const Segment &a, const I b) const { return a.l < b; }
    bool operator()(const I a, const Segment &b) const { return a < b.l; }
  };
  std::set<Segment, Compare> segs;
  T dflt;

  template <class Add, class Del>
  void replace(I l, I r, const T *x, Add &add, Del &del)
  {
    assert(!(r < l));
    if (!(l < r)) return;

    if (x)
    {
      auto it = segs.lower_bound(l);
      if (it != segs.begin())
      {
        --it;
        if (!(it->r < l) && it->x == *x) l = it->l;
      }
      it = segs.upper_bound(r);
      if (it != segs.begin())
      {
        --it;
        if (r < it->r && it->x == *x) r = it->r;
      }
    }

    auto it = next_it(l);
    while (it != segs.end() && it->l < r)
    {
      const Segment a = *it;
      del(a.l, a.r, a.x);
      it = segs.erase(it);
      if (a.l < l)
      {
        segs.insert(it, Segment{a.l, l, a.x});
        add(a.l, l, a.x);
      }
      if (r < a.r)
      {
        it = segs.insert(it, Segment{r, a.r, a.x});
        add(r, a.r, a.x);
        break;
      }
    }
    if (x)
    {
      segs.insert(it, Segment{l, r, *x});
      add(l, r, *x);
    }
  }

public:
  using iterator = typename set<Segment, Compare>::const_iterator;

  explicit IntervalMap(const T &dflt = T()) : dflt(dflt) {}
  IntervalMap(const vc<T> &a, const T &dflt = T()) : dflt(dflt)
  {
    const int n = a.size();
    for (int l = 0, r; l < n; l = r)
    {
      for (r = l + 1; r < n && a[l] == a[r]; ++r) {}
      segs.insert(segs.end(), Segment{I(l), I(r), a[l]});
    }
  }

  // p を含む区間
  // 未登録なら end()
  iterator get_it(const I p) const
  {
    auto it = segs.upper_bound(p);
    if (it == segs.begin()) return segs.end();
    --it;
    return p < it->r ? it : segs.end();
  }
  // p を含む区間、またはその右側で最初の区間
  iterator next_it(const I p) const
  {
    auto it = segs.upper_bound(p);
    if (it != segs.begin() && p < prev(it)->r) return prev(it);
    return it;
  }
  T get(const I p) const
  {
    auto it = get_it(p);
    return it == segs.end() ? dflt : it->x;
  }
  T operator[](const I p) const { return get(p); }

  // [l, r) を x にする
  // 区間 [L, R), 値 X が追加されるとき add(L, R, X) を呼ぶ
  // 区間 [L, R), 値 X が削除されるとき del(L, R, X) を呼ぶ
  template <class Add, class Del>
  void set(I l, I r, T x, Add &&add, Del &&del)
  {
    replace(l, r, &x, add, del);
  }
  // [l, r) を x にする
  // 区間 [L, R), 値 X が追加されるとき add(L, R, X) を呼ぶ
  // 区間 [L, R), 値 X が削除されるとき del(L, R, X) を呼ぶ
  void set(I l, I r, T x)
  {
    set(l, r, move(x), [](const I, const I, const T &) {},
           [](const I, const I, const T &) {});
  }
  // [l, r) を未登録にする
  // 区間 [L, R), 値 X が追加されるとき add(L, R, X) を呼ぶ
  // 区間 [L, R), 値 X が削除されるとき del(L, R, X) を呼ぶ
  template <class Add, class Del>
  void erase(I l, I r, Add &&add, Del &&del)
  {
    replace(l, r, nullptr, add, del);
  }
  // [l, r) を未登録にする
  // 区間 [L, R), 値 X が追加されるとき add(L, R, X) を呼ぶ
  // 区間 [L, R), 値 X が削除されるとき del(L, R, X) を呼ぶ
  void erase(I l, I r)
  {
    erase(l, r, [](const I, const I, const T &) {},
          [](const I, const I, const T &) {});
  }

  template <class I = ll>
  I size() const { return segs.size(); }
  bool empty() const { return segs.empty(); }
  iterator begin() const { return segs.begin(); }
  iterator end() const { return segs.end(); }
};
