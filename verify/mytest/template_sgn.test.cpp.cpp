#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#define EPS 1e-11

#include "../../template/template_math.hpp"
#include "../../template/template_dump.hpp"

void test1()
{
  assert(SGN(-2) == -1);
  assert(SGN(-1) == -1);
  assert(SGN(0) == 0);
  assert(SGN(1) == 1);
  assert(SGN(2) == 1);

  assert(SGN(-EPS * 2) == -1);
  assert(SGN(-EPS / 2) == 0);
  assert(SGN(EPS / 2) == 0);
  assert(SGN(EPS * 2) == 1);

  long double EPS2 = EPS;
  assert(SGN(-EPS2 * 2) == -1);
  assert(SGN(-EPS2 / 2) == 0);
  assert(SGN(EPS2 / 2) == 0);
  assert(SGN(EPS2 * 2) == 1);

  dump(typeid(double).name(), typeid(long double).name());
  dump(typeid(EPS).name(), typeid(EPS2).name());
}

int main()
{
  test1();

  cout << "Hello World" << endl;
}