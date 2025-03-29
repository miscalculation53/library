#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  STR(T);
  set<ll> st;
  rep(i, N) if (T.at(i) == '1') st.insert(i);
  rep(_, Q)
  {
    LL(c, k);
    if (c == 0)
      st.insert(k);
    else if (c == 1)
      st.erase(k);
    else if (c == 2)
      PRINT(st.find(k) != st.end());
    else if (c == 3)
    {
      auto it = geq_min(st, k);
      if (it == st.end())
        PRINT(-1);
      else
        PRINT(*it);
    }
    else if (c == 4)
    {
      auto it = leq_max(st, k);
      if (it == st.end())
        PRINT(-1);
      else
        PRINT(*it);
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
