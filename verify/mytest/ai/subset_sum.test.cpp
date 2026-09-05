#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "algo/subset_sum.hpp"

// Test focus: frequency input is not expanded, the reachable range is capped by
// the usable total, and both reconstruction strategies agree with brute force.
void test_frequency()
{
  mt19937 rng(123456789);
  repi(_, 500)
  {
    int vmax = rng() % 10 + 1, s = rng() % 40;
    vc<int> freq(vmax + 1);
    repi(v, 1, vmax + 1) freq[v] = rng() % 6;
    SubsetSumFromFrequency ss(freq, s);

    int sm = 0;
    repi(v, 1, vmax + 1) if (v <= s)
      sm = min(s, sm + v * min(freq[v], s / v));
    assert(ss.reachable().size() == sm + 1);

    vc<bool> dp(s + 1);
    dp[0] = true;
    repi(v, 1, vmax + 1) repi(_, freq[v]) repi(x, s, v - 1, -1)
      dp[x] = dp[x] || dp[x - v];

    repi(x, s + 1)
    {
      assert(ss.exists(x) == dp[x]);
      if (x <= sm) assert(ss.reachable()[x] == dp[x]);
      auto [ok, ans] = ss.answer(x);
      assert(ok == dp[x]);
      if (!ok)
        continue;
      vc<int> used(vmax + 1);
      int sum = 0;
      for (auto [v, c] : ans)
      {
        assert(1 <= v && v <= vmax);
        assert(used[v] == 0);
        used[v] = c;
        sum += v * c;
      }
      assert(sum == x);
      repi(v, 1, vmax + 1) assert(0 <= used[v] && used[v] <= freq[v]);
    }
  }

  vc<ll> freq(3);
  freq[1] = freq[2] = (ll)1e18;
  SubsetSumFromFrequency ss(freq, 1000);
  repi(x, 1001)
  {
    assert(ss.exists(x));
    assert(ss.reachable()[x]);
    auto [ok, ans] = ss.answer(x);
    assert(ok);
    ll sum = 0;
    for (auto [v, c] : ans)
      sum += v * c;
    assert(sum == x);
  }

  vc<int> dense_freq(101, 1);
  dense_freq[0] = 0;
  SubsetSumFromFrequency dense(dense_freq, 1000);
  auto [ok, ans] = dense.answer(1000);
  assert(ok);
  int sum = 0;
  for (auto [v, c] : ans)
  {
    assert(c == 1);
    sum += v;
  }
  assert(sum == 1000);
}

void test_original_array()
{
  mt19937 rng(987654321);
  repi(_, 500)
  {
    int n = rng() % 13, smax = int(rng() % 31) - 10;
    vc<int> a(n);
    repi(i, n) a[i] = int(rng() % 15) - 7;
    SubsetSum ss(a, smax);

    int mn = 0;
    fec(x : a) if (x < 0) mn += x;
    vc<bool> possible(max(0, smax - mn + 1));
    rep(mask, 1 << n)
    {
      int sum = 0;
      repi(i, n) if (mask >> i & 1) sum += a[i];
      if (mn <= sum && sum <= smax)
        possible[sum - mn] = true;
    }

    repi(x, mn - 1, smax + 2)
    {
      bool expected = mn <= x && x <= smax && possible[x - mn];
      assert(ss.exists(x) == expected);
      auto [ok, ans] = ss.answer(x);
      assert(ok == expected);
      if (!ok)
        continue;
      assert(SZ(ans) == n);
      int sum = 0;
      repi(i, n) if (ans[i]) sum += a[i];
      assert(sum == x);
    }

    assert(ss.min_sum() == mn);
    if (mn <= smax)
    {
      int lim = smax - mn, sm = 0;
      vc<int> freq(lim + 1);
      fec(x : a) if (abs(x) <= lim) freq[abs(x)]++;
      repi(v, 1, lim + 1)
        sm = min(lim, sm + v * min(freq[v], lim / v));
      assert(ss.reachable().size() == sm + 1);
      repi(i, sm + 1) assert(ss.reachable()[i] == possible[i]);
    }
    else
      assert(ss.reachable().size() == 0);
  }

  SubsetSum sparse(vc<int>{200000}, 200000);
  assert(sparse.reachable().size() == 200001);
  assert(sparse.reachable().count() == 2);
  auto [ok, ans] = sparse.answer(200000);
  assert(ok && ans == vc<bool>{true});
}

int main()
{
  test_frequency();
  test_original_array();
  cout << "Hello World" << endl;
}
