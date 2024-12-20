#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../template/template_math.hpp"
#include "../../template/template_dump.hpp"

mt19937 mt;
ll randll() { return (mt() << 30) + mt(); }

template <class T>
void test1()
{
  for (int t = 0; t < 100000; t++)
  {
    dump(t);
    T a = randll(), b = randll();
    if (b == 0)
      continue;
    if (mt() % 2) a *= -1;
    if (mt() % 2) b *= -1;

    T c = divfloor<T>(a, b), d = divceil<T>(a, b), e = safemod<T>(a, b);
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
  test1<ll>();
  test1<i128>();

  cout << "Hello World" << endl;
}