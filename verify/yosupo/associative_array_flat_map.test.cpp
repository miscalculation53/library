#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"

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

#include "ds/flat_map.hpp"

void init() {}

void main2()
{
  LL(Q);
  vc<tlll> queries(Q);
  vc<ll> keys;
  rep(i, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(k, v);
      queries.at(i) = {t, k, v};
      keys.eb(k);
    }
    else if (t == 1)
    {
      LL(k);
      queries.at(i) = {t, k, -1};
      keys.eb(k);
    }
  }
  FlatMap<ll, ll> mp(keys);
  fec([ key, val ] : mp) key + val;
  fec([t, k, v] : queries)
  {
    if (t == 0)
    {
      mp[k] = v;
    }
    else if (t == 1)
    {
      PRINT(mp[k]);
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
