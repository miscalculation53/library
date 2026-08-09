#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/quadratic_equation_integer.hpp"

// Test focus: degree zero/one/two cases and coefficients requiring widened intermediates.
int main()
{
  for (int a = -5; a <= 5; a++)
    for (int b = -5; b <= 5; b++)
      for (int c = -5; c <= 5; c++)
      {
        auto [cnt, sol] = quadratic_equation_integer(a, b, c);
        if (a == 0 && b == 0 && c == 0)
        {
          assert(cnt == -1);
          continue;
        }
        set<ll> expected;
        for (ll x = -20; x <= 20; x++)
          if (a * x * x + b * x + c == 0)
            expected.insert(x);
        assert(cnt == (int)expected.size());
        assert(set<ll>(sol.begin(), sol.begin() + cnt) == expected);
      }

  auto [cnt_int, sol_int] = quadratic_equation_integer<int>(1, -90'000, 2'000'000'000);
  assert(cnt_int == 2);
  assert((set<int>(sol_int.begin(), sol_int.end()) == set<int>{40'000, 50'000}));

  auto [cnt_ll, sol_ll] = quadratic_equation_integer(
      1LL, -4'000'000'000LL, 3'999'999'999LL);
  assert(cnt_ll == 2);
  assert((set<ll>(sol_ll.begin(), sol_ll.end()) ==
          set<ll>{1, 3'999'999'999LL}));

  auto [cnt_min, sol_min] = quadratic_equation_integer(0LL, -1LL, numeric_limits<ll>::min());
  assert(cnt_min == 1 && sol_min[0] == numeric_limits<ll>::min());

  PRINT("Hello World");
}
