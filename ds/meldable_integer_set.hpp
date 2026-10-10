#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 併合可能な疎な整数集合の共有ノードプール
 * @docs docs/ds/meldable_integer_set.md
 */

struct MeldableIntegerSetPool
{
  class Set;

private:
  using Root = int;
  static constexpr Root null = -1;
  struct Node
  {
    ll count;
    union
    {
      ull bits;
      array<Root, 2> child;
    };
    explicit Node(ull bits) : count(__builtin_popcountll(bits)), bits(bits) {}
    Node(Root left, Root right, ll count) : count(count), child{left, right} {}
  };

  ll n = 0;
  int height = 0;
  vc<Node> nodes;

  static ull low_mask(int k) { return k == 64 ? ~0ULL : (1ULL << k) - 1; }
  ll count_of(Root root) const { return root == null ? 0 : nodes[root].count; }
  void check_root(Root root) const
  {
    assert(root == null || (0 <= root && root < SZ<int>(nodes)));
  }
  void pull(Root root)
  {
    nodes[root].count = count_of(nodes[root].child[0]) + count_of(nodes[root].child[1]);
  }

  Root make_path(ull key, int h)
  {
    Root root = nodes.size();
    nodes.eb(1ULL << (key & 63));
    key >>= 6;
    repi(i, h)
    {
      Root parent = nodes.size();
      if ((key >> i) & 1) nodes.eb(null, root, 1);
      else nodes.eb(root, null, 1);
      root = parent;
    }
    return root;
  }

  Root insert_impl(Root root, ull key, int h, bool &inserted)
  {
    if (root == null)
    {
      inserted = true;
      return make_path(key, h);
    }
    if (h == 0)
    {
      ull bit = 1ULL << (key & 63);
      inserted = !(nodes[root].bits & bit);
      nodes[root].bits |= bit;
      nodes[root].count += inserted;
      return root;
    }
    int b = (key >> (h + 5)) & 1;
    Root child = insert_impl(nodes[root].child[b], key, h - 1, inserted);
    nodes[root].child[b] = child;
    pull(root);
    return root;
  }

  Root erase_impl(Root root, ull key, int h, bool &erased)
  {
    if (root == null) return null;
    if (h == 0)
    {
      ull bit = 1ULL << (key & 63);
      erased = (nodes[root].bits & bit) != 0;
      nodes[root].bits &= ~bit;
      nodes[root].count -= erased;
    }
    else
    {
      int b = (key >> (h + 5)) & 1;
      nodes[root].child[b] = erase_impl(nodes[root].child[b], key, h - 1, erased);
      pull(root);
    }
    return nodes[root].count == 0 ? null : root;
  }

  Root merge_impl(Root a, Root b, int h)
  {
    if (a == null) return b;
    if (b == null) return a;
    if (h == 0)
    {
      nodes[a].bits |= nodes[b].bits;
      nodes[a].count = __builtin_popcountll(nodes[a].bits);
    }
    else
    {
      nodes[a].child[0] = merge_impl(nodes[a].child[0], nodes[b].child[0], h - 1);
      nodes[a].child[1] = merge_impl(nodes[a].child[1], nodes[b].child[1], h - 1);
      pull(a);
    }
    return a;
  }

  Root clone_impl(Root root, int h)
  {
    if (root == null) return null;
    Node node = nodes[root];
    Root res = nodes.size();
    nodes.eb(node);
    if (h > 0)
    {
      Root left = clone_impl(node.child[0], h - 1);
      Root right = clone_impl(node.child[1], h - 1);
      nodes[res].child = {left, right};
    }
    return res;
  }

  template <class F>
  void enumerate_impl(Root root, int h, ull prefix, ull l, ull r, const F &f) const
  {
    if (root == null) return;
    ull a = prefix << (h + 6), b = a + (1ULL << (h + 6));
    if (b <= l || r <= a) return;
    if (h == 0)
    {
      int lo = max(a, l) - a, hi = min(b, r) - a;
      ull bits = nodes[root].bits & low_mask(hi) & ~low_mask(lo);
      while (bits)
      {
        f(ll(a + __builtin_ctzll(bits)));
        bits &= bits - 1;
      }
      return;
    }
    enumerate_impl(nodes[root].child[0], h - 1, prefix << 1, l, r, f);
    enumerate_impl(nodes[root].child[1], h - 1, (prefix << 1) | 1, l, r, f);
  }

public:
  explicit MeldableIntegerSetPool(ll n = 0, size_t expected_insertions = 0) : n(n)
  {
    assert(n >= 0);
    ull blocks = (ull(n) + 63) / 64;
    while ((1ULL << height) < blocks) height++;
    nodes.reserve(expected_insertions * (height + 1));
  }

