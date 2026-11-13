#pragma once

#include "../../ds/dynamic_bitset.hpp"
#include "../../utils/is_integral_ext.hpp"
#include "../../utils/make_unsigned_ext.hpp"
#include "internal_basis_tag.hpp"

/**
 * @brief XOR 基底
 * @docs docs/math/linalg/xor_basis.md
 */

namespace internal
{
template <class V, class = void>
struct XorBasisVector;

template <class V>
struct XorBasisVector<V, enable_if_t<is_integral_ext<V> && is_unsigned_ext<V> && !is_same_v<V, bool>>>
{
  static constexpr int default_dimension = numeric_limits<V>::digits;
  static_assert(default_dimension <= 128);
  using Coefficient = V;
  static bool valid(const V &value, int dimension)
  {
    return 0 <= dimension && dimension <= default_dimension &&
           (dimension == default_dimension || (value >> dimension) == V(0));
  }
  static V zero(int) { return V(0); }
  static bool empty(const V &value) { return value == V(0); }
  static bool test(const V &value, int i) { return (value >> i) & V(1); }
  static int top(const V &value)
  {
    if (value == V(0)) return -1;
    if constexpr (default_dimension <= 64) return 63 - __builtin_clzll(ull(value));
    else
    {
      ull high = ull(value >> 64);
      return high ? 127 - __builtin_clzll(high) : 63 - __builtin_clzll(ull(value));
    }
  }
  static int top_xor(const V &a, const V &b) { return top(a ^ b); }
  static Coefficient coefficient_zero(int) { return V(0); }
  static Coefficient coefficient_unit(int i) { return V(1) << i; }
  static void coefficient_add(Coefficient &a, const Coefficient &b) { a ^= b; }
  static bool coefficient_test(const Coefficient &a, int i) { return test(a, i); }
};

template <>
struct XorBasisVector<DynamicBitset>
{
  static constexpr int default_dimension = 0;
  using Coefficient = DynamicBitset;
  static bool valid(const DynamicBitset &value, int dimension) { return value.size() == dimension; }
  static DynamicBitset zero(int dimension) { return DynamicBitset(dimension); }
  static bool empty(const DynamicBitset &value) { return value.none(); }
  static bool test(const DynamicBitset &value, int i) { return value.test(i); }
  static int top(const DynamicBitset &value) { return value.find_last(); }
  static int top_xor(const DynamicBitset &a, const DynamicBitset &b)
  {
    for (int i = a.word_count() - 1; i >= 0; i--)
    {
      ull word = a.word(i) ^ b.word(i);
      if (word) return i * 64 + 63 - __builtin_clzll(word);
    }
    return -1;
  }
  static Coefficient coefficient_zero(int rank) { return Coefficient(rank); }
  static Coefficient coefficient_unit(int i)
  {
    Coefficient value(i + 1);
    value.set(i);
    return value;
  }
  static void coefficient_add(Coefficient &a, const Coefficient &b)
  {
    if (a.size() < b.size()) a.resize(b.size());
    a.xor_slice(0, b.size(), b, 0);
  }
  static bool coefficient_test(const Coefficient &a, int i) { return a.test(i); }
};
} // namespace internal

template <class V = ull, bool Track = false, class Tag = int>
class XorBasis
{
  template <class, bool, class> friend class XorBasis;
  using Traits = internal::XorBasisVector<V>;
  using TagData = internal::BasisTag<Tag>;
  using StoredTag = typename TagData::Type;
  static constexpr bool tagged = !is_void_v<Tag>;

public:
  using Value = V;
  using Coefficient = typename Traits::Coefficient;
  using TagType = Tag;

private:
  using StoredCoefficient = conditional_t<Track, Coefficient, monostate>;
  struct Row : TagData
  {
    int pivot;
    V value;
    StoredCoefficient coefficient;
#ifdef LOCAL
    auto dump_data() const
    {
      auto fields = make_tuple(_p_LIBRARY_CPP_DUMP_MEMBER(pivot), _p_LIBRARY_CPP_DUMP_MEMBER(value));
      if constexpr (tagged && Track)
        return tuple_cat(fields, make_tuple(_p_LIBRARY_CPP_DUMP_MEMBER(this->tag), _p_LIBRARY_CPP_DUMP_MEMBER(coefficient)));
      else if constexpr (tagged)
        return tuple_cat(fields, make_tuple(_p_LIBRARY_CPP_DUMP_MEMBER(this->tag)));
      else if constexpr (Track)
        return tuple_cat(fields, make_tuple(_p_LIBRARY_CPP_DUMP_MEMBER(coefficient)));
      else return fields;
    }
#endif
    Row(int pivot, V value, StoredCoefficient coefficient, StoredTag tag)
        : TagData(std::move(tag)), pivot(pivot), value(std::move(value)), coefficient(std::move(coefficient)) {}
  };
  int dimension_;
  vc<Row> rows_;
  vc<V> original_;

