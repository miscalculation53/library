#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../template/template_math.hpp"
#include "../../template/template_dump.hpp"

mt19937 mt;

void test1()
{
  assert(ipow(0, 0) == 1);
  assert(ipow(0, 1) == 0);
  assert(ipow(1, 1LL << 60) == 1);
  assert(ipow(-1, 1LL << 60) == 1);
  assert(ipow(-1, (1LL << 60) - 1) == -1);
  assert(ipow(2, 10) == 1024);
  assert(ipow(2, 60) == 1LL << 60);
  assert(ipow(3, 10) == 59049);
  assert(ipow(-4, 10) == 1048576);
  assert(ipow(-4, 11) == -4194304);
}

void test2()
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

void test3()
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
  test3();

  cout << "Hello World" << endl;
}