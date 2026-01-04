#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/fps/rational_sum.hpp"
using mint = modint998244353;
using fps = FormalPowerSeries<mint>;

void test1()
{
  ll k = randint(1, 1000);
  ll l = randint(1, 1000);
  ll m = randint(1, 1000);
  ll n = randint(1, 1000);
  fps p(k), q(l), r(m), s(n);
  fem(e : p) e = randrange(0, mint::mod());
  fem(e : q) e = randrange(0, mint::mod());
  fem(e : r) e = randrange(0, mint::mod());
  fem(e : s) e = randrange(0, mint::mod());
  auto ans = rational_plus(pair{p, q}, pair{r, s});
  pair<fps, fps> god;
  god.first = p * s + q * r;
  god.second = q * s;
  assert(ans == god);
}

int main()
{
  rep(_, 1000) test1();

  cout << "Hello World" << endl;
}
