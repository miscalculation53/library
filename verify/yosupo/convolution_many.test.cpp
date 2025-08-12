#define PROBLEM "https://judge.yosupo.jp/problem/product_of_polynomial_sequence"

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
#include "math/fps/fps.hpp"
#include "math/convolution/convolution_many.hpp"
using mint = modint998244353;
using fps = FormalPowerSeries<mint>;

void init() {}

void main2()
{
  LL(N);
  vvc<mint> fs(N);
  rep(i, N)
  {
    LL(d);
    fs.at(i).resize(d + 1);
    READ(fs.at(i));
  }
  PRINT(convolution_many(fs));

  vc<fps> fs_(N);
  rep(i, N) fs_.at(i) = fs.at(i);
  assert(convolution_many(fs_) == convolution_many(fs));
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
