#define PROBLEM "https://judge.yosupo.jp/problem/discrete_logarithm_mod"

#include "math/discrete_log_monoid.hpp"
#include "algebra/algebra_basic_ops.hpp"
#include "math/modint/modint.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  using mint = dynamic_modint32<719>;
  int cases;
  cin >> cases;
  while (cases--)
  {
    ll x, y, modulus;
    cin >> x >> y >> modulus;
    mint::set_mod(modulus);
    auto result = discrete_log_monoid<MonoidMul<mint>>(mint(x), mint(y), modulus);
    cout << result.value_or(-1) << '\n';
  }
}
