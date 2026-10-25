#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite_large_array"
#define SINGLE_TESTCASE
#define FAST_IO

#include "ds/segtree/meldable_sparse_segtree.hpp"
#include "math/modint/modint.hpp"
#include "algebra/affine_function.hpp"
using mint = modint998244353;

void init() {}
void main2()
{
  LL(N, Q);
  MeldableSparseSegmentTreePool<OppositeMonoid<GroupAffineFunction<mint>>> pool(N);
  auto seg = pool.make_tree();
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(p, c, d);
      seg.set(p, {c, d});
    }
    else
    {
      LL(l, r, x);
      auto [a, b] = seg.prod(l, r);
      PRINT(a * x + b);
    }
  }
}
void test() {}
#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
