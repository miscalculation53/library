#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/modint/modint.hpp"
#include "math/modint/to_rational.hpp"

void test1()
{
  using mint = modint998244353;
  rep(p, -100, 101) rep(q, -100, 101)
  {
    if (q == 0)
      continue;
    mint r = mint(p) / mint(q);
    auto [pp, qq] = mint_to_rat(r);
    assert(p * qq == pp * q);
  }
}

int main()
{
  test1();

  cout << "Hello World" << endl;
}
