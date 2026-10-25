#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"
#define SINGLE_TESTCASE
#define FAST_IO

#include "ds/segtree/meldable_sparse_segtree.hpp"

void init() {}
void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  MeldableSparseSegmentTreePool<MonoidAdd<ll>> pool(N);
  auto seg = pool.make_tree();
  rep(p, N) seg.set(p, A[p]);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(p, x);
      seg.modify(p, [&](ll &value) { value += x; });
    }
    else
    {
      LL(l, r);
      PRINT(seg.prod(l, r));
    }
  }
}
void test() {}
#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
