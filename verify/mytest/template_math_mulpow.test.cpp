#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "../../template/template_math.hpp"
#include "../../template/template_dump.hpp"

mt19937 mt;

void test1()
{
  for (int t = 0; t < 100000; t++)
  {
    int a = mt() % (mt() % 2 == 0 ? 1 << 15 : 1 << 30);
    int b = mt() % (mt() % 2 == 0 ? 1 << 15 : 1 << 30);
    int m = mt() % (1 << 30);
    int god = min(ll(a) * ll(b), ll(m));
    int ans = mul_limited<int>(a, b, m);
    dump(t, god == m);
    assert(god == ans);
  }
}

void test2()
{
  for (int t = 0; t < 100000; t++)
  {
    int a = mt() % 11;
    int b = mt() % 19;
    int m = mt() % (mt() % 2 == 0 ? 10 : 1 << 30);
    int god = min(ipow(a, b), (ll)m);
    int ans = pow_limited<int>(a, b, m);
    dump(t, a, b, ipow(a, b), m, god, ans);
    assert(god == ans);
  }
}

int main()
{
  test1();
  test2();

  ll A, B;
  cin >> A >> B;
  cout << A + B << endl;
}