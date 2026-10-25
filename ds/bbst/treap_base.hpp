#pragma once

#include "../../algebra/algebra_base.hpp"

/**
 * @brief 平衡二分木の共通実装（Treap）
 * @docs docs/ds/bbst/treap_base.md
 */

namespace bbst_detail
{
inline mt19937_64 &treap_random()
{
  static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
  return rng;
}

struct NoKey {};
template <class Key>
struct KeyData
{
  Key key;
  explicit KeyData(const Key &key) : key(key) {}
};
template <>
struct KeyData<NoKey> {};

template <bool Reversible>
struct ReverseData {};
template <>
struct ReverseData<true>
{
  bool reversed = false;
};

template <class A, bool Lazy>
struct LazyData
{
  struct F {};
};
template <class A>
struct LazyData<A, true>
{
  using F = typename A::F;
  F lazy = A::id();
  bool has_lazy = false;
};

template <class A, bool Reversible, bool Lazy, class Key = NoKey>
struct TreapNode : KeyData<Key>, ReverseData<Reversible>, LazyData<A, Lazy>
{
  using S = typename A::S;
  using F = typename LazyData<A, Lazy>::F;
  using ProductMonoid = conditional_t<Reversible, NormalAndOppositeMonoid<A>, A>;
  using Product = typename ProductMonoid::S;
  TreapNode *left = nullptr, *right = nullptr;
  uint64_t priority = treap_random()();
  int size = 1;
  S value;
  Product product;

  explicit TreapNode(const S &value) : value(value), product(value) {}
  TreapNode(const Key &key, const S &value) : KeyData<Key>(key), value(value), product(value) {}

  static int count(const TreapNode *t) { return t ? t->size : 0; }
  static Product node_product(const TreapNode *t) { return t ? t->product : ProductMonoid::e(); }
  static S prod(const TreapNode *t)
  {
    if (!t) return A::e();
    if constexpr (Reversible) return t->product.normal;
    else return t->product;
  }
  static S op(const S &x, const S &y) { return A::op(x, y); }
  static S e() { return A::e(); }

  void pull()
  {
    size = count(left) + 1 + count(right);
    product = ProductMonoid::op(ProductMonoid::op(node_product(left), Product(value)), node_product(right));
  }
  void reverse()
  {
    static_assert(Reversible);
    swap(left, right);
    swap(product.normal, product.opposite);
    this->reversed ^= true;
  }
  void apply(const F &f)
  {
    static_assert(Lazy);
    value = A::mapping(f, value);
    if constexpr (Reversible)
    {
      product.normal = A::mapping(f, product.normal);
      product.opposite = A::mapping(f, product.opposite);
    }
    else product = A::mapping(f, product);
    this->lazy = this->has_lazy ? A::composition(f, this->lazy) : f;
    this->has_lazy = true;
  }
  void apply_value(const F &f) { value = A::mapping(f, value); }
  void push()
  {
    if constexpr (Reversible)
      if (this->reversed)
      {
        if (left) left->reverse();
        if (right) right->reverse();
        this->reversed = false;
      }
    if constexpr (Lazy)
      if (this->has_lazy)
      {
        if (left) left->apply(this->lazy);
        if (right) right->apply(this->lazy);
        this->lazy = A::id();
        this->has_lazy = false;
      }
  }
};

template <class Node>
struct TreapBase
{
  using S = typename Node::S;
  using F = typename Node::F;
  Node *root = nullptr;

  TreapBase() = default;
  explicit TreapBase(Node *root) : root(root) {}
  TreapBase(const TreapBase &) = delete;
  TreapBase &operator=(const TreapBase &) = delete;
  TreapBase(TreapBase &&other) noexcept : root(exchange(other.root, nullptr)) {}
  TreapBase &operator=(TreapBase &&other) noexcept
  {
    if (this != &other)
    {
      clear();
      root = exchange(other.root, nullptr);
    }
    return *this;
  }
  ~TreapBase() { clear(); }

  static int count(const Node *t) { return Node::count(t); }
  int size() const { return count(root); }
  static void destroy(Node *t)
  {
    // 回転で左の子を取り除き、追加メモリ O(1) で解放する。
    while (t)
      if (t->left)
      {
        Node *l = t->left;
        t->left = l->right;
        l->right = t;
        t = l;
      }
      else
      {
        Node *r = t->right;
        delete t;
        t = r;
      }
  }
  void clear() { destroy(exchange(root, nullptr)); }

