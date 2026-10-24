#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/algebra_base.hpp"
#include "algebra/clamp_add.hpp"
#include "algebra/clamp_add_cost.hpp"
#include "ds/segtree/dual_segtree.hpp"
#include "ds/segtree/segtree.hpp"

static_assert(MonoidClampAddCost<ll, 1, 1>::e()(7).first == 7);
static_assert(MonoidClampAddCost<ll, 1, 1>::S::clamp(1, 4)(7).second == 3);
static_assert(MonoidClampAddCost<ll, -1, -1>::S::shift_then_clamp(7, 0, 10, 7)(8).second == 2);
static_assert(MonoidClampAdd<ll>::op(MonoidClampAdd<ll>::S::add(3), MonoidClampAdd<ll>::e())(2) == 5);
static_assert(MonoidClampAdd<ll>::S::shift_then_clamp(7, 0, 10)(8) == 10);
static_assert(MonoidClampAddCost<ll, 0, 1>::S::clamp(2, 5)(0).second == 0);
static_assert(MonoidClampAddCost<ll, 0, 1>::S::clamp(2, 5)(8).second == 3);
static_assert(MonoidClampAddCost<ll, 1, 0>::S::clamp(2, 5)(0).second == 2);
static_assert(MonoidClampAddCost<ll, 1, 0>::S::clamp(2, 5)(8).second == 0);
static_assert(MonoidClampAddCost<ll, 2, 3>::S::clamp(2, 5)(0).second == 4);
static_assert(MonoidClampAddCost<ll, 2, 3>::S::clamp(2, 5)(8).second == 9);
static_assert(MonoidClampAddCost<ll, -2, 3>::S::clamp(2, 5)(0).second == -4);

template <class S>
void assert_same(const S &a, const S &b)
{
  assert(a.lo == b.lo && a.hi == b.hi && a.bias == b.bias && a.base == b.base);
}

template <class M>
void test_laws()
{
  using S = typename M::S;
  vc<S> values{M::e(), S::add(-3), S::add(4), S::chmin(0), S::chmax(2), S::set(-1)};
  for (ll l = -2; l <= 2; ++l)
    for (ll r = l; r <= 2; ++r)
      for (ll a : {-3, 0, 4}) values.emplace_back(l, r, a, l - r);
  for (const S &f : values)
  {
    assert_same(M::op(f, M::e()), f);
    assert_same(M::op(M::e(), f), f);
    for (const S &g : values)
    {
      const S fg = M::op(f, g);
      assert(fg.lo <= fg.hi);
      for (ll x = -8; x <= 8; ++x)
      {
        const auto [y, c1] = g(x);
        const auto [z, c2] = f(y);
        assert((fg(x) == pair<ll, ll>{z, c1 + c2}));
      }
      for (const S &h : values)
        assert_same(M::op(M::op(f, g), h), M::op(f, M::op(g, h)));
    }
  }
}

template <class M, int chmax_cost, int chmin_cost>
void test_sequence_and_tree()
{
  using S = typename M::S;
  mt19937 rng(792807);
  for (int n : {0, 1, 2, 19, 64})
  {
    auto make_leaf = [&]()
    {
      ll l = int(rng() % 31) - 15, r = int(rng() % 31) - 15;
      if (l > r) swap(l, r);
      const ll bias = int(rng() % 21) - 10, base = int(rng() % 21) - 10;
      switch (rng() % 6)
      {
      case 0: return S::add(bias, base);
      case 1: return S::chmin(l, base);
      case 2: return S::chmax(r, base);
      case 3: return S::set(l, base);
      case 4: return S::shift_then_clamp(bias, l, r, base);
      default: return S(l, r, bias, base);
      }
    };
    vc<S> a(n);
    for (auto &f : a) f = make_leaf();
    SegmentTree<OppositeMonoid<M>> seg(a);
    for (int it = 0; it < 1000; ++it)
    {
      if (n && it % 3 == 0)
      {
        int p = rng() % n;
        a[p] = make_leaf();
        seg.set(p, a[p]);
      }
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      if (it % 7 == 0) l = 0, r = n;
      for (ll x : {-40, -1, 0, 3, 40})
      {
        ll y = x, cost = 0;
        repi(i, l, r)
        {
          const ll z = std::clamp(y, a[i].lo, a[i].hi);
          const ll correction = chmax_cost * max(z - y, 0LL) + chmin_cost * max(y - z, 0LL);
          cost += a[i].base + correction;
          y = z + a[i].bias;
        }
        assert((seg.prod(l, r)(x) == pair<ll, ll>{y, cost}));
      }
    }
  }
}

