#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/static_range/linear_rmq.hpp"

struct AffineMonoid
{
  using S = pair<ll, ll>;
  static constexpr ll mod = 998244353;
  static S e() { return {1, 0}; }
  static S op(S f, S g)
  {
    return {f.first * g.first % mod, (f.second * g.first + g.second) % mod};
  }
};

template <class Word, int B>
void check(const vl &a)
{
  const int n = a.size();
  LinearRMQ<MonoidMin<ll>, Word> rmq(a);
  LinearRMQ<MonoidMax<ll>, Word> max_rmq(a);
  BlockSparseTable<MonoidMin<ll>, B> block(a);
  vc<AffineMonoid::S> fs;
  for (ll x : a) fs.eb((x + 100) % 17, (x + 100) % 23);
  DisjointSparseTable<AffineMonoid> dst(fs);
  BlockDisjointSparseTable<AffineMonoid, B> bdst(fs);
  assert(rmq.content() == a && block.content() == a);
  assert(dst.content() == fs && bdst.content() == fs);
  repi(i, n) assert(rmq.get(i) == a[i] && dst.get(i) == fs[i]);
  repi(l, n + 1)
  {
    ll mn = MonoidMin<ll>::e(), mx = MonoidMax<ll>::e();
    auto f = AffineMonoid::e();
    repi(r, l, n + 1)
    {
      assert(rmq.prod(l, r) == mn && block.prod(l, r) == mn);
      assert(max_rmq.prod(l, r) == mx);
      assert(dst.prod(l, r) == f && bdst.prod(l, r) == f);
      auto [left, right] = dst.prod_left_right(l, r);
      assert(AffineMonoid::op(left, right) == f);
      if (r < n)
      {
        chmin(mn, a[r]), chmax(mx, a[r]);
        f = AffineMonoid::op(f, fs[r]);
      }
    }
  }
}

int main()
{
  mt19937 rng(20261022);
  for (int n : {0, 1, 2, 3, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128, 129, 257})
    for (int mode = 0; mode < 4; mode++)
    {
      vl a(n);
      repi(i, n) a[i] = mode == 0 ? int(rng() % 101) - 50 : mode == 1 ? i % 7 :
                        mode == 2 ? -(i % 7) : 0;
      check<uint8_t, 1>(a);
      check<uint16_t, 3>(a);
      check<uint32_t, 16>(a);
      check<uint64_t, 64>(a);
    }
  cout << "Hello World" << endl;
}
