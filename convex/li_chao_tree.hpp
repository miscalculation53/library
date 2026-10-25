#pragma once

//! SPDX-License-Identifier: CC0-1.0
//! The mathematical core was frozen before reading the project's public API.
//! This revision adapts that core and uses exact integer intermediates.

#include <optional>

#include "../template/template_all_but_modint.hpp"
#include "../utils/is_integral_ext.hpp"
#include "../utils/larger_int.hpp"
#include "../utils/make_unsigned_ext.hpp"
#include "../utils/resolved_infty.hpp"

/**
 * @brief Li Chao Tree（動的・座標圧縮）
 * @docs docs/convex/li_chao_tree.md
 */

template <class T>
struct LiChaoTreeLine
{
  T a{}, b{};
  int id = -1;
  CPP_DUMP_DEFINE_DATA(a, b, id);

  friend bool operator==(const LiChaoTreeLine &x, const LiChaoTreeLine &y)
  {
    return x.a == y.a && x.b == y.b && x.id == y.id;
  }
};

namespace li_chao_tree_detail
{

  template <class T, class U, bool Integral = is_integral_ext<T> && is_integral_ext<U>>
  struct Evaluation
  {
    using Type = U;
    static Type evaluate(const T &a, const T &x, const T &b)
    {
      return U(a) * U(x) + U(b);
    }
    static U output(const Type &result) { return result; }
  };

  template <class T, class U>
  struct Evaluation<T, U, true>
  {
    using Type = conditional_t<(sizeof(T) <= 4), long long, i128>;
    static Type evaluate(const T &a, const T &x, const T &b)
    {
      return Type(a) * Type(x) + Type(b);
    }
    static U output(const Type &result) { return U(result); }
  };

  template <class T, class U, class Compare, auto inftyT, auto inftyU>
  struct Traits
  {
    static_assert(!is_integral_ext<T> || is_signed_ext<T>);
    static_assert(!is_integral_ext<U> || is_signed_ext<U>);
    static_assert(is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ||
                  is_same_v<Compare, greater<>> || is_same_v<Compare, greater<T>>);
    using Line = LiChaoTreeLine<T>;
    static constexpr bool minimize = is_same_v<Compare, less<>> || is_same_v<Compare, less<T>>;
    static constexpr bool integral_endpoints = is_integral_ext<T> && is_integral_ext<U>;
    using Arithmetic = Evaluation<T, U>;
    using Calculation = typename Arithmetic::Type;

    static Calculation exact_eval(const Line &f, const T &x)
    {
      return Arithmetic::evaluate(f.a, x, f.b);
    }
    static U eval(const Line &f, const T &x) { return Arithmetic::output(exact_eval(f, x)); }
    static bool calculation_better(const Calculation &x, const Calculation &y)
    {
      if constexpr (minimize) return x < y;
      else return y < x;
    }
    static bool coefficient_better(const T &x, const T &y)
    {
      if constexpr (minimize) return x < y;
      else return y < x;
    }
    static bool better(const U &x, const U &y)
    {
      if constexpr (minimize) return x < y;
      else return y < x;
    }
    static bool better(const Line &f, const Line &g, const T &x)
    {
      return calculation_better(exact_eval(f, x), exact_eval(g, x));
    }
    class QueryState
    {
      optional<pair<Calculation, Line>> best;

    public:
      void update(const Line &f, const T &x)
      {
        Calculation result = exact_eval(f, x);
        if (!best || calculation_better(result, best->first)) best.emplace(std::move(result), f);
      }
      optional<pair<U, Line>> finish() const
      {
        if (!best) return nullopt;
        return pair<U, Line>{Arithmetic::output(best->first), best->second};
      }
    };
  };

  enum class Direction { done, left, right };

  template <class T, class U, class Compare, auto inftyT, auto inftyU>
  class CandidateBank
  {
    using Tr = Traits<T, U, Compare, inftyT, inftyU>;
    using Line = typename Tr::Line;
    vc<Line> lines;

