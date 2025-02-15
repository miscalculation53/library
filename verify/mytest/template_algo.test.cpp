#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_algo.hpp"
#include "template/template_dump.hpp"

// 集約
void test1()
{
  vl a = {2, 7, 1, 8, 2, 8, 1};
  assert(SUM(a) == 29);
  assert(MAX(a) == 8);
  assert(MIN(a) == 1);
  assert(ARGMAX(a) == 3);
  assert(ARGMIN(a) == 2);
  assert(mex(a) == 0);

  vl b = {4, 0, 1, 0, 1, 100};
  assert(mex(b) == 2);
}

// 順列
void test2()
{
  {
    assert(permid(5) == vl({0, 1, 2, 3, 4}));
    assert(permid(5, 1) == vl({1, 2, 3, 4, 5}));
  }

  {
    vl p = {1, 2, 0, 4, 3};
    vl q = perminv(p);
    repi(i, 5) assert(q[p[i]] == i);
    assert(perminv(q) == p);
  }

  {
    vl p = {1, 4, 2, 0, 3};
    vl q = {3, 1, 4, 0, 2};
    vl r = permuted(p, q);
    repi(i, 5) assert(r[i] == p[q[i]]);
  }
}

// string や vector の操作
void test3()
{
  {
    string s = "abcde";
    vl v = {0, 1, 2, 3, 4};
    assert(reversed(s) == "edcba");
    assert(s == "abcde");
    assert(reversed(v) == vl({4, 3, 2, 1, 0}));
    assert(v == vl({0, 1, 2, 3, 4}));
  }

  {
    string s = "bdcae";
    vl v = {4, 2, 1, 0, 3};
    assert(sorted(s) == "abcde");
    assert(sorted(s, greater{}) == "edcba");
    assert(s == "bdcae");
    assert(sorted(v) == vl({0, 1, 2, 3, 4}));
    assert(sorted(v, greater{}) == vl({4, 3, 2, 1, 0}));
    assert(v == vl({4, 2, 1, 0, 3}));
  }

  {
    string s = "bbabbccc";
    vl v = {1, 1, 0, 1, 1, 2, 2, 2};
    assert(uniqued(s) == "babc");
    assert(uniqued(v) == vl({1, 0, 1, 2}));
    assert(s == "bbabbccc");
    assert(v == vl({1, 1, 0, 1, 1, 2, 2, 2}));
    unique(s);
    unique(v);
    assert(s == "babc");
    assert(v == vl({1, 0, 1, 2}));

    assert(sortuniqued(s) == "abc");
    assert(sortuniqued(v) == vl({0, 1, 2}));
    assert(s == "babc");
    assert(v == vl({1, 0, 1, 2}));
    sortunique(s);
    sortunique(v);
    assert(s == "abc");
    assert(v == vl({0, 1, 2}));
  }

  {
    string s = "abcde";
    vl v = {0, 1, 2, 3, 4};
    assert(rotated(s, 1) == "bcdea");
    assert(rotated(s, 2) == "cdeab");
    assert(rotated(s, 1'000'000'000'000'003LL) == "deabc");
    assert(rotated(s, -1) == "eabcd");
    assert(rotated(v, 1) == vl({1, 2, 3, 4, 0}));
    assert(rotated(v, 2) == vl({2, 3, 4, 0, 1}));
    assert(rotated(v, 1'000'000'000'000'003LL) == vl({3, 4, 0, 1, 2}));
    assert(rotated(v, -1) == vl({4, 0, 1, 2, 3}));
    assert(s == "abcde");
    assert(v == vl({0, 1, 2, 3, 4}));
    rotate(s, 1'000'000'000'000'003LL);
    rotate(v, 1'000'000'000'000'003LL);
    assert(s == "deabc");
    assert(v == vl({3, 4, 0, 1, 2}));
  }
}

// 二次元配列の操作
void test4()
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

  vvl top_v = {
    {1, 3, 5},
    {2, 4, 6}
  };
  vstr top_s = {
    "135",
    "246"
  };

  vvl rot_v = {
    {2, 4, 6},
    {1, 3, 5}
  };
  vstr rot_s = {
    "246",
    "135"
  };

  assert(top(v) == top_v);
  assert(top(s) == top_s);
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

// 累積和・差分
void test5()
{
  vl a = {3, 1, 4, 1};
  assert(cuml(a) == vl({0, 3, 4, 8, 9}));
  assert(cumr(a) == vl({9, 6, 5, 1, 0}));
  assert(cumlmax(a) == vl({-INF, 3, 3, 4, 4}));
  assert(cumrmax(a) == vl({4, 4, 4, 1, -INF}));
  assert(cumlmin(a) == vl({INF, 3, 1, 1, 1}));
  assert(cumrmin(a) == vl({1, 1, 1, 1, INF}));
  assert(adjd(a) == vl({3, -2, 3, -3, -1}));
}

int main()
{
  test1();
  test2();
  test3();
  test4();
  test5();

  cout << "Hello World" << endl;
}