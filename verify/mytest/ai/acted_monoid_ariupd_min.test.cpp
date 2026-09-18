#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/acted_monoid/ariupd_min.hpp"
#include "ds/segtree/lazy_segtree.hpp"

template <class S>
void assert_same(const S &a, const S &b)
{
  assert(a.l == b.l && a.r == b.r && a.mn == b.mn);
}

// 恒等操作・0 代入・正負の公差について、作用則と空区間の保持を確認する。
void test_laws()
{
  using AM = ActedMonoidAriUpdMin<ll>;
  using S = AM::S;
  using F = AM::F;
  const S a(2, 7), b(5, -3), c(9, 4), e = AM::e();
  const S x = AM::op(a, b);
  const vc<F> actions{AM::id(), F({0, 0}), F({4, 2}), F({3, -7})};
  assert_same(AM::op(AM::op(a, b), c), AM::op(a, AM::op(b, c)));
  assert_same(AM::op(e, x), x);
  assert_same(AM::op(x, e), x);
  for (const F &f : actions)
  {
    assert_same(AM::mapping(f, e), e);
    assert_same(AM::mapping(f, x), AM::op(AM::mapping(f, a), AM::mapping(f, b)));
    for (const F &g : actions)
    {
      assert_same(AM::mapping(AM::composition(f, g), x), AM::mapping(f, AM::mapping(g, x)));
      for (const F &h : actions)
        assert_same(AM::mapping(AM::composition(AM::composition(f, g), h), x),
                    AM::mapping(AM::composition(f, AM::composition(g, h)), x));
    }
  }
  assert_same(AM::mapping(AM::id(), x), x);
}

// 遅延した代入を点操作や部分区間への代入と混ぜ、配列への逐次代入と照合する。
template <class T>
void test_tree()
{
  using AM = ActedMonoidAriUpdMin<T>;
  using S = typename AM::S;
  using F = typename AM::F;
  mt19937 rng(117210);
  for (int n : {0, 1, 2, 7, 31, 65})
  {
    vc<T> a(n);
    vc<S> init;
    repi(i, n)
    {
      a[i] = T(int(rng() % 201) - 100);
      init.emplace_back(i, a[i]);
    }
    LazySegmentTree<AM> seg(init);
    auto check = [&](int l, int r)
    {
      T mn = resolved_infty<T>();
      repi(i, l, r) mn = min(mn, a[i]);
      const S got = seg.prod(l, r);
      assert(got.mn == mn);
      if (l == r) assert_same(got, AM::e());
      else assert(got.l == l && got.r == r);
    };
    repi(iter, 1500)
    {
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      if (iter % 11 == 0) l = 0, r = n;
      T c = T(int(rng() % 101) - 50), d = T(int(rng() % 21) - 10);
      if (iter % 13 == 0) c = d = T{};
      if (n && iter % 7 == 0)
      {
        const int p = rng() % n;
        a[p] = c;
        seg.set(p, S(p, c));
      }
      else if (iter % 9 == 0)
        seg.apply(l, r, AM::id());
      else
      {
        if (n && iter % 5 == 0)
        {
          l = rng() % n, r = l + 1;
          seg.apply(l, F({c, d}));
        }
        else
          seg.apply(l, r, F({c, d}));
        repi(i, l, r) a[i] = c + d * T(i);
      }
      check(l, r);
      l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      check(l, r);
      assert_same(seg.all_prod(), seg.prod(0, n));
    }
    const auto result = seg.content();
    repi(i, n) assert_same(result[i], S(i, a[i]));
    repi(l, n + 1) repi(r, l, n + 1) check(l, r);
  }
}

ll custom_infty() { return numeric_limits<ll>::max(); }

void test_infty_and_wide_intermediates()
{
  using AM = ActedMonoidAriUpdMin<ll>;
  const AM::S x(3, 0);
  assert(AM::mapping(AM::F({-8'000'000'000'000'000'000LL, 4'000'000'000'000'000'000LL}), x).mn == 4'000'000'000'000'000'000LL);
  assert(AM::mapping(AM::F({8'000'000'000'000'000'000LL, -4'000'000'000'000'000'000LL}), x).mn == -4'000'000'000'000'000'000LL);
  using Small = ActedMonoidAriUpdMin<int>;
  assert(Small::mapping(Small::F({-2'000'000'000, 1'000'000'000}), Small::S(3, 0)).mn == 1'000'000'000);

  using Custom = ActedMonoidAriUpdMin<ll, custom_infty>;
  assert(Custom::e().mn == custom_infty());
  const auto s = Custom::mapping(Custom::F({INF + 1, 0}), Custom::S(2, 3));
  assert(s.mn == INF + 1);
  assert_same(Custom::op(Custom::e(), s), s);
  assert(Custom::mapping(Custom::F({custom_infty(), 0}), s).mn == custom_infty());
  using Explicit = ActedMonoidAriUpdMin<ll, 1'000'000LL>;
  assert(Explicit::e().mn == 1'000'000);
  assert(Explicit::mapping(Explicit::F({1'000'000, 0}), Explicit::S(0, 0)).mn == 1'000'000);
}

int main()
{
  test_laws();
  test_tree<ll>();
  test_tree<int>();
  test_tree<i128>();
  test_tree<double>();
  test_infty_and_wide_intermediates();
  cout << "Hello World" << endl;
}