    void keep(int &slot, const Line &moving, int &moving_id, int &original_id)
    {
      if (moving_id < 0)
      {
        moving_id = int(lines.size());
        lines.push_back(moving);
        original_id = moving_id;
      }
      slot = moving_id;
    }

  public:
    void reserve(int count) { lines.reserve(count); }
    void clear() { lines.clear(); }
    const Line &get(int slot) const { return lines[slot]; }

    bool hidden(int slot, const Line &incoming, const T &first, const T &last) const
    {
      if constexpr (!Tr::integral_endpoints) return false;
      else
      {
        if (slot < 0) return false;
        const Line &held = lines[slot];
        if (incoming.a == held.a) return !Tr::coefficient_better(incoming.b, held.b);
        bool loses_to_right;
        if constexpr (Tr::minimize) loses_to_right = held.a < incoming.a;
        else loses_to_right = incoming.a < held.a;
        return !Tr::better(incoming, held, loses_to_right ? first : last);
      }
    }

    template <class Middle>
    Direction advance(int &slot, Line &moving, int &moving_id, int &original_id,
                      const T &first, const T &last, bool leaf, Middle middle)
    {
      if (slot < 0)
      {
        keep(slot, moving, moving_id, original_id);
        return Direction::done;
      }
      const int held_id = slot;
      const Line held = lines[held_id];
      if constexpr (Tr::integral_endpoints)
      {
        if (moving.a == held.a)
        {
          if (Tr::coefficient_better(moving.b, held.b)) keep(slot, moving, moving_id, original_id);
          return Direction::done;
        }
        bool loses_to_right;
        if constexpr (Tr::minimize) loses_to_right = held.a < moving.a;
        else loses_to_right = moving.a < held.a;
        if (!Tr::better(moving, held, loses_to_right ? first : last)) return Direction::done;
        if (!Tr::better(held, moving, loses_to_right ? last : first))
        {
          keep(slot, moving, moving_id, original_id);
          return Direction::done;
        }
        const bool exchange = Tr::better(moving, held, middle());
        if (exchange)
        {
          keep(slot, moving, moving_id, original_id);
          moving = held;
          moving_id = held_id;
        }
        return loses_to_right != exchange ? Direction::left : Direction::right;
      }
      else
      {
        if (Tr::better(moving, held, middle()))
        {
          keep(slot, moving, moving_id, original_id);
          moving = held;
          moving_id = held_id;
        }
        if (leaf) return Direction::done;
        const Line &winner = lines[slot];
        if (Tr::better(moving, winner, first)) return Direction::left;
        if (Tr::better(moving, winner, last)) return Direction::right;
        return Direction::done;
      }
    }
  };
}

template <class T, class U = larger_int_t<T>, class Compare = less<>,
          auto inftyT = nullptr, auto inftyU = nullptr>
struct LiChaoTree
{
  static_assert(is_integral_ext<T> && is_signed_ext<T>);
  using Line = LiChaoTreeLine<T>;

private:
  using Traits = li_chao_tree_detail::Traits<T, U, Compare, inftyT, inftyU>;
  using Direction = li_chao_tree_detail::Direction;
  using Bank = li_chao_tree_detail::CandidateBank<T, U, Compare, inftyT, inftyU>;
  struct Node
  {
    int candidate = -1;
    int child[2] = {-1, -1};
  };
  T xmin, xmax;
  Bank bank;
  vc<Node> nodes;
  int add_cnt = 0;

