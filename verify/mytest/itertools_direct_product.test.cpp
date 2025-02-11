#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../itertools/direct_product.hpp"

void test1()
{
  vvl vs;
  fec(v : direct_product({2, 1, 3})) vs.push_back(v);

  vvl model = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 0, 2},
    {1, 0, 0},
    {1, 0, 1},
    {1, 0, 2}
  };
  assert(vs == model);
}

int main()
{
  test1();

  cout << "Hello World" << endl;
}