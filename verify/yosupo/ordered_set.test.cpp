#define PROBLEM "https://judge.yosupo.jp/problem/ordered_set"

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
#include "ds/pbds_set.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  pbds_set<ll> st(ALL(A));
  rep(_, Q)
  {
    LL(t, x);
    if (t == 0)
      st.insert(x);
    else if (t == 1)
      st.erase(x);
    else if (t == 2)
    {
      x--;
      auto it = st.find_by_order(x);
      if (it == st.end())
        PRINT(-1);
      else
        PRINT(*it);
    }
    else if (t == 3)
      PRINT(st.leq_cnt(x));
    else if (t == 4)
    {
      auto it = st.leq_max_it(x);
      if (it == st.end())
        PRINT(-1);
      else
        PRINT(*it);
    }
    else if (t == 5)
    {
      auto it = st.geq_min_it(x);
      if (it == st.end())
        PRINT(-1);
      else
        PRINT(*it);
    }
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
