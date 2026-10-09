#pragma once

#include "treap_base.hpp"
#include "../../algebra/trivial.hpp"
#include "../../utils/identity.hpp"

/**
 * @brief 順位とモノイド積を持つ multiset
 * @docs docs/ds/bbst/ordered_multiset.md
 */

template <class T, class M = GroupTrivial, class Compare = less<T>, class Projection = Identity>
struct OrderedMultisetTree
{
  using value_type = T;
  using monoid_type = M;
  using projection_type = Projection;
  using S = typename M::S;
  static_assert(is_same_v<M, GroupTrivial> ||
                    is_same_v<decay_t<invoke_result_t<const Projection &, const T &>>, S>,
                "The projection result and M::S must have the same type");

private:
  using Node = bbst_detail::TreapNode<M, false, false, T>;
  using Base = bbst_detail::TreapBase<Node>;
  Base tree;
  Compare comp;
  Projection proj{};

public:
  OrderedMultisetTree() = default;
  explicit OrderedMultisetTree(const Compare &comp, Projection projection = Projection())
      : comp(comp), proj(std::move(projection)) {}
  template <class It>
  OrderedMultisetTree(It first, It last, const Compare &comp = Compare(), Projection projection = Projection())
      : OrderedMultisetTree(comp, std::move(projection))
  {
    for (; first != last; ++first) push(*first);
  }
  OrderedMultisetTree(const OrderedMultisetTree &) = delete;
  OrderedMultisetTree &operator=(const OrderedMultisetTree &) = delete;
  OrderedMultisetTree(OrderedMultisetTree &&other)
      noexcept(is_nothrow_copy_constructible_v<Compare> && is_nothrow_copy_constructible_v<Projection>)
      : tree(std::move(other.tree)), comp(other.comp), proj(other.proj) {}
  OrderedMultisetTree &operator=(OrderedMultisetTree &&other)
      noexcept(is_nothrow_copy_assignable_v<Compare> && is_nothrow_copy_assignable_v<Projection>)
  {
    if (this != &other)
    {
      comp = other.comp;
      proj = other.proj;
      tree = std::move(other.tree);
    }
    return *this;
  }

  int size() const { return tree.size(); }
  bool empty() const { return size() == 0; }
  void clear() { tree.clear(); }
  int order_of_key(const T &x) const
  {
    int res = 0;
    Node *t = tree.root;
    while (t)
      if (comp(t->key, x)) res += Base::count(t->left) + 1, t = t->right;
      else t = t->left;
    return res;
  }
  int upper_order_of_key(const T &x) const
  {
    int res = 0;
    Node *t = tree.root;
    while (t)
      if (!comp(x, t->key)) res += Base::count(t->left) + 1, t = t->right;
      else t = t->left;
    return res;
  }
  T get_by_order(int k) const
  {
    assert(0 <= k && k < size());
    return Base::kth(tree.root, k)->key;
  }
  int count(const T &x) const { return upper_order_of_key(x) - order_of_key(x); }
  bool contains(const T &x) const
  {
    int k = order_of_key(x);
    return k < size() && !comp(x, get_by_order(k));
  }
  void push(const T &x)
  {
    if constexpr (is_same_v<M, GroupTrivial>) tree.insert(upper_order_of_key(x), new Node(x, M::e()));
    else tree.insert(upper_order_of_key(x), new Node(x, std::invoke(projection(), x)));
  }
  void insert(const T &x) { push(x); }
  bool erase(const T &x)
  {
    int k = order_of_key(x);
    if (k == size() || comp(x, get_by_order(k))) return false;
    tree.erase(k, k + 1);
    return true;
  }
  void erase_by_order(int k)
  {
    assert(0 <= k && k < size());
    tree.erase(k, k + 1);
  }
  T front() const { return get_by_order(0); }
  T back() const { return get_by_order(size() - 1); }
  void pop_front() { erase_by_order(0); }
  void pop_back() { erase_by_order(size() - 1); }
  S all_prod() const { return Node::prod(tree.root); }
  const Projection &projection() const { return proj; }
  S prod_by_order(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= size());
    return Base::prod(tree.root, l, r);
  }
  S prod_by_key(const T &lo, const T &hi) const
  {
    assert(!comp(hi, lo));
    return prod_by_order(order_of_key(lo), order_of_key(hi));
  }
  OrderedMultisetTree split_by_order(int k)
  {
    assert(0 <= k && k <= size());
    OrderedMultisetTree other(comp, proj);
    auto [l, r] = Base::split(tree.root, k);
    tree.root = l;
    other.tree.root = r;
    return other;
  }
  OrderedMultisetTree split_by_key(const T &x) { return split_by_order(order_of_key(x)); }
  void join(OrderedMultisetTree &&other)
  {
    assert(this != &other);
    assert(empty() || other.empty() || !comp(other.front(), back()));
    tree.root = Base::merge(tree.root, exchange(other.tree.root, nullptr));
  }
  vector<T> content() const
  {
    vector<T> res;
    res.reserve(size());
    Base::inorder(tree.root, [&](Node *t) { res.push_back(t->key); });
    return res;
  }
};
