#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/cumulative_sum/cumulative_sum_dynamic.hpp"
#include "ds/cumulative_sum/cumulative_sum_dynamic_2d.hpp"
#include "ds/cumulative_sum/imos_dynamic.hpp"
#include "ds/cumulative_sum/imos_dynamic_2d.hpp"
#include "algebra/bit.hpp"
#include "math/modint/modint.hpp"

using mint = modint998244353;

// Compare legal interleavings with plain arrays and independent update-range masks.
// Cover additive, XOR, multiplicative, and (1D cumulative sums only) noncommutative groups.
template <class G, class F>
void test_cumulative_sum(F value)
{
  using S = typename G::S;
  for (int n = 0; n <= 25; ++n)
  {
    vc<S> a(n);
    for (auto &v : a) v = value();
    CumulativeSumDynamic<G> s(a);
    int fixed = 0;
    repi(step, 250)
    {
      if (step % 3 == 0 && fixed < n)
      {
        int i = randint<int>(fixed, n - 1);
        S v = value();
        assert(s.get(i) == a[i]);
        if (step % 2)
          s.set(i, v), a[i] = v;
        else
          s.add(i, v), a[i] = G::op(a[i], v);
      }
      else
      {
        int r = randint<int>(0, min(n, 1 + step / 8));
        int l = randint<int>(0, r);
        bool prefix = step % 2;
        if (prefix) l = 0;
        S expected = G::e();
        for (int i = l; i < r; ++i) expected = G::op(expected, a[i]);
        assert((prefix ? s.sum(r) : s.sum(l, r)) == expected);
        if (l < r) fixed = max(fixed, r);
      }
      assert(as_const(s).content() == a);
    }
    repi(i, n) assert(s.get(i) == a[i]);
  }
}

template <class G, class F>
void test_imos(F value)
{
  using S = typename G::S;
  for (int n = 0; n <= 25; ++n)
  {
    vc<S> a(n);
    for (auto &v : a) v = value();
    ImosDynamic<G> s(a);
    int fixed = 0;
    repi(step, 250)
    {
      if (step % 3 == 0 || n == 0)
      {
        int l = randint<int>(fixed, n), r = randint<int>(l, n);
        S v = value();
        s.add(l, r, v);
        for (int i = l; i < r; ++i) a[i] = G::op(a[i], v);
        int empty = randint<int>(0, n);
        s.add(empty, empty, v);
      }
      else
      {
        int i = randint<int>(0, min(n - 1, step / 8));
        if (step % 3 == 1)
          s.set(i, a[i] = value());
        else
          assert(s.get(i) == a[i]);
        fixed = max(fixed, i + 1);
      }
      assert(as_const(s).content() == a);
    }
    repi(i, n) assert(s.get(i) == a[i]);
  }
}

template <class G>
typename G::S rectangle_sum(const vvc<typename G::S> &a, int li, int ri, int lj, int rj)
{
  auto res = G::e();
  for (int i = li; i < ri; ++i)
    for (int j = lj; j < rj; ++j)
      res = G::op(res, a[i][j]);
  return res;
}

