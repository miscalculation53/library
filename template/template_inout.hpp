#pragma once

#include "template_types.hpp"
#include "template_arithmetic.hpp"
#include "template_rep.hpp"
#include "template_dump.hpp"

/**
 * @brief テンプレート（入出力）
 * @docs docs/template/template_inout.md
 */

#if LIBRARY_HAS_FLOAT128
inline istream &operator>>(istream &is, f128 &x)
{
  string s;
  if (is >> s)
  {
    f128 value;
    if (template_float128_detail::parse(s, value)) x = value;
    else is.setstate(ios::failbit);
  }
  return is;
}

inline ostream &operator<<(ostream &os, f128 x)
{
  const auto field = os.flags() & ios::floatfield;
  const auto format = field == ios::fixed ? chars_format::fixed :
                      field == ios::scientific ? chars_format::scientific : chars_format::general;
  string s = template_float128_detail::format(x, format, int(os.precision()));
  if ((os.flags() & ios::showpos) && !signbit(x)) s.insert(s.begin(), '+');
  if (os.flags() & ios::uppercase)
    for (char &c : s) c = char(toupper(static_cast<unsigned char>(c)));
  return os << s;
}
#endif

#if defined FAST_IO and not defined LOCAL
namespace fastio
{
  constexpr uint32_t SIZ = 1 << 17;
  inline char ibuf[SIZ + 1], obuf[SIZ];
  inline uint32_t pil = 0, pir = 0, por = 0;

  __attribute__((noinline)) inline void load()
  {
    const uint32_t remaining = pir - pil;
    if (remaining && pil) memmove(ibuf, ibuf + pil, remaining);
    pil = 0;
    pir = remaining + uint32_t(fread(ibuf + remaining, 1, SIZ - remaining, stdin));
    ibuf[pir] = '\x7f';
  }

  inline bool ascii_space(unsigned char c)
  {
    return c == ' ' || unsigned(c - '\t') < 5;
  }

  inline void find_token()
  {
    for (;;)
    {
      const char *p = ibuf + pil;
      const char *end = ibuf + pir;
      while (ascii_space(*p)) ++p;
      pil = uint32_t(p - ibuf);
      if (p != end) return;
      load();
      if (!pir) abort();
    }
  }

  __attribute__((noinline)) inline void flush()
  {
    size_t written = 0;
    while (written < por)
    {
      const size_t n = fwrite(obuf + written, 1, por - written, stdout);
      if (!n) abort();
      written += n;
    }
    por = 0;
  }

  inline void append_bytes(const char *data, size_t size)
  {
    while (size)
    {
      const size_t n = min(size, size_t(SIZ - por));
      memcpy(obuf + por, data, n);
      por += uint32_t(n);
      data += n;
      size -= n;
      if (por == SIZ) flush();
    }
  }

  inline void append_char(char c)
  {
    if (por == SIZ) flush();
    obuf[por++] = c;
  }

  template <class T>
  using integer_magnitude = conditional_t<(sizeof(T) > sizeof(ull)), u128,
                            conditional_t<(sizeof(T) > sizeof(uint)), ull, uint>>;

  // 各桁対、4 桁、8 桁の順に十進の重みをまとめる。
  inline uint decode_eight(uint64_t columns)
  {
    columns &= 0x0f0f0f0f0f0f0f0fULL;
    const uint64_t pairs = (columns & 0x00ff00ff00ff00ffULL) * 10
                         + ((columns >> 8) & 0x00ff00ff00ff00ffULL);
    const uint64_t quads = (pairs & 0x0000ffff0000ffffULL) * 100
                         + ((pairs >> 16) & 0x0000ffff0000ffffULL);
    return uint(quads) * 10000 + uint(quads >> 32);
  }