  static T midpoint(T first, T last)
  {
    using W = make_unsigned_ext_t<T>;
    const W width = W(W(last) - W(first));
    return T(first + T(width / 2));
  }
  int obtain_child(int parent, int side)
  {
    int child = nodes[parent].child[side];
    if (child < 0)
    {
      child = int(nodes.size());
      nodes.emplace_back();
      nodes[parent].child[side] = child;
    }
    return child;
  }
  void place(int at, T first, T last, const Line &fresh, int &original_id)
  {
    Line moving = fresh;
    int moving_id = original_id;
    while (true)
    {
      T middle = first;
      const Direction next = bank.advance(nodes[at].candidate, moving, moving_id,
          original_id, first, last, first == last,
          [&] { return middle = midpoint(first, last); });
      if (next == Direction::done) return;
      if (next == Direction::left) at = obtain_child(at, 0), last = middle;
      else at = obtain_child(at, 1), first = T(middle + 1);
    }
  }
  void add_range(int at, T first, T last, T qfirst, T qlast,
                 const Line &fresh, int &original_id)
  {
    if (first == qfirst && last == qlast)
    {
      place(at, first, last, fresh, original_id);
      return;
    }
    if (bank.hidden(nodes[at].candidate, fresh, qfirst, qlast)) return;
    const T middle = midpoint(first, last);
    if (qfirst <= middle)
    {
      const int child = obtain_child(at, 0);
      add_range(child, first, middle, qfirst, min(qlast, middle), fresh, original_id);
    }
    if (middle < qlast)
    {
      const int child = obtain_child(at, 1);
      add_range(child, T(middle + 1), last, max(qfirst, T(middle + 1)), qlast,
                fresh, original_id);
    }
  }

public:
  LiChaoTree(T xmin, T xmax) : xmin(xmin), xmax(xmax), nodes(1)
  {
    assert(xmin < xmax);
  }
  void reserve(int n)
  {
    assert(n >= 0);
    nodes.reserve(n);
    bank.reserve(n);
  }
  void add_line(T a, T b) { add_line(a, b, add_cnt); }
  void add_line(T a, T b, int id)
  {
    assert(id >= 0);
    ++add_cnt;
    int original_id = -1;
    place(0, xmin, T(xmax - 1), {a, b, id}, original_id);
  }
  void add_segment(T l, T r, T a, T b) { add_segment(l, r, a, b, add_cnt); }
  void add_segment(T l, T r, T a, T b, int id)
  {
    assert(l <= r && id >= 0);
    ++add_cnt;
    l = max(l, xmin), r = min(r, xmax);
    if (l < r)
    {
      int original_id = -1;
      add_range(0, xmin, T(xmax - 1), l, T(r - 1), {a, b, id}, original_id);
    }
  }
  optional<pair<U, Line>> query(T x) const
  {
    assert(xmin <= x && x < xmax);
    typename Traits::QueryState answer;
    T first = xmin, last = T(xmax - 1);
    int at = 0;
    while (at >= 0)
    {
      const Node &current = nodes[at];
      if (current.candidate >= 0) answer.update(bank.get(current.candidate), x);
      if (current.child[0] < 0 && current.child[1] < 0) break;
      const T middle = midpoint(first, last);
      if (x <= middle) at = current.child[0], last = middle;
      else at = current.child[1], first = T(middle + 1);
    }
    return answer.finish();
  }
  void clear()
  {
    bank.clear();
    nodes.clear();
    nodes.emplace_back();
    add_cnt = 0;
  }
};

template <class T, class U = larger_int_t<T>, class Compare = less<>,
          auto inftyT = nullptr, auto inftyU = nullptr>
struct LiChaoTreeCompressed
{
  using Line = LiChaoTreeLine<T>;

private:
  using Traits = li_chao_tree_detail::Traits<T, U, Compare, inftyT, inftyU>;
  using Direction = li_chao_tree_detail::Direction;
  using Bank = li_chao_tree_detail::CandidateBank<T, U, Compare, inftyT, inftyU>;
  vc<T> xs;
  Bank bank;
  vc<int> candidates;
  int add_cnt = 0;

