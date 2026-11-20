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
    rep(r, k, 13)
      assert(st.count(k, r) == std::count_if(ALL(a), [&](ll x) { return k <= x && x < r; }));
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
      ll x = 0 <= i && i < n ? a[i] : -1;
      ll y = it ? **it : -1;
      assert(x == y);
    }
    {
      auto i = leq_max(a, k);
      auto it = st.leq_max_it(k);
      ll x = 0 <= i && i < n ? a[i] : -1;
      ll y = it ? **it : -1;
      assert(x == y);
    }
    {
      auto i = gt_min(a, k);
      auto it = st.gt_min_it(k);
      ll x = 0 <= i && i < n ? a[i] : -1;
      ll y = it ? **it : -1;
      assert(x == y);
    }
    {
      auto i = geq_min(a, k);
      auto it = st.geq_min_it(k);
      ll x = 0 <= i && i < n ? a[i] : -1;
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
    rep(r, k, 13)
      assert(ms.count(k, r) == std::count_if(ALL(a), [&](ll x) { return k <= x && x < r; }));
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
      ll x = 0 <= i && i < n ? a[i] : -1;
      ll y = it ? (*it)->first : -1;
      assert(x == y);
    }
    {
      auto i = leq_max(a, k);
      auto it = ms.leq_max_it(k);
      ll x = 0 <= i && i < n ? a[i] : -1;
      ll y = it ? (*it)->first : -1;
      assert(x == y);
    }
    {
      auto i = gt_min(a, k);
      auto it = ms.gt_min_it(k);
      ll x = 0 <= i && i < n ? a[i] : -1;
      ll y = it ? (*it)->first : -1;
      assert(x == y);
    }
    {
      auto i = geq_min(a, k);
      auto it = ms.geq_min_it(k);
      ll x = 0 <= i && i < n ? a[i] : -1;
      ll y = it ? (*it)->first : -1;
      assert(x == y);
    }
  }
}

void test_count_boundaries()
{
  const i128 large = i128(1) << 100;
  const pbds_set<i128, greater<i128>> st{large, 0, -large, -large};
  const pbds_multiset<i128, greater<i128>> ms{large, 0, -large, -large};
  static_assert(is_same_v<decltype(st.count(large, 0)), ll>);
  static_assert(is_same_v<decltype(ms.count<u128>(0, -large)), u128>);
  assert(st.count(large, -large) == 2);
  assert(ms.count(0, -large) == 1);
  assert(ms.count<u128>(-large, -large - 1) == 2);
  assert(ms.count(-large, -large) == 0);
  assert(pbds_set<int>{}.count(-1, 1) == 0);
  assert(pbds_multiset<int>{}.count(-1, 1) == 0);
}

int main()
{
  rep(_, 100) test1();
  rep(_, 100) test2();
  test_count_boundaries();

  cout << "Hello World" << endl;
}
