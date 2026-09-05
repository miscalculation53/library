#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"
#include "template_dump.hpp"

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

#define UNZIP(vt, ...) auto [__VA_ARGS__] = unzip(vt)
#define ZIP(vt, ...) auto vt = zip(tuple{__VA_ARGS__})
// ----------
