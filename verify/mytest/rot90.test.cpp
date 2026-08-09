#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_algo.hpp"
#include "template/template_dump.hpp"

#include "algo/rot90.hpp"

// 二次元配列の回転
void test1()
{
  vvl v = {
    {1, 2},
    {3, 4},
    {5, 6}
  };
  vstr s = {
    "12",
    "34",
    "56"
  };

  vvl rot_v = {
    {2, 4, 6},
    {1, 3, 5}
  };
  vstr rot_s = {
    "246",
    "135"
  };

  assert(rot90(v) == rot_v);
  assert(rot90(s) == rot_s);

  assert(rot90(v, -11) == rot90(v));
  assert(rot90(v, -10) == rot90(rot90(v)));
  assert(rot90(v, -9) == rot90(rot90(rot90(v))));
  assert(rot90(v, 8) == v);
  assert(rot90(v, 9) == rot90(v));
  assert(rot90(v, 10) == rot90(rot90(v)));
  assert(rot90(v, 11) == rot90(rot90(rot90(v))));
}

int main()
{
  test1();

  cout << "Hello World" << endl;
}