  inline constexpr uint powers_of_ten[8] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};

  template <class T>
  __attribute__((always_inline)) inline void rd1_integer(T &x)
  {
    using U = integer_magnitude<T>;
    const char *p = ibuf + pil;
    for (;;)
    {
      while (ascii_space(*p)) ++p;
      if (p != ibuf + pir) break;
      pil = pir;
      load();
      if (!pir) abort();
      p = ibuf;
    }
    bool negative = false;
    if (*p == '+') ++p;
    else if constexpr (numeric_limits<T>::is_signed)
    {
      if (*p == '-') negative = true, ++p;
    }
    U value = 0;
    for (;;)
    {
      while (size_t(ibuf + pir - p) >= 8)
      {
        uint64_t columns;
        memcpy(&columns, p, 8);
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        columns = __builtin_bswap64(columns);
#endif
        const uint64_t stop = ((columns + 0x4646464646464646ULL) |
                               (columns - 0x3030303030303030ULL)) & 0x8080808080808080ULL;
        if (stop)
        {
          if constexpr (sizeof(U) > sizeof(ull))
          {
            const unsigned digits = unsigned(__builtin_ctzll(stop)) / 8;
            if (digits)
            {
              uint tail;
              if (digits <= 3)
              {
                tail = unsigned(p[0] - '0');
                if (digits >= 2) tail = tail * 10 + unsigned(p[1] - '0');
                if (digits == 3) tail = tail * 10 + unsigned(p[2] - '0');
              }
              else tail = decode_eight(columns << (8 * (8 - digits)));
              value = value * powers_of_ten[digits] + tail;
              p += digits;
            }
            goto integer_complete;
          }
          else
          {
            if (stop & 0x80808080ULL)
            {
              if (!(stop & 0x808080ULL))
              {
                value = value * 1000 + uint(columns & 15) * 100
                      + uint(columns >> 8 & 15) * 10 + uint(columns >> 16 & 15);
                p += 3;
              }
              else if (!(stop & 0x8080ULL))
              {
                value = value * 100 + uint(columns & 15) * 10 + uint(columns >> 8 & 15);
                p += 2;
              }
              else if (!(stop & 0x80ULL))
              {
                value = value * 10 + uint(columns & 15);
                ++p;
              }
              goto integer_complete;
            }
            break;

          }
        }
        columns &= 0x0f0f0f0f0f0f0f0fULL;
        const uint64_t pairs = (columns & 0x00ff00ff00ff00ffULL) * 10
                             + ((columns >> 8) & 0x00ff00ff00ff00ffULL);
        const uint64_t quads = (pairs & 0x0000ffff0000ffffULL) * 100
                             + ((pairs >> 16) & 0x0000ffff0000ffffULL);
        value = value * 100000000 + uint(quads) * 10000 + uint(quads >> 32);
        p += 8;
      }
      while (unsigned(*p - '0') < 10)
      {
        value = value * 10 + unsigned(*p - '0');
        ++p;
      }
      pil = uint32_t(p - ibuf);
      if (pil != pir) break;
      load();
      p = ibuf;
      if (!pir) break;
    }
integer_complete:
    pil = uint32_t(p - ibuf);
    if constexpr (numeric_limits<T>::is_signed)
    {
      if (negative && value) x = T(-T(value - 1) - 1);
      else x = T(value);
    }
    else x = T(value);
  }

  template <class T, enable_if_t<numeric_limits<T>::is_integer, int> = 0>
  __attribute__((always_inline)) inline void rd1(T &x) { rd1_integer(x); }
  inline void rd1(char &x)
  {
    find_token();
    x = ibuf[pil++];
  }
  inline void rd1(string &x)
  {
    find_token();
    x.clear();
    for (;;)
    {
      const char *p = ibuf + pil;
      const char *end = ibuf + pir;
      const char *first = p;
      while (p != end && !ascii_space(*p)) ++p;
      x.append(first, size_t(p - first));
      pil = uint32_t(p - ibuf);
      if (p != end) return;
      load();
      if (!pir) return;
    }
  }

  template <class T>
  void rd1_real(T &x)
  {
    string token;
    rd1(token);
    const char *first = token.data();
    const char *last = first + token.size();
    if (first != last && *first == '+') ++first;
    auto result = from_chars(first, last, x, chars_format::general);
    if (result.ec != errc{} || result.ptr != last)
    {
      char *end;
      if constexpr (is_same_v<T, float>) x = strtof(token.c_str(), &end);
      else if constexpr (is_same_v<T, double>) x = strtod(token.c_str(), &end);
      else x = strtold(token.c_str(), &end);
      if (end != token.data() + token.size()) abort();
    }
  }
  inline void rd1(float &x) { rd1_real(x); }
  inline void rd1(double &x) { rd1_real(x); }
  inline void rd1(long double &x) { rd1_real(x); }
