#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "convex/convex_hull_trick_dot_product.hpp"

// Test focus: min and max support queries, returned form IDs, rational directions,
// and increasing/decreasing x/y monotone queries with both signs of y.
mt19937_64 dot_rng(7654321);

ll random_dot_ll(ll l, ll r)
{
  return l + dot_rng() % (r - l + 1);
}

template <bool minimize>
i128 brute_dot(const vc<pair<ll, ll>> &forms, ll x, ll y)
{
  optional<i128> ans;
  for (auto [a, b] : forms)
  {
    i128 value = i128(a) * x + i128(b) * y;
    if (!ans)
      ans = value;
    else if constexpr (minimize)
      ans = min(*ans, value);
    else
      ans = max(*ans, value);
  }
  return *ans;
}

template <bool minimize, class CHT>
auto dot_query(const CHT &cht, ll x, ll y)
{
  if constexpr (minimize)
    return cht.min_query(x, y);
  else
    return cht.max_query(x, y);
}

template <bool minimize, class CHT>
auto dot_query_monotone(CHT &cht, ll x, ll y)
{
  if constexpr (minimize)
    return cht.min_query_monotone(x, y);
  else
    return cht.max_query_monotone(x, y);
}

vc<pair<ll, ll>> ratio_queries()
{
  vc<pair<ll, ll>> res;
  rep3(x, -5, 6, 1) rep3(y, -5, 6, 1) if (y) res.eb(x, y);
  sort(res.begin(), res.end(), [](auto p, auto q)
       {
         if (p.second < 0) p.first = -p.first, p.second = -p.second;
         if (q.second < 0) q.first = -q.first, q.second = -q.second;
         i128 lhs = i128(p.first) * q.second;
         i128 rhs = i128(q.first) * p.second;
         return lhs != rhs ? lhs < rhs : p < q;
       });
  return res;
}

template <bool minimize, class CHT>
void verify_dot_product(CHT &cht, const vc<pair<ll, ll>> &forms)
{
  repi(_, 200)
  {
    ll x = random_dot_ll(-50, 50), y = random_dot_ll(-50, 50);
    auto [value, form] = dot_query<minimize>(cht, x, y);
    assert(value == brute_dot<minimize>(forms, x, y));
    assert(value == i128(form.a) * x + i128(form.b) * y);
    assert(0 <= form.id && form.id < SZ(forms));
    assert((forms[form.id] == pair{form.a, form.b}));
  }

  vc<pair<ll, ll>> qs = ratio_queries();
  cht.reset_monotone_query();
  for (auto [x, y] : qs)
  {
    auto [value, form] = dot_query_monotone<minimize>(cht, x, y);
    assert(value == brute_dot<minimize>(forms, x, y));
    assert(value == i128(form.a) * x + i128(form.b) * y);
    assert(0 <= form.id && form.id < SZ(forms));
    assert((forms[form.id] == pair{form.a, form.b}));
  }
  reverse(qs.begin(), qs.end());
  cht.reset_monotone_query();
  for (auto [x, y] : qs)
    assert(dot_query_monotone<minimize>(cht, x, y).first == brute_dot<minimize>(forms, x, y));
}

template <class CHT>
void verify_interleaved_monotone(CHT &cht, const vc<pair<ll, ll>> &forms)
{
  cht.reset_monotone_query();
  for (auto [x, y] : ratio_queries())
  {
    assert(cht.min_query_monotone(x, y).first == brute_dot<true>(forms, x, y));
    assert(cht.max_query_monotone(x, y).first == brute_dot<false>(forms, x, y));
  }
}

void test_arbitrary_coefficient()
{
  repi(iter, 100)
  {
    ConvexHullTrickDotProduct<ll> cht;
    vc<pair<ll, ll>> forms;
    repi(i, 40)
    {
      ll a = random_dot_ll(-30, 30), b = random_dot_ll(-30, 30);
      int id = forms.size();
      forms.eb(a, b);
      cht.add(a, b, id);
    }
    verify_dot_product<true>(cht, forms);
    verify_dot_product<false>(cht, forms);
    verify_interleaved_monotone(cht, forms);
  }
}

void test_monotone_slope()
{
  repi(iter, 100)
  {
    vc<pair<ll, ll>> forms;
    repi(i, 40) forms.eb(random_dot_ll(-30, 30), random_dot_ll(-30, 30));
    stable_sort(forms.begin(), forms.end());
    if (iter & 1) reverse(forms.begin(), forms.end());

    ConvexHullTrickDotProductMonotoneSlope<ll> cht;
    repi(i, forms.size()) cht.add(forms[i].first, forms[i].second, i);
    verify_dot_product<true>(cht, forms);
    verify_dot_product<false>(cht, forms);
    verify_interleaved_monotone(cht, forms);
  }
}