  ll universe_size() const { return n; }
  size_t node_count() const { return nodes.size(); }
  void reserve(size_t count) { nodes.reserve(count); }

private:
  ll size(Root root) const { check_root(root); return count_of(root); }
  bool empty(Root root) const { check_root(root); return root == null; }

  Root singleton(ll key)
  {
    assert(0 <= key && key < n);
    return make_path(ull(key), height);
  }

  bool contains(Root root, ll key) const
  {
    check_root(root);
    assert(0 <= key && key < n);
    for (int h = height; h > 0 && root != null; h--)
      root = nodes[root].child[(ull(key) >> (h + 5)) & 1];
    return root != null && ((nodes[root].bits >> (key & 63)) & 1);
  }

  bool insert(Root &root, ll key)
  {
    check_root(root);
    assert(0 <= key && key < n);
    bool inserted = false;
    root = insert_impl(root, ull(key), height, inserted);
    return inserted;
  }

  bool erase(Root &root, ll key)
  {
    check_root(root);
    assert(0 <= key && key < n);
    bool erased = false;
    root = erase_impl(root, ull(key), height, erased);
    return erased;
  }

  // a を和集合の根に更新し、b を空にする。
  void merge(Root &a, Root &b)
  {
    check_root(a), check_root(b);
    if (&a == &b) return;
    assert(a == null || b == null || a != b);
    a = merge_impl(a, b, height);
    b = null;
  }

  Root clone(Root root) { check_root(root); return clone_impl(root, height); }

  // key 未満の要素数。key は値域の外でもよい。
  ll lt_cnt(Root root, ll key) const
  {
    check_root(root);
    if (key <= 0) return 0;
    if (key >= n) return count_of(root);
    ll res = 0;
    for (int h = height; h > 0 && root != null; h--)
    {
      int b = (ull(key) >> (h + 5)) & 1;
      if (b) res += count_of(nodes[root].child[0]);
      root = nodes[root].child[b];
    }
    if (root != null) res += __builtin_popcountll(nodes[root].bits & low_mask(key & 63));
    return res;
  }
  ll leq_cnt(Root root, ll key) const { return key >= n - 1 ? size(root) : lt_cnt(root, key + 1); }
  ll geq_cnt(Root root, ll key) const { return size(root) - lt_cnt(root, key); }
  ll gt_cnt(Root root, ll key) const { return size(root) - leq_cnt(root, key); }

  ll count(Root root, ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= n);
    return lt_cnt(root, r) - lt_cnt(root, l);
  }

  // 昇順で k 番目の要素。k は 0 始まり。
  ll kth(Root root, ll k) const
  {
    check_root(root);
    assert(0 <= k && k < count_of(root));
    ull block = 0;
    for (int h = height; h > 0; h--)
    {
      ll left = count_of(nodes[root].child[0]);
      int b = k >= left;
      if (b) k -= left;
      root = nodes[root].child[b];
      block = (block << 1) | b;
    }
    ull bits = nodes[root].bits;
    int offset = 0;
    for (int step = 32; step > 0; step >>= 1)
    {
      int left = __builtin_popcountll(bits & low_mask(step));
      if (k >= left)
      {
        k -= left;
        bits >>= step;
        offset += step;
      }
    }
    return ll((block << 6) + offset);
  }

  ll geq_min(Root root, ll key) const
  {
    ll k = lt_cnt(root, key);
    return k == size(root) ? n : kth(root, k);
  }
  ll leq_max(Root root, ll key) const
  {
    ll k = leq_cnt(root, key);
    return k == 0 ? -1 : kth(root, k - 1);
  }
  ll gt_min(Root root, ll key) const { return key >= n - 1 ? n : geq_min(root, key + 1); }
  ll lt_max(Root root, ll key) const { return key <= 0 ? -1 : leq_max(root, key - 1); }
  ll min_element(Root root) const { return geq_min(root, 0); }
  ll max_element(Root root) const { return leq_max(root, n - 1); }

  template <class F>
  void enumerate(Root root, ll l, ll r, const F &f) const
  {
    check_root(root);
    assert(0 <= l && l <= r && r <= n);
    if (l < r) enumerate_impl(root, height, 0, ull(l), ull(r), f);
  }

  vc<ll> content(Root root) const
  {
    vc<ll> res;
    res.reserve(size(root));
    enumerate(root, 0, n, [&](ll key) { res.eb(key); });
    return res;
  }

  class const_iterator
  {
    friend class Set;
    const MeldableIntegerSetPool *sets = nullptr;
    Root root = null;
    ll key = 0;
    const_iterator(const MeldableIntegerSetPool *sets, Root root, ll key) : sets(sets), root(root), key(key) {}

  public:
    using iterator_category = input_iterator_tag;
    using value_type = ll;
    using difference_type = ptrdiff_t;
    using reference = ll;
    using pointer = const ll *;
    const_iterator() = default;
    ll operator*() const { return key; }
    const ll *operator->() const { return &key; }
    const_iterator &operator++() { key = sets->gt_min(root, key); return *this; }
    const_iterator operator++(int) { auto old = *this; ++*this; return old; }
    bool operator==(const const_iterator &other) const { return sets == other.sets && root == other.root && key == other.key; }
    bool operator!=(const const_iterator &other) const { return !(*this == other); }
  };

  // 根だけを空にする。ノード領域はプールに残す。
  void clear(Root &root) { check_root(root); root = null; }