// AAL Q: 単位移動を繰り返した結果と照合する。
void test_bounded_moves()
{
  using M = MonoidClampAddCost<ll, -1, -1>;
  mt19937 rng(20261015);
  for (ll limit = 0; limit <= 8; ++limit)
    for (int it = 0; it < 200; ++it)
    {
      vc<ll> moves(15);
      auto f = M::e();
      for (ll &v : moves)
      {
        v = int(rng() % 31) - 15;
        f = M::op(M::S::shift_then_clamp(v, 0, limit, abs(v)), f);
      }
      for (ll x = 0; x <= limit; ++x)
      {
        ll y = x, gain = 0;
        for (ll v : moves)
          for (ll j = 0; j < abs(v); ++j)
          {
            ll z = y + (v < 0 ? -1 : 1);
            if (0 <= z && z <= limit) y = z, ++gain;
          }
        assert((f(x) == pair<ll, ll>{y, gain}));
      }
    }
}

// ABC365 F: 小さな連結通路の最短路を BFS と照合する。
void test_corridor()
{
  using M = MonoidClampAddCost<ll, 1, 1>;
  mt19937 rng(365);
  constexpr int n = 6, height = 6;
  repi(it, 120)
  {
    vc<int> lo(n), hi(n);
    vc<M::S> a;
    repi(i, n)
    {
      do
      {
        lo[i] = rng() % height, hi[i] = rng() % height;
        if (lo[i] > hi[i]) swap(lo[i], hi[i]);
      } while (i && max(lo[i - 1], lo[i]) > min(hi[i - 1], hi[i]));
      a.push_back(M::S::clamp(lo[i], hi[i]));
    }
    SegmentTree<OppositeMonoid<M>> seg(a);
    repi(sx, n) repi(sy, lo[sx], hi[sx] + 1)
    {
      vvc<int> dist(n, vc<int>(height, -1));
      queue<pair<int, int>> que;
      que.emplace(sx, sy), dist[sx][sy] = 0;
      while (!que.empty())
      {
        auto [x, y] = que.front();
        que.pop();
        for (auto [dx, dy] : {pair<int, int>{1, 0}, {-1, 0}, {0, 1}, {0, -1}})
        {
          int xx = x + dx, yy = y + dy;
          if (0 <= xx && xx < n && lo[xx] <= yy && yy <= hi[xx] && dist[xx][yy] == -1)
            dist[xx][yy] = dist[x][y] + 1, que.emplace(xx, yy);
        }
      }
      repi(tx, sx, n) repi(ty, lo[tx], hi[tx] + 1)
      {
        const auto [y, cost] = seg.prod(sx + 1, tx + 1)(sy);
        assert(tx - sx + cost + abs(y - ty) == dist[tx][ty]);
      }
    }
  }
}

