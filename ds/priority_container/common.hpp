#pragma once

#include "aggregate.hpp"
#include "../../utils/reverse_compare.hpp"

/**
 * @brief 優先度付きコンテナの共通操作と可換な集約
 * @docs docs/ds/priority_container.md
 */

namespace priority_container_detail
{
template <class C, class = void>
struct HasErase : false_type {};
template <class C>
struct HasErase<C, void_t<decltype(declval<C &>().erase(declval<const typename C::value_type &>()))>> : true_type {};
template <class C, class = void>
struct HasContains : false_type {};
template <class C>
struct HasContains<C, void_t<decltype(declval<const C &>().contains(declval<const typename C::value_type &>()))>> : true_type {};
template <class C, class = void>
struct HasFront : false_type {};
template <class C>
struct HasFront<C, void_t<decltype(declval<const C &>().front()), decltype(declval<C &>().pop_front())>> : true_type {};
template <class C, class = void>
struct HasBack : false_type {};
template <class C>
struct HasBack<C, void_t<decltype(declval<const C &>().back()), decltype(declval<C &>().pop_back())>> : true_type {};
template <class C, class = void>
struct HasClear : false_type {};
template <class C>
struct HasClear<C, void_t<decltype(declval<C &>().clear())>> : true_type {};
template <class C, class = void>
struct HasContent : false_type {};
template <class C>
struct HasContent<C, void_t<decltype(declval<const C &>().content())>> : true_type {};
template <class C>
struct Access
{
  using T = typename C::value_type;
  static constexpr bool can_erase = HasErase<C>::value;
  static constexpr bool can_contains = HasContains<C>::value;
  static constexpr bool can_front = HasFront<C>::value, can_back = HasBack<C>::value;
  static T front(const C &c) { return c.front(); }
  static T back(const C &c) { return c.back(); }
  static void push(C &c, const T &x) { c.push(x); }
  static void pop_front(C &c) { c.pop_front(); }
  static void pop_back(C &c) { c.pop_back(); }
  static bool contains(const C &c, const T &x) { return c.contains(x); }
  static void erase(C &c, const T &x)
  {
    if constexpr (is_same_v<decltype(c.erase(x)), bool>)
    {
      bool erased = c.erase(x);
      assert(erased);
      (void)erased;
    }
    else c.erase(x);
  }
  static void clear(C &c)
  {
    if constexpr (HasClear<C>::value) c.clear();
    else while (!c.empty())
    {
      if constexpr (can_front) pop_front(c);
      else pop_back(c);
    }
  }
  template <class Compare>
  static C make(const Compare &comp)
  {
    if constexpr (is_constructible_v<C, const Compare &>) return C(comp);
    else
    {
      static_assert(is_empty_v<Compare>, "Pass explicitly configured containers for this comparator");
      return C();
    }
  }
};

// top/pop を持つヒープを、固定した一端の操作として公開する。
template <class C, class Compare, bool Back, bool ReverseInput>
struct SingleEndedQueue
{
  using value_type = typename C::value_type;
private:
  C data;
  static auto raw_compare(const Compare &comp)
  {
    if constexpr (ReverseInput) return reverse_compare(comp);
    else return comp;
  }
public:
  SingleEndedQueue() : SingleEndedQueue(Compare()) {}
  explicit SingleEndedQueue(const Compare &comp) : data(raw_compare(comp)) {}
  template <class It>
  SingleEndedQueue(It first, It last, const Compare &comp = Compare())
      : data(first, last, raw_compare(comp)) {}
  int size() const { return data.size(); }
  bool empty() const { return data.empty(); }
  void push(const value_type &x) { data.push(x); }
  template <bool Enabled = !Back, enable_if_t<Enabled && !Back, int> = 0>
  value_type front() const { assert(!empty()); return data.top(); }
  template <bool Enabled = Back, enable_if_t<Enabled && Back, int> = 0>
  value_type back() const { assert(!empty()); return data.top(); }
  template <bool Enabled = !Back, enable_if_t<Enabled && !Back, int> = 0>
  void pop_front() { assert(!empty()); data.pop(); }
  template <bool Enabled = Back, enable_if_t<Enabled && Back, int> = 0>
  void pop_back() { assert(!empty()); data.pop(); }
  template <bool Enabled = !Back, enable_if_t<Enabled && !Back, int> = 0>
  value_type extract_front() { auto x = front(); pop_front(); return x; }
  template <bool Enabled = Back, enable_if_t<Enabled && Back, int> = 0>
  value_type extract_back() { auto x = back(); pop_back(); return x; }
  template <bool Enabled = HasErase<C>::value, enable_if_t<Enabled && HasErase<C>::value, int> = 0>
  void erase(const value_type &x) { data.erase(x); }
  void clear()
  {
    if constexpr (HasClear<C>::value) data.clear();
    else while (!data.empty()) data.pop();
  }
  vector<value_type> content() const
  {
    vector<value_type> result;
    if constexpr (HasContent<C>::value) result = data.content();
    else
    {
      auto copy = data;
      result.reserve(copy.size());
      while (!copy.empty()) { result.push_back(copy.top()); copy.pop(); }
    }
    if constexpr (Back) reverse(result.begin(), result.end());
    return result;
  }
  const C &container() const { return data; }
};

template <class C, class Compare, bool Back>
struct ContainerForEnd { using type = C; };
template <class C, class Order, bool CurrentBack, bool ReverseInput, class Compare, bool Back>
struct ContainerForEnd<SingleEndedQueue<C, Order, CurrentBack, ReverseInput>, Compare, Back>
    : ContainerForEnd<C, Compare, Back> {};

} // namespace priority_container_detail

