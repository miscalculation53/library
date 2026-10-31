#pragma once

#include "../template/template_all_but_modint.hpp"
#include "internal_dump_content.hpp"

#include "csr.hpp"
#include "../algo/stable_argsort.hpp"

/**
 * @brief 添字を値で分類
 * @docs docs/ds/group_index.md
 */

namespace internal
{
template <bool enabled>
struct GroupIndexLookup {};

template <>
struct GroupIndexLookup<true>
{
  vc<int> lookup;
  vc<ull> rank;
  int lookup_shift = 0;
};
}

template <class T = ll, bool compress = true, class I = ll>
struct GroupIndex : private internal::GroupIndexLookup<compress && is_integral_ext<T> && !is_same_v<T, bool>>
{
private:
  int n = 0;
  vc<T> vals;
  CSR<I> csr{vc<int>{}};

  void build_lookup()
  {
    if constexpr (compress && is_integral_ext<T> && !is_same_v<T, bool>)
    {
      auto &lookup = this->lookup;
      auto &lookup_shift = this->lookup_shift;
      const int k = vals.size();
      if (n >= 1024 && k <= 16)
      {
        auto &rank = this->rank;
        const int blocks = (n + 63) / 64;
        rank.resize(2 * k * blocks);
        for (int row = 0; row < k; ++row)
        {
          const int offset = 2 * row * blocks;
          for (I i : csr[row])
            rank[offset + 2 * (int(i) / 64)] |= 1ULL << (int(i) % 64);
          ull count = 0;
          for (int b = 0; b < blocks; ++b)
          {
            rank[offset + 2 * b + 1] = count;
            count += __builtin_popcountll(rank[offset + 2 * b]);
          }
        }
      }
      if (k < 256)
        return;
      using U = make_unsigned_ext_t<T>;
      U span = U(vals.back()) - U(vals.front());
      const int limit = min(k / 8, 1 << 16);
      while (span >= U(limit))
        span >>= 1, ++lookup_shift;
      const int buckets = int(span) + 1;
      lookup.resize(buckets + 1);
      int next = 0;
      for (int i = 0; i < k; ++i)
      {
        int bucket = int(U(U(vals[i]) - U(vals.front())) >> lookup_shift);
        while (next <= bucket)
          lookup[next++] = i;
      }
      while (next <= buckets)
        lookup[next++] = k;
    }
  }

  int row_id(const T &val) const
  {
    if constexpr (compress)
    {
      auto first = vals.begin(), last = vals.end();
      if constexpr (is_integral_ext<T> && !is_same_v<T, bool>)
      {
        const auto &lookup = this->lookup;
        if (!lookup.empty())
        {
          if (val < vals.front() || vals.back() < val)
            return -1;
          using U = make_unsigned_ext_t<T>;
          int bucket = int(U(U(val) - U(vals.front())) >> this->lookup_shift);
          first += lookup[bucket];
          last = vals.begin() + lookup[bucket + 1];
        }
      }
      auto it = lower_bound(first, last, val);
      return it == last || *it != val ? -1 : int(it - vals.begin());
    }
    else
    {
      using U = common_type_t<T, int>;
      return 0 <= val && U(val) < U(csr.size()) ? int(val) : -1;
    }
  }

  I row_count(int row, int i, bool inclusive) const
  {
    auto is = csr.at(row);
    if constexpr (compress && is_integral_ext<T> && !is_same_v<T, bool>)
      if (!this->rank.empty())
      {
        if (row < 0)
          return 0;
        ll p = ll(i) + inclusive;
        if (p <= 0)
          return 0;
        if (p >= n)
          return is.size();
        const int blocks = (n + 63) / 64;
        const int offset = 2 * (row * blocks + int(p) / 64);
        const ull mask = (1ULL << (int(p) % 64)) - 1;
        return I(this->rank[offset + 1] + __builtin_popcountll(this->rank[offset] & mask));
      }
    return inclusive ? ::leq_cnt(is, i) : ::lt_cnt(is, i);
  }

public:
  GroupIndex() = default;
  GroupIndex(const vc<T> &a) : n(a.size())
  {
    if constexpr (compress)
    {
      auto ord = stable_argsort(a);
      vc<int> counts;
      counts.reserve(n);
      vals.reserve(n);
      for (int i : ord)
      {
        if (vals.empty() || vals.back() != a[i])
          vals.eb(a[i]), counts.eb(0);
        counts.back()++;
      }
      csr = CSR<I>(counts);
      copy(ALL(ord), csr.get_elist().begin());
      build_lookup();
    }
    else
    {
      static_assert(is_integral_v<T>);
      const int m = a.empty() ? 0 : int(MAX(a)) + 1;
      vc<pair<int, I>> ies(n);
      repi(i, n)
      {
        assert(0 <= a[i]);
        ies[i] = {int(a[i]), i};
      }
      csr = CSR<I>(m, ies);
      repi(val, m) if (!csr[val].empty()) vals.eb(T(val));
    }
  }

  // 値が val になる添字たち
  auto idxs(const T &val) const { return csr.at(row_id(val)); }

