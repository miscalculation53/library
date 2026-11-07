#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/xor_basis.hpp"
#include "math/linalg/linear_basis.hpp"
#include "math/linalg/matrix.hpp"

template <class V, bool Track = false, class Encode>
void check_sequence(int dimension, const vc<pair<int, int>> &input, const Encode &encode,
                    bool all_queries)
{
  XorBasis<V, Track> basis(dimension);
  vc<pair<V, int>> inserted;
  const V zero = encode(0);
  auto enumerate = [&](int threshold)
  {
    set<V> span{zero};
    for (const auto &[value, tag] : inserted) if (tag >= threshold)
    {
      vc<V> previous(ALL(span));
      for (const V &old : previous) span.insert(old ^ value);
    }
    return span;
  };
  auto check = [&]()
  {
    for (int threshold = -5; threshold <= 5; threshold++)
    {
      const auto span = enumerate(threshold);
      int expected_rank = 0;
      while ((1u << expected_rank) < span.size()) expected_rank++;
      assert(basis.rank(threshold) == expected_rank);
      assert(basis.template span_size<ull>(threshold) == span.size());
      vc<V> ordered(span.begin(), span.end());
      for (int k = 0; k < int(ordered.size()); k++)
        assert(basis.kth(k, zero, threshold) == optional<V>(ordered[k]));
      assert(!basis.kth(-1, zero, threshold));
      assert(!basis.kth(ull(ordered.size()), zero, threshold));
      assert(basis.min_xor(zero, threshold) == zero);
      assert(basis.max_xor(zero, threshold) == *span.rbegin());
      const auto extracted = basis.basis(threshold);
      assert(extracted.dimension() == dimension && extracted.rank() == expected_rank);
      const int limit = all_queries ? 64 : 8;
      for (int code = 0; code < limit; code++)
      {
        const V value = encode(code);
        V expected_min = value, expected_max = value;
        for (const V &s : span)
        {
          const V x = value ^ s;
          if (x < expected_min) expected_min = x;
          if (expected_max < x) expected_max = x;
        }
        vc<V> affine;
        for (const auto &x : span) affine.push_back(value ^ x);
        sort(affine.begin(), affine.end());
        if (code < 3)
          for (int k = 0; k < int(affine.size()); k++)
            assert(basis.kth(k, value, threshold) == optional<V>(affine[k]));
        const V key = encode((code * 13 + threshold + 64) & 63);
        auto at = lower_bound(affine.begin(), affine.end(), key);
        auto after = upper_bound(affine.begin(), affine.end(), key);
        assert(basis.template order_of_key<ull>(key, value, threshold) == ull(at - affine.begin()));
        assert(basis.predecessor(key, value, threshold) == (after == affine.begin() ? nullopt : optional<V>(*prev(after))));
        assert(basis.predecessor_strict(key, value, threshold) == (at == affine.begin() ? nullopt : optional<V>(*prev(at))));
        if constexpr (Track)
        {
          auto c = basis.coordinates(value, threshold);
          assert(bool(c) == bool(span.count(value)));
          if (c)
          {
            V reconstructed = zero;
            for (int i = 0; i < basis.rank(); i++)
              if (internal::XorBasisVector<V>::coefficient_test(*c, i)) reconstructed ^= basis.basis()[i];
            assert(reconstructed == value);
          }
        }
        assert(basis.reduce(value, threshold) == expected_min);
        assert(basis.min_xor(value, threshold) == expected_min);
        assert(basis.max_xor(value, threshold) == expected_max);
        assert(basis.contains(value, threshold) == bool(span.count(value)));
        assert(extracted.contains(value) == bool(span.count(value)));
        assert(extracted.min_xor(value) == expected_min);
        assert(extracted.max_xor(value) == expected_max);
      }
    }
    int expected_rank = 0;
    const auto all = enumerate(-5);
    while ((1u << expected_rank) < all.size()) expected_rank++;
    assert(basis.rank() == expected_rank);
  };
  check();
  for (auto [code, tag] : input)
  {
    const V value = encode(code);
    const auto old_span = enumerate(-5);
    const bool increased = basis.insert(value, tag);
    assert(increased == !bool(old_span.count(value)));
    inserted.emplace_back(value, tag);
    check();
    auto copied = basis;
    auto moved = std::move(copied);
    assert(moved.rank() == basis.rank());
    for (int threshold = -5; threshold <= 5; threshold++)
      assert(moved.max_xor(zero, threshold) == basis.max_xor(zero, threshold));
  }
  basis.clear();
  assert(basis.rank() == 0 && basis.dimension() == dimension);
  assert(basis.rank(-100) == 0 && basis.max_xor(zero, -100) == zero);
  assert(!basis.insert(zero, 100));
}

