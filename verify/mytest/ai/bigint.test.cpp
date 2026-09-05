#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/bigint.hpp"

// Test focus: signed arithmetic, leading zeros, decimal and hexadecimal I/O,
// scalar operations, and term updates cover sign and limb boundaries.
int digit_value(char c)
{
  if ('0' <= c && c <= '9')
    return c - '0';
  if ('a' <= c && c <= 'z')
    return c - 'a' + 10;
  return c - 'A' + 10;
}

string to_base_string(i128 x, int base)
{
  if (x == 0)
    return "0";
  bool neg = x < 0;
  u128 y = x;
  if (neg)
    y = -y;
  string s;
  while (y > 0)
  {
    int d = int(y % base);
    s += d < 10 ? char('0' + d) : char('A' + d - 10);
    y /= base;
  }
  if (neg)
    s += '-';
  reverse(ALL(s));
  return s;
}

string normalize(string s)
{
  bool neg = !s.empty() && s[0] == '-';
  int p = neg;
  while (p < SZ(s) && s[p] == '0')
    ++p;
  if (p == SZ(s))
    return "0";
  string t = neg ? "-" : "";
  for (; p < SZ(s); ++p)
    t += char(toupper(s[p]));
  return t;
}

string random_large(mt19937 &rng, int base, int max_length = 100)
{
  int n = rng() % max_length + 1;
  string s(n, '0');
  repi(i, n)
  {
    int d = rng() % base;
    s[i] = d < 10 ? char('0' + d) : char((rng() & 1 ? 'A' : 'a') + d - 10);
  }
  if (rng() % 3 == 0)
    s = '-' + s;
  return s;
}

template <int base, int digit>
void test_base(mt19937 &rng)
{
  using bigint = BigInteger<base, digit>;
  repi(_, 2000)
  {
    ll xv = ull(rng()) * rng() % 1'000'000'000'000'000'000ULL;
    ll yv = ull(rng()) * rng() % 1'000'000'000'000'000'000ULL;
    if (rng() & 1)
      xv = -xv;
    if (rng() & 1)
      yv = -yv;
    i128 x = xv, y = yv;
    bigint a(to_base_string(x, base)), b(to_base_string(y, base));

    assert(a.to_string() == to_base_string(x, base));
    assert((a < b) == (x < y));
    assert((a == b) == (x == y));
    assert((a + b).to_string() == to_base_string(x + y, base));
    assert((a - b).to_string() == to_base_string(x - y, base));
    assert((a * b).to_string() == to_base_string(x * y, base));
    bigint twice = a, zero = a, square = a;
    twice += twice;
    zero -= zero;
    square *= square;
    assert(twice.to_string() == to_base_string(x * 2, base));
    assert(zero == 0);
    assert(square.to_string() == to_base_string(x * x, base));
    if (y)
    {
      auto [q, r] = a.divmod(b);
      assert(q.to_string() == to_base_string(x / y, base));
      assert(r.to_string() == to_base_string(x % y, base));
    }

    ll v = int(rng() % 2'000'000'001) - 1'000'000'000;
    assert((a + v).to_string() == to_base_string(x + v, base));
    assert((a - v).to_string() == to_base_string(x - v, base));
    assert((a * v).to_string() == to_base_string(x * v, base));
    if (v)
    {
      assert((a / v).to_string() == to_base_string(x / v, base));
      assert((a % v).to_string() == to_base_string(x % v, base));
    }

    int i = rng() % (base == 10 ? 30 : 20);
    int c = int(rng() % 2001) - 1000;
    i128 term = c;
    repi(_, i) term *= base;
    bigint add = a, sub = a;
    add.add_term(i, c);
    sub.sub_term(i, c);
    assert(add.to_string() == to_base_string(x + term, base));
    assert(sub.to_string() == to_base_string(x - term, base));
  }

  repi(_, 500)
  {
    string sa = random_large(rng, base), sb = random_large(rng, base);
    bigint a(sa), b(sb);
    assert(a.to_string() == normalize(sa));
    assert(b.to_string() == normalize(sb));
    assert(a + b - b == a);
    assert(a - b + b == a);
    if (b != 0)
    {
      auto [q, r] = a.divmod(b);
      assert(q * b + r == a);
    }
  }

  repi(_, 100)
  {
    bigint a(random_large(rng, base, 1000)), b(random_large(rng, base, 500));
    if (b == 0) continue;
    auto [q, r] = a.divmod(b);
    assert(q * b + r == a);
    assert(r.abs() < b.abs());
    assert(r == 0 || (r < 0) == (a < 0));
  }

  bigint large(random_large(rng, base));
  for (int d : {-123457, -1, 1, 123457})
  {
    bigint small(d);
    auto [q, r] = large.divmod(small);
    assert(large * small / small == large);
    assert(q * small + r == large);
  }
}

int main()
{
  mt19937 rng(123456789);
  test_base<10, 6>(rng);
  test_base<16, 5>(rng);
  assert(BigInteger<>(numeric_limits<ll>::min()).to_string() == std::to_string(numeric_limits<ll>::min()));
  cout << "Hello World" << endl;
}
