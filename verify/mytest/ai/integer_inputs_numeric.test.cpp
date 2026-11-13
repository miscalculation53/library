#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/convolution/convolution_ll.hpp"
#include "math/convolution/convolution64.hpp"
#include "math/prime/sieve/euler_product_prefix_sum.hpp"
#include "math/linalg/linear_transpose_cpp.hpp"

template <class I>
void numeric_cases()
{
  const vc<I> a{1, 2, 3};
  assert(convolution_4e18(a, vc<unsigned>{4, 5}) == vc<ll>({4, 13, 22, 15}));
  assert(convolution64(a, vc<unsigned>{4, 5}) == vc<ull>({4, 13, 22, 15}));
  assert(convolution_4e18(a, {}) == vc<ll>{});
  assert(convolution64({}, a) == vc<ull>{});
  assert(convolution(a, a) == vc<I>({1, 4, 10, 12, 9}));
  using R = RingAddSubMul<ll>;
  const vvc<pair<I, ll>> factors{{{2, 1}}, {{3, 2}, {4, 1}}};
  const auto actual = euler_product_prefix_sum<R>(12, factors);
  const auto expected = euler_product_prefix_sum<R>(12, {{{2, 1}}, {{3, 2}, {4, 1}}});
  for (int x = 0; x <= 12; x++) if (actual.contains(x)) assert(actual.F(x) == expected.F(x));
}

int main()
{
  numeric_cases<char>();
  numeric_cases<signed char>();
  numeric_cases<unsigned char>();
  numeric_cases<short>();
  numeric_cases<unsigned short>();
  numeric_cases<int>();
  numeric_cases<unsigned>();
  numeric_cases<long>();
  numeric_cases<unsigned long>();
  numeric_cases<ll>();
  numeric_cases<ull>();
  numeric_cases<i128>();
  numeric_cases<u128>();
  numeric_cases<wchar_t>();
  numeric_cases<char16_t>();
  numeric_cases<char32_t>();
#ifdef __cpp_char8_t
  numeric_cases<char8_t>();
#endif
  assert(convolution_4e18(vc<bool>{true, false, true}, vc<bool>{true, false, true}) == vc<ll>({1, 0, 2, 0, 1}));
  assert(convolution64(vc<bool>{true, false, true}, vc<bool>{true, false, true}) == vc<ull>({1, 0, 2, 0, 1}));
  assert(convolution_4e18({}, {}) == vc<ll>{});
  assert(convolution64({}, {}) == vc<ull>{});
  assert(convolution64(vc<i128>{-2, i128(1) << 100}, vc<int>{3}) == vc<ull>({ull(-6), 0}));
  using R = RingAddSubMul<ll>;
  const u128 huge = u128(1) << 100;
  auto product = euler_product_prefix_sum<R>(12, vvc<pair<u128, ll>>{{{huge, 7}, {2, 1}}});
  assert(product.F(12) == 2);
  auto program = linear_transpose::record<i128>(2, [](const auto &x)
  {
    using V = typename decay_t<decltype(x)>::value_type;
    return vc<V>{x[0] * i128(2) + x[1] * i128(3)};
  });
  assert(program.transpose(vc<i128>{4}) == vc<i128>({8, 12}));
  cout << "Hello World\n";
}