#if LIBRARY_HAS_FLOAT128
  inline void rd1(f128 &x)
  {
    string token;
    rd1(token);
    if (!template_float128_detail::parse(token, x)) abort();
  }
#endif

  template <class A, class B> void rd1(pair<A, B> &x);
  template <class... T> void rd1(tuple<T...> &x);
  template <class T, size_t N> void rd1(array<T, N> &x);
  template <class T> void rd1(vc<T> &x);

  template <class A, class B>
  void rd1(pair<A, B> &x) { rd1(x.first), rd1(x.second); }
  template <class... T>
  void rd1(tuple<T...> &x)
  {
    apply([](auto &...element) { (rd1(element), ...); }, x);
  }
  template <class T, size_t N>
  void rd1(array<T, N> &x)
  {
    for (auto &element : x) rd1(element);
  }
  template <class T>
  void rd1(vc<T> &x)
  {
    if constexpr (is_same_v<T, bool>)
      for (size_t i = 0; i < x.size(); ++i) { bool value; rd1(value); x[i] = value; }
    else
      for (auto &element : x) rd1(element);
  }

  inline constexpr auto decimal_triplets = []
  {
    array<array<char, 3>, 1000> result{};
    for (unsigned i = 0; i < 1000; ++i)
    {
      result[i][0] = char('0' + i / 100);
      result[i][1] = char('0' + i / 10 % 10);
      result[i][2] = char('0' + i % 10);
    }
    return result;
  }();

  inline void store_nine(char *out, uint x)
  {
    const uint q = x / 1000, r = q / 1000;
    memcpy(out, decimal_triplets[r].data(), 3);
    memcpy(out + 3, decimal_triplets[q - r * 1000].data(), 3);
    memcpy(out + 6, decimal_triplets[x - q * 1000].data(), 3);
  }

  inline char *prepend_small(char *end, uint x)
  {
    while (x >= 1000)
    {
      const uint q = x / 1000;
      end -= 3;
      memcpy(end, decimal_triplets[x - q * 1000].data(), 3);
      x = q;
    }
    const unsigned digits = 1 + unsigned(x >= 10) + unsigned(x >= 100);
    end -= digits;
    memcpy(end, decimal_triplets[x].data() + 3 - digits, digits);
    return end;
  }

  inline char *prepend_wide(char *end, ull x)
  {
    while (x >= 1000000000ULL)
    {
      const ull q = x / 1000000000ULL;
      end -= 9;
      store_nine(end, uint(x - q * 1000000000ULL));
      x = q;
    }
    return prepend_small(end, uint(x));
  }

  inline char *store_leading_triplet(char *out, uint x)
  {
    if (x >= 100)
    {
      memcpy(out, decimal_triplets[x].data(), 3);
      return out + 3;
    }
    if (x >= 10)
    {
      memcpy(out, decimal_triplets[x].data() + 1, 2);
      return out + 2;
    }
    *out = char('0' + x);
    return out + 1;
  }

  inline char *store_short(char *out, uint x)
  {
    if (x >= 1000000)
    {
      const uint q = x / 1000000;
      const uint r = x - q * 1000000;
      out = store_leading_triplet(out, q);
      memcpy(out, decimal_triplets[r / 1000].data(), 3);
      memcpy(out + 3, decimal_triplets[r % 1000].data(), 3);
      return out + 6;
    }
    if (x >= 1000)
    {
      const uint q = x / 1000;
      out = store_leading_triplet(out, q);
      memcpy(out, decimal_triplets[x - q * 1000].data(), 3);
      return out + 3;
    }
    return store_leading_triplet(out, x);
  }

  template <class U>
  inline char *store_ordinary(char *out, U x)
  {
    if constexpr (sizeof(U) > sizeof(uint))
    {
      if (x >= 1000000000000000000ULL)
      {
        const uint hi = uint(x / 1000000000000000000ULL);
        const ull tail = x - ull(hi) * 1000000000000000000ULL;
        const uint middle = uint(tail / 1000000000ULL);
        out = store_leading_triplet(out, hi);
        store_nine(out, middle);
        store_nine(out + 9, uint(tail - ull(middle) * 1000000000ULL));
        return out + 18;
      }
    }
    if (x >= 1000000000)
    {
      const uint hi = uint(x / 1000000000);
      out = store_short(out, hi);
      store_nine(out, uint(x - U(hi) * 1000000000));
      return out + 9;
    }
    return store_short(out, uint(x));
  }

  template <class T, bool newline = false>
  __attribute__((always_inline)) inline void wt1_integer(T x)
  {
    using U = integer_magnitude<T>;
    U magnitude = U(x);
    bool negative = false;
    if constexpr (numeric_limits<T>::is_signed)
    {
      negative = x < 0;
      const U sign_mask = U(0) - U(negative);
      magnitude = (magnitude ^ sign_mask) + U(negative);
    }
    if constexpr (sizeof(U) <= sizeof(ull))
    {
      if (por > SIZ - 40) flush();
      char *out = obuf + por;
      // 負号の位置だけを進め、非負なら数字で上書きする。
      *out = '-';
      out += negative;
      out = store_ordinary(out, magnitude);
      if constexpr (newline) *out++ = '\n';
      por = uint32_t(out - obuf);
      return;
    }
    char text[48];
    char *const end = text + sizeof(text);
    char *first = end;
    if constexpr (sizeof(U) > sizeof(ull))
    {
      constexpr ull base = 1000000000000000000ULL;
      while (magnitude >= base)
      {
        const U q = magnitude / base;
        const ull tail = ull(magnitude - q * base);
        const uint hi = uint(tail / 1000000000ULL);
        first -= 18;
        store_nine(first, hi);
        store_nine(first + 9, uint(tail - ull(hi) * 1000000000ULL));
        magnitude = q;
      }
      first = prepend_wide(first, ull(magnitude));
    }
    else if constexpr (sizeof(U) > sizeof(uint)) first = prepend_wide(first, magnitude);
    else first = prepend_small(first, magnitude);
    if (negative) *--first = '-';
    append_bytes(first, size_t(end - first));
    if constexpr (newline) append_char('\n');
  }

  template <class T, enable_if_t<numeric_limits<T>::is_integer, int> = 0>
  __attribute__((always_inline)) inline void wt1(T x) { wt1_integer(x); }
  inline void wt1(char x) { append_char(x); }
  inline void wt1(const char *x) { append_bytes(x, strlen(x)); }
  inline void wt1(string_view x) { append_bytes(x.data(), x.size()); }
  inline void wt1(const string &x) { append_bytes(x.data(), x.size()); }

  template <class T>
  void wt1_real(T x)
  {
    char text[numeric_limits<T>::max_exponent10 + 64];
    auto result = to_chars(text, text + sizeof(text), x, chars_format::fixed, 15);
    if (result.ec != errc{}) abort();
    append_bytes(text, size_t(result.ptr - text));
  }
  inline void wt1(float x) { wt1_real(x); }
  inline void wt1(double x) { wt1_real(x); }
  inline void wt1(long double x) { wt1_real(x); }