template <class V, bool Track = false, class Encode>
void random_sequences(int dimension, const Encode &encode, int trials)
{
  mt19937 rng(202611023);
  for (int trial = 0; trial < trials; trial++)
  {
    vc<pair<int, int>> input;
    for (int i = 0; i < 12; i++) input.emplace_back(rng() % 64, int(rng() % 7) - 3);
    check_sequence<V, Track>(dimension, input, encode, trial < 3);
    if (trial < 5)
    {
      stable_sort(ALL(input), [](const auto &a, const auto &b) { return a.second < b.second; });
      check_sequence<V, Track>(dimension, input, encode, false);
      reverse(ALL(input));
      check_sequence<V, Track>(dimension, input, encode, false);
      for (auto &p : input) p.second = -1;
      check_sequence<V, Track>(dimension, input, encode, false);
    }
  }
}

template <class UInt>
void integer_cases()
{
  constexpr int width = numeric_limits<UInt>::digits;
  const auto encode = [](int code)
  {
    constexpr int at[]{0, width / 5, width / 3, width / 2, width - 2, width - 1};
    UInt value = 0;
    for (int b = 0; b < 6; b++) if (code >> b & 1) value ^= UInt(1) << at[b];
    return value;
  };
  random_sequences<UInt>(width, encode, 30);
  random_sequences<UInt, true>(width, encode, 15);
  XorBasis<UInt> basis(3);
  assert(basis.insert(1, -2));
  assert(basis.insert(2, -2));
  assert(!basis.insert(3, 2));
  assert(basis.rank(0) == 1 && basis.contains(3, 0));
  assert(!basis.insert(1, 3));
  assert(basis.rank(0) == 2 && basis.rank(3) == 1);
  assert(!basis.insert(2, 4));
  assert(basis.rank(3) == 2 && basis.rank(4) == 1);
  assert(basis.contains(2, 4) && !basis.contains(1, 4));
  XorBasis<UInt> empty(0);
  assert(!empty.insert(0, 1) && empty.contains(0, 2));
}

void dynamic_cases()
{
  constexpr int dimension = 193;
  const auto encode = [](int code)
  {
    const int at[]{0, 2, 63, 64, 127, 192};
    DynamicBitset value(dimension);
    for (int b = 0; b < 6; b++) if (code >> b & 1) value.set(at[b]);
    return value;
  };
  random_sequences<DynamicBitset>(dimension, encode, 20);
  random_sequences<DynamicBitset, true>(dimension, encode, 10);
  XorBasis<DynamicBitset> full(300);
  for (int i = 0; i < 300; i++)
  {
    DynamicBitset value(300);
    value.set(i);
    assert(full.insert(value, i));
  }
  assert(full.rank() == 300 && full.rank(149) == 151);
  auto maximum = full.max_xor(DynamicBitset(300), 149);
  for (int i = 0; i < 300; i++) assert(maximum.test(i) == (i >= 149));
  auto extracted = full.basis(149);
  assert(extracted.rank() == 151 && extracted.max_xor() == maximum);
  for (int i = 0; i < 300; i++)
  {
    DynamicBitset value(300);
    value.set(i);
    assert(full.contains(value, 149) == (i >= 149));
    assert(!full.insert(value, 500 - i));
  }
  assert(full.rank(400) == 101);
  XorBasis<DynamicBitset> empty;
  assert(!empty.insert(DynamicBitset(0), 0));
  assert(empty.max_xor(DynamicBitset(0), 0).size() == 0);
}

