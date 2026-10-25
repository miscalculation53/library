#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ITP1_1_A"

#include "ds/binary_trie.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <map>
#include <type_traits>
#include <utility>
#include <vector>

/*!
 * Independently written API-only comparison/category fixtures.
 * Dedicated under CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/
 */



static void expect(bool condition) {
  if (!condition) std::abort();
}

struct R2ScalarMonoid {
  using S = long long;
  static S e() { return 0; }
  static S op(const S& a, const S& b) { return a + b; }
};

class Measure;

class Rank {
  struct Tag {};
  long long ordinal_;
  Rank(Tag, long long value) : ordinal_(value) {}

 public:
  Rank() : ordinal_(0) {}
  Rank(long long) = delete;
  Rank(const Rank&) = default;
  Rank(Rank&&) = default;
  Rank& operator=(const Rank&) = delete;
  Rank& operator=(Rank&&) = delete;

  static Rank at(long long value) { return Rank(Tag{}, value); }

  friend bool operator<=(const Rank& a, const Rank& b) {
    return a.ordinal_ <= b.ordinal_;
  }
  friend bool operator<(const Rank&, const Measure&);
  friend bool operator<=(const Measure&, const Rank&);
};

class Measure {
  struct Tag {};
  long long cardinality_;
  Measure(Tag, long long value) : cardinality_(value) {}

 public:
  Measure() = delete;
  Measure(long long) = delete;
  Measure(const Measure&) = default;
  Measure(Measure&&) = default;
  Measure& operator=(const Measure&) = delete;
  Measure& operator=(Measure&&) = delete;

  static Measure of(long long value) { return Measure(Tag{}, value); }

  friend bool operator<(const Rank&, const Measure&);
  friend bool operator<=(const Measure&, const Rank&);
};

bool operator<(const Rank& rank, const Measure& measure) {
  return rank.ordinal_ < measure.cardinality_;
}

bool operator<=(const Measure& measure, const Rank& rank) {
  return measure.cardinality_ <= rank.ordinal_;
}

class ZeroCount {
  struct Tag {};
  long long cardinality_;
  ZeroCount(Tag, long long value) : cardinality_(value) {}

 public:
  ZeroCount() : cardinality_(0) {}
  ZeroCount(long long) = delete;
  ZeroCount(const ZeroCount&) = default;
  ZeroCount(ZeroCount&&) = default;
  ZeroCount& operator=(const ZeroCount&) = delete;
  ZeroCount& operator=(ZeroCount&&) = delete;

  static ZeroCount of(long long value) { return ZeroCount(Tag{}, value); }

  friend bool operator==(const ZeroCount& a, const ZeroCount& b) {
    return a.cardinality_ == b.cardinality_;
  }
  friend bool operator!=(const ZeroCount& a, const ZeroCount& b) {
    return a.cardinality_ != b.cardinality_;
  }
};

struct FullMeasureCounter {
  Measure operator()(long long& value) const { return Measure::of(value); }
  Measure operator()(long long&& value) const { return Measure::of(value); }
  Measure operator()(const long long&) const = delete;
};

struct ZeroCounter {
  ZeroCount operator()(const long long& value) const { return ZeroCount::of(value); }
};

static_assert(!std::is_default_constructible_v<Measure>);
static_assert(!std::is_constructible_v<Measure, int>);
static_assert(!std::is_convertible_v<Measure, long long>);
static_assert(!std::is_assignable_v<Measure&, Measure>);
static_assert(std::is_default_constructible_v<Rank>);
static_assert(!std::is_constructible_v<Rank, int>);
static_assert(!std::is_convertible_v<Rank, long long>);
static_assert(!std::is_assignable_v<Rank&, Rank>);
static_assert(std::is_default_constructible_v<ZeroCount>);
static_assert(!std::is_constructible_v<ZeroCount, int>);
static_assert(!std::is_convertible_v<ZeroCount, long long>);
static_assert(!std::is_assignable_v<ZeroCount&, ZeroCount>);