#if LIBRARY_HAS_FLOAT128
  inline void wt1(f128 x)
  {
    wt1(template_float128_detail::format(x, chars_format::fixed, 35));
  }
#endif

  template <class A, class B> void wt1(const pair<A, B> &x);
  template <class... T> void wt1(const tuple<T...> &x);
  template <class T, size_t N> void wt1(const array<T, N> &x);
  template <class T> void wt1(const vc<T> &x);

  template <class A, class B>
  void wt1(const pair<A, B> &x) { wt1(x.first), append_char(' '), wt1(x.second); }
  template <class... T>
  void wt1(const tuple<T...> &x)
  {
    apply([](const auto &...element)
    {
      size_t index = 0;
      ((index++ ? void(append_char(' ')) : void(), wt1(element)), ...);
    }, x);
  }
  template <class T, size_t N>
  void wt1(const array<T, N> &x)
  {
    for (size_t i = 0; i < N; ++i)
    {
      if (i) append_char(' ');
      wt1(x[i]);
    }
  }
  template <class T>
  void wt1(const vc<T> &x)
  {
    for (size_t i = 0; i < x.size(); ++i)
    {
      if (i) append_char(' ');
      wt1(x[i]);
    }
  }

  template <class... T>
  void read(T &...x) { (rd1(x), ...); }
  template <class... T>
  void write(T &&...x) { (wt1(forward<T>(x)), ...); }
  template <class... T>
  void print(T &&...x)
  {
    if constexpr (sizeof...(T) == 1 &&
                  ((is_integral_v<decay_t<T>> && !is_same_v<decay_t<T>, char>) && ...))
      (wt1_integer<decay_t<T>, true>(forward<T>(x)), ...);
    else
    {
      size_t index = 0;
      ((index++ ? void(append_char(' ')) : void(), wt1(forward<T>(x))), ...);
      append_char('\n');
    }
  }
}
#endif