struct OrderedTag
{
  int value;
  explicit OrderedTag(int value) : value(value) {}
  friend bool operator<(const OrderedTag &a, const OrderedTag &b) { return a.value < b.value; }
};

template <uint P, bool Track>
void linear_tags()
{
  using S = static_modint32<P>;
  using F = FieldAddSubMulDiv<S>;
  mt19937 rng(2026110401 + P + Track);
  for (int trial = 0; trial < 50; trial++)
  {
    int d = rng() % 5;
    LinearBasis<F, Track> basis(d), other(d);
    vvc<S> input, other_input, originals;
    vc<int> tags, other_tags;
    auto plain = [&](const vvc<S> &values, const vc<int> &t, int threshold)
    {
      LinearBasis<F, false, void> result(d);
      for (int i = 0; i < int(values.size()); i++) if (t[i] >= threshold) result.insert(values[i]);
      return result;
    };
    for (int step = 0; step < 12; step++)
    {
      vc<S> value(d);
      for (auto &x : value) x = rng() % P;
      int tag = int(rng() % 7) - 3;
      bool independent = !basis.contains(value);
      assert(basis.insert(value, tag) == independent);
      if (independent) originals.push_back(value);
      assert(basis.basis() == originals);
      input.push_back(value), tags.push_back(tag);
      if (step % 3 == 0) other.insert(value, -tag), other_input.push_back(value), other_tags.push_back(-tag);
      for (int threshold = -4; threshold <= 4; threshold++)
      {
        const auto expected = plain(input, tags, threshold);
        assert(basis.rank(threshold) == expected.rank());
        auto selected = basis.basis(threshold);
        assert(selected.rank() == expected.rank());
        for (int q = 0; q < 8; q++)
        {
          vc<S> query(d);
          for (auto &x : query) x = rng() % P;
          if (q == 0) query = value;
          assert(basis.contains(query, threshold) == expected.contains(query));
          assert(selected.contains(query) == expected.contains(query));
          auto residual = basis.reduce(query, threshold);
          assert(basis.reduce(residual, threshold) == residual);
          if constexpr (Track)
          {
            auto c = basis.coordinates(query, threshold);
            assert(bool(c) == expected.contains(query));
            if (c)
            {
              vc<S> reconstructed(d, F::e0());
              for (int j = 0; j < basis.rank(); j++) for (int i = 0; i < d; i++)
                reconstructed[i] = F::add(reconstructed[i], F::mul((*c)[j], basis.basis()[j][i]));
              assert(reconstructed == query);
            }
          }
        }
      }
    }
    auto common = basis & other, united = basis | other;
    for (int threshold = -4; threshold <= 4; threshold++)
    {
      auto a = plain(input, tags, threshold), b = plain(other_input, other_tags, threshold);
      auto sum = a | b;
      assert(united.rank(threshold) == sum.rank());
      assert(common.rank(threshold) + sum.rank() == a.rank() + b.rank());
      auto selected = common.basis(threshold);
      for (const auto &v : selected.basis()) assert(a.contains(v) && b.contains(v));
      for (const auto &v : a.basis()) assert(united.contains(v, threshold));
      for (const auto &v : b.basis()) assert(united.contains(v, threshold));
    }
    auto copied = basis;
    basis.merge(basis);
    assert(copied.basis() == basis.basis());
    auto moved = std::move(copied);
    for (int threshold = -4; threshold <= 4; threshold++) assert(moved.rank(threshold) == basis.rank(threshold));
    basis.clear();
    assert(basis.rank() == 0 && basis.rank(0) == 0);
    if constexpr (Track) assert(basis.coordinates(vc<S>(d), 0) == optional<vc<S>>(vc<S>{}));
  }
}

