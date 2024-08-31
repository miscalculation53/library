#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "../../template/template_math.hpp"
#include "../../template/template_dump.hpp"

mt19937 mt;
ll randll() { return (mt() << 30) + mt(); }

void test1()
{
  for (int t = 0; t < 100000; t++)
  {
    dump(t);
    ll a = randll(), b = randll();
    if (b == 0)
      continue;
    if (mt() % 2) a *= -1;
    if (mt() % 2) b *= -1;

    ll c = divfloor(a, b), d = divceil(a, b), e = safemod(a, b);
    dump(a, b, c, d, e);
    if (b > 0)
    {
      assert(a >= b * c && a < b * (c + 1));
      assert(a > b * (d - 1) && a <= b * d);
      assert(0 <= e && e < b);
      assert((a - e) % b == 0);
    }
    else
    {
      assert(a < b * c && a >= b * (c + 1));
      assert(a <= b * (d - 1) && a > b * d);
      assert(b < e && e <= 0);
      assert((a - e) % b == 0);
    }
  }
}

int main()
{
  test1();

  ll A, B;
  cin >> A >> B;
  cout << A + B << endl;
}