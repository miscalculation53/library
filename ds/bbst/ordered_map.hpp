#pragma once

#include "treap_base.hpp"

/**
 * @brief キー順・順位区間のモノイド積を持つ map
 * @docs docs/ds/bbst/ordered_map.md
 */

namespace bbst_detail
{
template <class Key, class A, class Compare, bool Lazy>
struct OrderedMapTreeImpl
{
  using S = typename A::S;
  using Node = TreapNode<A, false, Lazy, Key>;
  using F = typename Node::F;

private:
  using Base = TreapBase<Node>;
  Compare cmp;
  Base tree;
  void check_range(int l, int r) const { assert(0 <= l && l <= r && r <= size()); }
  Node *find_node(const Key &key) const
  {
    Node *t = tree.root;
    while (t)
    {
      t->push();
      if (cmp(key, t->key)) t = t->left;
      else if (cmp(t->key, key)) t = t->right;
      else return t;
    }
    return nullptr;
  }
  pair<int, int> key_range(const Key &lo, const Key &hi) const
  {
    assert(!cmp(hi, lo));
    return {order_of_key(lo), order_of_key(hi)};
  }

public:
  OrderedMapTreeImpl() = default;
  explicit OrderedMapTreeImpl(const Compare &cmp) : cmp(cmp) {}
  template <class Iter>
  OrderedMapTreeImpl(Iter first, Iter last, const Compare &cmp = Compare()) : cmp(cmp)
  {
    for (; first != last; ++first) insert(first->first, S(first->second));
  }
  template <class T>
  explicit OrderedMapTreeImpl(const vector<pair<Key, T>> &v, const Compare &cmp = Compare())
      : OrderedMapTreeImpl(v.begin(), v.end(), cmp) {}
  OrderedMapTreeImpl(const OrderedMapTreeImpl &) = delete;
  OrderedMapTreeImpl &operator=(const OrderedMapTreeImpl &) = delete;
  OrderedMapTreeImpl(OrderedMapTreeImpl &&other) noexcept(is_nothrow_copy_constructible_v<Compare>)
      : cmp(other.cmp), tree(std::move(other.tree)) {}
  OrderedMapTreeImpl &operator=(OrderedMapTreeImpl &&other) noexcept(is_nothrow_copy_assignable_v<Compare>)
  {
    if (this != &other)
    {
      cmp = other.cmp;
      tree = std::move(other.tree);
    }
    return *this;
  }

  int size() const { return tree.size(); }
  bool empty() const { return size() == 0; }
  void clear() { tree.clear(); }
  bool contains(const Key &key) const { return find_node(key) != nullptr; }
  S get(const Key &key) const
  {
    Node *t = find_node(key);
    assert(t);
    return t->value;
  }
  pair<Key, S> get_by_order(int k) const
  {
    assert(0 <= k && k < size());
    Node *t = Base::kth(tree.root, k);
    return {t->key, t->value};
  }
  int order_of_key(const Key &key) const
  {
    int res = 0;
    Node *t = tree.root;
    while (t)
      if (cmp(t->key, key)) res += Base::count(t->left) + 1, t = t->right;
      else t = t->left;
    return res;
  }
  int upper_order_of_key(const Key &key) const
  {
    int res = 0;
    Node *t = tree.root;
    while (t)
      if (!cmp(key, t->key)) res += Base::count(t->left) + 1, t = t->right;
      else t = t->left;
    return res;
  }
  bool insert(const Key &key, const S &value)
  {
    int p = order_of_key(key);
    if (p < size() && !cmp(key, Base::kth(tree.root, p)->key)) return false;
    tree.insert(p, new Node(key, value));
    return true;
  }
  bool erase(const Key &key)
  {
    int p = order_of_key(key);
    if (p == size() || cmp(key, Base::kth(tree.root, p)->key)) return false;
    tree.erase(p, p + 1);
    return true;
  }
  void erase_by_order(int k)
  {
    assert(0 <= k && k < size());
    tree.erase(k, k + 1);
  }
  void set(const Key &key, const S &value)
  {
    int p = order_of_key(key);
    assert(p < size() && !cmp(key, Base::kth(tree.root, p)->key));
    Base::set(tree.root, p, value);
  }
  void set_by_order(int k, const S &value)
  {
    assert(0 <= k && k < size());
    Base::set(tree.root, k, value);
  }
  S prod_by_order(int l, int r) const { check_range(l, r); return Base::prod(tree.root, l, r); }
  S prod_by_key(const Key &lo, const Key &hi) const
  {
    auto [l, r] = key_range(lo, hi);
    return prod_by_order(l, r);
  }
  S all_prod() const { return Node::prod(tree.root); }
  OrderedMapTreeImpl split_by_order(int k)
  {
    assert(0 <= k && k <= size());
    OrderedMapTreeImpl other(cmp);
    auto [l, r] = Base::split(tree.root, k);
    tree.root = l;
    other.tree.root = r;
    return other;
  }
  OrderedMapTreeImpl split_by_key(const Key &key) { return split_by_order(order_of_key(key)); }
  void join(OrderedMapTreeImpl &&other)
  {
    assert(this != &other);
    assert(empty() || other.empty() || cmp(Base::kth(tree.root, size() - 1)->key, Base::kth(other.tree.root, 0)->key));
    tree.root = Base::merge(tree.root, exchange(other.tree.root, nullptr));
  }
  template <bool Enabled = Lazy, enable_if_t<Enabled && Lazy, int> = 0>
  void apply_by_order(int l, int r, const F &f)
  {
    check_range(l, r);
    Base::apply(tree.root, l, r, f);
  }
  template <bool Enabled = Lazy, enable_if_t<Enabled && Lazy, int> = 0>
  void apply_by_key(const Key &lo, const Key &hi, const F &f)
  {
    auto [l, r] = key_range(lo, hi);
    apply_by_order(l, r, f);
  }
  template <class G>
  int max_right_ok(int l, const G &g) const
  {
    assert(0 <= l && l <= size());
    S acc = A::e();
    assert(g(acc));
    int p = Base::max_right(tree.root, l, 0, acc, g);
    return p == -1 ? size() : p;
  }
  template <class G>
  int min_left_ok(int r, const G &g) const
  {
    assert(0 <= r && r <= size());
    S acc = A::e();
    assert(g(acc));
    int p = Base::min_left(tree.root, r, 0, acc, g);
    return p == -1 ? 0 : p;
  }
  vector<pair<Key, S>> content() const
  {
    vector<pair<Key, S>> res;
    res.reserve(size());
    Base::inorder(tree.root, [&](Node *t) { res.emplace_back(t->key, t->value); });
    return res;
  }
};
} // namespace bbst_detail

template <class Key, class M, class Compare = less<Key>>
using OrderedMapTree = bbst_detail::OrderedMapTreeImpl<Key, M, Compare, false>;