void tagged_intersection()
{
  mt19937 rng(2026110402);
  for (int trial = 0; trial < 100; trial++)
  {
    XorBasis<ull, true> a(6);
    XorBasis<ull> b(6);
    for (int i = 0; i < 16; i++)
      if (i & 1) a.insert(rng() % 64, int(rng() % 7) - 3);
      else b.insert(rng() % 64, int(rng() % 7) - 3);
    auto common = a & b, united = a | b;
    for (int t = -4; t <= 4; t++)
      for (ull x = 0; x < 64; x++)
      {
        assert(common.contains(x, t) == (a.contains(x, t) && b.contains(x, t)));
        bool expected = false;
        for (ull y = 0; y < 64; y++) if (a.contains(y, t) && b.contains(x ^ y, t)) expected = true;
        assert(united.contains(x, t) == expected);
      }
  }
}

void wide_tagged()
{
  XorBasis<u128, true, u128> basis;
  for (int i = 127; i >= 0; i--) basis.insert(u128(1) << i, u128(i));
  const u128 all = ~u128(0);
  assert(basis.kth(all, u128(0), u128(0)) == optional<u128>(all));
  assert(basis.order_of_key<u128>(all, u128(0), u128(0)) == all);
  XorBasis<DynamicBitset, true> dynamic(193);
  for (int i = 0; i < 193; i++)
  {
    DynamicBitset unit(193); unit.set(i);
    dynamic.insert(unit, i);
  }
  DynamicBitset expected(193); expected.set(0).set(2);
  assert(dynamic.kth(5ULL, DynamicBitset(193), 0) == optional<DynamicBitset>(expected));
  DynamicBitset high(193); high.set(192);
  assert(dynamic.kth(5ULL, high, 0) == optional<DynamicBitset>(expected));
  DynamicBitset max(193); max.set_range(0, 193);
  assert(dynamic.predecessor(max, DynamicBitset(193), 0) == optional<DynamicBitset>(max));
  expected.reset(0).reset(2).set(190);
  assert(dynamic.coordinates(expected, 190));
}

int main()
{
  linear_tags<2, false>();
  linear_tags<2, true>();
  linear_tags<3, false>();
  linear_tags<3, true>();
  linear_tags<998244353, false>();
  linear_tags<998244353, true>();
  tagged_intersection();
  wide_tagged();
  integer_cases<uint32_t>();
  integer_cases<uint64_t>();
  integer_cases<u128>();
  dynamic_cases();
  XorBasis<ull, false, OrderedTag> tagged;
  assert(tagged.insert(1, OrderedTag(3)));
  assert(tagged.insert(2, OrderedTag(1)));
  assert(!tagged.insert(1, OrderedTag(5)));
  assert(tagged.rank(OrderedTag(4)) == 1);
  assert(tagged.max_xor(0ULL, OrderedTag(4)) == 1);
  tagged.clear();
  assert(tagged.rank() == 0);
  using F = FieldAddSubMulDiv<modint998244353>;
  LinearBasis<F, true, OrderedTag> linear(2), other(2);
  assert(linear.insert({1, 0}, OrderedTag(0)));
  assert(linear.insert({0, 1}, OrderedTag(1)));
  assert(!linear.insert({2, 3}, OrderedTag(5)));
  assert(linear.contains({2, 3}, OrderedTag(5)));
  assert(linear.coordinates({2, 3}, OrderedTag(5)) == optional<vc<modint998244353>>(vc<modint998244353>{2, 3}));
  other.insert({2, 3}, OrderedTag(7));
  auto common = linear & other;
  assert(common.rank(OrderedTag(5)) == 1 && common.rank(OrderedTag(6)) == 0);
  linear.merge(other);
  assert(linear.rank(OrderedTag(7)) == 1);
  cout << "Hello World\n";
}
