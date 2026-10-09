#pragma once

#include "priority_container/all.hpp"
#include "../algebra/algebra_basic_ops.hpp"

template <class T, class M, class Compare, class Projection>
struct OrderedMultisetTree;

/**
 * @brief 多重集合を $2$ 分割するやつ（中央値、上位 $K$ 個の和、和が $K$ など）
 * @docs docs/ds/two_multisets.md
 */

template <class S>
struct TwoMultisetsState
{
  int left_size, right_size;
  S left_prod, right_prod;
  int size() const { return left_size + right_size; }
};

struct PartitionByMedian
{
  template <class S>
  bool operator()(const TwoMultisetsState<S> &s) const { return s.left_size <= (s.size() + 1) / 2; }
};

struct PartitionBySize
{
  int k = 0;
  template <class S>
  bool operator()(const TwoMultisetsState<S> &s) const { assert(k >= 0); return s.left_size <= k; }
};

template <class S>
struct PartitionBySum
{
  S limit;
  bool operator()(const TwoMultisetsState<S> &s) const { return s.left_prod <= limit; }
};

namespace two_multisets_detail
{
// 基底ごとの比較器・集約のテンプレート引数へ変換する。
template <template <class...> class Base, class T, class Compare, class M, class Projection, bool Back>
struct ContainerType
{
  using type = typename priority_container_detail::ContainerForEnd<Base<T, Compare>, Compare, Back>::type;
};

template <class T, class Compare, class M, class Projection, bool Back>
struct ContainerType<priority_queue, T, Compare, M, Projection, Back>
{
  using type = conditional_t<Back, MaxPriorityQueue<T, Compare>, MinPriorityQueue<T, Compare>>;
};

template <class T, class Compare, class M, class Projection, bool Back>
struct ContainerType<OrderedMultisetTree, T, Compare, M, Projection, Back>
{
  using type = OrderedMultisetTree<T, M, Compare, Projection>;
};
} // namespace two_multisets_detail

// 「中央値」の仕様: 奇数のとき .left().back(), 偶数のとき .left().back() と .right().front() の間
// 「上位 K 個」の書き方: TwoMultisets<ll, multiset, PartitionBySize> ms{PartitionBySize{K}};
// 「和が K」の書き方: TwoMultisets<ll, multiset, PartitionBySum<ll>, GroupAddSub<ll>> ms{PartitionBySum<ll>{K}};
template <class T, template <class...> class Base = multiset,
          class Condition = PartitionByMedian, class M = GroupTrivial,
          class Compare = less<T>, class Projection = Identity>