  int rank_impl(const StoredTag *threshold) const
  {
    if (!threshold) return rank();
    int result = 0;
    for (const auto &row : rows_) if (row.accepts(threshold)) result++;
    return result;
  }

  V reduce_impl(V value, const StoredTag *threshold, Coefficient *coefficient = nullptr) const
  {
    assert(Traits::valid(value, dimension_));
    for (const auto &row : rows_) if (row.accepts(threshold) && Traits::test(value, row.pivot))
    {
      value ^= row.value;
      if constexpr (Track) if (coefficient) Traits::coefficient_add(*coefficient, row.coefficient);
    }
    return value;
  }

  optional<Coefficient> coordinates_impl(V value, const StoredTag *threshold) const
  {
    Coefficient coefficient = Traits::coefficient_zero(rank());
    if (!Traits::empty(reduce_impl(std::move(value), threshold, &coefficient))) return nullopt;
    return coefficient;
  }

  bool insert_impl(const V &value, StoredTag tag)
  {
    assert(Traits::valid(value, dimension_));
    if constexpr (!tagged) if (rank() == dimension_) return false;
    V residual = value;
    StoredCoefficient coefficient;
    if constexpr (Track)
    {
      optional<Coefficient> c;
      if constexpr (tagged)
      {
        bool may_swap = any_of(rows_.begin(), rows_.end(), [&](const Row &row) { return row.tag < tag; });
        if (!may_swap && rank() == dimension_) return false;
        if (may_swap) c = coordinates_impl(value, nullptr);
      }
      coefficient = c ? std::move(*c) : Traits::coefficient_unit(rank());
    }
    int pivot = Traits::top(residual);
    auto it = rows_.begin();
    for (; it != rows_.end() && pivot != -1; ++it)
    {
      if (pivot < it->pivot) continue;
      if (pivot > it->pivot) break;
      if constexpr (tagged) if (it->tag < tag)
      {
        swap(residual, it->value);
        swap(tag, it->tag);
        if constexpr (Track) swap(coefficient, it->coefficient);
      }
      residual ^= it->value;
      if constexpr (Track) Traits::coefficient_add(coefficient, it->coefficient);
      pivot = Traits::top(residual);
    }
    if (pivot == -1) return false;
    rows_.emplace(it, pivot, std::move(residual), std::move(coefficient), std::move(tag));
    original_.push_back(value);
    return true;
  }

  V max_xor_impl(V value, const StoredTag *threshold, int start = 0) const
  {
    assert(Traits::valid(value, dimension_));
    for (int i = start; i < rank(); i++)
      if (rows_[i].accepts(threshold) && !Traits::test(value, rows_[i].pivot)) value ^= rows_[i].value;
    return value;
  }

  template <class Index>
  static void add_power(Index &value, int exponent)
  {
    static_assert(is_integral_ext<Index> && is_unsigned_ext<Index> && !is_same_v<Index, bool>);
    static_assert(numeric_limits<Index>::digits <= 128);
    assert(exponent < numeric_limits<Index>::digits);
    Index power = Index(1) << exponent;
    assert(value <= numeric_limits<Index>::max() - power);
    value += power;
  }

  template <class Index>
  optional<V> kth_impl(Index k, V value, const StoredTag *threshold) const
  {
    static_assert(is_integral_ext<Index> && !is_same_v<Index, bool>);
    using U = make_unsigned_ext_t<Index>;
    constexpr int width = numeric_limits<U>::digits;
    static_assert(width <= 128);
    assert(Traits::valid(value, dimension_));
    if constexpr (is_signed_ext<Index>) if (k < 0) return nullopt;
    U index = U(k);
    int remaining = rank_impl(threshold);
    if (remaining < width && (index >> remaining) != U(0)) return nullopt;
    for (const auto &row : rows_) if (row.accepts(threshold))
    {
      int bit = --remaining;
      bool desired = bit < width && ((index >> bit) & U(1));
      if (Traits::test(value, row.pivot) != desired) value ^= row.value;
    }
    return value;
  }

