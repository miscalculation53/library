#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"
#define SINGLE_TESTCASE

#ifndef LOCAL
#define FAST_IO
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"
#include "ds/hash_map.hpp"

void init() {}

void main2()
{
  LL(q);
  HashMap<ull, ll> values;
  rep(_, q)
  {
    int type;
    ull key;
    READ(type, key);
    if (type == 0)
    {
      LL(value);
      values[key] = value;
    }
    else
    {
      const ll *value = values.find_ptr(key);
      PRINT(value ? *value : 0LL);
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
