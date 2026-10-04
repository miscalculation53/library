#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/bigint.hpp"
#include "math/quadratic_equation_integer.hpp"
#include "math/linear_equations_integer.hpp"
#include "math/svp2d.hpp"
#include "math/modint/modint.hpp"
#include "math/crt.hpp"
#include <boost/multiprecision/cpp_int.hpp>

using Big = boost::multiprecision::cpp_int;

string base_string(Big v, int base)
{
  if (v == 0) return "0";
  bool negative = v < 0;
  if (negative) v = -v;
  string s;
  while (v != 0)
  {
    int d = (v % base).convert_to<int>();
    s += d < 10 ? char('0' + d) : char('A' + d - 10);
    v /= base;
  }
  if (negative) s += '-';
  reverse(ALL(s));
  return s;
}

template <int base, int digit, class T>
void check_scalar(const Big &x, T v)
{
  using BI = BigInteger<base, digit>;
  BI a(base_string(x, base));
  Big y = v;
  auto check = [&](const BI &got, const Big &expected) {
    assert(got.to_string() == base_string(expected, base));
  };
  check(a + v, x + y);
  check(a - v, x - y);
  check(a * v, x * y);
  if (v != 0)
  {
    check(a / v, x / y);
    check(a % v, x % y);
  }
  for (int position : {0, digit - 1, digit, digit + 1, 5 * digit - 1})
  {
    Big term = y;
    for (int i = 0; i < position; ++i) term *= base;
    BI add = a, sub = a;
    check(add.add_term(position, v), x + term);
    check(sub.sub_term(position, v), x - term);
  }
}

template <int base, int digit>
void bigint_boundaries(mt19937_64 &rng)
{
  constexpr ull radix = ipow(base, digit);
  constexpr ull limit = ULLONG_MAX / radix;
  vl signed_values{LLONG_MIN, LLONG_MAX, -1, 0, 1, INT_MIN, INT_MAX,
                   -ll(limit) - 1, -ll(limit), -ll(limit) + 1,
                   ll(limit) - 1, ll(limit)};
  if constexpr (limit < LLONG_MAX) signed_values.push_back(ll(limit) + 1);
  vc<ull> unsigned_values{0, 1, UINT_MAX, ULLONG_MAX, limit - 1, limit, limit + 1};
  for (int i = 0; i < 60; ++i)
  {
    Big x = 0;
    for (int j = 0; j < i % 12; ++j) { x <<= 64; x += rng(); }
    if (i & 1) x = -x;
    for (ll v : signed_values) check_scalar<base, digit>(x, v);
    for (ull v : unsigned_values) check_scalar<base, digit>(x, v);
    check_scalar<base, digit>(x, int(rng()));
    check_scalar<base, digit>(x, uint(rng()));
    // Small values stored in 128 bits also select the 64-bit loop.
    check_scalar<base, digit>(x, i128(limit));
    check_scalar<base, digit>(x, u128(limit));
  }
}

void check_quadratic(ll a, ll b, ll c)
{
  auto [count, roots] = quadratic_equation_integer(a, b, c);
  vc<Big> expected;
  if (a == 0)
  {
    if (b == 0) { assert(count == (c == 0 ? -1 : 0)); return; }
    if (-Big(c) % b == 0) expected.push_back(-Big(c) / b);
  }
  else
  {
    Big d = Big(b) * b - 4 * Big(a) * c;
    if (d >= 0)
    {
      Big s = sqrt(d), den = 2 * Big(a);
      if (s * s == d)
      {
        if ((-Big(b) - s) % den == 0) expected.push_back((-Big(b) - s) / den);
        if (s != 0 && (-Big(b) + s) % den == 0) expected.push_back((-Big(b) + s) / den);
      }
    }
  }
  assert(count == int(expected.size()));
  for (int i = 0; i < count; ++i) assert(Big(roots[i]) == expected[i]);
}

