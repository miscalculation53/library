#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/pbds_set.hpp"

void test1()
{
  ll n = randint(1, 10);
  vl a(n);
  rep(i, n) a[i] = randint(1, 10);
  sortunique(a);
  n = a.size();
  pbds_set<ll> st(ALL(a));
  rep(i, n)
  {
    assert(a[i] == *st.find_by_order(i));
  }
  rep(k, -1, 12)
  {
    assert(lt_cnt(a, k) == st.lt_cnt(k));
    assert(leq_cnt(a, k) == st.leq_cnt(k));
    assert(gt_cnt(a, k) == st.gt_cnt(k));
    assert(geq_cnt(a, k) == st.geq_cnt(k));
    assert(lt_max(a, k) == st.lt_max_id(k));
    assert(leq_max(a, k) == st.leq_max_id(k));
    assert(gt_min(a, k) == st.gt_min_id(k));
    assert(geq_min(a, k) == st.geq_min_id(k));
    {
      auto i = lt_max(a, k);
      auto it = st.lt_max_it(k);
      ll x = i ? a[*i] : -1;
      ll y = it ? **it : -1;
      assert(x == y);
    }
    {
      auto i = leq_max(a, k);
      auto it = st.leq_max_it(k);
      ll x = i ? a[*i] : -1;
      ll y = it ? **it : -1;
      assert(x == y);
    }
    {
      auto i = gt_min(a, k);
      auto it = st.gt_min_it(k);
      ll x = i ? a[*i] : -1;
      ll y = it ? **it : -1;
      assert(x == y);
    }
    {
      auto i = geq_min(a, k);
      auto it = st.geq_min_it(k);
      ll x = i ? a[*i] : -1;
      ll y = it ? **it : -1;
      assert(x == y);
    }
  }
}

void test2()
{
  ll n = randint(1, 10);
  vl a(n);
  rep(i, n) a[i] = randint(1, 10);
  sort(ALL(a));
  pbds_multiset<ll> ms(ALL(a));
  rep(i, n)
  {
    assert(a[i] == ms.find_by_order(i)->first);
  }
  rep(k, -1, 12)
  {
    assert(lt_cnt(a, k) == ms.lt_cnt(k));
    assert(leq_cnt(a, k) == ms.leq_cnt(k));
    assert(gt_cnt(a, k) == ms.gt_cnt(k));
    assert(geq_cnt(a, k) == ms.geq_cnt(k));
    assert(lt_max(a, k) == ms.lt_max_id(k));
    assert(leq_max(a, k) == ms.leq_max_id(k));
    assert(gt_min(a, k) == ms.gt_min_id(k));
    assert(geq_min(a, k) == ms.geq_min_id(k));
    {
      auto i = lt_max(a, k);
      auto it = ms.lt_max_it(k);
      ll x = i ? a[*i] : -1;
      ll y = it ? (*it)->first : -1;
      assert(x == y);
    }
    {
      auto i = leq_max(a, k);
      auto it = ms.leq_max_it(k);
      ll x = i ? a[*i] : -1;
      ll y = it ? (*it)->first : -1;
      assert(x == y);
    }
    {
      auto i = gt_min(a, k);
      auto it = ms.gt_min_it(k);
      ll x = i ? a[*i] : -1;
      ll y = it ? (*it)->first : -1;
      assert(x == y);
    }
    {
      auto i = geq_min(a, k);
      auto it = ms.geq_min_it(k);
      ll x = i ? a[*i] : -1;
      ll y = it ? (*it)->first : -1;
      assert(x == y);
    }
  }
}

int main()
{
  rep(_, 100) test1();
  rep(_, 100) test2();

  cout << "Hello World" << endl;
}