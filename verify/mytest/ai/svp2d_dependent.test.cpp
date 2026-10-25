#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/svp2d.hpp"

// Test focus: a linearly dependent two-vector basis still returns its shortest vector.
int main()
{
  auto v = svp2d(pair{2LL, 0LL}, pair{4LL, 0LL});
  assert((v == pair<ll, ll>{2, 0}));
  for (auto [a, b] : vc<pair<pll, pll>>{{{1, 1}, {2, 3}}, {{1, 1}, {-2, -3}}, {{1, 2}, {4, 7}}})
  {
    auto [v1, v2] = svp2d(a, b);
    ll expected = numeric_limits<ll>::max();
    for (ll x = -16; x <= 16; ++x)
      for (ll y = -16; y <= 16; ++y)
      {
        ll p = x * a.first + y * b.first, q = x * a.second + y * b.second;
        if (p != 0 || q != 0) chmin(expected, p * p + q * q);
      }
    assert(v1 * v1 + v2 * v2 == expected);
  }
  // 射影係数が ll の正の範囲を超えても、更新後のベクトルは ll に収まる。
  auto [v1, v2] = svp2d(pair{-1LL, 0LL}, pair{numeric_limits<ll>::lowest(), 1LL});
  assert(i128(v1) * v1 + i128(v2) * v2 == 1);
  PRINT("Hello World");
}
