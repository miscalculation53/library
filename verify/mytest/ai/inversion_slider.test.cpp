#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/fenwick_tree/inversion.hpp"

template <class T>
void check(const vc<T> &a)
{
  const int n = a.size();
  InversionSlider slider(a);
  static_assert(is_same_v<decltype(slider), InversionSlider<T>>);
  assert(slider.n == n && slider.l == 0 && slider.r == 0 && slider.inversion_num == 0);
  vvc<ll> expected(n + 1, vl(n + 1));
  vc<pair<int, int>> ranges;
  repi(l, n + 1) repi(r, l, n + 1)
  {
    repi(i, l, r) repi(j, i + 1, r) expected[l][r] += a[j] < a[i];
    ranges.emplace_back(l, r);
  }
  auto verify = [&](int l, int r)
  {
    assert(slider.l == l && slider.r == r);
    assert(slider.inversion_num == expected[l][r]);
  };
  for (auto [l, r] : ranges)
  {
    // 離れた区間や空区間も含め、すべての区間間の移動を確認する。
    for (auto [nl, nr] : ranges)
    {
      slider.set(l, r);
      verify(l, r);
      slider.set(nl, nr);
      verify(nl, nr);
    }
    slider.set(l, r);
    if (l < r)
    {
      slider.lpp(); verify(l + 1, r);
      slider.lmm(); verify(l, r);
      slider.rmm(); verify(l, r - 1);
      slider.rpp(); verify(l, r);
    }
    if (l > 0)
    {
      slider.lmm(); verify(l - 1, r);
      slider.lpp(); verify(l, r);
    }
    if (r < n)
    {
      slider.rpp(); verify(l, r + 1);
      slider.rmm(); verify(l, r);
    }
  }
}

struct LessOnly
{
  int value;
  bool operator<(const LessOnly &other) const { return value < other.value; }
};

int main()
{
  InversionSlider empty;
  empty.set(0, 0);
  assert(empty.n == 0 && empty.l == 0 && empty.r == 0 && empty.inversion_num == 0);

  // {-1, 0, 1} からなる長さ 5 以下の全列。
  int count = 1;
  for (int n = 0; n <= 5; ++n, count *= 3)
    repi(mask, count)
    {
      vc<int> a(n);
      int code = mask;
      for (auto &x : a) x = code % 3 - 1, code /= 3;
      check(a);
    }
  check<ll>({LLONG_MAX, LLONG_MIN, 0, LLONG_MAX, -1, LLONG_MIN});
  check<ull>({ULLONG_MAX, 0, ULLONG_MAX, 1, 0});
  check<string>({"pear", "apple", "pear", "", "banana"});
  check<pair<int, int>>({{1, 2}, {1, 1}, {-1, 3}, {1, 2}});
  check<LessOnly>({{2}, {1}, {2}, {-1}, {1}});

  const int n = 100000;
  vc<int> descending(n);
  repi(i, n) descending[i] = n - i;
  InversionSlider large(descending);
  large.set(0, n);
  assert(large.inversion_num == ll(n) * (n - 1) / 2);
  large.set(1, n - 1);
  assert(large.inversion_num == ll(n - 2) * (n - 3) / 2);
  large.set(n, n);
  assert(large.inversion_num == 0);

  InversionSlider equal(vc<ll>(n, LLONG_MAX));
  equal.set(0, n);
  assert(equal.inversion_num == 0);
  equal.set(n / 2, n / 2);
  assert(equal.inversion_num == 0);
  PRINT("Hello World");
}
