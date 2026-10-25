#define PROBLEM "https://yukicoder.me/problems/no/3756"
#define SINGLE_TESTCASE
#define FAST_IO

#include "ds/coordinate_compression.hpp"
#include "ds/meldable_heap.hpp"
#include "ds/meldable_integer_set.hpp"
#include "ds/uf/uf.hpp"

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
  MeldableIntegerSetPool sets(colors, N);
  MeldableHeapPool<pll> pending(Q);
  vc<MeldableIntegerSet> components;
  vc<MeldableHeap<pll>> heaps;
  components.reserve(N), heaps.reserve(N);
  repi(v, N)
  {
    components.eb(sets.make_set(A[v]));
    heaps.eb(pending.make_heap());
  }
  vl ans(Q, -1);
  repi(q, Q)
  {
    auto [s, c] = SC[q];
    if (c <= 1) ans[q] = 0;
    else heaps[s].push({c, q});
  }
  UnionFind<UFDataEmpty<>> uf(N);
  for (auto [u, v, w] : UVW)
  {
    int x = uf.leader(u), y = uf.leader(v);
    if (x == y) continue;
    int r = uf.merge(x, y), other = r == x ? y : x;
    components[r].merge(components[other]);
    heaps[r].merge(heaps[other]);
    while (!heaps[r].empty() && heaps[r].top().first <= components[r].size())
    {
      ans[heaps[r].top().second] = w;
      heaps[r].pop();
    }
  }
  PRINTV(ans);
}
void test() {}
#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
