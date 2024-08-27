#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "../../template/template_inout.hpp"
#include "../../template/template_dump.hpp"

void test1()
{
  vc<pair<int, string>> vt_god = {
    {1, "a"},
    {2, "bcd"},
    {3, "ef"}
  };
  pair<vc<int>, vc<string>> tv_god = {
    {1, 2, 3},
    {"a", "bcd", "ef"}
  };

  auto tv_ans = top(vt_god);
  auto vt_ans = top(tv_god);
  dump(tv_ans, vt_ans);
  assert(tv_ans == tv_god);
  assert(vt_ans == vt_god);
}

void test2()
{
  vc<array<int, 5>> vt_god = {
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10},
    {11, 12, 13, 14, 15}
  };
  array<vc<int>, 5> tv_god = {{
    {1, 6, 11},
    {2, 7, 12},
    {3, 8, 13},
    {4, 9, 14},
    {5, 10, 15}
  }};

  auto tv_ans = top(vt_god);
  auto vt_ans = top(tv_god);
  dump(tv_ans, vt_ans);
  assert(tv_ans == tv_god);
  assert(vt_ans == vt_god);
}

void test3()
{
  vc<tuple<int, int, int, int, string>> vt_god = {
    {1, 2, 3, 4, "a"},
    {5, 6, 7, 8, "bcd"},
    {9, 10, 11, 12, "ef"}
  };
  tuple<vc<int>, vc<int>, vc<int>, vc<int>, vc<string>> tv_god = {
    {1, 5, 9},
    {2, 6, 10},
    {3, 7, 11},
    {4, 8, 12},
    {"a", "bcd", "ef"}
  };

  auto tv_ans = top(vt_god);
  auto vt_ans = top(tv_god);
  dump(tv_ans, vt_ans);
  assert(tv_ans == tv_god);
  assert(vt_ans == vt_god);
}

int main()
{
  test1();
  test2();
  test3();

  LL(A, B);
  PRINT(A + B);
}