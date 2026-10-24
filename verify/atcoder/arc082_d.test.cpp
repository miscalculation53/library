#define PROBLEM "https://atcoder.jp/contests/arc082/tasks/arc082_d"

#include "algebra/clamp_add.hpp"

int main()
{
  LL(X);
  INT(K);
  VEC(ll, K, R);
  using M = MonoidClampAdd<ll>;
  using S = M::S;
  // f(a) は、初期にパーツ A に a 入っていたときの、現在のパーツ A の砂量。
  auto f = M::e();
  // パーツ A が下にある間は increasing = true。
  auto advance = [&](ll dt, bool increasing)
  {
    return S::shift_then_clamp(increasing ? dt : -dt, 0, X);
  };
  int k = 0;
  ll last = 0;
  INT(Q);
  repi(i, Q)
  {
    LL(t, a);
    while (k < K && R[k] <= t)
    {
      f = M::op(advance(R[k] - last, k % 2), f);
      last = R[k++];
    }
    PRINT(M::op(advance(t - last, k % 2), f)(a));
  }
}