public:
  // プールへの参照と根を持つ集合ハンドル。複製には clone を使う。
  class Set
  {
    friend struct MeldableIntegerSetPool;
    MeldableIntegerSetPool *sets;
    Root root;
    Set(MeldableIntegerSetPool *sets, Root root) : sets(sets), root(root) {}

  public:
    Set(const Set &) = delete;
    Set &operator=(const Set &) = delete;
    Set(Set &&other) noexcept : sets(other.sets), root(exchange(other.root, null)) {}
    Set &operator=(Set &&other) noexcept
    {
      if (this != &other)
      {
        sets = other.sets;
        root = exchange(other.root, null);
      }
      return *this;
    }

    ll universe_size() const { return sets->universe_size(); }
    ll size() const { return sets->size(root); }
    bool empty() const { return sets->empty(root); }
    bool contains(ll key) const { return sets->contains(root, key); }
    bool insert(ll key) { return sets->insert(root, key); }
    bool erase(ll key) { return sets->erase(root, key); }
    void merge(Set &other)
    {
      assert(sets == other.sets);
      sets->merge(root, other.root);
    }
    Set clone() const { return Set(sets, sets->clone(root)); }

    ll lt_cnt(ll key) const { return sets->lt_cnt(root, key); }
    ll leq_cnt(ll key) const { return sets->leq_cnt(root, key); }
    ll geq_cnt(ll key) const { return sets->geq_cnt(root, key); }
    ll gt_cnt(ll key) const { return sets->gt_cnt(root, key); }
    ll count(ll l, ll r) const { return sets->count(root, l, r); }
    ll kth(ll k) const { return sets->kth(root, k); }
    ll geq_min(ll key) const { return sets->geq_min(root, key); }
    ll gt_min(ll key) const { return sets->gt_min(root, key); }
    ll leq_max(ll key) const { return sets->leq_max(root, key); }
    ll lt_max(ll key) const { return sets->lt_max(root, key); }
    ll min_element() const { return sets->min_element(root); }
    ll max_element() const { return sets->max_element(root); }

    template <class F>
    void enumerate(ll l, ll r, const F &f) const { sets->enumerate(root, l, r, f); }
    vc<ll> content() const { return sets->content(root); }
    using iterator = MeldableIntegerSetPool::const_iterator;
    using const_iterator = MeldableIntegerSetPool::const_iterator;
    iterator begin() const { return iterator(sets, root, min_element()); }
    iterator end() const { return iterator(sets, root, universe_size()); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    void clear() { sets->clear(root); }

    void swap(Set &other) noexcept
    {
      std::swap(sets, other.sets), std::swap(root, other.root);
    }
    friend void swap(Set &a, Set &b) noexcept { a.swap(b); }
  };

  Set make_set() { return Set(this, null); }
  Set make_set(ll key) { return Set(this, singleton(key)); }

  // 全ての根を無効にし、確保済みの容量を再利用する。
  void clear() { nodes.clear(); }
};

using MeldableIntegerSet = MeldableIntegerSetPool::Set;
