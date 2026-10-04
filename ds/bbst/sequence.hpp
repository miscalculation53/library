#pragma once

#include "treap_base.hpp"

/**
 * @brief 挿入・削除・反転・区間積ができる列
 * @docs docs/ds/bbst/sequence.md
 */

namespace bbst_detail
{
template <class A, bool Lazy>
struct SequenceTreeImpl
{
  using S = typename A::S;
  using Node = TreapNode<A, true, Lazy>;
  using F = typename Node::F;

private:
  using Base = TreapBase<Node>;
  Base tree;
  explicit SequenceTreeImpl(Node *root) : tree(root) {}
  void check_range(int l, int r) const { assert(0 <= l && l <= r && r <= size()); }

public:
  SequenceTreeImpl() = default;
  template <class Iter>
  SequenceTreeImpl(Iter first, Iter last)
  {
    tree.build(first, last, [](const auto &x) { return new Node(S(x)); });
  }
  template <class T>
  explicit SequenceTreeImpl(const vector<T> &v) : SequenceTreeImpl(v.begin(), v.end()) {}
  SequenceTreeImpl(initializer_list<S> v) : SequenceTreeImpl(v.begin(), v.end()) {}
  SequenceTreeImpl(const SequenceTreeImpl &) = delete;
  SequenceTreeImpl &operator=(const SequenceTreeImpl &) = delete;
  SequenceTreeImpl(SequenceTreeImpl &&) noexcept = default;
  SequenceTreeImpl &operator=(SequenceTreeImpl &&) noexcept = default;

  int size() const { return tree.size(); }
  bool empty() const { return size() == 0; }
  void clear() { tree.clear(); }
  S get(int p) const
  {
    assert(0 <= p && p < size());
    return Base::kth(tree.root, p)->value;
  }
  void set(int p, const S &x)
  {
    assert(0 <= p && p < size());
    Base::set(tree.root, p, x);
  }
  void insert(int p, const S &x)
  {
    assert(0 <= p && p <= size());
    tree.insert(p, new Node(x));
  }
  void erase(int p) { assert(0 <= p && p < size()); tree.erase(p, p + 1); }
  void erase(int l, int r) { check_range(l, r); tree.erase(l, r); }
  S prod(int l, int r) const { check_range(l, r); return Base::prod(tree.root, l, r); }
  S all_prod() const { return Node::prod(tree.root); }

  SequenceTreeImpl split(int k)
  {
    assert(0 <= k && k <= size());
    auto [l, r] = Base::split(tree.root, k);
    tree.root = l;
    return SequenceTreeImpl(r);
  }
  void concat(SequenceTreeImpl &&other)
  {
    assert(this != &other);
    tree.root = Base::merge(tree.root, exchange(other.tree.root, nullptr));
  }
  SequenceTreeImpl extract(int l, int r)
  {
    check_range(l, r);
    return SequenceTreeImpl(tree.extract(l, r));
  }
  void reverse(int l, int r)
  {
    check_range(l, r);
    if (l == r) return;
    auto right = split(r), middle = split(l);
    middle.tree.root->reverse();
    concat(std::move(middle));
    concat(std::move(right));
  }
  // [l, r) を取り除いた列の k 番目の直前へ移す。
  void move(int l, int r, int k)
  {
    check_range(l, r);
    assert(0 <= k && k <= size() - (r - l));
    auto middle = extract(l, r), right = split(k);
    concat(std::move(middle));
    concat(std::move(right));
  }
  void rotate(int l, int m, int r)
  {
    check_range(l, r);
    assert(l <= m && m <= r);
    move(l, m, r - (m - l));
  }
  template <bool Enabled = Lazy, enable_if_t<Enabled && Lazy, int> = 0>
  void apply(int l, int r, const F &f)
  {
    check_range(l, r);
    Base::apply(tree.root, l, r, f);
  }
  template <bool Enabled = Lazy, enable_if_t<Enabled && Lazy, int> = 0>
  void apply(int p, const F &f)
  {
    assert(0 <= p && p < size());
    apply(p, p + 1, f);
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
  vector<S> content() const
  {
    vector<S> res;
    res.reserve(size());
    Base::inorder(tree.root, [&](Node *t) { res.push_back(t->value); });
    return res;
  }
};
} // namespace bbst_detail

template <class M>
using SequenceTree = bbst_detail::SequenceTreeImpl<M, false>;