  template <class Index>
  Index order_of_key_impl(const V &key, V value, const StoredTag *threshold) const
  {
    assert(Traits::valid(key, dimension_) && Traits::valid(value, dimension_));
    Index count = 0;
    int remaining = rank_impl(threshold);
    for (const auto &row : rows_) if (row.accepts(threshold))
    {
      if (Traits::top_xor(value, key) > row.pivot)
      {
        if (value < key) add_power(count, remaining);
        return count;
      }
      --remaining;
      bool bit = Traits::test(key, row.pivot);
      if (bit) add_power(count, remaining);
      if (Traits::test(value, row.pivot) != bit) value ^= row.value;
    }
    if (value < key) add_power(count, 0);
    return count;
  }

  optional<V> predecessor_impl(const V &key, V value, bool strict, const StoredTag *threshold) const
  {
    assert(Traits::valid(key, dimension_) && Traits::valid(value, dimension_));
    optional<V> backup;
    int resume = 0;
    bool blocked = false;
    for (int i = 0; i < rank(); i++)
    {
      const auto &row = rows_[i];
      if (!row.accepts(threshold)) continue;
      if (Traits::top_xor(value, key) > row.pivot)
      {
        if (value < key) return max_xor_impl(std::move(value), threshold, i);
        blocked = true;
        break;
      }
      bool bit = Traits::test(key, row.pivot);
      if (Traits::test(value, row.pivot)) value ^= row.value;
      if (bit)
      {
        backup = value;
        resume = i + 1;
        value ^= row.value;
      }
    }
    if (!blocked && (strict ? value < key : !(key < value))) return value;
    if (!backup) return nullopt;
    return max_xor_impl(std::move(*backup), threshold, resume);
  }

public:
  explicit XorBasis(int dimension = Traits::default_dimension) : dimension_(dimension)
  {
    assert(dimension >= 0);
    assert(Traits::valid(Traits::zero(dimension), dimension));
  }
  XorBasis(int dimension, const vc<V> &vectors) : XorBasis(dimension)
  {
    for (const auto &value : vectors) insert(value);
  }
  explicit XorBasis(const vc<V> &vectors)
      : XorBasis([&] {
          if constexpr (is_same_v<V, DynamicBitset>) return vectors.empty() ? 0 : vectors[0].size();
          else return Traits::default_dimension;
        }(), vectors) {}

  CPP_DUMP_DEFINE_DATA(dimension_, rows_, original_);

  int dimension() const { return dimension_; }
  int rank() const { return int(rows_.size()); }
  const vc<V> &basis() const { return original_; }
  void clear() { rows_.clear(), original_.clear(); }
  bool insert(const V &value) { return insert_impl(value, StoredTag{}); }
  V reduce(V value) const { return reduce_impl(std::move(value), nullptr); }
  bool contains(const V &value) const { return Traits::empty(reduce(value)); }

  template <bool Enabled = Track, enable_if_t<Enabled && Track, int> = 0>
  optional<Coefficient> coordinates(V value) const { return coordinates_impl(std::move(value), nullptr); }

  V min_xor(V affine) const { return reduce(std::move(affine)); }
  V min_xor() const { return Traits::zero(dimension_); }
  V max_xor(V affine) const { return max_xor_impl(std::move(affine), nullptr); }
  V max_xor() const { return max_xor(Traits::zero(dimension_)); }

  template <class Index = ull>
  Index span_size() const
  {
    static_assert(is_integral_ext<Index> && is_unsigned_ext<Index> && !is_same_v<Index, bool>);
    static_assert(numeric_limits<Index>::digits <= 128);
    assert(rank() < numeric_limits<Index>::digits);
    return Index(1) << rank();
  }

