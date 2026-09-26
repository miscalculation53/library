#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/sqrt_decomposition/sqrt_decomposition.hpp"

template <int B>
void check_ranges(int n)
{
  for (int l = 0; l <= n; l++)
    for (int r = l; r <= n; r++)
    {
      int next = l;
      auto point = [&](int i)
      {
        assert(i == next++);
        // 全体が区間に含まれるブロックは block に渡す。
        const int a = i / B * B, b = min(n, a + B);
        assert(a < l || r < b);
      };
      auto block = [&](int b)
      {
        assert(b * B == next);
        next = min(n, (b + 1) * B);
        assert(next <= r);
      };
      sqrt_decomposition<B>(n, l, r, point, block);
      assert(next == r);
    }
}

// コピーできない、状態を持つ callable も参照で呼び出す。
struct Counter
{
  int count = 0;
  Counter() = default;
  Counter(const Counter &) = delete;
  void operator()(int) { count++; }
};

int main()
{
  for (int n = 0; n <= 40; n++)
  {
    check_ranges<1>(n);
    check_ranges<2>(n);
    check_ranges<3>(n);
    check_ranges<7>(n);
    check_ranges<16>(n);
    check_ranges<512>(n);
  }
  check_ranges<512>(517);
  Counter point, block;
  sqrt_decomposition<4>(15, 1, 15, point, block);
  assert(point.count == 3 && block.count == 3);
  sqrt_decomposition<4>(0, 0, 0, Counter{}, Counter{});
  int called = 0;
  sqrt_decomposition(513, 0, 513,
      [p = make_unique<int>(0)](int) mutable { assert(false); (*p)++; },
      [&](int b) { assert(b == called++); });
  assert(called == 2);
  // 配列を持たず、大きな添字の区間を少ない呼び出しで処理する。
  constexpr int large_n = numeric_limits<int>::max(), large_b = large_n / 2 + 1;
  called = 0;
  sqrt_decomposition<large_b>(large_n, 0, large_n,
      [](int) { assert(false); }, [&](int b) { assert(b == called++); });
  assert(called == 2);
  int next = large_n - 3;
  sqrt_decomposition<large_b>(large_n, next, large_n,
      [&](int i) { assert(i == next++); }, [](int) { assert(false); });
  assert(next == large_n);

  // 演算順に意味がある例：文字列の区間を復元する。
  string a = "abcdefghijklmn", result;
  vc<string> blocks{"abcd", "efgh", "ijkl", "mn"};
  sqrt_decomposition<4>(a.size(), 1, a.size(),
      [&](int i) { result += a[i]; },
      [&](int b) { result += blocks[b]; });
  assert(result == a.substr(1));

  // 区間加算：各点の値とブロックの遅延加算を利用者側で管理する。
  constexpr int n = 31, B = 7;
  vc<ll> values(n), lazy((n + B - 1) / B), expected(n);
  mt19937 rng(20260926);
  repi(q, 1000)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    ll x = int(rng() % 21) - 10;
    sqrt_decomposition<B>(n, l, r,
        [&](int i) { values[i] += x; },
        [&](int b) { lazy[b] += x; });
    repi(i, l, r) expected[i] += x;
    repi(i, n) assert(values[i] + lazy[i / B] == expected[i]);
  }
  PRINT("Hello World");
}
