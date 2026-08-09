#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/fenwick_tree/fenwick_tree_01.hpp"

// fenwick tree の二分探索
void test1()
{
  ll n = randint(1, 10);
  vl a(n);
  rep(i, n) a[i] = randint(0, 10);
  sort(ALL(a));
  auto s = cumlsum(a);
  FenwickTree<GroupAddSub<ll>> fw(a);
  rep(k, -1, 100)
  {
    assert(leq_max(s, k) == fw.leq_max(k));
    assert(lt_max(s, k) == fw.lt_max(k));
    assert(gt_min(s, k) == fw.gt_min(k));
    assert(geq_min(s, k) == fw.geq_min(k));
  }
}

// fenwick tree 01 の二分探索
void test2()
{
  ll n = randint(1, 20);
  vl a(n);
  rep(i, n) a[i] = randint(0, 1);
  sortunique(a);
  auto s = cumlsum(a);
  FenwickTree01<ll, uint8_t> fw(a);
  rep(k, -1, 100)
  {
    assert(leq_max(s, k) == fw.leq_max(k));
    assert(lt_max(s, k) == fw.lt_max(k));
    assert(gt_min(s, k) == fw.gt_min(k));
    assert(geq_min(s, k) == fw.geq_min(k));
  }
}

// fenwick tree を使った多重集合の管理
void test3()
{
  ll n = randint(1, 10);
  vl a(n);
  rep(i, n) a[i] = randint(1, 10);
  sort(ALL(a));
  FenwickTree<GroupAddSub<ll>> fw(11);
  fe(ai : a) fw.add(ai, 1);
  rep(i, n)
  {
    assert(a[i] == fw.kth_in_multiset(i));
  }
  rep(k, -1, 15)
  {
    assert(lt_max(a, k) == fw.lt_max_in_multiset(k));
    assert(leq_max(a, k) == fw.leq_max_in_multiset(k));
    assert(gt_min(a, k) == fw.gt_min_in_multiset(k));
    assert(geq_min(a, k) == fw.geq_min_in_multiset(k));
  }
}

// fenwick tree 01 を使った集合の管理
void test4()
{
  ll n = randint(1, 20);
  vl a(n);
  rep(i, n) a[i] = randint(1, 10);
  sortunique(a);
  n = a.size();
  FenwickTree01<ll, uint8_t> fw(11);
  fe(ai : a) fw.set(ai, 1);
  rep(i, n)
  {
    assert(a[i] == fw.kth_in_set(i));
  }
  rep(k, -1, 15)
  {
    assert(lt_max(a, k) == fw.lt_max_in_set(k));
    assert(leq_max(a, k) == fw.leq_max_in_set(k));
    assert(gt_min(a, k) == fw.gt_min_in_set(k));
    assert(geq_min(a, k) == fw.geq_min_in_set(k));
  }
}

int main()
{
  rep(_, 100) test1();
  rep(_, 100) test2();
  rep(_, 100) test3();
  rep(_, 100) test4();

  cout << "Hello World" << endl;
}
