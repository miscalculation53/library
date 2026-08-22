#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "algo/subset_sum.hpp"

// Test focus: frequency input is not expanded, and both reconstruction formats
// agree with brute force, including negative elements in the original array.
void test_frequency()
{
  mt19937 rng(123456789);
  repi(_, 500)
  {
    int vmax = rng() % 10 + 1, s = rng() % 40;
    vc<int> freq(vmax + 1);
    repi(v, 1, vmax + 1) freq[v] = rng() % 6;
    SubsetSumFromFrequency ss(freq, s);

    vc<bool> dp(s + 1);
    dp[0] = true;
    repi(v, 1, vmax + 1) repi(_, freq[v]) repi(x, s, v - 1, -1)
      dp[x] = dp[x] || dp[x - v];

    repi(x, s + 1)
    {
      assert(ss.exists(x) == dp[x]);
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
    auto [ok, ans] = ss.answer(x);
    assert(ok);
    ll sum = 0;
    for (auto [v, c] : ans)
      sum += v * c;
    assert(sum == x);
  }
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
  }
}

int main()
{
  test_frequency();
  test_original_array();
  cout << "Hello World" << endl;
}
