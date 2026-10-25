#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/modint/binomial_prefix_sum.hpp"
#include "ds/mo/mo.hpp"

// Pascal の三角形を加算だけで作り、各行の累積和を期待値とする。
template <class mint>
vvc<mint> naive_prefix(int n)
{
  vvc<mint> a(n + 1, vc<mint>(n + 1));
  a[0][0] = 1;
  for (int i = 1; i <= n; ++i)
  {
    a[i][0] = 1;
    for (int j = 1; j <= n; ++j)
      a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
  }
  for (auto &row : a)
    for (int j = 1; j <= n; ++j) row[j] += row[j - 1];
  return a;
}

template <class mint, int B = 512>
void check_online(const vvc<mint> &expected, bool exhaustive = true)
{
  const int n = int(expected.size()) - 1;
  const auto get = [&](int x, int k) -> mint
  {
    return k < 0 ? mint(0) : expected[x][min(k, n)];
  };
  BinomialPrefixSum<mint, B> online;
  if (exhaustive)
    for (int x = 0; x <= n; ++x)
    {
      // n を 1 ずつ増やし、同じブロック内と境界を越える自動拡張を検証する。
      for (int k = -2; k <= n + 2; ++k)
        assert(online.sum(x, k) == get(x, k));
      assert(online.sum(x, numeric_limits<int>::min()) == 0);
      assert(online.sum(x, numeric_limits<int>::max()) == expected[x][n]);
    }

  // 部分的な前計算から拡張し、計算済み範囲への reserve も挟む。
  BinomialPrefixSum<mint, B> reserved(n / 3);
  reserved.reserve(0);
  reserved.reserve(n / 2);
  reserved.reserve(n / 2);
  mt19937 rng(982341);
  mint last = 0;
  for (int t = 0; t < 3000; ++t)
  {
    const int x = (rng() + last.val()) % (n + 1), k = int(rng() % (n + 5)) - 2;
    last = online.sum(x, k);
    assert(last == get(x, k));
    assert(reserved.sum(x, k) == last);
  }
  reserved.reserve(n);
  for (int x : {0, max(0, B - 1), B, B + 1, 2 * B - 1, 2 * B, n})
    if (x <= n)
      for (int k = -1; k <= n + 1; ++k)
        assert(reserved.sum(x, k) == get(x, k));
}

template <class mint>
void check(int n)
{
  const auto expected = naive_prefix<mint>(n);
  const auto get = [&](int x, int k) -> mint
  {
    return k < 0 ? mint(0) : expected[x][min(k, n)];
  };
  BinomialPrefixSumSlider<mint> slider;
  const auto check_slider = [&]() { assert(slider.sum == get(slider.l, slider.r)); };
  check_slider();

  // 事前の reserve がなくても、全方向の移動で境界を越えられる。
  slider.rmm();
  check_slider();
  for (int i = 0; i < n; ++i) slider.lpp(), check_slider();
  for (int i = -1; i < n + 3; ++i) slider.rpp(), check_slider();
  for (int i = 0; i < n; ++i) slider.lmm(), check_slider();
  for (int i = n + 3; i >= 0; --i) slider.rmm(), check_slider();

  mt19937 rng(712367);
  for (int t = 0; t < 10000; ++t)
  {
    const int x = rng() % (n + 1), k = int(rng() % (n + 5)) - 1;
    slider.set(x, k);
    assert(slider.l == x && slider.r == k);
    check_slider();
    if (x < n) slider.lpp(), check_slider();
    if (slider.l > 0) slider.lmm(), check_slider();
    slider.rpp();
    check_slider();
    slider.rmm();
    check_slider();
  }

  check_online<mint, 1>(expected);
  check_online<mint, 2>(expected);
  check_online<mint, 3>(expected);
  check_online<mint, 7>(expected);
  check_online<mint, 16>(expected);
  check_online<mint>(expected);

  vc<pair<int, int>> queries;
  for (int x = 0; x <= n; ++x)
    for (int k = 0; k <= n; ++k) queries.emplace_back(x, k);
  shuffle(queries.begin(), queries.end(), rng);
  vc<int> visited(queries.size());
  // 初期位置が (0,0) 以外の slider も渡せる。
  slider.set(n, n + 2);
  mo(n, queries, slider, [&](int q)
  {
    const auto [x, k] = queries[q];
    assert(slider.l == x && slider.r == k && slider.sum == get(x, k));
    ++visited[q];
  });
  for (int count : visited) assert(count == 1);
  mo(n, vc<pair<int, int>>{}, slider, [](int) { assert(false); });
}

int main()
{
  // 小さい素数では n = p-1 まで検証する。
  check<static_modint32<3>>(2);
  check<static_modint32<7>>(6);
  check<static_modint32<17>>(16);
  for (int n : {0, 1, 2, 7, 8, 9, 31, 32, 33, 127, 240})
    check<modint998244353>(n);
  check<modint1000000007>(100);
  check<modint61>(40);
  check_online<modint998244353>(naive_prefix<modint998244353>(1031), false);
  using dmint = dynamic_modint32<876123>;
  for (int p : {101, 17, 257, 101})
  {
    dmint::set_mod(p);
    check<dmint>(min(p - 1, 100));
  }
  PRINT("Hello World");
}
