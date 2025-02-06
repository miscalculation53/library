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
namespace fastio {
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
  } while (isspace(c));
}

void rd1(string &x) {
  x.clear();
  char c;
  do {
    if (pil + 1 > pir) load();
    c = ibuf[pil++];
  } while (isspace(c));
  do {
    x += c;
    if (pil == pir) load();
    c = ibuf[pil++];
  } while (!isspace(c));
}

template <typename T>
void rd1_real(T &x) {
  string s;
  rd1(s);
  x = stod(s);
}

template <typename T>
void rd1_integer(T &x) {
  if (pil + 100 > pir) load();
  char c;
  do
    c = ibuf[pil++];
  while (c < '-');
  bool minus = 0;
  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {
    if (c == '-') { minus = 1, c = ibuf[pil++]; }
  }
  x = 0;
  while ('0' <= c) { x = x * 10 + (c & 15), c = ibuf[pil++]; }
  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {
    if (minus) x = -x;
  }
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
template <size_t N = 0, typename T>
void rd1_tuple(T &t) {
  if constexpr (N < std::tuple_size<T>::value) {
    auto &x = std::get<N>(t);
    rd1(x);
    rd1_tuple<N + 1>(t);
  }
}
template <class... T>
void rd1(tuple<T...> &tpl) {
  rd1_tuple(tpl);
}

template <size_t N = 0, typename T>
void rd1(array<T, N> &x) {
  for (auto &d: x) rd1(d);
}
template <class T>
void rd1(vc<T> &x) {
  for (auto &d: x) rd1(d);
}

void read() {}
template <class H, class... T>
void read(H &h, T &... t) {
  rd1(h), read(t...);
}

void wt1(const char c) {
  if (por == SIZ) flush();
  obuf[por++] = c;
}
void wt1(const string s) {
  for (char c: s) wt1(c);
}
void wt1(const char *s) {
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) wt1(s[i]);
}

template <typename T>
void wt1_integer(T x) {
  if (por > SIZ - 100) flush();
  if (x < 0) { obuf[por++] = '-', x = -x; }
  int outi;
  for (outi = 96; x >= 10000; outi -= 4) {
    memcpy(out + outi, pre.num[x % 10000], 4);
    x /= 10000;
  }
  if (x >= 1000) {
    memcpy(obuf + por, pre.num[x], 4);
    por += 4;
  } else if (x >= 100) {
    memcpy(obuf + por, pre.num[x] + 1, 3);
    por += 3;
  } else if (x >= 10) {
    int q = (x * 103) >> 10;
    obuf[por] = q | '0';
    obuf[por + 1] = (x - q * 10) | '0';
    por += 2;
  } else
    obuf[por++] = x | '0';
  memcpy(obuf + por, out + outi + 4, 96 - outi);
  por += 96 - outi;
}

template <typename T>
void wt1_real(T x) {
  ostringstream oss;
  oss << fixed << setprecision(15) << double(x);
  string s = oss.str();
  wt1(s);
}

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
template <size_t N = 0, typename T>
void wt1_tuple(const T &t) {
  if constexpr (N < std::tuple_size<T>::value) {
    if constexpr (N > 0) { wt1(' '); }
    const auto x = std::get<N>(t);
    wt1(x);
    wt1_tuple<N + 1>(t);
  }
}
template <class... T>
void wt1(const tuple<T...> &tpl) {
  wt1_tuple(tpl);
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

void write() {}
template <class Head, class... Tail>
void write(Head &&head, Tail &&... tail) {
  wt1(head);
  write(forward<Tail>(tail)...);
}

void print() { wt1('\n'); }
template <class Head, class... Tail>
void print(Head &&head, Tail &&... tail) {
  wt1(head);
  if (sizeof...(Tail)) wt1(' ');
  print(forward<Tail>(tail)...);
}

} // namespace fastio

#if defined FAST_IO and not defined LOCAL
struct Dummy {
  Dummy() { atexit(fastio::flush); }
} dummy;
#endif

// https://trap.jp/post/1224/

// ---- 入力 ----
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

