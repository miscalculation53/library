#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linear_equations_integer.hpp"

// Test focus: no/unique/infinite integer solution spaces and widened intermediate arithmetic.
bool is_multiple(pair<ll, ll> v, pair<ll, ll> d)
{
  if (d.first != 0)
    return v.first % d.first == 0 &&
           v.second == v.first / d.first * d.second;
  return d.second != 0 && v.first == 0 && v.second % d.second == 0;
}

int main()
{
  for (ll a = -2; a <= 2; a++)
    for (ll b = -2; b <= 2; b++)
      for (ll c = -2; c <= 2; c++)
        for (ll d = -2; d <= 2; d++)
          for (ll e = -2; e <= 2; e++)
            for (ll f = -2; f <= 2; f++)
            {
              auto res = linear_equations_integer(a, b, c, d, e, f);
              vc<pair<ll, ll>> brute;
              for (ll x = -8; x <= 8; x++)
                for (ll y = -8; y <= 8; y++)
                  if (a * x + b * y == c && d * x + e * y == f)
                    brute.eb(x, y);

              if (brute.empty())
              {
                assert(res.dim == -1);
                continue;
              }
              const ll det = a * e - b * d;
              const int expected_dim = (a == 0 && b == 0 && d == 0 && e == 0)
                                         ? 2 : (det == 0 ? 1 : 0);
              assert(res.dim == expected_dim);
              auto [x0, y0] = res.sol;
              assert(a * x0 + b * y0 == c && d * x0 + e * y0 == f);
              for (int i = 0; i < res.dim; i++)
              {
                auto [x, y] = res.basis[i];
                assert(a * x + b * y == 0 && d * x + e * y == 0);
              }
              for (auto [x, y] : brute)
              {
                pair<ll, ll> dif{x - x0, y - y0};
                if (res.dim == 0)
                  assert((dif == pair<ll, ll>{0, 0}));
                else if (res.dim == 1)
                  assert(is_multiple(dif, res.basis[0]));
              }
            }

  auto no_integer = linear_equations_integer(2, 4, 1, 4, 8, 2);
  assert(no_integer.dim == -1);

  auto all = linear_equations_integer(0, 0, 0, 0, 0, 0);
  assert(all.dim == 2);
  assert((all.basis == array<pair<ll, ll>, 2>{{{1, 0}, {0, 1}}}));

  auto large = linear_equations_integer<int>(
      50'000, 49'999, 500'030'000,
      49'999, 49'998, 500'020'000);
  assert((large.dim == 0 && large.sol == pair<int, int>{40'000, -30'000}));

  const ll n = 4'000'000'000LL;
  auto large_ll = linear_equations_integer(
      n, n - 1, n + 1,
      n - 1, n - 2, n);
  assert((large_ll.dim == 0 && large_ll.sol == pair<ll, ll>{2, -1}));

  PRINT("Hello World");
}