  static int right_child(int at, int first, int middle)
  {
    return at + 2 * (middle - first + 1);
  }
  void place(int at, int first, int last, const Line &fresh, int &original_id)
  {
    Line moving = fresh;
    int moving_id = original_id;
    while (true)
    {
      int middle = first;
      const Direction next = bank.advance(candidates[at], moving, moving_id,
          original_id, xs[first], xs[last], first == last, [&] {
            middle = first + (last - first) / 2;
            return xs[middle];
          });
      if (next == Direction::done) return;
      if (next == Direction::left) ++at, last = middle;
      else at = right_child(at, first, middle), first = middle + 1;
    }
  }
  void add_range(int at, int first, int last, int qfirst, int qlast,
                 const Line &fresh, int &original_id)
  {
    if (first == qfirst && last == qlast)
    {
      place(at, first, last, fresh, original_id);
      return;
    }
    if (bank.hidden(candidates[at], fresh, xs[qfirst], xs[qlast])) return;
    const int middle = first + (last - first) / 2;
    if (qfirst <= middle)
      add_range(at + 1, first, middle, qfirst, min(qlast, middle), fresh, original_id);
    if (middle < qlast)
      add_range(right_child(at, first, middle), middle + 1, last,
                max(qfirst, middle + 1), qlast, fresh, original_id);
  }
  template <bool Indexed>
  optional<pair<U, Line>> query_impl(const T &x, int position = -1) const
  {
    typename Traits::QueryState answer;
    int at = 0, first = 0, last = int(xs.size()) - 1;
    while (true)
    {
      if (candidates[at] >= 0) answer.update(bank.get(candidates[at]), x);
      if (first == last) break;
      const int middle = first + (last - first) / 2;
      bool left;
      if constexpr (Indexed) left = position <= middle;
      else left = !(xs[middle] < x);
      if (left) ++at, last = middle;
      else at = right_child(at, first, middle), first = middle + 1;
    }
    assert(xs[first] == x);
    return answer.finish();
  }

public:
  explicit LiChaoTreeCompressed(vc<T> xs) : xs(std::move(xs))
  {
    sort(this->xs.begin(), this->xs.end());
    this->xs.erase(unique(this->xs.begin(), this->xs.end()), this->xs.end());
    if (!this->xs.empty()) candidates.assign(2 * this->xs.size() - 1, -1);
  }
  const vc<T> &coordinates() const { return xs; }
  int lower_bound(const T &x) const
  {
    return std::lower_bound(xs.begin(), xs.end(), x) - xs.begin();
  }
  void add_line(T a, T b) { add_line(a, b, add_cnt); }
  void add_line(T a, T b, int id)
  {
    assert(id >= 0);
    ++add_cnt;
    if (!xs.empty())
    {
      int original_id = -1;
      place(0, 0, int(xs.size()) - 1, {a, b, id}, original_id);
    }
  }
  void add_segment(T l, T r, T a, T b) { add_segment(l, r, a, b, add_cnt); }
  void add_segment(T l, T r, T a, T b, int id)
  {
    assert(l <= r);
    add_segment_index(lower_bound(l), lower_bound(r), a, b, id);
  }
  void add_segment_index(int l, int r, T a, T b) { add_segment_index(l, r, a, b, add_cnt); }
  void add_segment_index(int l, int r, T a, T b, int id)
  {
    assert(0 <= l && l <= r && r <= int(xs.size()) && id >= 0);
    ++add_cnt;
    if (l < r)
    {
      int original_id = -1;
      add_range(0, 0, int(xs.size()) - 1, l, r - 1, {a, b, id}, original_id);
    }
  }
  optional<pair<U, Line>> query(const T &x) const
  {
    assert(!xs.empty());
    return query_impl<false>(x);
  }
  optional<pair<U, Line>> query_index(int k) const
  {
    assert(0 <= k && k < int(xs.size()));
    return query_impl<true>(xs[k], k);
  }
  void clear()
  {
    bank.clear();
    fill(candidates.begin(), candidates.end(), -1);
    add_cnt = 0;
  }
};