  static pair<Node *, Node *> split(Node *t, int k)
  {
    if (!t) return {nullptr, nullptr};
    if (k == 0) return {nullptr, t};
    if (k == count(t)) return {t, nullptr};
    t->push();
    int n = count(t->left);
    if (k <= n)
    {
      auto [l, r] = split(t->left, k);
      t->left = r;
      t->pull();
      return {l, t};
    }
    auto [l, r] = split(t->right, k - n - 1);
    t->right = l;
    t->pull();
    return {t, r};
  }
  static Node *merge(Node *l, Node *r)
  {
    if (!l) return r;
    if (!r) return l;
    if (l->priority > r->priority)
    {
      l->push();
      l->right = merge(l->right, r);
      l->pull();
      return l;
    }
    r->push();
    r->left = merge(l, r->left);
    r->pull();
    return r;
  }

  template <class Iter, class MakeNode>
  void build(Iter first, Iter last, MakeNode make_node)
  {
    clear();
    vector<Node *> path;
    for (; first != last; ++first)
    {
      Node *t = make_node(*first), *l = nullptr;
      while (!path.empty() && path.back()->priority < t->priority)
      {
        l = path.back();
        l->pull();
        path.pop_back();
      }
      t->left = l;
      if (path.empty()) root = t;
      else path.back()->right = t;
      path.push_back(t);
    }
    while (!path.empty())
    {
      path.back()->pull();
      path.pop_back();
    }
  }

  static Node *kth(Node *t, int k)
  {
    while (true)
    {
      t->push();
      int n = count(t->left);
      if (k == n) return t;
      if (k < n) t = t->left;
      else k -= n + 1, t = t->right;
    }
  }
  static void set(Node *t, int k, const S &x)
  {
    t->push();
    int n = count(t->left);
    if (k < n) set(t->left, k, x);
    else if (k == n) t->value = x;
    else set(t->right, k - n - 1, x);
    t->pull();
  }
  void insert(int k, Node *t)
  {
    auto [l, r] = split(root, k);
    root = merge(merge(l, t), r);
  }
  Node *extract(int l, int r)
  {
    auto [a, bc] = split(root, l);
    auto [b, c] = split(bc, r - l);
    root = merge(a, c);
    return b;
  }
  void erase(int l, int r) { destroy(extract(l, r)); }

  static S prod(Node *t, int l, int r)
  {
    if (!t || l == r) return Node::e();
    if (l == 0 && r == count(t)) return Node::prod(t);
    t->push();
    int n = count(t->left);
    S res = Node::e();
    if (l < n) res = prod(t->left, l, min(r, n));
    if (l <= n && n < r) res = Node::op(res, t->value);
    if (n + 1 < r) res = Node::op(res, prod(t->right, max(0, l - n - 1), r - n - 1));
    return res;
  }
  static void apply(Node *t, int l, int r, const F &f)
  {
    if (!t || l == r) return;
    if (l == 0 && r == count(t)) return t->apply(f);
    t->push();
    int n = count(t->left);
    if (l < n) apply(t->left, l, min(r, n), f);
    if (l <= n && n < r) t->apply_value(f);
    if (n + 1 < r) apply(t->right, max(0, l - n - 1), r - n - 1, f);
    t->pull();
  }

  template <class G>
  static int max_right(Node *t, int l, int offset, S &acc, const G &g)
  {
    if (!t || offset + count(t) <= l) return -1;
    if (l <= offset)
    {
      S x = Node::op(acc, Node::prod(t));
      if (g(x)) { acc = std::move(x); return -1; }
    }
    t->push();
    int p = offset + count(t->left);
    int res = max_right(t->left, l, offset, acc, g);
    if (res != -1) return res;
    if (l <= p)
    {
      S x = Node::op(acc, t->value);
      if (!g(x)) return p;
      acc = std::move(x);
    }
    return max_right(t->right, l, p + 1, acc, g);
  }
  template <class G>
  static int min_left(Node *t, int r, int offset, S &acc, const G &g)
  {
    if (!t || r <= offset) return -1;
    if (offset + count(t) <= r)
    {
      S x = Node::op(Node::prod(t), acc);
      if (g(x)) { acc = std::move(x); return -1; }
    }
    t->push();
    int p = offset + count(t->left);
    int res = min_left(t->right, r, p + 1, acc, g);
    if (res != -1) return res;
    if (p < r)
    {
      S x = Node::op(t->value, acc);
      if (!g(x)) return p + 1;
      acc = std::move(x);
    }
    return min_left(t->left, r, offset, acc, g);
  }
  template <class G>
  static void inorder(Node *t, const G &g)
  {
    if (!t) return;
    t->push();
    inorder(t->left, g);
    g(t);
    inorder(t->right, g);
  }
};
} // namespace bbst_detail
