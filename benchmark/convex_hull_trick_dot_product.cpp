#include "template/template_all_but_modint.hpp"
#include "math/rational.hpp"
#include "convex/convex_hull_trick.hpp"
#include "convex/convex_hull_trick_dot_product.hpp"

struct Query
{
  ll x, y;
};

template <bool monotone>
struct Wrapper
{
  using CHT = conditional_t<monotone,
                            ConvexHullTrickDotProductMonotoneCoefficient<ll>,
                            ConvexHullTrickDotProduct<ll>>;
  CHT cht;

  void add(ll a, ll b, int id) { cht.add(a, b, id); }
  int query_id(ll x, ll y) { return cht.min_query_with_form(x, y).second.id; }
};

template <class X, bool direct, bool monotone>
struct ViaCHT
{
  using T = conditional_t<direct, X, ll>;
  template <class Compare>
  using Hull = conditional_t<monotone,
                             ConvexHullTrickMonotoneSlope<T, Compare, INF, X>,
                             ConvexHullTrick<T, Compare, INF, X>>;

  Hull<less<>> lo;
  Hull<greater<>> hi;
  int fst = -1, mna = -1, mxa = -1;
  ll min_a = 0, max_a = 0;

  void add(ll a, ll b, int id)
  {
    if constexpr (direct)
      lo.add_line(X(a), X(b), id), hi.add_line(X(a), X(b), id);
    else
      lo.add_line(a, b, id), hi.add_line(a, b, id);
    if (fst == -1) fst = mna = mxa = id, min_a = max_a = a;
    if (a < min_a) min_a = a, mna = id;
    if (max_a < a) max_a = a, mxa = id;
  }

  static X ratio(ll x, ll y)
  {
    if constexpr (is_rational_v<X>)
      return X(x, y);
    else
      return X(x) / X(y);
  }

  int query_id(ll x, ll y)
  {
    if (x == 0 && y == 0) return fst;
    if (y == 0) return x > 0 ? mna : mxa;
    X t = ratio(x, y);
    return y > 0 ? lo.query_with_line(t).second.id : hi.query_with_line(t).second.id;
  }
};

struct Result
{
  double add_ms, query_ms;
  i128 checksum;
};

template <class DS>
Result run(const vc<pair<ll, ll>> &forms, const vc<Query> &queries)
{
  auto begin = chrono::steady_clock::now();
  DS ds;
  repi(i, forms.size()) ds.add(forms[i].first, forms[i].second, i);
  auto built = chrono::steady_clock::now();
  i128 checksum = 0;
  for (auto [x, y] : queries)
  {
    int id = ds.query_id(x, y);
    checksum += i128(forms[id].first) * x + i128(forms[id].second) * y;
  }
  auto end = chrono::steady_clock::now();
  return {
      chrono::duration<double, milli>(built - begin).count(),
      chrono::duration<double, milli>(end - built).count(),
      checksum};
}

template <bool monotone>
void benchmark(const vc<pair<ll, ll>> &forms, const vc<Query> &queries)
{
  using R = Rational<ll>;
  vc<pair<string, Result>> results;
  results.eb("wrapper", run<Wrapper<monotone>>(forms, queries));
  results.eb("rational-all", run<ViaCHT<R, true, monotone>>(forms, queries));
  results.eb("rational-x", run<ViaCHT<R, false, monotone>>(forms, queries));
  results.eb("long-double-x", run<ViaCHT<long double, false, monotone>>(forms, queries));
  for (auto &[name, result] : results)
  {
    assert(result.checksum == results[0].second.checksum);
    cout << left << setw(16) << name << " add " << right << setw(9) << fixed << setprecision(3)
         << result.add_ms << " ms, query " << setw(9) << result.query_ms << " ms\n";
  }
}

void benchmark_case(string name, vc<pair<ll, ll>> forms, const vc<Query> &queries, mt19937_64 &rng)
{
  cout << name << '\n';
  shuffle(ALL(forms), rng);
  cout << "arbitrary coefficient order\n";
  benchmark<false>(forms, queries);
  sort(ALL(forms));
  cout << "monotone coefficient order\n";
  benchmark<true>(forms, queries);
}

int main()
{
  constexpr int p = numeric_limits<long double>::digits;
  constexpr ll bound = p >= 64 ? 1'000'000'000 : 10'000'000;
  static_assert(i128(8) * bound * bound < (i128(1) << p));

  mt19937_64 rng(123456789);
  cout << "long double digits = " << p << ", |input| <= " << bound << '\n';

  vc<pair<ll, ll>> random_forms(100'000);
  for (auto &[a, b] : random_forms)
  {
    a = ll(rng() % (2 * bound + 1)) - bound;
    b = ll(rng() % (2 * bound + 1)) - bound;
  }
  vc<Query> random_queries(500'000);
  for (auto &[x, y] : random_queries)
  {
    x = ll(rng() % (2 * bound + 1)) - bound;
    y = ll(rng() % (2 * bound + 1)) - bound;
  }
  benchmark_case("random coefficients", random_forms, random_queries, rng);

  constexpr int n = p >= 64 ? 60'000 : 6'000;
  vc<pair<ll, ll>> convex_forms;
  repi(i, n)
  {
    ll a = i - n / 2;
    convex_forms.eb(a, a * a);
  }
  vc<Query> convex_queries;
  repi(_, 500'000)
  {
    ll y = 1 + rng() % (p >= 64 ? 10'000 : 1'000);
    if (rng() & 1) y = -y;
    ll t = ll(rng() % (2 * n + 1)) - n;
    ll x = t * y + ll(rng() % (2 * abs(y) + 1)) - abs(y);
    convex_queries.eb(Query{x, y});
  }
  benchmark_case("large convex hull", convex_forms, convex_queries, rng);
}
