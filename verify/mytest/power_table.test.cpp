#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../template/template_all.hpp"
#include "../../math/modint/modint.hpp"
#include "../../math/modint/power_table.hpp"
using mint = modint;

void test1()
{
  mint::set_mod(998244353);
  PowerTable<mint> pw2(2);
  rep(i, 1000) assert(pw2.pow(i) == mint(2).pow(i));
  mint::set_mod(1'000'000'007);
  rep(i, 2000) assert(pw2.pow(i) == mint(2).pow(i));
}

int main()
{
  test1();

  cout << "Hello World" << endl;
}