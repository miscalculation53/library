#define PROBLEM "https://yukicoder.me/problems/no/3756"
#define SINGLE_TESTCASE
#define FAST_IO

#include "ds/coordinate_compression.hpp"
#include "ds/meldable_integer_set.hpp"
#include "graph/tree/merge_tree.hpp"
#include "graph/tree/rooted_tree.hpp"

void init() {}
void main2()
{
  LL(N, M, Q);
  VEC(ll, N, A_);
  auto A = compressed<ll, int>(A_);
  int colors = *max_element(ALL(A)) + 1;
  VEC(tlll, M, UVW);
  offset(UVW, tlll{-1, -1, 0});
  VEC(pll, Q, SC);
  offset(SC, pll{-1, 0});
  sort(ALL(UVW), [](const auto &a, const auto &b) { return get<2>(a) < get<2>(b); });

  MergeTree merges(N);
  vl weight(N, 0);
  for (auto [u, v, w] : UVW)
  {
    int z = merges.merge(u, v);
    if (z != -1) weight.eb(w);
  }
  auto par = merges.parents();
  RootedTree tree(par.size(), par);
  MeldableIntegerSetPool pool(colors, N);
  auto sets = GEN_VEC(tree.size(), v, v < N ? pool.make_set(A[v]) : pool.make_set());
  vc<int> count(tree.size(), 1);
  repi(v, N, tree.size())
  {
    for (int child : tree.children(v)) sets[v].merge(sets[child]);
    count[v] = sets[v].size();
  }

  vl ans(Q);
  repi(q, Q)
  {
    auto [s, c] = SC[q];
    int v = tree.first_ancestor(s, [&](int x) { return count[x] >= c; }).second;
    ans[q] = v == -1 ? -1 : weight[v];
  }
  PRINTV(ans);
}
void test() {}
#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
