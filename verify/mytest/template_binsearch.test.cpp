#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../template/template_binsearch.hpp"
#include "../../template/template_dump.hpp"

void test1()
{
  vl a = {1, 3, 3, 3, 4, 5, 5, 6};
  rep(k, 10)
  {
    ll i1, i2, i3;

    i1 = LB(a, k);
    i2 = binsearch([&](ll i)
                   { return k <= vecget(a, i); }, SZI(a), -1)
             .first;
    i3 = expsearch([&](ll i)
                   { return k <= vecget(a, i); }, 0, true)
             .first;
    assert(i1 == i2 && i2 == i3);

    i1 = UB(a, k);
    i2 = binsearch([&](ll i)
                   { return k < vecget(a, i); }, SZI(a), -1)
             .first;
    i3 = expsearch([&](ll i)
                   { return k < vecget(a, i); }, 0, true)
             .first;
    assert(i1 == i2 && i2 == i3);
  }
}

int main()
{
  test1();

  cout << "Hello World" << endl;
}