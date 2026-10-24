#define PROBLEM "https://atcoder.jp/contests/aalc001/tasks/aalc001_q"

#include "algebra/algebra_base.hpp"
#include "algebra/clamp_add_cost.hpp"
#include "ds/segtree/segtree.hpp"

int main()
{
  INT(N);
  LL(M);
  INT(Q);
  STR(S);
  VEC(ll, N, A);
  using Mon = MonoidClampAddCost<ll, -1, -1>;
  auto leaf = [&](int i)
  {
    return Mon::S::shift_then_clamp(S[i] == '1' ? A[i] : -A[i], 0, M, A[i]);
  };
  vc<Mon::S> a;
  repi(i, N) a.push_back(leaf(i));
  SegmentTree<OppositeMonoid<Mon>> seg(a);
  repi(q, Q)
  {
    INT(t);
    if (t == 1)
    {
      INT(i);
      --i;
      S[i] ^= 1;
      seg.set(i, leaf(i));
    }
    else
    {
      LL(x);
      INT(l, r);
      PRINT(seg.prod(l - 1, r)(x).second);
    }
  }
}
