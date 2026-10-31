#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/dynamic_bitset.hpp"

// Test focus: ascending enumeration agrees with reading individual bits;
// borrowed and owning ranges preserve their source and lifetime contracts.
using BitRange = decltype(declval<const DynamicBitset &>().ones());
using BitIterator = decltype(declval<const BitRange &>().begin());
static_assert(is_same_v<typename iterator_traits<BitIterator>::value_type, int>);
static_assert(is_same_v<decltype(*declval<BitIterator>()), int>);
#if __cplusplus >= 202002L
static_assert(input_iterator<BitIterator>);
static_assert(ranges::input_range<BitRange>);
static_assert(!ranges::sized_range<BitRange>);
static_assert(!ranges::range<DynamicBitset>);
#endif

template <class Range> vc<int> collect(const Range &range)
{
  return vc<int>(range.begin(), range.end());
}

void verify_ones(const DynamicBitset &bits)
{
  vc<int> expected;
  repi(i, bits.size()) if (bits[i]) expected.eb(i);
  const auto range = bits.ones();
  assert(collect(range) == expected);
  assert(vc<int>(range.cbegin(), range.cend()) == expected);
  assert(distance(range.begin(), range.end()) == ll(expected.size()));
  assert(accumulate(range.begin(), range.end(), 0LL) == accumulate(ALL(expected), 0LL));
  assert(bits.count() == int(expected.size()));
#if __cplusplus >= 202002L
  assert(ranges::distance(range) == ll(expected.size()));
  assert(ranges::equal(range, expected));
#endif
  auto it = range.begin();
  for (int p : expected)
  {
    assert(it != range.end());
    auto old = it++;
    assert(*old == p);
    auto copied = old;
    assert(&++copied == &copied);
    assert(copied == it);
  }
  assert(it == range.end());
  assert(range.begin() == bits.ones().begin());
  assert(collect(bits.ones()) == expected);
}

void verify_clear_current(DynamicBitset bits)
{
  vc<int> expected, visited;
  repi(i, bits.size()) if (bits[i]) expected.eb(i);
  const int n = bits.size();
  auto remaining = bits;
  fe(i : bits.ones())
  {
    assert(bits[i]);
    visited.eb(i);
    bits[i] = 0;
  }
  assert(visited == expected);
  assert(bits.size() == n && bits.none());

  visited.clear();
  int ordinal = 0;
  for (int i : remaining.ones())
  {
    assert(remaining[i]);
    visited.eb(i);
    if (ordinal++ % 2 == 0) remaining.reset(i);
  }
  assert(visited == expected);
  repi(i, expected.size()) assert(remaining[expected[i]] == bool(i % 2));
  assert(remaining.count() == int(expected.size() / 2));
}

int main()
{
  BitIterator default_a, default_b;
  assert(default_a == default_b);
  mt19937_64 rng(20261031);
  for (int n : {0, 1, 2, 7, 31, 32, 33, 63, 64, 65, 127, 128, 129,
                191, 192, 193, 255, 256, 257, 1000, 100003})
  {
    repi(pattern, 6)
    {
      DynamicBitset bits(n);
      repi(i, n)
      {
        bool value = pattern == 1 || (pattern == 2 && i % 2 == 0)
                   || (pattern == 3 && (i == 0 || i == n - 1))
                   || (pattern == 4 && rng() % 3 == 0)
                   || (pattern == 5 && i % 251 == 0);
        bits.set(i, value);
      }
      verify_ones(bits);
      verify_clear_current(bits);
      vc<int> expected = collect(bits.ones());
      auto borrowed = bits.ones();
      auto borrowed_copy = borrowed;
      assert(collect(borrowed_copy) == expected);
      assert(collect(std::move(borrowed)) == expected);

      DynamicBitset source = bits;
      auto owned = std::move(source).ones();
      assert(source.empty() && source.count() == 0);
      source.resize(n, true);
      assert(collect(owned) == expected);
      auto owned_copy = owned;
      auto owned_move = std::move(owned);
      assert(collect(owned_copy) == expected);
      assert(collect(owned_move) == expected);
      owned_copy = owned_move;
      assert(collect(owned_copy) == expected);

      auto from_const = [&]()
      {
        const DynamicBitset temporary = bits;
        return std::move(temporary).ones();
      }();
      assert(collect(from_const) == expected);
      vc<int> intersection;
      for (int i : (bits & bits).ones()) intersection.eb(i);
      assert(intersection == expected);
      verify_ones(bits);
    }
  }

  repi(n, 11) repi(mask, 1 << n)
  {
    DynamicBitset bits(n);
    repi(i, n) bits[i] = (mask >> i) & 1;
    verify_ones(bits);
    verify_clear_current(bits);
  }

  DynamicBitset bits;
  auto borrowed = bits.ones();
  repi(step, 3000)
  {
    int n = rng() % 514;
    bits.resize(n, rng() & 1);
    if (n) bits.flip(rng() % n);
    bits.rotate_left(int(rng() % 1025) - 512);
    verify_ones(bits);
    verify_clear_current(bits);
    assert(collect(borrowed) == collect(bits.ones()));
  }
  DynamicBitset padded(65);
  padded.set_word(1, ~0ULL);
  assert(collect(padded.ones()) == vc<int>{64});
  padded.resize(130, true);
  verify_ones(padded);
  assert((collect(DynamicBitset("10010").ones()) == vc<int>{1, 4}));
  cout << "Hello World" << endl;
}
