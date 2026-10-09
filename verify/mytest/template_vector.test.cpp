#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_vector.hpp"
#include "template/template_dump.hpp"
#include "math/modint/modint.hpp"

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

void test_mixed_sizes()
{
  ll n = 3;
  int m = 4;
  auto dp = dvec({n, m, 2}, 7LL);
  static_assert(is_same_v<decltype(dp), vvvc<ll>>);
  assert(dp.size() == 3 && dp[0].size() == 4 && dp[0][0].size() == 2);
  for (const auto &plane : dp)
    for (const auto &row : plane)
      for (ll x : row) assert(x == 7);
  dp[1][2][0] = 9;
  assert(dp[0][2][0] == 7 && dp[1][2][1] == 7);

  auto ints = dvec({2, n}, 0);
  static_assert(is_same_v<decltype(ints), vvc<int>>);
  assert(ints.size() == 2 && ints[0].size() == 3);
  auto empty = dvec({n, 0, m}, -1);
  assert(empty.size() == 3 && empty[0].empty());

  // 既存の配列を渡す場合の型推論も保つ。
  size_t sizes[] = {2, 3};
  auto strings = dvec(sizes, string("value"));
  assert(strings.size() == 2 && strings[0].size() == 3 && strings[1][2] == "value");
  ll sizes_ll[] = {2, 3};
  assert(dvec(sizes_ll, 5) == vvc<int>(2, vc<int>(3, 5)));
}

void test_explicit_type()
{
  using mint = modint998244353;
  ll n = 3;
  int m = 4;
  auto dp = dvec<mint>({n, 2}, 0);
  static_assert(is_same_v<decltype(dp), vvc<mint>>);
  assert(dp.size() == 3 && dp[0].size() == 2);
  for (const auto &row : dp)
    for (mint x : row) assert(x == 0);
  dp[1][0] += 7;
  assert(dp[1][0] == 7 && dp[0][0] == 0 && dp[1][1] == 0);
  fill(dp, 2);
  for (const auto &row : dp)
    for (mint x : row) assert(x == 2);

  auto three = dvec<mint>({2, n, m}, 1);
  static_assert(is_same_v<decltype(three), vvvc<mint>>);
  assert(three.size() == 2 && three[0].size() == 3 && three[0][0].size() == 4);
  assert(three[1][2][3] == 1);
  auto ints = dvec<mint>({2, 3}, 0);
  static_assert(is_same_v<decltype(ints), vvc<mint>>);
  assert(ints.size() == 2 && ints[0].size() == 3 && ints[1][2] == 0);
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
  test_mixed_sizes();
  test_explicit_type();
  test2();
  test3();

  cout << "Hello World" << endl;
}
