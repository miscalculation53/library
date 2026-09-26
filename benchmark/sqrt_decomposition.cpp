// g++-15 -std=c++20 -O2 -Wall -Wextra -I. benchmark/sqrt_decomposition.cpp -o /tmp/sqrt-bench
#include "ds/sqrt_decomposition/range_sum.hpp"
#include "math/modint/modint.hpp"

// 変更前の実装。計測を再現するために残す。
template <class G, int B = 512>
struct SqrtBefore
{
  using S = typename G::S;
  int n;
  vc<S> dat, block;
  SqrtBefore(const vc<S> &v) : n(v.size()), dat(v)
  {
    block.resize(divceil(n, B), G::e());
    repi(bid, block.size())
    {
      const int l = bid * B, r = min(n, (bid + 1) * B);
      repi(i, l, r)
      {
        auto &b = block[bid];
        b = G::op(b, dat[i]);
      }
    }
  }
  void add(int p, const S &x)
  {
    const int bid = p / B;
    auto &b = block[bid], &d = dat[p];
    b = G::op(b, x);
    d = G::op(d, x);
  }
  S sum(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    S sm = G::e();
    int bl = divceil(l, B), br = divfloor(r, B);
    if (bl >= br)
      repi(i, l, r) sm = G::op(sm, dat[i]);
    else
    {
      repi(i, l, bl * B) sm = G::op(sm, dat[i]);
      repi(j, bl, br) sm = G::op(sm, block[j]);
      repi(i, br * B, r) sm = G::op(sm, dat[i]);
    }
    return sm;
  }
};

struct Query { int t, l, r; };
volatile ull sqrt_bench_sink = 0;
ull digest(ll x) { return x; }
ull digest(modint998244353 x) { return x.val(); }

template <class DS, class S>
pair<double, vc<S>> run(const vc<S> &a, const vc<Query> &queries)
{
  DS ds(a);
  vc<S> answers;
  answers.reserve(queries.size());
  auto start = chrono::steady_clock::now();
  for (auto [t, l, r] : queries)
  {
    if (t == 0) ds.add(l, S(r));
    else answers.push_back(ds.sum(l, r));
  }
  double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - start).count();
  ull checksum = 0;
  for (const auto &x : answers) checksum = checksum * 1000000007 + digest(x);
  sqrt_bench_sink = checksum;
  return {ms, move(answers)};
}

template <class S, int B>
void bench(const string &type)
{
  constexpr int n = 200003, q = 100000;
  mt19937 rng(20260926);
  vc<S> a(n);
  for (auto &x : a) x = S(rng() % 1000);
  for (string kind : {"random", "prefix", "suffix", "short", "aligned", "last_block", "updates_99pct"})
  {
    vc<Query> queries;
    repi(i, q)
    {
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      if (kind == "prefix") l = 0;
      if (kind == "suffix") r = n;
      if (kind == "short") r = min(n, l + int(rng() % 32));
      if (kind == "aligned") l = l / B * B, r = r / B * B;
      if (kind == "last_block") l = n / B * B, r = n;
      if (kind == "updates_99pct" && i % 100 != 0)
        queries.push_back({0, int(rng() % n), int(rng() % 1000)});
      else
        queries.push_back({1, l, r});
    }
    using Old = SqrtBefore<GroupAddSub<S>, B>;
    using New = SqrtDecompositionRangeSum<GroupAddSub<S>, B>;
    vc<double> times[2];
    repi(round, -1, 5)
    {
      pair<double, vc<S>> results[2];
      repi(order, 2)
      {
        const int which = order ^ (round & 1);
        results[which] = which ? run<New>(a, queries) : run<Old>(a, queries);
      }
      assert(results[0].second == results[1].second);
      if (round >= 0)
        repi(which, 2) times[which].push_back(results[which].first);
    }
    for (auto &v : times) sort(v.begin(), v.end());
    cout << type << ',' << B << ',' << kind << ',' << times[0][2] << ',' << times[1][2]
         << ',' << times[0][2] / times[1][2] << endl;
  }
}

int main()
{
  cout << fixed << setprecision(3) << "type,B,case,before_ms,after_ms,speedup\n";
  bench<ll, 512>("ll");
  bench<modint998244353, 512>("modint");
  bench<ll, 1000>("ll");
  bench<modint998244353, 1000>("modint");
}