void check_linear(array<ll, 6> q)
{
  auto [a, b, c, d, e, f] = q;
  auto r = linear_equations_integer(a, b, c, d, e, f);
  Big det = Big(a) * e - Big(b) * d;
  if (det != 0)
  {
    Big x = Big(c) * e - Big(b) * f, y = Big(a) * f - Big(c) * d;
    if (x % det != 0 || y % det != 0) { assert(r.dim == -1); return; }
    assert(r.dim == 0 && Big(r.sol.first) == x / det && Big(r.sol.second) == y / det);
  }
  else
  {
    bool z1 = a == 0 && b == 0, z2 = d == 0 && e == 0;
    if ((z1 && c != 0) || (z2 && f != 0) ||
        Big(a) * f != Big(c) * d || Big(b) * f != Big(c) * e)
    { assert(r.dim == -1); return; }
    if (z1 && z2) { assert(r.dim == 2); return; }
    ll p = z1 ? d : a, q = z1 ? e : b, rhs = z1 ? f : c;
    ll g = std::gcd(p, q);
    if (rhs % g) { assert(r.dim == -1); return; }
    assert(r.dim == 1);
    assert(r.basis[0].first == q / g && r.basis[0].second == -p / g);
  }
  auto [x, y] = r.sol;
  assert(Big(a) * x + Big(b) * y == c && Big(d) * x + Big(e) * y == f);
}

void equations_boundaries(mt19937_64 &rng)
{
  constexpr ll bound = (1LL << 30) - 1;
  vl values{0, 1, -1, -bound - 1, -bound, -bound + 1, bound - 1, bound, bound + 1,
            -ll(INT_MAX) - 1, -ll(INT_MAX), INT_MAX, ll(INT_MAX) + 1};
  for (ll a : values) for (ll b : values) for (ll c : values) check_quadratic(a, b, c);
  check_linear({INT_MIN, INT_MIN, INT_MIN, INT_MAX, INT_MIN, INT_MAX});
  for (ll a : values) for (ll b : values)
  {
    check_linear({a, b, 1, a, b, 1});
    check_linear({a, b, a, -b, a, -b});
    if (a == 0 && b == 0) continue;
    for (ll c : values) for (ll d : values)
    {
      if (c == 0 && d == 0) continue;
      auto actual = svp2d(pair<ll, ll>{a, b}, {c, d});
      // Explicit i128 coordinates keep the reference on the wide path.
      auto expected = svp2d<i128, i128>({a, b}, {c, d});
      assert(actual.first == expected.first && actual.second == expected.second);
    }
  }
  for (int i = 0; i < 20000; ++i)
  {
    ll limit = i & 1 ? (1LL << 50) : bound;
    auto draw = [&] { return ll(rng() % (2 * limit + 1)) - limit; };
    check_quadratic(draw(), draw(), draw());
    check_linear({draw(), draw(), draw(), draw(), draw(), draw()});
    ll a = draw(), b = draw();
    check_linear({a, b, 1, a, b, 1});
    ll c = draw(), d = draw();
    if ((a || b) && (c || d))
    {
      auto actual = svp2d(pair<ll, ll>{a, b}, {c, d});
      auto expected = svp2d<i128, i128>({a, b}, {c, d});
      assert(actual.first == expected.first && actual.second == expected.second);
    }
  }
  assert((quadratic_equation_integer(0LL, 1LL, LLONG_MIN + 1).second[0] == LLONG_MAX));
}

template <ll mod>
void modint_boundaries(mt19937_64 &rng)
{
  using Mint = static_modint64<mod>;
  static_assert(is_same_v<decltype(Mint().val()), ll>);
  for (ll a : {0LL, 1LL, mod - 1}) for (ll b : {0LL, 1LL, mod - 1})
    assert((Mint(a) * Mint(b)).val() == i128(a % mod) * (b % mod) % mod);
  for (int i = 0; i < 10000; ++i)
  {
    ll a = rng() % mod, b = rng() % mod;
    assert((Mint(a) * Mint(b)).val() == i128(a) * b % mod);
  }
}