#if defined FAST_IO and not defined LOCAL
struct Dummy {
  Dummy() { atexit(fastio::flush); }
} dummy;
#endif

//! https://trap.jp/post/1224/

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
  (v.resize(size_t(n)), ...);
  READnodump(v...);
}

template <class... T>
void READVEC2nodump(int n, int m, vvc<T> &...v)
{
  (v.assign(size_t(n), vc<T>(size_t(m))), ...);
  READnodump(v...);
}

template <class... T>
void READJAGnodump(int n, vvc<T> &...vs)
{
  auto read_one = [&](auto &v)
  {
    v.resize(size_t(n));
    for (auto &row : v)
    {
      int k;
      READnodump(k);
      row.resize(size_t(k));
      READnodump(row);
    }
  };
  (read_one(vs), ...);
}

}; // namespace internal

#define READ(...) internal::READnodump(__VA_ARGS__); dump(__VA_ARGS__)

#define LIBRARY_INOUT_TYPE(...) LIBRARY_INOUT_TYPE_IMPL(__VA_ARGS__)(__VA_ARGS__)
#define LIBRARY_INOUT_TYPE_IMPL(T, ...) LIBRARY_INOUT_TYPE_SELECT(LIBRARY_INOUT_TYPE_PROBE T, LIBRARY_INOUT_TYPE_KEEP)
#define LIBRARY_INOUT_TYPE_PROBE(...) _, LIBRARY_INOUT_TYPE_REMOVE
#define LIBRARY_INOUT_TYPE_SELECT(...) LIBRARY_INOUT_TYPE_SECOND(__VA_ARGS__)
#define LIBRARY_INOUT_TYPE_SECOND(_0, _1, ...) _1
#define LIBRARY_INOUT_TYPE_KEEP(...) __VA_ARGS__
#define LIBRARY_INOUT_TYPE_REMOVE(T) LIBRARY_INOUT_TYPE_KEEP T

#define IN(T, ...) LIBRARY_INOUT_TYPE(T) __VA_ARGS__; READ(__VA_ARGS__)

#define CHAR(...) IN(char, __VA_ARGS__)
#define INT(...) IN(int, __VA_ARGS__)
#define LL(...) IN(ll, __VA_ARGS__)
#define STR(...) IN(string, __VA_ARGS__)
#define ARR(T, n, ...) array<LIBRARY_INOUT_TYPE(T), n> __VA_ARGS__; READ(__VA_ARGS__)

#define READVEC(...) internal::READVECnodump(__VA_ARGS__); dump(__VA_ARGS__)
#define READVEC2(...) internal::READVEC2nodump(__VA_ARGS__); dump(__VA_ARGS__)

#define VEC(T, n, ...) vc<LIBRARY_INOUT_TYPE(T)> __VA_ARGS__; READVEC(n, __VA_ARGS__)
#define VEC2(T, n, m, ...) vvc<LIBRARY_INOUT_TYPE(T)> __VA_ARGS__; READVEC2(n, m, __VA_ARGS__)

#define READJAG(...) internal::READJAGnodump(__VA_ARGS__); dump(__VA_ARGS__)

#define JAG(T, n, ...) vvc<LIBRARY_INOUT_TYPE(T)> __VA_ARGS__; READJAG(n, __VA_ARGS__)

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