  // 値が val になる添字のうち i 未満で最大のもの (左番兵 -1)
  I lt_max(const T &val, int i) const
  {
    static_assert(numeric_limits<I>::is_signed, "index type must represent -1");
    int row = row_id(val);
    auto is = csr.at(row);
    I j = row_count(row, i, false);
    return j == 0 ? I(-1) : is[j - 1];
  }
  // 値が val になる添字のうち i 以下で最大のもの (左番兵 -1)
  I leq_max(const T &val, int i) const
  {
    static_assert(numeric_limits<I>::is_signed, "index type must represent -1");
    int row = row_id(val);
    auto is = csr.at(row);
    I j = row_count(row, i, true);
    return j == 0 ? I(-1) : is[j - 1];
  }
  // 値が val になる添字のうち i 超過で最小のもの (右番兵 n)
  I gt_min(const T &val, int i) const
  {
    int row = row_id(val);
    auto is = csr.at(row);
    I j = row_count(row, i, true);
    return j == is.size() ? I(n) : is[j];
  }
  // 値が val になる添字のうち i 以上で最小のもの (右番兵 n)
  I geq_min(const T &val, int i) const
  {
    int row = row_id(val);
    auto is = csr.at(row);
    I j = row_count(row, i, false);
    return j == is.size() ? I(n) : is[j];
  }
  // 値が val になる i 未満の添字の個数
  // i 番目が val のとき、「これは何番目の val か？」に一致
  I lt_cnt(const T &val, int i) const { return row_count(row_id(val), i, false); }
  // 値が val になる i 以下の添字の個数
  I leq_cnt(const T &val, int i) const { return row_count(row_id(val), i, true); }
  // 値が val になる i 超過の添字の個数
  I gt_cnt(const T &val, int i) const
  {
    int row = row_id(val);
    return csr.at(row).size() - row_count(row, i, true);
  }
  // 値が val になる i 以上の添字の個数
  I geq_cnt(const T &val, int i) const
  {
    int row = row_id(val);
    return csr.at(row).size() - row_count(row, i, false);
  }
  // 値が val になる [l, r) の添字の個数
  I in_cnt(const T &val, int l, int r) const
  {
    if (l > r)
      return 0;
    int row = row_id(val);
    if constexpr (compress && is_integral_ext<T> && !is_same_v<T, bool>)
      if (!this->rank.empty())
        return row_count(row, r, false) - row_count(row, l, false);
    return ::in_cnt(csr.at(row), l, r);
  }

  // 出現する値の種類数
  I size() const { return vals.size(); }
  bool empty() const { return vals.empty(); }

  struct Iterator
  {
    using iterator_category = input_iterator_tag;
    using value_type = pair<T, decltype(declval<const CSR<I> &>().at(0))>;
    using difference_type = ptrdiff_t;
    using reference = value_type;
    using pointer = void;

  private:
    const GroupIndex *grp = nullptr;
    int i = 0;

  public:
    Iterator() = default;
    Iterator(const GroupIndex *grp, int i) : grp(grp), i(i) {}
    value_type operator*() const
    {
      int row = i;
      if constexpr (!compress) row = int(grp->vals[i]);
      return {grp->vals[i], grp->csr[row]};
    }
    Iterator &operator++()
    {
      ++i;
      return *this;
    }
    Iterator operator++(int)
    {
      auto res = *this;
      ++*this;
      return res;
    }
    bool operator==(const Iterator &other) const { return grp == other.grp && i == other.i; }
    bool operator!=(const Iterator &other) const { return !(*this == other); }
  };
  Iterator begin() const { return Iterator(this, 0); }
  Iterator end() const { return Iterator(this, vals.size()); }

  auto &to_csr() const { return csr; }
  vvc<I> to_vv() const { return csr.to_vv(); }
  vc<pair<T, vc<I>>> content() const
  {
    vc<pair<T, vc<I>>> res;
    res.reserve(vals.size());
    for (auto [value, indices] : *this) res.eb(value, indices.to_v());
    return res;
  }
};

// 値の型をコンストラクタの引数から推論できる非圧縮版
template <class T = ll, class I = ll>
struct GroupIndexRaw : GroupIndex<T, false, I>
{
  GroupIndexRaw() = default;
  GroupIndexRaw(const vc<T> &a) : GroupIndex<T, false, I>(a) {}
};

#ifdef LOCAL
namespace cpp_dump::_detail
{
  template <class T, bool compress, class I>
  inline constexpr bool is_container<GroupIndex<T, compress, I>> = false;

  template <class T, bool compress, class I>
  inline string export_object_generic(
      const GroupIndex<T, compress, I> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    return export_named_content<library_dump_detail::GroupFields>(
        value, [&]() -> decltype(auto) { return (value); },
        indent, last_line_length, current_depth, fail_on_newline, command);
  }

  template <class T, class I>
  inline constexpr bool is_container<GroupIndexRaw<T, I>> = false;

  template <class T, class I>
  inline string export_object_generic(
      const GroupIndexRaw<T, I> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    return export_named_content<library_dump_detail::GroupFields>(
        value, [&]() -> decltype(auto) { return (value); },
        indent, last_line_length, current_depth, fail_on_newline, command);
  }

} // namespace cpp_dump::_detail
#endif
