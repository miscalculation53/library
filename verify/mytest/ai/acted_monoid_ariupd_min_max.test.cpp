#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/acted_monoid/ariupd_min.hpp"
#include "ds/segtree/lazy_segtree.hpp"

template <class AM, class Equal>
void test_laws(const Equal &equal)
{
  using S = typename AM::S;
  using F = typename AM::F;
  const S a(2, 7), b(5, -3), c(9, 4), e = AM::e();
  const S x = AM::op(a, b);
  const vc<F> actions{AM::id(), F({0, 0}), F({4, 2}), F({3, -7})};
  assert(equal(AM::op(AM::op(a, b), c), AM::op(a, AM::op(b, c))));
  assert(equal(AM::op(e, x), x));
  assert(equal(AM::op(x, e), x));
  assert(equal(AM::mapping(AM::id(), x), x));
  for (const F &f : actions)
  {
    assert(equal(AM::mapping(f, e), e));
    assert(equal(AM::mapping(f, x), AM::op(AM::mapping(f, a), AM::mapping(f, b))));
    for (const F &g : actions)
    {
      assert(equal(AM::mapping(AM::composition(f, g), x), AM::mapping(f, AM::mapping(g, x))));
      for (const F &h : actions)
        assert(equal(AM::mapping(AM::composition(AM::composition(f, g), h), x),
                     AM::mapping(AM::composition(f, AM::composition(g, h)), x)));
    }
  }
}

// 正負の公差、0 代入、空区間、点操作を混ぜ、逐次代入した配列と照合する。
template <class T>
void test_tree()
{
  using Max = ActedMonoidAriUpdMax<T>;
  using MinMax = ActedMonoidAriUpdMinMax<T>;
  using F = typename Max::F;
  mt19937 rng(117211);
  for (int n : {0, 1, 2, 7, 31, 65})
  {
    vc<T> a(n);
    vc<typename Max::S> max_init;
    vc<typename MinMax::S> min_max_init;
    repi(i, n)
    {
      a[i] = T(int(rng() % 201) - 100);
      max_init.emplace_back(i, a[i]);
      min_max_init.emplace_back(i, a[i]);
    }
    LazySegmentTree<Max> seg_max(max_init);
    LazySegmentTree<MinMax> seg_min_max(min_max_init);
    auto check = [&](int l, int r)
    {
      T mn = resolved_infty<T>(), mx = -resolved_infty<T>();
      repi(i, l, r) mn = min(mn, a[i]), mx = max(mx, a[i]);
      const auto x = seg_max.prod(l, r);
      const auto y = seg_min_max.prod(l, r);
      assert(x.mx == mx && y.mn == mn && y.mx == mx);
      if (l == r) assert(x.l == x.r && y.l == y.r);
      else assert(x.l == l && x.r == r && y.l == l && y.r == r);
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
        seg_max.set(p, typename Max::S(p, c));
        seg_min_max.set(p, typename MinMax::S(p, c));
      }
      else if (iter % 9 == 0)
      {
        seg_max.apply(l, r, Max::id());
        seg_min_max.apply(l, r, MinMax::id());
      }
      else
      {
        if (n && iter % 5 == 0)
        {
          l = rng() % n, r = l + 1;
          seg_max.apply(l, F({c, d}));
          seg_min_max.apply(l, F({c, d}));
        }
        else
        {
          seg_max.apply(l, r, F({c, d}));
          seg_min_max.apply(l, r, F({c, d}));
        }
        repi(i, l, r) a[i] = c + d * T(i);
      }
      check(l, r);
      l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      check(l, r);
      const auto whole = seg_min_max.all_prod();
      T mn = resolved_infty<T>(), mx = -resolved_infty<T>();
      for (T value : a) mn = min(mn, value), mx = max(mx, value);
      assert(whole.mn == mn && whole.mx == mx && seg_max.all_prod().mx == mx);
    }
    const auto x = seg_max.content();
    const auto y = seg_min_max.content();
    repi(i, n)
    {
      assert(x[i].l == i && x[i].r == i + 1 && x[i].mx == a[i]);
      assert(y[i].l == i && y[i].r == i + 1 && y[i].mn == a[i] && y[i].mx == a[i]);
    }
    repi(l, n + 1) repi(r, l, n + 1) check(l, r);
  }
}

ll custom_infty() { return numeric_limits<ll>::max(); }
i128 wide_infty() { return numeric_limits<i128>::max(); }