template <class G, class F>
void test_2d(F value)
{
  using S = typename G::S;
  repi(trial, 90)
  {
    int n = randint<int>(0, 9), m = randint<int>(0, 9);
    vvc<S> a(n, vc<S>(m));
    for (auto &row : a)
      for (auto &v : row) v = value();
    CumulativeSumDynamic2D<G> cs(n, m);
    ImosDynamic2D<G> im(n, m);
    if (n > 0)
      cs = CumulativeSumDynamic2D<G>(a), im = ImosDynamic2D<G>(a);
    vvc<S> b = a;
    vvb cs_fixed(n, vb(m)), im_fixed(n, vb(m));
    repi(step, 150)
    {
      if (n > 0 && m > 0)
      {
        int i = randint<int>(0, n - 1), j = randint<int>(0, m - 1);
        assert(cs.get(i, j) == a[i][j]);
        if (!cs_fixed[i][j])
        {
          S v = value();
          if (step % 2)
            cs.set(i, j, v), a[i][j] = v;
          else
            cs.add(i, j, v), a[i][j] = G::op(a[i][j], v);
        }
      }

      int li = randint<int>(0, n), ri = randint<int>(li, n);
      int lj = randint<int>(0, m), rj = randint<int>(lj, m);
      bool can_add = true;
      for (int i = li; i < ri; ++i)
        for (int j = lj; j < rj; ++j)
          if (im_fixed[i][j]) can_add = false;
      if (can_add)
      {
        S v = value();
        im.add(li, ri, lj, rj, v);
        for (int i = li; i < ri; ++i)
          for (int j = lj; j < rj; ++j)
            b[i][j] = G::op(b[i][j], v);
      }

      // Alternate row/column sweeps and incomparable prefix rectangles.
      ri = randint<int>(0, trial % 3 == 0 ? min(n, step / 15 + 1) : n);
      rj = randint<int>(0, trial % 3 == 1 ? min(m, step / 15 + 1) : m);
      li = step % 2 ? 0 : randint<int>(0, ri);
      lj = step % 2 ? 0 : randint<int>(0, rj);
      S expected = rectangle_sum<G>(a, li, ri, lj, rj);
      assert((step % 2 ? cs.sum(ri, rj) : cs.sum(li, ri, lj, rj)) == expected);
      if (li < ri && lj < rj)
        repi(i, ri) repi(j, rj) cs_fixed[i][j] = true;
      if (ri > 0 && rj > 0)
      {
        if (step % 3 == 0)
          im.set(ri - 1, rj - 1, b[ri - 1][rj - 1] = value());
        else
          assert(im.get(ri - 1, rj - 1) == b[ri - 1][rj - 1]);
        repi(i, ri) repi(j, rj) im_fixed[i][j] = true;
      }
      assert(as_const(cs).content() == a);
      assert(as_const(im).content() == b);
    }
    assert(cs.sum(n, m) == rectangle_sum<G>(a, 0, n, 0, m));
    repi(i, n) repi(j, m)
    {
      assert(cs.get(i, j) == a[i][j]);
      assert(im.get(i, j) == b[i][j]);
    }
  }
}

// Every staircase of expanded cells and every target of point assignment.
// The right/bottom neighbors can independently be values, differences, or row frontiers.
template <class G, class F>
void test_imos_set_2d(F value)
{
  using S = typename G::S;
  const int n = 3, m = 4;
  vvc<S> initial(n, vc<S>(m));
  for (auto &row : initial)
    for (auto &v : row) v = value();
  vc<int> built(n);
  auto enumerate = [&](auto &&self, int row) -> void
  {
    if (row < n)
    {
      for (built[row] = 0; built[row] <= (row == 0 ? m : built[row - 1]); ++built[row])
        self(self, row + 1);
      return;
    }
    ImosDynamic2D<G> base(initial);
    repi(i, n) if (built[i] > 0) base.get(i, built[i] - 1);
    repi(i, n) repi(j, m)
    {
      auto im = base;
      auto expected = initial;
      im.set(i, j, expected[i][j] = value());
      assert(im.get(i, j) == expected[i][j]);
      assert(as_const(im).content() == expected);
      // Inspecting content preserves the legal future rectangle updates.
      repi(x, n)
        for (int y = max(built[x], x <= i ? j + 1 : 0); y < m; ++y)
        {
          S v = value();
          im.add(x, n, y, m, v);
          for (int u = x; u < n; ++u)
            for (int w = y; w < m; ++w)
              expected[u][w] = G::op(expected[u][w], v);
        }
      assert(as_const(im).content() == expected);
      repi(x, n) repi(y, m) assert(im.get(x, y) == expected[x][y]);
    }
  };
  enumerate(enumerate, 0);
}

// Permutation composition makes the order of inverse/prefix multiplication observable.
struct PermutationGroup
{
  using S = array<int, 4>;
  static S e() { return {0, 1, 2, 3}; }
  static S op(const S &a, const S &b)
  {
    S c;
    repi(i, 4) c[i] = a[b[i]];
    return c;
  }
  static S inv(const S &a)
  {
    S b;
    repi(i, 4) b[a[i]] = i;
    return b;
  }
};