template <class T>
void check_crt(const vc<T> &residues, const vl &moduli)
{
  auto [r, m] = crt_mod<BigInteger<>>(residues, moduli);
  Big value(r.to_string()), product(m.to_string()), expected = 1;
  for (int i = 0; i < int(moduli.size()); ++i)
  {
    Big rem = Big(residues[i]) % moduli[i];
    if (rem < 0) rem += moduli[i];
    assert(value % moduli[i] == rem);
    expected *= moduli[i];
  }
  assert(product == expected && 0 <= value && value < product);
}

void crt_boundaries(mt19937_64 &rng)
{
  for (int n : {0, 1, 2, 3, 4, 8, 15, 16, 17, 32})
    for (bool large : {false, true})
    {
      vl moduli;
      for (ll p = 1000000007; int(moduli.size()) < n; p += 2)
        if (internal::isprime_constexpr(p)) moduli.push_back(p);
      if (large && n) moduli[n / 2] = (1LL << 61) - 1;
      for (int k = 0; k < 6; ++k)
      {
        vl signed_values(n);
        vc<ull> unsigned_values(n);
        for (int i = 0; i < n; ++i)
        {
          signed_values[i] = k == 0 ? LLONG_MIN : k == 1 ? LLONG_MAX : ll(rng());
          unsigned_values[i] = k == 0 ? ULLONG_MAX : rng();
        }
        check_crt(signed_values, moduli);
        check_crt(unsigned_values, moduli);
        if (!large)
        {
          vc<int> narrow_moduli(moduli.begin(), moduli.end());
          assert(crt_mod<BigInteger<>>(signed_values, narrow_moduli) ==
                 crt_mod<BigInteger<>>(signed_values, moduli));
          assert(crt_mod<BigInteger<>>(unsigned_values, narrow_moduli) ==
                 crt_mod<BigInteger<>>(unsigned_values, moduli));
        }
      }
      if (n) { moduli[0] = 1; check_crt(vl(n, -1), moduli); }
    }
}

template <size_t n>
void crt_constexpr_boundaries(mt19937_64 &rng)
{
  array<ll, n> moduli{}, residues{};
  int i = 0;
  for (ll p = 1000000007; i < int(n); p += 2)
    if (internal::isprime_constexpr(p)) moduli[i++] = p;
  for (bool large : {false, true})
  {
    if (large && n) moduli[n / 2] = (1LL << 61) - 1;
    for (int k = 0; k < 20; ++k)
    {
      for (size_t j = 0; j < n; ++j) residues[j] = rng() % moduli[j];
      auto [r, m] = crt_mod_constexpr<BigInteger<>>(residues, moduli);
      array<ull, n> unsigned_residues{};
      copy(residues.begin(), residues.end(), unsigned_residues.begin());
      assert(crt_mod_constexpr<BigInteger<>>(unsigned_residues, moduli) == make_pair(r, m));
      Big value(r.to_string()), product(m.to_string()), expected = 1;
      for (size_t j = 0; j < n; ++j)
      {
        assert(value % moduli[j] == residues[j]);
        expected *= moduli[j];
      }
      assert(product == expected && 0 <= value && value < product);
    }
  }
}

int main()
{
  mt19937_64 rng(20260930);
  bigint_boundaries<10, 6>(rng);
  bigint_boundaries<16, 5>(rng);
  bigint_boundaries<2, 21>(rng);
  bigint_boundaries<2, 1>(rng);
  equations_boundaries(rng);
  modint_boundaries<1>(rng);
  modint_boundaries<1000000007>(rng);
  modint_boundaries<4294967295LL>(rng);
  modint_boundaries<4294967296LL>(rng);
  modint_boundaries<(1LL << 61) - 1>(rng);
  crt_boundaries(rng);
  crt_constexpr_boundaries<0>(rng);
  crt_constexpr_boundaries<1>(rng);
  crt_constexpr_boundaries<2>(rng);
  crt_constexpr_boundaries<4>(rng);
  crt_constexpr_boundaries<16>(rng);
  cout << "Hello World\n";
}