void test_infty_and_wide_intermediates()
{
  using Max = ActedMonoidAriUpdMax<ll>;
  using MinMax = ActedMonoidAriUpdMinMax<ll>;
  const Max::F f({-8'000'000'000'000'000'000LL, 4'000'000'000'000'000'000LL});
  const Max::F g({8'000'000'000'000'000'000LL, -4'000'000'000'000'000'000LL});
  assert(Max::mapping(f, Max::S(2, 4, 0)).mx == 4'000'000'000'000'000'000LL);
  assert(Max::mapping(g, Max::S(2, 4, 0)).mx == 0);
  const auto up = MinMax::mapping(f, MinMax::S(2, 4, 0, 0));
  const auto down = MinMax::mapping(g, MinMax::S(2, 4, 0, 0));
  assert(up.mn == 0 && up.mx == 4'000'000'000'000'000'000LL);
  assert(down.mn == -4'000'000'000'000'000'000LL && down.mx == 0);
  using SmallMax = ActedMonoidAriUpdMax<int>;
  using SmallMinMax = ActedMonoidAriUpdMinMax<int>;
  const SmallMax::F small({-2'000'000'000, 1'000'000'000});
  assert(SmallMax::mapping(small, SmallMax::S(2, 4, 0)).mx == 1'000'000'000);
  const auto small_result = SmallMinMax::mapping(small, SmallMinMax::S(2, 4, 0, 0));
  assert(small_result.mn == 0 && small_result.mx == 1'000'000'000);

  using CustomMax = ActedMonoidAriUpdMax<ll, custom_infty>;
  using CustomMinMax = ActedMonoidAriUpdMinMax<ll, custom_infty>;
  assert(CustomMax::e().mx == -custom_infty());
  assert(CustomMinMax::e().mn == custom_infty() && CustomMinMax::e().mx == -custom_infty());
  for (ll value : {custom_infty(), -custom_infty(), INF + 1, -INF - 1})
  {
    const CustomMax::F f({value, 0});
    const auto a = CustomMax::mapping(f, CustomMax::S(2, 3));
    const auto b = CustomMinMax::mapping(f, CustomMinMax::S(2, 3));
    assert(a.mx == value && b.mn == value && b.mx == value);
    assert(CustomMax::op(CustomMax::e(), a).mx == value);
    assert(CustomMinMax::op(CustomMinMax::e(), b).mx == value);
    assert(CustomMax::mapping(f, CustomMax::e()).mx == -custom_infty());
    assert(CustomMinMax::mapping(f, CustomMinMax::e()).mn == custom_infty());
  }
  using Explicit = ActedMonoidAriUpdMinMax<ll, 1'000'000LL>;
  assert(Explicit::e().mn == 1'000'000 && Explicit::e().mx == -1'000'000);

  // unsigned の途中結果を T に戻してから、負数を含む大小比較を行う。
  using WideMin = ActedMonoidAriUpdMin<i128, wide_infty>;
  using WideMax = ActedMonoidAriUpdMax<i128, wide_infty>;
  using WideMinMax = ActedMonoidAriUpdMinMax<i128, wide_infty>;
  const i128 h = i128(1) << 126;
  const WideMin::F wide_up({-h, h});
  const auto wide = WideMinMax::mapping(wide_up, WideMinMax::S(0, 3, 0, 0));
  assert(wide.mn == -h && wide.mx == h);
  assert(WideMin::mapping(wide_up, WideMin::S(0, 3, 0)).mn == -h);
  assert(WideMax::mapping(wide_up, WideMax::S(0, 3, 0)).mx == h);
  const WideMin::F wide_down({wide_infty(), -h});
  const auto negative = WideMinMax::mapping(wide_down, WideMinMax::S(2, 4, 0, 0));
  assert(negative.mn == -h - 1 && negative.mx == -1);
  assert(WideMin::mapping(wide_down, WideMin::S(2, 4, 0)).mn == -h - 1);
  assert(WideMax::mapping(wide_down, WideMax::S(2, 4, 0)).mx == -1);
}

int main()
{
  test_laws<ActedMonoidAriUpdMax<ll>>([](const auto &a, const auto &b)
  { return a.l == b.l && a.r == b.r && a.mx == b.mx; });
  test_laws<ActedMonoidAriUpdMinMax<ll>>([](const auto &a, const auto &b)
  { return a.l == b.l && a.r == b.r && a.mn == b.mn && a.mx == b.mx; });
  test_tree<ll>();
  test_tree<int>();
  test_tree<i128>();
  test_tree<double>();
  test_infty_and_wide_intermediates();
  cout << "Hello World" << endl;
}
