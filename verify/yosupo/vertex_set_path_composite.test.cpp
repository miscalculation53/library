#define PROBLEM "https://judge.yosupo.jp/problem/vertex_set_path_composite"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "math/modint/modint.hpp"
using mint = modint998244353;

#include "graph/tree/rooted_tree.hpp"
#include "ds/segtree/segtree.hpp"
#include "algebra/affine_function.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(pll, N, AB);
  VEC(pll, N - 1, UV);
  RootedTree G(N, UV, randrange(0, N));
  using M = NormalAndOppositeGroup<OppositeGroup<GroupAffineFunction<mint>>>;
  SegmentTree<M> seg(G.reordered_vertex_info(AB));
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(p, c, d);
      seg.set(G.preorder(p), pair{c, d});
    }
    else if (t == 1)
    {
      LL(u, v, x);
      auto res = M::e();
      G.path_query(u, v, [&](ll l, ll r, bool isrev)
                   {
      auto tmp = seg.prod(l, r);
      res = M::op(res, isrev ? tmp.rev() : tmp);});
      auto [c, d] = res.normal;
      PRINT(c * x + d);
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
