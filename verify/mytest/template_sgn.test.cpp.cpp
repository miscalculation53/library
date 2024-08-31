#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#define EPS 1e-11

#include "../../template/template_math.hpp"
#include "../../template/template_dump.hpp"

void test1()
{
  assert(sgn(-2) == -1);
  assert(sgn(-1) == -1);
  assert(sgn(0) == 0);
  assert(sgn(1) == 1);
  assert(sgn(2) == 1);

  assert(sgn(-EPS * 2) == -1);
  assert(sgn(-EPS / 2) == 0);
  assert(sgn(EPS / 2) == 0);
  assert(sgn(EPS * 2) == 1);

  long double EPS2 = EPS;
  assert(sgn(-EPS2 * 2) == -1);
  assert(sgn(-EPS2 / 2) == 0);
  assert(sgn(EPS2 / 2) == 0);
  assert(sgn(EPS2 * 2) == 1);

  dump(typeid(double).name(), typeid(long double).name());
  dump(typeid(EPS).name(), typeid(EPS2).name());
}

int main()
{
  test1();

  ll A, B;
  cin >> A >> B;
  cout << A + B << endl;
}