template <class Container,
          class M = typename priority_container_detail::NativeMonoid<Container>::type,
          class Projection = typename priority_container_detail::NativeProjection<Container>::type>
struct PriorityContainer : private priority_container_detail::AggregateCache<M,
    priority_container_detail::HasNativeAggregate<Container, M, Projection>::value>
{
  using value_type = typename Container::value_type;
  using monoid_type = M;
  using projection_type = Projection;
  using S = typename M::S;
  static_assert(is_same_v<M, GroupTrivial> ||
                    is_same_v<decay_t<invoke_result_t<const Projection &, const value_type &>>, S>,
                "The projection result and M::S must have the same type");
  static constexpr bool supports_erase = priority_container_detail::Access<Container>::can_erase;
  static constexpr bool supports_contains = priority_container_detail::Access<Container>::can_contains;
  static constexpr bool supports_front = priority_container_detail::Access<Container>::can_front;
  static constexpr bool supports_back = priority_container_detail::Access<Container>::can_back;
  static_assert(supports_front || supports_back, "A priority container needs front/pop_front or back/pop_back");

private:
  using Access = priority_container_detail::Access<Container>;
  static constexpr bool native = priority_container_detail::HasNativeAggregate<Container, M, Projection>::value;
  static_assert(native || HasInverse<M>::value,
                "Aggregation needs M::inv or a container with the same monoid and projection and all_prod");
  Container data;
  Projection proj{};
  static Projection initial_projection(const Container &data)
  {
    if constexpr (is_same_v<Projection, typename priority_container_detail::NativeProjection<Container>::type>)
      return priority_container_detail::NativeProjection<Container>::get(data);
    else return Projection();
  }
  void initialize_aggregate()
  {
    if constexpr (!native && !is_same_v<M, GroupTrivial>)
    {
      auto copy = data;
      while (!copy.empty())
      {
        if constexpr (supports_front) { add(Access::front(copy)); Access::pop_front(copy); }
        else { add(Access::back(copy)); Access::pop_back(copy); }
      }
    }
  }
  void add(const value_type &x)
  {
    if constexpr (!native && !is_same_v<M, GroupTrivial>) this->product = M::op(this->product, std::invoke(projection(), x));
  }
  void remove(const value_type &x)
  {
    if constexpr (!native && !is_same_v<M, GroupTrivial>) this->product = M::op(this->product, M::inv(std::invoke(projection(), x)));
  }
  template <bool Back>
  value_type get_end() const
  {
    assert(!empty());
    if constexpr (Back) return Access::back(data);
    else return Access::front(data);
  }
  template <bool Back>
  void pop_data_end()
  {
    if constexpr (Back) Access::pop_back(data);
    else Access::pop_front(data);
  }
  template <bool Back>
  void pop_end()
  {
    assert(!empty());
    if constexpr (!native && !is_same_v<M, GroupTrivial>)
    {
      auto x = get_end<Back>();
      pop_data_end<Back>();
      remove(x);
    }
    else pop_data_end<Back>();
  }
  template <bool Back>
  value_type extract_end()
  {
    auto x = get_end<Back>();
    pop_data_end<Back>();
    remove(x);
    return x;
  }

public:
  PriorityContainer() = default;
  explicit PriorityContainer(Container data) : data(std::move(data)), proj(initial_projection(this->data))
  {
    initialize_aggregate();
  }
  template <bool Native = native, enable_if_t<!Native && !native, int> = 0>
  PriorityContainer(Container data, Projection projection)
      : data(std::move(data)), proj(std::move(projection)) { initialize_aggregate(); }
  PriorityContainer(const PriorityContainer &) = default;
  PriorityContainer &operator=(const PriorityContainer &) = default;
  PriorityContainer(PriorityContainer &&other) : data(std::move(other.data)), proj(other.proj)
  {
    if constexpr (!native && !is_same_v<M, GroupTrivial>) this->product = std::move(other.product);
    other.clear();
  }
  PriorityContainer &operator=(PriorityContainer &&other)
  {
    if (this != &other)
    {
      proj = other.proj;
      data = std::move(other.data);
      if constexpr (!native && !is_same_v<M, GroupTrivial>) this->product = std::move(other.product);
      other.clear();
    }
    return *this;
  }
  template <class Compare>
  static PriorityContainer with_compare(const Compare &comp, Projection projection = Projection())
  {
    if constexpr (native)
    {
      if constexpr (is_constructible_v<Container, const Compare &, const Projection &>)
        return PriorityContainer(Container(comp, projection));
      else
      {
        static_assert(is_same_v<Projection, Identity>, "The native container must accept the projection");
        return PriorityContainer(Access::make(comp));
      }
    }
    else return PriorityContainer(Access::make(comp), std::move(projection));
  }
  int size() const { return data.size(); }
  bool empty() const { return data.empty(); }
  void push(const value_type &x)
  {
    if constexpr (!native && !is_same_v<M, GroupTrivial>)
    {
      S value = std::invoke(projection(), x);
      Access::push(data, x);
      this->product = M::op(this->product, value);
    }
    else Access::push(data, x);
  }
  template <bool Enabled = supports_front, enable_if_t<Enabled && supports_front, int> = 0>
  value_type front() const { return get_end<false>(); }
  template <bool Enabled = supports_back, enable_if_t<Enabled && supports_back, int> = 0>
  value_type back() const { return get_end<true>(); }
  template <bool Enabled = supports_front, enable_if_t<Enabled && supports_front, int> = 0>
  void pop_front() { pop_end<false>(); }
  template <bool Enabled = supports_back, enable_if_t<Enabled && supports_back, int> = 0>
  void pop_back() { pop_end<true>(); }
  template <bool Enabled = supports_front, enable_if_t<Enabled && supports_front, int> = 0>
  value_type extract_front() { return extract_end<false>(); }
  template <bool Enabled = supports_back, enable_if_t<Enabled && supports_back, int> = 0>
  value_type extract_back() { return extract_end<true>(); }
  template <bool Enabled = supports_erase, enable_if_t<Enabled && supports_erase, int> = 0>
  void erase(const value_type &x)
  {
    if constexpr (!native && !is_same_v<M, GroupTrivial>)
    {
      S value = std::invoke(projection(), x);
      Access::erase(data, x);
      this->product = M::op(this->product, M::inv(value));
    }
    else Access::erase(data, x);
  }
  template <bool Enabled = supports_contains, enable_if_t<Enabled && supports_contains, int> = 0>
  bool contains(const value_type &x) const { return Access::contains(data, x); }
  template <bool Enabled = supports_erase && supports_contains,
            enable_if_t<Enabled && supports_erase && supports_contains, int> = 0>
  bool try_erase(const value_type &x)
  {
    if (!contains(x)) return false;
    erase(x);
    return true;
  }
  S all_prod() const
  {
    if constexpr (native) return data.all_prod();
    else if constexpr (is_same_v<M, GroupTrivial>) return {};
    else return this->product;
  }
  void clear()
  {
    Access::clear(data);
    if constexpr (!native && !is_same_v<M, GroupTrivial>) this->product = M::e();
  }
  vector<value_type> content() const
  {
    if constexpr (priority_container_detail::HasContent<Container>::value) return data.content();
    else
    {
      auto copy = data;
      vector<value_type> result;
      result.reserve(copy.size());
      while (!copy.empty())
      {
        if constexpr (supports_front) { result.push_back(Access::front(copy)); Access::pop_front(copy); }
        else { result.push_back(Access::back(copy)); Access::pop_back(copy); }
      }
      if constexpr (!supports_front) reverse(result.begin(), result.end());
      return result;
    }
  }
  const Container &container() const { return data; }
  const Projection &projection() const { return proj; }
};
