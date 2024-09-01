#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "../../template/template_vector.hpp"
#include "../../template/template_dump.hpp"

void test1()
{
  auto dp = dvec({3, 4, 5}, 0LL);
  dump(dp);
  assert(SZ(dp) == 3);
  rep(i, 3)
  {
    assert(SZ(dp.at(i)) == 4);
    rep(j, 4) assert(SZ(dp.at(i).at(j)) == 5);
  }
}

void test2()
{
  assert(ctol('J', "JOI") == 0);
  assert(ctol('O', "JOI") == 1);
  assert(ctol('I', "JOI") == 2);
  assert(ctol('?', "JOI") == -1);

  vl v = {0, 1, 2, 3, 4};
  auto v1 = stov("ABCDE", 'A');
  auto v2 = stov("abcde", 'a');
  auto v3 = stov("01234", '0');
  assert(v == v1 && v == v2 && v == v3);

  vl w = {0, 1, 2, 1, 0, 2};
  auto w1 = stov("RSPSRP", "RSP");
  assert(w == w1);
}

void test3()
{
  vvl vv = {
    {0, 1},
    {2, 3, 4},
    {5}
  };
  vl v = {0, 1, 2, 3, 4, 5};

  vl v1 = concat(vv);
  vl v2 = concat(vv.at(0), vv.at(1), vv.at(2));
  vl v3 = concat(vv.at(0), vc{2, 3, 4}, vc{5});
  dump(v, v1, v2, v3);
  assert(v == v1 && v == v2 && v == v3);
}

int main()
{
  test1();
  test2();
  test3();

  ll A, B;
  cin >> A >> B;
  cout << A + B << endl;
}