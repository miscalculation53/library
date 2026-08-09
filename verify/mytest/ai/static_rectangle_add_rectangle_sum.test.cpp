#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/modint/modint.hpp"
#include "math/algebra/algebra_basic_ops.hpp"
#include "math/algebra/bit.hpp"
#include "ds/2d/offline/static_rectangle_add_rectangle_sum.hpp"

using mint = modint998244353;

// Test focus: generalized add/sum operations work for additive, XOR, and
// multiplicative groups, including lower-left and full-rectangle query APIs.
int main()
{
  struct R
  {
    ll lx, rx, ly, ry, w;
  };
  vc<R> adds;
  StaticRectangleAddRectangleSum<GroupAddSub<ll>, ll> rs;
  mt19937_64 rng(3141592653);

  rep(_, 40)
  {
    ll lx = ll(rng() % 11) - 5, ly = ll(rng() % 11) - 5;
    ll w = ll(rng() % 11) - 5;
    rs.upper_right_add(lx, ly, w);
    adds.push_back({lx, 20, ly, 20, w});
  }
  rep(_, 40)
  {
    ll lx = ll(rng() % 11) - 5, rx = ll(rng() % 11) - 5;
    ll ly = ll(rng() % 11) - 5, ry = ll(rng() % 11) - 5;
    if (lx > rx) swap(lx, rx);
    if (ly > ry) swap(ly, ry);
    ll w = ll(rng() % 11) - 5;
    rs.rectangle_add(lx, rx, ly, ry, w);
    adds.push_back({lx, rx, ly, ry, w});
  }

  vc<ll> expected;
  rep(_, 200)
  {
    if (rng() & 1)
    {
      ll rx = ll(rng() % 13) - 6, ry = ll(rng() % 13) - 6;
      rs.lower_left_sum(rx, ry);
      ll ans = 0;
      fec(a : adds)
        ans += max(0LL, min(a.rx, rx) - a.lx) *
               max(0LL, min(a.ry, ry) - a.ly) * a.w;
      expected.push_back(ans);
    }
    else
    {
      ll lx = ll(rng() % 13) - 6, rx = ll(rng() % 13) - 6;
      ll ly = ll(rng() % 13) - 6, ry = ll(rng() % 13) - 6;
      if (lx > rx) swap(lx, rx);
      if (ly > ry) swap(ly, ry);
      rs.rectangle_sum(lx, rx, ly, ry);
      ll ans = 0;
      fec(a : adds)
        ans += max(0LL, min(a.rx, rx) - max(a.lx, lx)) *
               max(0LL, min(a.ry, ry) - max(a.ly, ly)) * a.w;
      expected.push_back(ans);
    }
  }
  assert(rs.run() == expected);

  {
    StaticRectangleAddRectangleSum<GroupXor<unsigned>, int> xr;
    struct XorRect
    {
      int lx, rx, ly, ry;
      unsigned w;
    };
    vc<XorRect> rects;
    rep(_, 60)
    {
      int lx = int(rng() % 11) - 5, rx = int(rng() % 11) - 5;
      int ly = int(rng() % 11) - 5, ry = int(rng() % 11) - 5;
      if (lx > rx) swap(lx, rx);
      if (ly > ry) swap(ly, ry);
      unsigned w = rng();
      xr.rectangle_add(lx, rx, ly, ry, w);
      rects.push_back({lx, rx, ly, ry, w});
    }
    vc<unsigned> xor_expected;
    rep(_, 100)
    {
      int lx = int(rng() % 13) - 6, rx = int(rng() % 13) - 6;
      int ly = int(rng() % 13) - 6, ry = int(rng() % 13) - 6;
      if (lx > rx) swap(lx, rx);
      if (ly > ry) swap(ly, ry);
      xr.rectangle_sum(lx, rx, ly, ry);
      unsigned ans = 0;
      fec(r : rects)
      {
        int width = max(0, min(r.rx, rx) - max(r.lx, lx));
        int height = max(0, min(r.ry, ry) - max(r.ly, ly));
        if ((width * height) & 1)
          ans ^= r.w;
      }
      xor_expected.push_back(ans);
    }
    assert(xr.run() == xor_expected);
  }

  {
    StaticRectangleAddRectangleSum<GroupMulDiv<mint>, int> mul;
    struct MulRect
    {
      int lx, rx, ly, ry;
      mint w;
    };
    vc<MulRect> rects;
    rep(_, 30)
    {
      int lx = int(rng() % 9) - 4, rx = int(rng() % 9) - 4;
      int ly = int(rng() % 9) - 4, ry = int(rng() % 9) - 4;
      if (lx > rx) swap(lx, rx);
      if (ly > ry) swap(ly, ry);
      mint w = rng() % 100 + 1;
      mul.rectangle_add(lx, rx, ly, ry, w);
      rects.push_back({lx, rx, ly, ry, w});
    }
    vc<mint> mul_expected;
    rep(_, 50)
    {
      int lx = int(rng() % 11) - 5, rx = int(rng() % 11) - 5;
      int ly = int(rng() % 11) - 5, ry = int(rng() % 11) - 5;
      if (lx > rx) swap(lx, rx);
      if (ly > ry) swap(ly, ry);
      mul.rectangle_sum(lx, rx, ly, ry);
      mint ans = 1;
      fec(r : rects)
      {
        int width = max(0, min(r.rx, rx) - max(r.lx, lx));
        int height = max(0, min(r.ry, ry) - max(r.ly, ly));
        ans *= r.w.pow(width * height);
      }
      mul_expected.push_back(ans);
    }
    assert(mul.run() == mul_expected);
  }
  cout << "Hello World" << endl;
}