void test_wide_and_infinite()
{
  using C = MonoidClampAddCost<int, 1, 1, nullptr, ll>;
  const int h = 1'000'000'000;
  auto c = C::S::clamp(-h, -h, 3'000'000'000LL);
  assert(c(h).second == 5'000'000'000LL);
  using L = MonoidClampAddCost<ll, 1, 1>;
  const ll large = 3'000'000'000'000'000'000LL;
  // g.hi + g.bias は ll を超えるが、合成結果の各成分は ll に収まる。
  const L::S f(large, large, -large, -large), g(-large, large, 3 * large);
  const auto fg = L::op(f, g);
  assert(fg.lo == -large && fg.hi == -large && fg.bias == large && fg.base == 0);
  const auto result = fg(-large);
  assert(result.first == 0 && result.second == 0);

  using M = MonoidClampAdd<ll>;
  const M::S cf(large, large, -large), cg(-large, large, 3 * large);
  assert(M::op(cf, cg)(-large) == 0);
  using D = MonoidClampAddCost<double, 1, 1>;
  const auto de = D::op(D::e(), D::e());
  assert(de(2.5).first == 2.5 && de(2.5).second == 0);
  const auto df = D::op(D::S::chmin(3), D::S::add(2));
  assert(df(-1).first == 1 && df(-1).second == 0);
  assert(df(5).first == 3 && df(5).second == 4);
  using F = MonoidClampAdd<double>;
  assert(F::op(F::e(), F::e())(2.5) == 2.5);
  assert(F::op(F::S::add(3), F::S::add(-2))(0) == 1);
  const double inf = numeric_limits<double>::infinity();
  const auto lower_open = F::S::shift_then_clamp(2, -inf, 3);
  const auto upper_open = F::S::shift_then_clamp(-2, 0, inf);
  assert(lower_open(-10) == -8 && lower_open(10) == 3 && lower_open.lo == -inf);
  assert(upper_open(-10) == 0 && upper_open(10) == 8 && upper_open.hi == inf);
  assert(F::S::shift_then_clamp(2, -inf, inf)(5) == 7);
  using Down = MonoidClampAddCost<double, 0, 1>;
  const auto down = Down::op(Down::S::clamp(2, 5), Down::S::add(1));
  assert(down(-10).first == 2 && down(-10).second == 0);
  assert(down(10).first == 5 && down(10).second == 6);
  using Small = MonoidClampAddCost<ll, 1, 1, 1000>;
  auto small = Small::e();
  repi(i, 100) small = Small::op(Small::S::add(i % 2 ? -7 : 7), small);
  assert_same(small, Small::e());

  // 重みとの積は ll を超え、base を加えると ll に収まる。
  using Weighted = MonoidClampAddCost<ll, 3'000'000'000'000'000'000LL, 2>;
  const auto weighted = Weighted::S::clamp(0, 1, -2 * large);
  assert(weighted(-4).second == 2 * large);
  const auto merged = Weighted::op(Weighted::S::clamp(4, 5), Weighted::S::clamp(0, 0, -2 * large));
  assert(merged(0).first == 4 && merged(0).second == 2 * large);

  // 費用を浮動小数点で持つ場合も、整数座標の精度を保つ。
  using Mixed = MonoidClampAddCost<ll, 2, 3, nullptr, double>;
  const ll coordinate = (1LL << 60) + 1;
  const auto shifted = Mixed::S::shift_then_clamp(3, coordinate, coordinate + 10);
  assert(shifted(coordinate + 1).first == coordinate + 4);
  assert(shifted(coordinate - 4).second == 2);
  const auto composed = Mixed::op(Mixed::S::clamp(coordinate, coordinate + 10), Mixed::S::add(3));
  assert(composed(coordinate + 1).first == coordinate + 4);
  assert(composed(coordinate - 4).second == 2);
}

int chmax_rate_calls = 0, chmin_rate_calls = 0;
double chmax_rate() { ++chmax_rate_calls; return 0.5; }
double chmin_rate() { ++chmin_rate_calls; return -1.5; }

void test_weight_providers()
{
  using M = MonoidClampAddCost<double, chmax_rate, chmin_rate>;
  const auto g = M::S::shift_then_clamp(1, 0, 4, 2);
  assert((g(-3) == pair<double, double>{0, 3}));
  assert((g(6) == pair<double, double>{4, -2.5}));
  const auto f = M::S::clamp(3, 5, 1), fg = M::op(f, g);
  for (double x : {-3, 0, 2, 6})
  {
    const auto [y, c1] = g(x);
    const auto [z, c2] = f(y);
    assert((fg(x) == pair<double, double>{z, c1 + c2}));
  }
  assert(chmax_rate_calls == 1 && chmin_rate_calls == 1);
}

void test_prefix_search()
{
  using M = MonoidClampAddCost<ll, -1, -1>;
  using S = M::S;
  SegmentTree<OppositeMonoid<M>> seg(vc<S>{S::shift_then_clamp(7, 0, 10, 7),
                                         S::shift_then_clamp(-4, 0, 10, 4),
                                         S::shift_then_clamp(9, 0, 10, 9)});
  assert(seg.max_right_ok(0, [](const S &f) { return f(8).second <= 5; }) == 1);
  assert(seg.max_right_ok(0, [](const S &f) { return f(8).second <= 6; }) == 2);
  assert(seg.max_right_ok(0, [](const S &f) { return f(8).second <= 10; }) == 3);
}

void test_directional_adjustments()
{
  using M = MonoidClampAddCost<ll, 1, 1>;
  using Position = MonoidClampAdd<ll>;
  using Down = MonoidClampAddCost<ll, 0, 1>;
  using Up = MonoidClampAddCost<ll, 1, 0>;
  mt19937 rng(2890);
  repi(it, 1000)
  {
    ll x = int(rng() % 31) - 15, y = x, add = 0, up = 0, down = 0;
    auto f = M::e();
    auto position = Position::e();
    auto down_cost = Down::e();
    auto up_cost = Up::e();
    repi(i, 30)
    {
      ll l = int(rng() % 31) - 15, r = int(rng() % 31) - 15;
      if (l > r) swap(l, r);
      const ll v = int(rng() % 31) - 15;
      y += v, add += v;
      up += max(l - y, 0LL), down += max(y - r, 0LL);
      y = std::clamp(y, l, r);
      f = M::op(M::S::shift_then_clamp(v, l, r), f);
      position = Position::op(Position::S::shift_then_clamp(v, l, r), position);
      down_cost = Down::op(Down::S::shift_then_clamp(v, l, r), down_cost);
      up_cost = Up::op(Up::S::shift_then_clamp(v, l, r), up_cost);
    }
    const auto [z, adjustment] = f(x);
    assert(z == y);
    assert(position(x) == y);
    assert((down_cost(x) == pair<ll, ll>{y, down}));
    assert((up_cost(x) == pair<ll, ll>{y, up}));
    assert((adjustment + z - x - add) / 2 == up);
    assert((adjustment - z + x + add) / 2 == down);
  }
}

// IOI2014 Wall: 区間の chmin・chmax と最後の各点取得。
void test_wall()
{
  using M = MonoidClampAdd<int>;
  constexpr int n = 43;
  DualSegmentTree<ActCompoundAssignment<M>> seg(vc<M::S>(n, M::e()));
  vc<int> a(n);
  mt19937 rng(2014);
  repi(it, 2000)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1), h = rng() % 100;
    if (l > r) swap(l, r);
    const bool increasing = rng() % 2;
    seg.apply(l, r, increasing ? M::S::chmax(h) : M::S::chmin(h));
    repi(i, l, r) a[i] = increasing ? max(a[i], h) : min(a[i], h);
    if (it % 17 == 0) repi(i, n) assert(seg.get(i)(0) == a[i]);
  }
  repi(i, n) assert(seg.get(i)(0) == a[i]);
}

