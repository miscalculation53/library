#define PROBLEM "https://yukicoder.me/problems/no/3756"
#define SINGLE_TESTCASE
#define FAST_IO

#include "algo/parallel_binsearch.hpp"
#include "ds/coordinate_compression.hpp"
#include "ds/meldable_integer_set.hpp"

void init() {}
void main2()
{
  LL(N, M, Q);
  VEC(ll, N, A_);
  auto A = compressed<ll, int>(A_);
  int colors = *max_element(ALL(A)) + 1;
  VEC(tlll, M, UVW);
  offset(UVW, tlll{-1, -1, 0});
  UNZIP(UVW, U, V, W);
  CoordinateCompression cc(W);
  int K = cc.size();
  vvc<pll> uvs(K + 1);
  fec([u, v, w] : UVW) uvs[cc.get_id(w)].eb(u, v);
  VEC(pll, Q, SC);
  offset(SC, pll{-1, 0});

  MeldableIntegerSetPool sets(colors, N);
  vc<int> par(N);
  vc<MeldableIntegerSet> components;
  components.reserve(N);
  vvl qs(K + 1);
  auto leader = [&](auto &&self, int v) -> int
  {
    return par[v] < 0 ? v : par[v] = self(self, par[v]);
  };
  auto judge = [&](const vl &D)
  {
    sets.clear();
    fill(ALL(par), -1);
    components.clear();
    repi(v, N) components.eb(sets.make_set(A[v]));
    for (auto &bucket : qs) bucket.clear();
    vb res(Q);
    rep(q, Q)
    {
      if (D[q] == K + 1) res[q] = true;
      else if (D[q] == -1) res[q] = false;
      else qs[D[q]].eb(q);
    }
    rep(j, K + 1)
    {
      for (int q : qs[j])
      {
        auto [s, c] = SC[q];
        res[q] = components[leader(leader, s)].size() >= c;
      }
      for (auto [u, v] : uvs[j])
      {
        int x = leader(leader, u), y = leader(leader, v);
        if (x == y) continue;
        if (-par[x] < -par[y]) swap(x, y);
        par[x] += par[y], par[y] = x;
        components[x].merge(components[y]);
      }
    }
    return res;
  };
  auto ans = parallel_binsearch(Q, judge, K + 1, -1, false, false).first;
  for (auto &x : ans)
  {
    if (x == K + 1) x = -1;
    else if (x > 0) x = cc.get_val(x - 1);
  }
  PRINTV(ans);
}
void test() {}
#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
