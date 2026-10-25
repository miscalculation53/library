#define PROBLEM "https://atcoder.jp/contests/abc196/tasks/abc196_e"

#include "algebra/clamp_add.hpp"

int main()
{
  INT(N);
  using M = MonoidClampAdd<ll>;
  using S = M::S;
  auto f = M::e();
  repi(i, N)
  {
    LL(a);
    INT(t);
    auto g = t == 1 ? S::add(a) : t == 2 ? S::chmax(a) : S::chmin(a);
    f = M::op(g, f);
  }
  INT(Q);
  repi(i, Q)
  {
    LL(x);
    PRINT(f(x));
  }
}
