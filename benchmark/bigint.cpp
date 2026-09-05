#include "template/template_all_but_modint.hpp"
#include "math/bigint.hpp"

using bigint = BigInteger<>;

volatile ull bigint_sink = 0;

template <class F>
double median_ms(F f)
{
  constexpr int rounds = 5;
  array<double, rounds> times;
  repi(i, rounds)
  {
    auto begin = chrono::steady_clock::now();
    bigint_sink ^= f();
    auto end = chrono::steady_clock::now();
    times[i] = chrono::duration<double, milli>(end - begin).count();
  }
  sort(ALL(times));
  return times[rounds / 2];
}

string random_decimal(mt19937 &rng, int n)
{
  string s(n, '0');
  s[0] = char('1' + rng() % 9);
  repi(i, 1, n) s[i] = char('0' + rng() % 10);
  return s;
}

ull parse_many(const vc<string> &src)
{
  ull res = 0;
  fec(s : src)
  {
    bigint x(s);
    res += x > 0;
  }
  return res;
}

ull format_many(const vc<bigint> &src)
{
  ull res = 0;
  fec(x : src) res += x.to_string().size();
  return res;
}

ull add_many(const vc<pair<bigint, bigint>> &src, bool subtract)
{
  ull res = 0;
  for (const auto &[a, b] : src)
  {
    bigint c = subtract ? a - b : a + b;
    res += c >= 0;
  }
  return res;
}

ull multiply_many(const bigint &a, const bigint &b, int count)
{
  ull res = 0;
  repi(_, count)
  {
    bigint c = a * b;
    res += c > a;
  }
  return res;
}

ull divide_many(const bigint &a, const bigint &b, int count)
{
  ull res = 0;
  repi(_, count)
  {
    auto [q, r] = a.divmod(b);
    res += q > r;
  }
  return res;
}

ull quotient_many(const bigint &a, const bigint &b, int count)
{
  ull res = 0;
  repi(_, count) res += a / b > 0;
  return res;
}

int main()
{
  mt19937 rng(123456789);

  vc<string> text(2000);
  fem(s : text) s = random_decimal(rng, 2000);
  vc<bigint> parsed;
  parsed.reserve(text.size());
  fec(s : text) parsed.eb(s);

  vc<pair<bigint, bigint>> add_src;
  add_src.reserve(1000);
  repi(i, 1000) add_src.eb(parsed[2 * i], parsed[2 * i + 1]);

  bigint small_a(random_decimal(rng, 600)), small_b(random_decimal(rng, 600));
  bigint medium_a(random_decimal(rng, 1800)), medium_b(random_decimal(rng, 1800));
  bigint large_a(random_decimal(rng, 30000)), large_b(random_decimal(rng, 30000));
  bigint one_limb(123457);
  bigint div_a(random_decimal(rng, 6000)), div_b(random_decimal(rng, 3000));
  bigint div_g(random_decimal(rng, 7200)), div_h(random_decimal(rng, 3600));
  bigint div_i(random_decimal(rng, 9000)), div_j(random_decimal(rng, 4500));
  bigint div_c(random_decimal(rng, 12000)), div_d(random_decimal(rng, 6000));
  bigint div_e(random_decimal(rng, 30000)), div_f(random_decimal(rng, 15000));

  cout << fixed << setprecision(3);
  cout << "parse: 2000 x 2000 decimal digits  " << median_ms([&] { return parse_many(text); }) << " ms\n";
  cout << "format: 2000 x 2000 decimal digits " << median_ms([&] { return format_many(parsed); }) << " ms\n";
  cout << "add: 1000 x 2000 decimal digits    " << median_ms([&] { return add_many(add_src, false); }) << " ms\n";
  cout << "sub: 1000 x 2000 decimal digits    " << median_ms([&] { return add_many(add_src, true); }) << " ms\n";
  cout << "multiply: 600 decimal digits x100  " << median_ms([&] { return multiply_many(small_a, small_b, 100); }) << " ms\n";
  cout << "multiply: 1800 decimal digits x100 " << median_ms([&] { return multiply_many(medium_a, medium_b, 100); }) << " ms\n";
  cout << "multiply: 30000 decimal digits x3  " << median_ms([&] { return multiply_many(large_a, large_b, 3); }) << " ms\n";
  cout << "multiply: 30000 digits by one limb " << median_ms([&] { return multiply_many(large_a, one_limb, 1000); }) << " ms\n";
  cout << "divmod: 6000 / 3000 digits x3      " << median_ms([&] { return divide_many(div_a, div_b, 3); }) << " ms\n";
  cout << "quotient: 6000 / 3000 digits x3    " << median_ms([&] { return quotient_many(div_a, div_b, 3); }) << " ms\n";
  cout << "divmod: 7200 / 3600 digits         " << median_ms([&] { return divide_many(div_g, div_h, 1); }) << " ms\n";
  cout << "divmod: 9000 / 4500 digits         " << median_ms([&] { return divide_many(div_i, div_j, 1); }) << " ms\n";
  cout << "divmod: 12000 / 6000 digits        " << median_ms([&] { return divide_many(div_c, div_d, 1); }) << " ms\n";
  cout << "divmod: 30000 / 15000 digits       " << median_ms([&] { return divide_many(div_e, div_f, 1); }) << " ms\n";
  cout << "divmod: 6000 digits by one limb    " << median_ms([&] { return divide_many(div_a, one_limb, 1000); }) << " ms\n";
  cout << bigint_sink << '\n';
}
