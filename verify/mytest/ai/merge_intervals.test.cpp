#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algo/merge_intervals.hpp"

void test_examples()
{
  using Intervals = vc<pair<int, int>>;
  assert(merge_intervals(Intervals{}).empty());
  assert(merge_intervals(Intervals{{0, 0}, {-3, -3}, {4, 4}, {5, -5}}).empty());
  assert(merge_intervals(Intervals{{2, 5}}) == (Intervals{{2, 5}}));
  assert(merge_intervals(Intervals{{5, 8}, {1, 3}, {2, 4}, {4, 5}, {10, 12}, {9, 9}})
         == (Intervals{{1, 8}, {10, 12}}));
  assert(merge_intervals(Intervals{{2, 3}, {1, 10}, {1, 10}, {4, 7}, {10, 10}})
         == (Intervals{{1, 10}}));
  assert(merge_intervals(Intervals{{5, 6}, {3, 4}, {1, 2}, {2, 2}, {3, 3}, {6, 0}})
         == (Intervals{{1, 2}, {3, 4}, {5, 6}}));
}

void test_endpoint_types()
{
  const ll lo = numeric_limits<ll>::min(), hi = numeric_limits<ll>::max();
  const vc<pair<ll, ll>> a = {{0, hi}, {lo, 0}, {hi, hi}, {lo, lo}};
  assert(merge_intervals(a) == (vc<pair<ll, ll>>{{lo, hi}}));
  const ull umax = numeric_limits<ull>::max();
  assert(merge_intervals(vc<pair<ull, ull>>{{umax - 1, umax}, {0, 1}})
         == (vc<pair<ull, ull>>{{0, 1}, {umax - 1, umax}}));
  const i128 big = i128(1) << 100;
  assert(merge_intervals(vc<pair<i128, i128>>{{0, big}, {-big, 0}})
         == (vc<pair<i128, i128>>{{-big, big}}));
  assert(merge_intervals(vc<pair<double, double>>{{1.5, 2.5}, {-0.5, 1.5}, {3.0, 3.25}})
         == (vc<pair<double, double>>{{-0.5, 2.5}, {3.0, 3.25}}));
}

// 単位区間ごとの被覆を愚直に求め、和集合と区間数の最小性を確かめる。
void test_random()
{
  mt19937 rng(123456789);
  repi(_, 2000)
  {
    vc<pair<int, int>> intervals;
    vc<bool> covered(20);
    const int n = rng() % 40;
    repi(i, n)
    {
      int l = int(rng() % 21) - 10, r = int(rng() % 21) - 10;
      intervals.eb(l, r);
      for (int x = l; x < r; x++)
        covered[x + 10] = true;
    }
    const auto original = intervals;
    const auto merged = merge_intervals(intervals);
    assert(intervals == original);
    int components = 0;
    repi(x, 20)
      if (covered[x] && (x == 0 || !covered[x - 1]))
        components++;
    assert(SZ<int>(merged) == components);
    repi(i, merged.size())
    {
      const auto [l, r] = merged[i];
      assert(-10 <= l && l < r && r <= 10);
      if (i > 0)
        assert(merged[i - 1].second < l);
    }
    repi(x, -10, 10)
    {
      int count = 0;
      for (const auto &[l, r] : merged)
        count += l <= x && x < r;
      assert(count == int(covered[x + 10]));
    }
    assert(merge_intervals(merged) == merged);
  }
}

int main()
{
  test_examples();
  test_endpoint_types();
  test_random();
  PRINT("Hello World");
}
