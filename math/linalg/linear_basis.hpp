#pragma once

#include "../../algebra/algebra_basic_ops.hpp"
#include "internal_basis_tag.hpp"

/**
 * @brief 体上の線形基底
 * @docs docs/math/linalg/linear_basis.md
 */

template <class F, bool Track = false, class Tag = int>
class LinearBasis
{
  template <class, bool, class> friend class LinearBasis;
  using TagData = internal::BasisTag<Tag>;
  using StoredTag = typename TagData::Type;
  static constexpr bool tagged = !is_void_v<Tag>;

public:
  using S = typename F::S;
  using TagType = Tag;

private:
  using Coefficient = conditional_t<Track, vc<S>, monostate>;
  struct Row : TagData
  {
    int pivot;
    vc<S> value;
    Coefficient coefficient;
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
    Row(int pivot, vc<S> value, Coefficient coefficient, StoredTag tag)
        : TagData(std::move(tag)), pivot(pivot), value(std::move(value)), coefficient(std::move(coefficient)) {}
  };
  int dimension_;
  vc<Row> rows_;
  vvc<S> original_;

  static void subtract(vc<S> &value, const vc<S> &row, const S &factor, int size)
  {
    for (int i = 0; i < size; i++)
      value[i] = F::add(value[i], F::minus(F::mul(factor, row[i])));
  }
  static void scale(vc<S> &value, const S &factor)
  {
    for (auto &x : value) x = F::mul(x, factor);
  }
  vc<S> eliminate(vc<S> value, vc<S> *coefficient, const StoredTag *threshold) const
  {
    assert(int(value.size()) == dimension_);
    for (const auto &row : rows_) if (row.accepts(threshold))
    {
      S factor = value[row.pivot];
      if (factor == F::e0()) continue;
      subtract(value, row.value, factor, dimension_);
      if constexpr (Track)
      {
        if (coefficient)
          for (int i = 0; i < int(row.coefficient.size()); i++)
            (*coefficient)[i] = F::add((*coefficient)[i], F::mul(factor, row.coefficient[i]));
      }
    }
    return value;
  }

  optional<vc<S>> coordinates_impl(const vc<S> &value, const StoredTag *threshold) const
  {
    vc<S> coefficient(rank(), F::e0());
    auto residual = eliminate(value, &coefficient, threshold);
    for (const auto &x : residual) if (x != F::e0()) return nullopt;
    return coefficient;
  }

  bool insert_impl(const vc<S> &value, StoredTag tag)
  {
    assert(int(value.size()) == dimension_);
    if constexpr (!tagged) if (rank() == dimension_) return false;
    vc<S> residual = value;
    Coefficient coefficient;
    if constexpr (Track)
    {
      optional<vc<S>> c;
      if constexpr (tagged)
      {
        bool may_swap = any_of(rows_.begin(), rows_.end(), [&](const Row &row) { return row.tag < tag; });
        if (!may_swap && rank() == dimension_) return false;
        if (may_swap) c = coordinates_impl(value, nullptr);
      }
      if (c) coefficient = std::move(*c);
      else coefficient.assign(rank() + 1, F::e0()), coefficient.back() = F::e1();
    }
    int pivot = 0;
    while (pivot < dimension_ && residual[pivot] == F::e0()) pivot++;
    auto at = rows_.begin();
    for (; at != rows_.end() && pivot < dimension_; ++at)
    {
      if (pivot > at->pivot) continue;
      if (pivot < at->pivot) break;
      if constexpr (tagged) if (at->tag < tag)
      {
        S inverse = F::inv(residual[pivot]);
        scale(residual, inverse);
        if constexpr (Track) scale(coefficient, inverse);
        swap(residual, at->value);
        swap(tag, at->tag);
        if constexpr (Track) swap(coefficient, at->coefficient);
      }
      S factor = residual[pivot];
      subtract(residual, at->value, factor, dimension_);
      if constexpr (Track)
      {
        if (coefficient.size() < at->coefficient.size()) coefficient.resize(at->coefficient.size(), F::e0());
        subtract(coefficient, at->coefficient, factor, at->coefficient.size());
      }
      while (pivot < dimension_ && residual[pivot] == F::e0()) pivot++;
    }
    if (pivot == dimension_) return false;
    S inverse = F::inv(residual[pivot]);
    scale(residual, inverse);
    if constexpr (Track) scale(coefficient, inverse);
    rows_.emplace(at, pivot, std::move(residual), std::move(coefficient), std::move(tag));
    original_.push_back(value);
    return true;
  }

public:
  explicit LinearBasis(int dimension = 0) : dimension_(dimension) { assert(dimension >= 0); }
  LinearBasis(int dimension, const vvc<S> &vectors) : LinearBasis(dimension)
  {
    for (const auto &value : vectors) insert(value);
  }

