#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_binsearch.hpp"
#include "template/template_dump.hpp"

mt19937 mt;
void test1()
{
  ll n = 1 + mt() % 10;
  vl a(n);
  rep(i, n) a[i] = 1 + mt() % 10;
  sort(ALL(a));
  ll k = -1 + mt() % 12;

  ll i1, i2, i3;

  i1 = LB(a, k);
  i2 = binsearch([&](ll i)
                  { return k <= vecget(a, i); }, SZ<int>(a), -1)
            .first;
  i3 = expsearch([&](ll i)
                  { return k <= vecget(a, i); }, 0, true)
            .first;
  assert(i1 == i2 && i2 == i3);

  i1 = UB(a, k);
  i2 = binsearch([&](ll i)
                  { return k < vecget(a, i); }, SZ<int>(a), -1)
            .first;
  i3 = expsearch([&](ll i)
                  { return k < vecget(a, i); }, 0, true)
            .first;
  assert(i1 == i2 && i2 == i3);
}

void test2()
{
  ll n = mt() % 11;
  vl a(n);
  rep(i, n) a[i] = 1 + mt() % 10;
  sort(ALL(a));
  ll k = -1 + mt() % 12;
  optional<ll> lt, leq, gt, geq;
  rep(i, n)
  {
    if (a[i] < k) lt = i;
    if (a[i] <= k) leq = i;
    if (!gt && a[i] > k) gt = i;
    if (!geq && a[i] >= k) geq = i;
  }
  assert(lt_max(a, k) == lt.value_or(-1));
  assert(leq_max(a, k) == leq.value_or(-1));
  assert(gt_min(a, k) == gt.value_or(n));
  assert(geq_min(a, k) == geq.value_or(n));
  multiset<ll> values(ALL(a));
  auto check_iterator = [&](auto result, optional<ll> expected)
  {
    assert(bool(result) == bool(expected));
    if (result) assert(**result == a[*expected]);
  };
  check_iterator(lt_max(values, k), lt);
  check_iterator(leq_max(values, k), leq);
  check_iterator(gt_min(values, k), gt);
  check_iterator(geq_min(values, k), geq);
  assert(lt_cnt(a, k) == count_if(ALL(a), [&](ll x) { return x < k; }));
  assert(leq_cnt(a, k) == count_if(ALL(a), [&](ll x) { return x <= k; }));
  assert(gt_cnt(a, k) == count_if(ALL(a), [&](ll x) { return x > k; }));
  assert(geq_cnt(a, k) == count_if(ALL(a), [&](ll x) { return x >= k; }));
}

void test3()
{
  vc<int> v = {1, 3, 5, 7, 9};
  auto i = leq_max(v, 6);
  assert(i == 2 && v[i] == 5);
  set<int> s = {1, 3, 5, 7, 9};
  auto it = leq_max(s, 6);
  assert(it && **it == 5);
}

// https://atcoder.jp/contests/kupc2013/tasks/kupc2013_a
void test4()
{
  using P = pair<int, string>;
  auto solve = [](int q, vc<P> v) -> string
  {
    v.insert(v.begin(), {1, "kogakubu10gokan"});
    int i = leq_max(v, q, {}, [](const P &p)
                    { return p.first; });
    return v[i].second;
  };

  assert(solve(12, {
    {5, "sogo5gokan"},
    {10, "sogo10gokan"},
    {15, "sogo15gokan"}
  }) == "sogo10gokan");
  assert(solve(10, {
    {5, "kogakubu11gokan"},
    {10, "sogo10gokan"},
    {15, "KyotoUniversityResearchPark"}
  }) == "sogo10gokan");
  assert(solve(3, {
    {5, "kogakubu11gokan"},
    {10, "sogo10gokan"},
    {15, "KyotoUniversityResearchPark"}
  }) == "kogakubu10gokan");
}

int main()
{
  rep(_, 10000) test1();
  rep(_, 10000) test2();
  static_assert(is_same_v<decltype(lt_max<int>(vc<int>{}, 0)), int>);
  static_assert(is_same_v<decltype(lt_max<i128>(vc<int>{}, 0)), i128>);
  static_assert(is_same_v<decltype(geq_min<unsigned>(vc<int>{}, 0)), unsigned>);
  assert(lt_max<int>(vc<int>{}, 0) == -1);
  assert(geq_min<unsigned>(vc<int>{}, 0) == 0u);
  assert(geq_min<unsigned>(vc<int>{0}, 0) == 0u);
  assert(gt_min(vc<int>{0}, 0) == 1);
  assert(!lt_max(set<int>{}, 0));
  test3();
  test4();

  cout << "Hello World" << endl;
}