  template <class Index = ull>
  optional<V> kth(Index k, V affine) const { return kth_impl(k, std::move(affine), nullptr); }
  template <class Index = ull>
  optional<V> kth(Index k) const { return kth(k, Traits::zero(dimension_)); }
  template <class Index = ull>
  Index order_of_key(const V &key, V affine) const { return order_of_key_impl<Index>(key, std::move(affine), nullptr); }
  template <class Index = ull>
  Index order_of_key(const V &key) const { return order_of_key<Index>(key, Traits::zero(dimension_)); }
  optional<V> predecessor(const V &key, V affine) const { return predecessor_impl(key, std::move(affine), false, nullptr); }
  optional<V> predecessor(const V &key) const { return predecessor(key, Traits::zero(dimension_)); }
  optional<V> predecessor_strict(const V &key, V affine) const { return predecessor_impl(key, std::move(affine), true, nullptr); }
  optional<V> predecessor_strict(const V &key) const { return predecessor_strict(key, Traits::zero(dimension_)); }

  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  bool insert(const V &value, StoredTag tag) { return insert_impl(value, std::move(tag)); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  int rank(const StoredTag &threshold) const { return rank_impl(&threshold); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  V reduce(V value, const StoredTag &threshold) const { return reduce_impl(std::move(value), &threshold); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  bool contains(const V &value, const StoredTag &threshold) const { return Traits::empty(reduce(value, threshold)); }
  template <bool Enabled = Track && tagged, enable_if_t<Enabled && Track && tagged, int> = 0>
  optional<Coefficient> coordinates(V value, const StoredTag &threshold) const { return coordinates_impl(std::move(value), &threshold); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  V min_xor(V value, const StoredTag &threshold) const { return reduce(std::move(value), threshold); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  V max_xor(V value, const StoredTag &threshold) const { return max_xor_impl(std::move(value), &threshold); }
  template <class Index = ull, bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  Index span_size(const StoredTag &threshold) const
  {
    static_assert(is_integral_ext<Index> && is_unsigned_ext<Index> && !is_same_v<Index, bool>);
    static_assert(numeric_limits<Index>::digits <= 128);
    int r = rank(threshold);
    assert(r < numeric_limits<Index>::digits);
    return Index(1) << r;
  }
  template <class Index = ull, bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  optional<V> kth(Index k, V affine, const StoredTag &threshold) const { return kth_impl(k, std::move(affine), &threshold); }
  template <class Index = ull, bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  Index order_of_key(const V &key, V affine, const StoredTag &threshold) const { return order_of_key_impl<Index>(key, std::move(affine), &threshold); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  optional<V> predecessor(const V &key, V affine, const StoredTag &threshold) const { return predecessor_impl(key, std::move(affine), false, &threshold); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  optional<V> predecessor_strict(const V &key, V affine, const StoredTag &threshold) const { return predecessor_impl(key, std::move(affine), true, &threshold); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  XorBasis<V, Track, void> basis(const StoredTag &threshold) const
  {
    XorBasis<V, Track, void> result(dimension_);
    for (const auto &row : rows_) if (row.accepts(&threshold)) result.insert(row.value);
    return result;
  }

  template <bool OtherTrack>
  void merge(const XorBasis<V, OtherTrack, Tag> &other)
  {
    assert(dimension_ == other.dimension());
    if constexpr (Track == OtherTrack) if (this == &other) return;
    if constexpr (tagged)
      for (const auto &row : other.rows_) insert(row.value, row.tag);
    else for (const auto &value : other.basis()) insert(value);
  }

  template <bool OtherTrack>
  XorBasis intersection(const XorBasis<V, OtherTrack, Tag> &other) const
  {
    assert(dimension_ == other.dimension());
    XorBasis<V, true, void> work(dimension_);
    XorBasis result(dimension_);
    struct Entry { const V *value; const StoredTag *tag; bool left; };
    vc<Entry> entries;
    auto append = [&](const auto &basis, bool left)
    {
      if constexpr (tagged)
        for (const auto &row : basis.rows_) entries.push_back({&row.value, &row.tag, left});
      else for (const auto &value : basis.basis()) entries.push_back({&value, nullptr, left});
    };
    append(*this, true), append(other, false);
    if constexpr (tagged)
      stable_sort(entries.begin(), entries.end(), [](const Entry &a, const Entry &b) { return *b.tag < *a.tag; });
    vc<bool> left;
    bool seen[2]{};
    for (const auto &entry : entries)
    {
      auto coefficient = seen[!entry.left] ? work.coordinates(*entry.value) : nullopt;
      seen[entry.left] = true;
      if (!coefficient) work.insert(*entry.value), left.push_back(entry.left);
      else
      {
        V common = entry.left ? *entry.value : Traits::zero(dimension_);
        for (int i = 0; i < work.rank(); i++)
          if (left[i] && Traits::coefficient_test(*coefficient, i)) common ^= work.basis()[i];
        if constexpr (tagged) result.insert(common, *entry.tag);
        else result.insert(common);
      }
    }
    return result;
  }

  template <bool OtherTrack>
  XorBasis operator|(const XorBasis<V, OtherTrack, Tag> &other) const
  {
    auto result = *this;
    result.merge(other);
    return result;
  }
  template <bool OtherTrack>
  XorBasis operator&(const XorBasis<V, OtherTrack, Tag> &other) const { return intersection(other); }
};