void test_boundaries()
{
  CumulativeSumDynamic<GroupAddSub<ll>> empty;
  assert(empty.sum(0) == 0 && empty.sum(0, 0) == 0);
  ImosDynamic<GroupAddSub<ll>> empty_im;
  empty_im.add(0, 0, 10);
  CumulativeSumDynamic2D<GroupAddSub<ll>> empty2;
  assert(empty2.sum(0, 0) == 0);
  ImosDynamic2D<GroupAddSub<ll>> empty_im2;
  empty_im2.add(0, 0, 0, 0, 10);
  assert(as_const(empty).content().empty());
  assert(as_const(empty_im).content().empty());
  assert(as_const(empty2).content().empty());
  assert(as_const(empty_im2).content().empty());

  ImosDynamic<GroupAddSub<ll>> point(vl{1, 2, 3, 4});
  assert(point.get(1) == 2);
  point.set(1, 9);
  assert(point.content() == vl({1, 9, 3, 4}));
  point.add(2, 4, 5);
  point.set(0, -1);
  point.set(3, 0);
  point.set(3, 7);
  assert(point.content() == vl({-1, 9, 8, 7}));
  auto snapshot = point.content();
  snapshot[0] = 100;
  assert(point.get(0) == -1);

  CumulativeSumDynamic<GroupAddSub<ll>> cs(6);
  assert(cs.sum(6, 6) == 0);
  cs.set(0, 3);
  assert(cs.get(0) == 3);
  cs.add(0, 2);
  assert(cs.sum(1) == 5);

  CumulativeSumDynamic2D<GroupAddSub<ll>> cs2(6, 6);
  assert(cs2.sum(6, 6, 0, 6) == 0);
  assert(cs2.sum(0, 6, 6, 6) == 0);
  cs2.set(0, 0, 5);
  assert(cs2.sum(2, 5) == 5);
  assert(cs2.sum(5, 2) == 5);
  cs2.set(3, 3, 7);
  assert(cs2.sum(2, 4, 2, 4) == 7);
  assert(cs2.sum(6, 6) == 12);
  assert(cs2.sum(1, 1) == 5);

  ImosDynamic2D<GroupAddSub<ll>> im2(6, 6);
  im2.add(0, 6, 0, 6, 1);
  assert(im2.get(1, 4) == 1);
  assert(im2.get(4, 1) == 1);
  im2.add(2, 6, 2, 6, 7);
  im2.add(0, 0, 0, 6, 100);
  im2.add(0, 6, 0, 0, 100);
  assert(im2.get(3, 3) == 8);
  assert(im2.get(0, 0) == 1);
  assert(im2.get(5, 5) == 8);
}

void test_dp()
{
  const int n = 70, m = 55;
  CumulativeSumDynamic<GroupAddSub<mint>> cs(n);
  ImosDynamic<GroupAddSub<mint>> im(n);
  vc<mint> a(n);
  a[0] = 1;
  im.set(0, 1);
  repi(i, n)
  {
    if (i > 0)
      for (int k = max(0, i - 3); k < i; ++k) a[i] += a[k];
    cs.set(i, i == 0 ? 1 : cs.sum(max(0, i - 3), i));
    assert(cs.get(i) == a[i]);
    assert(im.get(i) == a[i]);
    im.add(i + 1, min(n, i + 4), im.get(i));
  }

  repi(order, 3)
  {
    CumulativeSumDynamic2D<GroupAddSub<mint>> cs2(n, m);
    ImosDynamic2D<GroupAddSub<mint>> im2(n, m);
    vvc<mint> a2(n, vc<mint>(m));
    im2.set(0, 0, 1);
    vc<pair<int, int>> positions;
    repi(i, n) repi(j, m) positions.emplace_back(i, j);
    if (order == 1)
      sort(positions.begin(), positions.end(), [](auto a, auto b) { return pair{a.second, a.first} < pair{b.second, b.first}; });
    if (order == 2)
      sort(positions.begin(), positions.end(), [](auto a, auto b) { return a.first + a.second < b.first + b.second; });
    for (auto [i, j] : positions)
    {
      int li = max(0, i - 2), lj = max(0, j - 3);
      mint v = i == 0 && j == 0 ? 1 : rectangle_sum<GroupAddSub<mint>>(a2, li, i, lj, j);
      a2[i][j] = v;
      cs2.set(i, j, i == 0 && j == 0 ? 1 : cs2.sum(li, i, lj, j));
      assert(cs2.get(i, j) == v);
      assert(im2.get(i, j) == v);
      im2.add(i + 1, min(n, i + 3), j + 1, min(m, j + 4), v);
    }
  }

  // Updates to later rows may start at a smaller column than the current point.
  ImosDynamic2D<GroupAddSub<ll>> rows(8, 9);
  rows.add(0, 1, 0, 9, 1);
  ll expected = 1;
  repi(i, 8)
  {
    repi(j, 9)
    {
      ll v = rows.get(i, j);
      assert(v == expected);
      rows.add(i + 1, min(8, i + 2), 0, 9, v);
    }
    expected *= 9;
  }
}

