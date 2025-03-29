#define PROBLEM "https://yukicoder.me/problems/no/2786"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "ds/uf/uf_partially_persistent.hpp"

void init() {}

void main2()
{
  LL(H, W);
  VEC2(ll, H, W, A);

  UnionFindPartiallyPersistent<UFDataEmpty<>> uf(H * W);
  vc<tlll> UVW;
  rep(i, H) rep(j, W)
  {
    ll u = i * W + j;
    if (i != H - 1)
    {
      ll v = (i + 1) * W + j;
      ll w = max(A.at(i).at(j), A.at(i + 1).at(j));
      UVW.eb(u, v, w);
    }
    if (j != W - 1)
    {
      ll v = i * W + (j + 1);
      ll w = max(A.at(i).at(j), A.at(i).at(j + 1));
      UVW.eb(u, v, w);
    }
  }
  sort(ALL(UVW), [&](tlll t1, tlll t2)
       { return get<2>(t1) < get<2>(t2); });
  fec([ u, v, w ] : UVW) uf.merge(u, v, w);

  LL(Q);
  rep(_, Q)
  {
    LL(si, sj, ti, tj);
    si--, sj--, ti--, tj--;
    ll s = si * W + sj;
    ll t = ti * W + tj;
    auto judge = [&](ll time)
    { return uf.same(time, s, t); };
    ll time = expsearch(judge, 0, true).first;
    PRINT(get<2>(UVW.at(time - 1)));
  }
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