int main()
{
  test_laws<MonoidClampAddCost<ll, 1, 1>>();
  test_laws<MonoidClampAddCost<ll, -1, -1>>();
  test_laws<MonoidClampAddCost<ll, 0, 1>>();
  test_laws<MonoidClampAddCost<ll, 1, 0>>();
  test_laws<MonoidClampAddCost<ll, 2, 3>>();
  test_laws<MonoidClampAddCost<ll, -2, 3>>();
  test_laws<MonoidClampAddCost<ll, 0, 0>>();
  test_sequence_and_tree<MonoidClampAddCost<ll, 1, 1>, 1, 1>();
  test_sequence_and_tree<MonoidClampAddCost<ll, -1, -1>, -1, -1>();
  test_sequence_and_tree<MonoidClampAddCost<ll, 0, 1>, 0, 1>();
  test_sequence_and_tree<MonoidClampAddCost<ll, 1, 0>, 1, 0>();
  test_sequence_and_tree<MonoidClampAddCost<ll, 2, 3>, 2, 3>();
  test_sequence_and_tree<MonoidClampAddCost<ll, -2, 3>, -2, 3>();
  test_sequence_and_tree<MonoidClampAddCost<ll, 0, 0>, 0, 0>();
  test_bounded_moves();
  test_corridor();
  test_wide_and_infinite();
  test_weight_providers();
  test_prefix_search();
  test_directional_adjustments();
  test_wall();
  cout << "Hello World" << endl;
}
