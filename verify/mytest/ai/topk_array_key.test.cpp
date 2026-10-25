#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/topk_array.hpp"

using Candidate = pair<ll, int>;
struct SameColor
{
  bool operator()(const Candidate &a, const Candidate &b) const { return a.second == b.second; }
};

template <int k, class Compare>
void test_candidates()
{
  using M = TopKArrayMonoid<Candidate, k, Compare, SameColor>;
  using S = typename M::S;
  mt19937 rng(20260908 + k);
  repi(tc, 1000)
  {
    S a, b, c;
    map<int, Candidate> all;
    repi(i, 30)
    {
      Candidate x = {int(rng() % 21) - 10, int(rng() % 9)};
      S &cur = i % 3 == 0 ? a : i % 3 == 1 ? b : c;
      cur.add(x);
      if (!all.count(x.second) || Compare()(x, all[x.second])) all[x.second] = x;
    }
    vc<Candidate> expected;
    for (auto [color, val] : all) expected.eb(val);
    sort(ALL(expected), Compare());
    if (int(expected.size()) > k) expected.resize(k);
    const S abc = M::op(M::op(a, b), c);
    assert(abc.content() == expected);
    assert(abc.content() == M::op(a, M::op(b, c)).content());
    assert(M::op(a, b).content() == M::op(b, a).content());
    assert(M::op(a, a).content() == a.content());
    assert(M::op(a, M::e()).content() == a.content());
    for (const auto &x : a.content()) assert(!a.add(x));
  }
}

// dp[i][j][color] の「最後の色が異なる」遷移を上位 2 色に圧縮し、全部分列と比較。
void test_dp()
{
  using Best = TopKArray<Candidate, 2, greater<Candidate>, SameColor>;
  mt19937 rng(123456);
  repi(tc, 800)
  {
    int n = 1 + rng() % 9, k = rng() % n;
    vc<Candidate> balls(n);
    for (auto &[v, c] : balls) v = int(rng() % 21) - 10, c = rng() % 4;
    vc<Best> dp(k + 1);
    dp[0].add({0, -1});
    for (auto [value, color] : balls)
    {
      vc<Best> next(k + 1);
      repi(j, k + 1)
      {
        if (j < k) next[j + 1].merge(dp[j]);
        repi(r, dp[j].size())
        {
          auto [score, last] = dp[j].get(r);
          if (last != color) next[j].add({score + value, color});
        }
      }
      dp.swap(next);
    }
    ll expected = -INF;
    repi(mask, 1 << n) if (__builtin_popcount(unsigned(mask)) == n - k)
    {
      ll sum = 0;
      int last = -1;
      bool ok = true;
      repi(i, n) if (mask >> i & 1)
      {
        auto [value, color] = balls[i];
        if (color == last) ok = false;
        sum += value, last = color;
      }
      if (ok) chmax(expected, sum);
    }
    assert((dp[k].size() ? dp[k].get(0).first : -INF) == expected);
  }
}

int main()
{
  test_candidates<1, less<Candidate>>();
  test_candidates<2, less<Candidate>>();
  test_candidates<5, less<Candidate>>();
  test_candidates<2, greater<Candidate>>();
  test_dp();
  PRINT("Hello World");
}