struct TwoMultisets
{
  using value_type = T;
  using monoid_type = M;
  using projection_type = Projection;
  using S = typename M::S;
  using State = TwoMultisetsState<S>;
  using Left = typename two_multisets_detail::ContainerType<Base, T, Compare, M, Projection, true>::type;
  using Right = typename two_multisets_detail::ContainerType<Base, T, Compare, M, Projection, false>::type;
  using LeftContainer = PriorityContainer<Left, M, Projection>;
  using RightContainer = PriorityContainer<Right, M, Projection>;
  static_assert(LeftContainer::supports_back && RightContainer::supports_front,
                "TwoMultisets needs a back endpoint on the left and a front endpoint on the right");
  static constexpr bool supports_erase = LeftContainer::supports_erase && RightContainer::supports_erase;
  static constexpr bool supports_contains = LeftContainer::supports_contains && RightContainer::supports_contains;

private:
  optional<Condition> cond;
  Compare comp;
  LeftContainer lo;
  RightContainer hi;
  State state() const { return {lo.size(), hi.size(), lo.all_prod(), hi.all_prod()}; }
  void move_left_to_right() { hi.push(lo.extract_back()); }
  void move_right_to_left() { lo.push(hi.extract_front()); }
  void check_empty_prefix() const
  {
    assert((*cond)(State{0, size(), M::e(), all_prod()}));
  }
  void balance()
  {
    check_empty_prefix();
    while (!(*cond)(state())) move_left_to_right();
    while (!hi.empty())
    {
      if constexpr (HasInverse<M>::value)
      {
        auto s = state();
        ++s.left_size;
        --s.right_size;
        if constexpr (!is_same_v<M, GroupTrivial>)
        {
          auto x = std::invoke(hi.projection(), hi.front());
          s.left_prod = M::op(s.left_prod, x);
          s.right_prod = M::op(s.right_prod, M::inv(x));
        }
        if (!(*cond)(s)) break;
        move_right_to_left();
      }
      else
      {
        move_right_to_left();
        if (!(*cond)(state())) { move_left_to_right(); break; }
      }
    }
  }

public:
  explicit TwoMultisets(Condition cond = Condition(), Compare comp = Compare(), Projection projection = Projection())
      : cond(std::move(cond)), comp(comp),
        lo(LeftContainer::with_compare(comp, projection)),
        hi(RightContainer::with_compare(comp, projection))
  {
    check_empty_prefix();
  }
  int size() const { return lo.size() + hi.size(); }
  bool empty() const { return size() == 0; }
  S all_prod() const { return M::op(lo.all_prod(), hi.all_prod()); }
  const LeftContainer &left() const { return lo; }
  const RightContainer &right() const { return hi; }
  const Condition &condition() const { return *cond; }
  void set_condition(Condition condition) { cond.emplace(std::move(condition)); balance(); }
  void insert(const T &x)
  {
    if (lo.empty() ? (hi.empty() || !comp(hi.front(), x)) : !comp(lo.back(), x)) lo.push(x);
    else hi.push(x);
    balance();
  }
  template <bool Enabled = supports_erase, enable_if_t<Enabled && supports_erase, int> = 0>
  void erase(const T &x)
  {
    if constexpr (supports_contains)
    {
      if (lo.contains(x)) lo.erase(x);
      else hi.erase(x);
    }
    else
    {
      if (!lo.empty() && !comp(lo.back(), x)) lo.erase(x);
      else hi.erase(x);
    }
    balance();
  }
  template <bool Enabled = supports_contains, enable_if_t<Enabled && supports_contains, int> = 0>
  bool contains(const T &x) const { return lo.contains(x) || hi.contains(x); }
  template <bool Enabled = supports_erase && supports_contains,
            enable_if_t<Enabled && supports_erase && supports_contains, int> = 0>
  bool try_erase(const T &x)
  {
    if (!contains(x)) return false;
    erase(x);
    return true;
  }
  T extract_left() { T x = lo.extract_back(); balance(); return x; }
  T extract_right() { T x = hi.extract_front(); balance(); return x; }
  void clear() { lo.clear(); hi.clear(); check_empty_prefix(); }
};

// 「中央値」の仕様: 奇数のとき .left().back(), 偶数のとき .left().back() と .right().front() の間
// 「上位 K 個」の書き方: SumTwoMultisets<ll, multiset, PartitionBySize> ms{PartitionBySize{K}};
// 「和が K」の書き方: SumTwoMultisets<ll, multiset, PartitionBySum<ll>> ms{PartitionBySum<ll>{K}};
template <class T, template <class...> class Base = multiset,
          class Condition = PartitionByMedian, class Compare = less<T>, class Projection = Identity>
using SumTwoMultisets = TwoMultisets<T, Base, Condition,
    GroupAddSub<decay_t<invoke_result_t<const Projection &, const T &>>>, Compare, Projection>;

template <class T, template <class...> class Base = multiset, class M = GroupTrivial,
          class Condition = PartitionByMedian, class Compare = less<T>, class Projection = Identity>
auto make_two_multisets(Condition cond = Condition(), Compare comp = Compare(), Projection projection = Projection())
{
  return TwoMultisets<T, Base, Condition, M, Compare, Projection>(std::move(cond), std::move(comp), std::move(projection));
}

#ifdef LOCAL
namespace cpp_dump::_detail
{
template <class T, template <class...> class Base, class Condition, class M, class Compare, class Projection>
inline string export_object_generic(
    const TwoMultisets<T, Base, Condition, M, Compare, Projection> &value, const string &indent,
    size_t last_line_length, size_t current_depth, bool fail_on_newline, const export_command &command)
{
  string class_name = es::class_name(get_typename<TwoMultisets<T, Base, Condition, M, Compare, Projection>>());
  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;
  append_output("left", value.left().content());
  append_output("right", value.right().content());
  if constexpr (!is_same_v<M, GroupTrivial>)
  {
    append_output("left_prod", value.left().all_prod());
    append_output("right_prod", value.right().all_prod());
  }
  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;
}
} // namespace cpp_dump::_detail
#endif
