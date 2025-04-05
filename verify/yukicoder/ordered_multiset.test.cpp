#define PROBLEM "https://yukicoder.me/problems/no/649"

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
  LL(Q, K);
  K--;
  pbds_multiset<ll> ms;
  rep(_, Q)
  {
    LL(t);
    if (t == 1)
    {
      LL(v);
      ms.insert(v);
    }
    else if (t == 2)
    {
      auto it = ms.find_by_order(K);
      if (it == ms.end())
        PRINT(-1);
      else
      {
        PRINT(it->first);
        ms.erase(it);
      }
    }
    dump(ms.content());
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
