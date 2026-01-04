#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum_with_upper_bound"

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

void init() {}

#define sum prod
#include "ds/cumulative_sum/cumulative_sum.hpp"
#include "ds/2d/sparse/segtree.hpp"

struct G
{
  using S = pair<int, ll>;
  static constexpr S op(const S &a, const S &b)
  { return {a.first + b.first, a.second + b.second}; }
  static constexpr S e() { return {0, 0}; }
  static constexpr S inv(const S &a) { return {-a.first, -a.second}; }
};

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);

  vc<tuple<ll, ll, typename G::S>> XYW(N);
  rep(i, N)
  {
    XYW.at(i) = {i, A.at(i), {1, A.at(i)}};
  }

  SegmentTree2DSparse<CumulativeSum<G>, G, ll> seg(XYW);
  rep(_, Q)
  {
    LL(l, r, x);
    auto [c, s] = seg.prod(l, r, 0, x + 1);
    PRINT(c, s);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
