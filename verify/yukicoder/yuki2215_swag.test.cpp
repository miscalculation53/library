#define PROBLEM "https://yukicoder.me/problems/no/2215"

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

#include "math/convolution/convolution.hpp"
using mint = modint998244353;

ll K;

struct Mn
{
  using S = vc<mint>;
  static S op(const S &f, const S &g)
  {
    auto h = convolution(f, g);
    vc<mint> res(K);
    rep(i, SZ(h)) res.at(i % K) += h.at(i);
    return res;
  }
  static S e()
  {
    vc<mint> res(K);
    res.at(0) = 1;
    return res;
  }
};

#include "ds/swag.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, M, K_);
  K = K_;
  VEC(ll, N, A);

  vvc<mint> fs(N);
  rep(i, N)
  {
    vc<mint> f(K);
    f.at(0) += 1;
    f.at(A.at(i)) += 1;
    fs.at(i) = f;
  }

  FoldableQueue<Mn> que;
  rep(i, M)
  {
    que.push(fs.at(i));
  }
  rep(i, M, N + 1)
  {
    auto [f, g] = que.all_prod_left_right();
    mint ans = 0;
    rep(i, K) ans += f.at(i) * g.at((K - i) % K);
    PRINT(ans - 1);
    if (i == N)
      break;
    que.push(fs.at(i));
    que.pop();
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