  CPP_DUMP_DEFINE_DATA(dimension_, rows_, original_);

  int dimension() const { return dimension_; }
  int rank() const { return int(rows_.size()); }
  const vvc<S> &basis() const { return original_; }
  void clear() { rows_.clear(), original_.clear(); }
  bool insert(const vc<S> &value) { return insert_impl(value, StoredTag{}); }
  vc<S> reduce(const vc<S> &value) const { return eliminate(value, nullptr, nullptr); }
  bool contains(const vc<S> &value) const
  {
    auto residual = reduce(value);
    return all_of(residual.begin(), residual.end(), [](const S &x) { return x == F::e0(); });
  }
  template <bool Enabled = Track, enable_if_t<Enabled && Track, int> = 0>
  optional<vc<S>> coordinates(const vc<S> &value) const { return coordinates_impl(value, nullptr); }

  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  bool insert(const vc<S> &value, StoredTag tag) { return insert_impl(value, std::move(tag)); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  int rank(const StoredTag &threshold) const
  {
    int result = 0;
    for (const auto &row : rows_) if (row.accepts(&threshold)) result++;
    return result;
  }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  vc<S> reduce(const vc<S> &value, const StoredTag &threshold) const { return eliminate(value, nullptr, &threshold); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  bool contains(const vc<S> &value, const StoredTag &threshold) const
  {
    auto residual = reduce(value, threshold);
    return all_of(residual.begin(), residual.end(), [](const S &x) { return x == F::e0(); });
  }
  template <bool Enabled = Track && tagged, enable_if_t<Enabled && Track && tagged, int> = 0>
  optional<vc<S>> coordinates(const vc<S> &value, const StoredTag &threshold) const { return coordinates_impl(value, &threshold); }
  template <bool Enabled = tagged, enable_if_t<Enabled && tagged, int> = 0>
  LinearBasis<F, Track, void> basis(const StoredTag &threshold) const
  {
    LinearBasis<F, Track, void> result(dimension_);
    for (const auto &row : rows_) if (row.accepts(&threshold)) result.insert(row.value);
    return result;
  }

  template <bool OtherTrack>
  void merge(const LinearBasis<F, OtherTrack, Tag> &other)
  {
    assert(dimension_ == other.dimension());
    if constexpr (Track == OtherTrack) if (this == &other) return;
    if constexpr (tagged)
      for (const auto &row : other.rows_) insert(row.value, row.tag);
    else for (const auto &value : other.basis()) insert(value);
  }

  template <bool OtherTrack>
  LinearBasis intersection(const LinearBasis<F, OtherTrack, Tag> &other) const
  {
    assert(dimension_ == other.dimension());
    LinearBasis<F, true, void> work(dimension_);
    LinearBasis result(dimension_);
    struct Entry { const vc<S> *value; const StoredTag *tag; bool left; };
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
        vc<S> common(dimension_, F::e0());
        for (int j = 0; j < work.rank(); j++) if (left[j] && (*coefficient)[j] != F::e0())
          for (int i = 0; i < dimension_; i++)
            common[i] = F::add(common[i], F::mul((*coefficient)[j], work.basis()[j][i]));
        if (entry.left) subtract(common, *entry.value, F::e1(), dimension_);
        if constexpr (tagged) result.insert(common, *entry.tag);
        else result.insert(common);
      }
    }
    return result;
  }

  template <bool OtherTrack>
  LinearBasis operator|(const LinearBasis<F, OtherTrack, Tag> &other) const
  {
    auto result = *this;
    result.merge(other);
    return result;
  }
  template <bool OtherTrack>
  LinearBasis operator&(const LinearBasis<F, OtherTrack, Tag> &other) const { return intersection(other); }
};
