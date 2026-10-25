#define PROBLEM "https://yukicoder.me/problems/no/2890"

#include "algebra/clamp_add_cost.hpp"

int main()
{
  LL(N);
  INT(K);
  VEC(ll, K, A);
  const ll origin = A[0], circumference = 2 * N;
  for (auto &x : A) x -= origin;
  A.push_back(circumference);
  A.push_back(circumference + A[1]);
  using M = MonoidClampAddCost<ll, 1, 1>;
  auto feasible = [&](ll distance)
  {
    auto f = M::e();
    repi(i, K)
      f = M::op(M::S::shift_then_clamp(distance, A[i + 1] + 1, A[i + 2] - 1), f);
    for (ll x = 1; x < A[1]; x += 2)
    {
      const auto [y, adjustment] = f(x);
      const ll decrease = (adjustment + x + ll(K) * distance - y) / 2;
      if (decrease == 0 && y <= x + circumference) return true;
    }
    return false;
  };
  ll lo = 0, hi = N + 1;
  while (hi - lo > 1)
  {
    const ll mid = (lo + hi) / 2;
    if (feasible(2 * mid)) lo = mid;
    else hi = mid;
  }
  PRINT(2 * lo);
}