namespace internal
{

template <class... Ts>
void CIN(Ts &...a) { (cin >> ... >> a); }

#if defined FAST_IO and not defined LOCAL
template <class... Ts>
void READnodump(Ts &...a) { fastio::read(a...); }
#else
template <class... Ts>
void READnodump(Ts &...a) { CIN(a...); }
#endif

template <class T>
void READVECnodump(int n, vc<T> &v)
{
  v.resize(n);
  READnodump(v);
}
template <class T, class... Ts>
void READVECnodump(int n, vc<T> &v, vc<Ts> &...vs)
{ READVECnodump(n, v), READVECnodump(n, vs...); }

template <class T>
void READVEC2nodump(int n, int m, vvc<T> &v)
{
  v.assign(n, vc<T>(m));
  READnodump(v);
}
template <class T, class... Ts>
void READVEC2nodump(int n, int m, vvc<T> &v, vvc<Ts> &...vs)
{ READVEC2nodump(n, m, v), READVEC2nodump(n, m, vs...); }

template <class T>
void READJAGnodump(int n, vvc<T> &v)
{
  v.resize(n);
  repi(i, n)
  {
    int k;
    READnodump(k);
    READVECnodump(k, v[i]);
  }
}
template <class T, class... Ts>
void READJAGnodump(int n, vvc<T> &v, vvc<Ts> &...vs)
{ READJAGnodump(n, v), READJAGnodump(n, vs...); }

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

template <class T, class U>
ostream &operator<<(ostream &os, const pair<T, U> &p)
{
  os << p.first << ' ' << p.second;
  return os;
}

namespace internal
{

template <size_t N = 0, typename T>
void cout_tuple(ostream &os, const T &t) {
  if constexpr (N < std::tuple_size<T>::value) {
    if constexpr (N > 0) { os << ' '; }
    const auto x = std::get<N>(t);
    os << x;
    cout_tuple<N + 1>(os, t);
  }
}

}; // namespace internal

template <class... Ts>
ostream &operator<<(ostream &os, const tuple<Ts...> &t)
{
  internal::cout_tuple(os, t);
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

template <class T>
void COUTW() {}
template <class... Ts>
void COUTW(const Ts &...a) { (cout << ... << a); }

template <class T>
void COUTP() { cout << ENDL; }
template <class T>
void COUTP(const T &a) { cout << a << ENDL; }
template <class T, class... Ts>
void COUTP(const T &a, const Ts &...b)
{
  cout << a;
  (cout << ... << (cout << ' ', b));
  cout << ENDL;
}

}; // namespace internal

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
template <class T, class U>
pair<T, U> operator+=(pair<T, U> &a, cauto &b)
{
  a.first += b.first;
  a.second += b.second;
  return a;
}
template <class T, class U>
pair<T, U> operator+(pair<T, U> &a, cauto &b) { return a += b; }

template <class T, size_t n>
array<T, n> operator+=(array<T, n> &a, cauto &b)
{
  for (size_t i = 0; i < n; i++)
    a[i] += b[i];
  return a;
}
template <class T, size_t n>
array<T, n> operator+(array<T, n> &a, cauto &b) { return a += b; }

namespace internal
{

template <size_t... I>
auto tuple_add_impl(auto &a, cauto &b, const index_sequence<I...>)
{
  ((get<I>(a) += get<I>(b)), ...);
  return a;
}

}; // namespace internal

template <class... Ts>
tuple<Ts...> operator+=(tuple<Ts...> &a, cauto &b)
{ return internal::tuple_add_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{}); }
template <class... Ts>
tuple<Ts...> operator+(tuple<Ts...> &a, cauto &b) { return a += b; }

template <class T>
void offset(vc<T> &v, cauto &add) { for (auto &vi : v) vi += add; }
template <class T>
void offset(vvc<T> &v, cauto &add) { for (auto &vi : v) for (auto &vij : vi) vij += add; }
// ----------

// ----- 転置 -----
template <class T, const size_t m>
array<vc<T>, m> top(const vc<array<T, m>> &vt)
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
vc<array<T, m>> top(const array<vc<T>, m> &tv)
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
pair<vc<T>, vc<U>> top(const vc<pair<T, U>> &vt)
{
  const size_t n = vt.size();
  pair<vc<T>, vc<U>> tv;
  tv.first.resize(n), tv.second.resize(n);
  for (size_t i = 0; i < n; i++)
    tie(tv.first[i], tv.second[i]) = vt[i];
  return tv;
}
template <class T, class U>
vc<pair<T, U>> top(const pair<vc<T>, vc<U>> &tv)
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

template <size_t... I>
auto vt_to_tv_impl(auto &tv, cauto &t, index_sequence<I...>, size_t index)
{ ((get<I>(tv)[index] = get<I>(t)), ...); }

template <size_t... I>
auto tv_to_vt_impl(cauto &tv, index_sequence<I...>, size_t index)
{ return make_tuple(get<I>(tv)[index]...); }

};

template <class... Ts>
auto top(const vc<tuple<Ts...>> &vt)
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
auto top(const tuple<vc<Ts>...> &tv)
{
  size_t n = get<0>(tv).size();
  apply([&](auto &...v)
        { ((assert(v.size() == n)), ...); }, tv);
  vc<tuple<Ts...>> vt(n);
  for (size_t i = 0; i < n; i++)
    vt[i] = internal::tv_to_vt_impl(tv, index_sequence_for<Ts...>{}, i);
  return vt;
}
// ----------