static void compare_only_rank_and_zero_count() {
  using Trie = BinaryTrie<5, R2ScalarMonoid, int>;
  Trie trie;
  std::array<long long, 32> values{};
  trie.set(1, 2);
  values[1] = 2;
  trie.set(6, 0);
  trie.set(29, 3);
  values[29] = 3;
  const FullMeasureCounter measure;
  const ZeroCounter count;
  expect(trie.content() == std::map<int, long long>{{1, 2}, {6, 0}, {29, 3}});

  for (int x : {0, 7, 31}) {
    std::vector<int> expanded;
    for (int q = 0; q < 32; ++q) {
      const int key = q ^ x;
      for (long long k = 0; k < values[key]; ++k) expanded.push_back(key);
    }
    for (long long k = 0; k < 5; ++k) {
      expect(trie.kth_by(Rank::at(k), measure, x) == expanded[k]);
    }
    expect(trie.min_element_by(count, x) == expanded.front());
    expect(trie.max_element_by(count, x) == expanded.back());
    for (int a = 0; a < 32; ++a) {
      int lt = -1, leq = -1, geq = -1, gt = -1;
      for (int q = 0; q < 32; ++q) {
        const int key = q ^ x;
        if (values[key] == 0) continue;
        if (q < a) lt = key;
        if (q <= a) leq = key;
        if (q >= a && geq == -1) geq = key;
        if (q > a && gt == -1) gt = key;
      }
      expect(trie.lt_max_by(a, count, x).value_or(-1) == lt);
      expect(trie.leq_max_by(a, count, x).value_or(-1) == leq);
      expect(trie.geq_min_by(a, count, x).value_or(-1) == geq);
      expect(trie.gt_min_by(a, count, x).value_or(-1) == gt);
      for (long long limit = 0; limit <= 5; ++limit) {
        const auto f = [limit](const long long& value) { return value <= limit; };
        long long sum = 0;
        int right_ng = 32, right_ok = -1;
        for (int q = a; q < 32; ++q) {
          const int key = q ^ x;
          if (sum + values[key] > limit) { right_ng = key; break; }
          sum += values[key];
          if (values[key] != 0) right_ok = key;
        }
        sum = 0;
        int left_ng = -1, left_ok = 32;
        for (int q = a; q >= 0; --q) {
          const int key = q ^ x;
          if (sum + values[key] > limit) { left_ng = key; break; }
          sum += values[key];
          if (values[key] != 0) left_ok = key;
        }
        expect(trie.max_right_ok_by(a, f, count, x).value_or(-1) == right_ok);
        expect(trie.min_left_ok_by(a, f, count, x).value_or(32) == left_ok);
        expect(trie.max_right_ng(a, f, x) == right_ng);
        expect(trie.min_left_ng(a, f, x) == left_ng);
      }
    }
  }
  trie.clear();
  const auto always = [](const long long&) { return true; };
  for (int a = 0; a < 32; ++a) {
    expect(trie.lt_max_by(a, count).value_or(-1) == -1);
    expect(trie.leq_max_by(a, count).value_or(-1) == -1);
    expect(trie.geq_min_by(a, count).value_or(-1) == -1);
    expect(trie.gt_min_by(a, count).value_or(-1) == -1);
    expect(!trie.max_right_ok_by(a, always, count));
    expect(!trie.min_left_ok_by(a, always, count));
  }
}

template<int Width, class Key>
static void volatile_key_api() {
  using Trie = BinaryTrie<Width, R2ScalarMonoid, Key>;
  using Plain = std::remove_cv_t<Key>;
  using End = std::remove_cv_t<typename Trie::Boundary>;
  const Plain maximum = static_cast<Plain>((End(1) << Width) - 1);
  Trie trie;
  const FullMeasureCounter measure;
  const ZeroCounter count;
  const auto f = [](const long long& value) { return value <= 3; };
  trie.set(Plain(0), 0);
  trie.set(Plain(1), 2);
  trie.add(maximum, 3);
  trie.modify(Plain(1), [](long long& value) { ++value; });
  trie.reserve(128);
  expect(trie.get(Plain(1)) == 3);
  expect(trie.all_sum() == 6);
  expect(trie.lt_sum(maximum) == 3);
  expect(trie.leq_sum(maximum) == 6);
  expect(trie.geq_sum(Plain(1)) == 6);
  expect(trie.gt_sum(Plain(1)) == 3);
  expect(trie.kth_by(Rank::at(0), measure) == 1);
  expect(trie.min_element_by(count) == 1);
  expect(trie.max_element_by(count) == maximum);
  expect(trie.lt_max_by(maximum, count).value_or(-1) == 1);
  expect(trie.leq_max_by(maximum, count).value_or(-1) == maximum);
  expect(trie.geq_min_by(Plain(1), count).value_or(-1) == 1);
  expect(trie.gt_min_by(Plain(1), count).value_or(-1) == maximum);
  expect(trie.max_right_ng(Plain(0), f) == maximum);
  expect(trie.max_right_ok_by(Plain(0), f, count) == 1);
  expect(trie.min_left_ng(maximum, f) == 1);
  expect(trie.min_left_ok_by(maximum, f, count) == maximum);

  const auto normalized_content = [&]() {
    std::map<Plain, long long> result;
    for (const auto& entry : trie.content()) {
      result.emplace(static_cast<Plain>(entry.first), entry.second);
    }
    return result;
  };
  expect(normalized_content() ==
         std::map<Plain, long long>{{0, 0}, {1, 3}, {maximum, 3}});
  trie.xor_all(maximum);
  expect(normalized_content() ==
         std::map<Plain, long long>{{0, 3}, {Plain(maximum ^ 1), 3}, {maximum, 0}});
  expect(trie.get(Plain(maximum ^ 1)) == 3);
  expect(trie.kth_by(Rank::at(0), measure, maximum) == (maximum ^ 1));
  expect(trie.max_right_ng(Plain(0), f, maximum) == 0);
  expect(trie.max_right_ok_by(Plain(0), f, count, maximum) == (maximum ^ 1));
  expect(trie.min_left_ng(maximum, f, maximum) == (maximum ^ 1));
  expect(trie.min_left_ok_by(maximum, f, count, maximum) == 0);
  trie.clear();
  expect(trie.content().empty());
}

template<class Key>
static void extended_volatile_if_admitted() {
  if constexpr (std::is_integral_v<Key> && std::is_signed_v<Key>) {
    volatile_key_api<64, Key>();
    volatile_key_api<126, Key>();
  } else {
  }
}

int main() {
  compare_only_rank_and_zero_count();
  volatile_key_api<5, volatile int>();
  extended_volatile_if_admitted<volatile __int128>();
  std::cout << "Hello World\n";
}