struct CountingGroup : GroupAddSub<ll>
{
  static inline ll calls = 0;
  static ll op(ll a, ll b) { ++calls; return a + b; }
};

void test_linear_work()
{
  // Thin grids also exercise the shapes that caused deep recursion in the old code.
  for (auto [n, m] : vc<pair<int, int>>{{1, 200000}, {200000, 1}, {240, 350}})
  {
    CumulativeSumDynamic2D<CountingGroup> cs(n, m);
    ImosDynamic2D<CountingGroup> im(n, m);
    cs.set(0, 0, 3);
    im.add(0, n, 0, m, 3);
    CountingGroup::calls = 0;
    assert(cs.sum(n, m) == 3);
    assert(im.get(n - 1, m - 1) == 3);
    assert(CountingGroup::calls <= 4LL * n * m);
    CountingGroup::calls = 0;
    repi(_, 1000)
    {
      assert(cs.sum(n, m) == 3);
      assert(im.get(n - 1, m - 1) == 3);
    }
    assert(CountingGroup::calls == 0);
  }
  const int n = 100, m = 120;
  repi(order, 2)
  {
    CumulativeSumDynamic2D<CountingGroup> cs(n, m);
    ImosDynamic2D<CountingGroup> im(n, m);
    CountingGroup::calls = 0;
    repi(k, n * m)
    {
      int i = order ? k % n : k / m, j = order ? k / n : k % m;
      assert(cs.sum(i + 1, j + 1) == 0);
      assert(im.get(i, j) == 0);
    }
    assert(CountingGroup::calls <= 4LL * n * m);
  }
}

int main()
{
  mt.seed(20260918);
  auto integer = []() { return randint<ll>(-10, 10); };
  auto bits = []() { return randint<unsigned>(0, 255); };
  auto nonzero = []() { return mint(randint<int>(1, 20)); };
  test_cumulative_sum<GroupAddSub<ll>>(integer);
  test_cumulative_sum<GroupXor<unsigned>>(bits);
  test_cumulative_sum<GroupMulDiv<mint>>(nonzero);
  test_cumulative_sum<PermutationGroup>([]() {
    auto p = PermutationGroup::e();
    shuffle(p.begin(), p.end(), mt);
    return p;
  });
  test_imos<GroupAddSub<ll>>(integer);
  test_imos<GroupXor<unsigned>>(bits);
  test_imos<GroupMulDiv<mint>>(nonzero);
  test_2d<GroupAddSub<ll>>(integer);
  test_2d<GroupXor<unsigned>>(bits);
  test_2d<GroupMulDiv<mint>>(nonzero);
  test_imos_set_2d<GroupAddSub<ll>>(integer);
  test_imos_set_2d<GroupXor<unsigned>>(bits);
  test_imos_set_2d<GroupMulDiv<mint>>(nonzero);
  test_boundaries();
  test_dp();
  test_linear_work();
  cout << "Hello World" << endl;
}
