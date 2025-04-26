#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/sparse_table.hpp"
#include "ds/disjoint_sparse_table.hpp"
#include "ds/dc_range_prod.hpp"
#include "ds/block_sparse_table.hpp"
#include "ds/linear_rmq.hpp"

ll naive(vl A, ll l, ll r)
{
  ll ans = INF;
  rep(i, l, r) chmin(ans, A.at(i));
  return ans;
}

ll solve_sparse_table(vl A, ll l, ll r)
{
  SparseTable<MonoidMin<ll>> spt(A);
  return spt.prod(l, r);
}

ll solve_block_sparse_table(vl A, ll l, ll r)
{
  BlockSparseTable<MonoidMin<ll>> spt(A);
  return spt.prod(l, r);
}

ll solve_dc_range_prod(vl A, ll l, ll r)
{
  return dc_range_prod<MonoidMin<ll>>(A, vpll{{l, r}}).at(0);
}

ll solve_disjoint_sparse_table(vl A, ll l, ll r)
{
  DisjointSparseTable<MonoidMin<ll>> spt(A);
  return spt.prod(l, r);
}

ll solve_block_disjoint_sparse_table(vl A, ll l, ll r)
{
  BlockDisjointSparseTable<MonoidMin<ll>> spt(A);
  return spt.prod(l, r);
}

ll solve_linear_rmq(vl A, ll l, ll r)
{
  LinearRMQ<MonoidMin<ll>, uint8_t> spt(A);
  return spt.prod(l, r);
}

int main()
{
  rep(t, 10000)
  {
    ll N = randint(0, 20);
    vl A(N);
    rep(i, N) A.at(i) = randint(-20, 20);
    ll l = randint(0, N), r = randint(0, N);
    if (l > r)
      swap(l, r);
    dump(A | cp::index(), l, r);
    ll god = naive(A, l, r);
    dump(god);
    ll ans1 = solve_sparse_table(A, l, r);
    dump(ans1);
    ll ans2 = solve_block_sparse_table(A, l, r);
    dump(ans2);
    ll ans3 = solve_dc_range_prod(A, l, r);
    dump(ans3);
    ll ans4 = solve_disjoint_sparse_table(A, l, r);
    dump(ans4);
    ll ans5 = solve_block_disjoint_sparse_table(A, l, r);
    dump(ans5);
    ll ans6 = solve_linear_rmq(A, l, r);
    dump(ans6);
    assert(god == ans1);
    assert(god == ans2);
    assert(god == ans3);
    assert(god == ans4);
    assert(god == ans5);
    assert(god == ans6);
  }

  PRINT("Hello World");
}