void test_convex_hull_and_clear()
{
  ConvexHullTrickDotProduct cht;
  assert(cht.min_query(1, 2).first == i128(INF));
  assert(cht.max_query(1, 2).first == -i128(INF));
  assert((cht.min_query(1, 2).second == decltype(cht)::LinearForm{0, 0, -1}));
  assert((cht.max_query(1, 2).second == decltype(cht)::LinearForm{0, 0, -1}));

  cht.add(0, 0, 0);
  cht.add(4, 0, 1);
  cht.add(4, 3, 2);
  cht.add(0, 3, 3);
  cht.add(2, 1, 4);
  cht.add(2, 0, 5);
  cht.add(0, 0, 6);
  auto hull = cht.convex_hull();
  assert(hull.size() == 4);
  repi(i, hull.size())
  {
    auto o = hull[i], a = hull[(i + 1) % hull.size()], b = hull[(i + 2) % hull.size()];
    i128 cr = i128(a.a - o.a) * (b.b - o.b) - i128(a.b - o.b) * (b.a - o.a);
    assert(cr > 0);
  }
  assert(cht.min_query(1, 0).first == 0);
  assert(cht.min_query(-1, 0).first == -4);
  assert(cht.min_query(0, 1).first == 0);
  assert(cht.min_query(0, -1).first == -3);
  assert(cht.min_query(0, 0).first == 0);
  assert(cht.max_query(1, 0).first == 4);
  assert(cht.max_query(-1, 0).first == 0);
  assert(cht.max_query(0, 1).first == 3);
  assert(cht.max_query(0, -1).first == 0);
  assert(cht.max_query(0, 0).first == 0);
  cht.clear();
  assert(cht.min_query(3, 4).first == i128(INF));
  assert(cht.max_query(3, 4).first == -i128(INF));
}

template <class CHT>
void verify_auto_id()
{
  CHT cht;
  cht.add(1, 0);
  cht.add(0, 1, 42);
  cht.add(-1, 0);
  assert(cht.min_query(-1, 0).second.id == 0);
  assert(cht.min_query(0, -1).second.id == 42);
  assert(cht.min_query(1, 0).second.id == 2);
  assert(cht.max_query(-1, 0).second.id == 2);
  assert(cht.max_query(0, 1).second.id == 42);
  assert(cht.max_query(1, 0).second.id == 0);

  cht.clear();
  cht.add(2, 3);
  assert(cht.min_query(1, 1).second.id == 0);
  assert(cht.max_query(1, 1).second.id == 0);
}

void test_auto_id()
{
  verify_auto_id<ConvexHullTrickDotProduct<>>();
  verify_auto_id<ConvexHullTrickDotProductMonotoneSlope<>>();
}

void test_online()
{
  ConvexHullTrickDotProduct cht;
  vc<pair<ll, ll>> forms;
  repi(i, 500)
  {
    if (forms.empty() || random_dot_ll(0, 2) != 0)
    {
      ll a = random_dot_ll(-100, 100), b = random_dot_ll(-100, 100);
      int id = forms.size();
      forms.eb(a, b);
      cht.add(a, b, id);
    }
    else
    {
      ll x = random_dot_ll(-100, 100), y = random_dot_ll(-100, 100);
      assert(cht.min_query(x, y).first == brute_dot<true>(forms, x, y));
    }
  }
}

void test_wide_inner_product()
{
  constexpr ll v = 4'000'000'000LL;
  ConvexHullTrickDotProduct<ll> cht;
  cht.add(v, v, 0);
  cht.add(-v, v, 1);
  auto [value, form] = cht.max_query(v, v);
  assert(value == i128(2) * v * v);
  assert(form.id == 0);
}

void test_floating_point()
{
#if __cplusplus >= 202002L
  ConvexHullTrickDotProduct<long double> cht;
  vc<pair<long double, long double>> forms;
  repi(i, 40)
  {
    long double a = random_dot_ll(-100, 100) / 7.0L;
    long double b = random_dot_ll(-100, 100) / 11.0L;
    forms.eb(a, b);
    cht.add(a, b, i);
  }
  repi(_, 100)
  {
    long double x = random_dot_ll(-100, 100) / 13.0L;
    long double y = random_dot_ll(-100, 100) / 17.0L;
    long double ans = numeric_limits<long double>::lowest();
    for (auto [a, b] : forms) ans = max(ans, a * x + b * y);
    assert(abs(cht.max_query(x, y).first - ans) < 1e-14L);
  }
#endif
}

int main()
{
  test_arbitrary_coefficient();
  test_monotone_slope();
  test_convex_hull_and_clear();
  test_auto_id();
  test_online();
  test_wide_inner_product();
  test_floating_point();
  cout << "Hello World" << endl;
}
