#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../template/template_binsearch.hpp"
#include "../../template/template_dump.hpp"

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
  ll n = 1 + mt() % 10;
  vl a(n);
  rep(i, n) a[i] = 1 + mt() % 10;
  sort(ALL(a));
  ll k = -1 + mt() % 12;

  ll i1, i2;
  i1 = lt_max(a, k);
  i2 = expsearch([&](ll i)
                 { return vecget(a, i) < k; }, 0, true)
           .first;
  assert(i1 == i2);
  i1 = leq_max(a, k);
  i2 = expsearch([&](ll i)
                 { return vecget(a, i) <= k; }, 0, true)
           .first;
  assert(i1 == i2);
  i1 = gt_min(a, k);
  i2 = expsearch([&](ll i)
                 { return k < vecget(a, i); }, 0, true)
           .first;
  assert(i1 == i2);
  i1 = geq_min(a, k);
  i2 = expsearch([&](ll i)
                 { return k <= vecget(a, i); }, 0, true)
           .first;
  assert(i1 == i2);

  i1 = lt_cnt(a, k);
  i2 = count_if(ALL(a), [&](ll ai)
                { return ai < k; });
  assert(i1 == i2);
  i1 = leq_cnt(a, k);
  i2 = count_if(ALL(a), [&](ll ai)
                { return ai <= k; });
  assert(i1 == i2);
  i1 = gt_cnt(a, k);
  i2 = count_if(ALL(a), [&](ll ai)
                { return k < ai; });
  assert(i1 == i2);
  i1 = geq_cnt(a, k);
  i2 = count_if(ALL(a), [&](ll ai)
                { return k <= ai; });
  assert(i1 == i2);
}

int main()
{
  rep(_, 10000) test1();
  rep(_, 10000) test2();

  cout << "Hello World" << endl;
}