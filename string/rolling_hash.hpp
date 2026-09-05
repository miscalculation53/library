#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../math/modint/modint.hpp"
#include "../math/modint/power_table.hpp"

/**
 * @brief Rolling Hash
 * @docs docs/string/rolling_hash.md
 */

template <class V, class mint, int id>
struct RollingHash;
template <class V, class mint, int id>
struct RollingHashConcat;
template <class Sequence>
struct RollingHashRange;
template <class L, class R>
struct RollingHashConcatView;

template <class T>
inline constexpr bool is_rolling_hash_view = false;
template <class Sequence>
inline constexpr bool is_rolling_hash_view<RollingHashRange<Sequence>> = true;
template <class L, class R>
inline constexpr bool is_rolling_hash_view<RollingHashConcatView<L, R>> = true;

template <class mint = modint61, int id = INT_MIN>
struct RollingHashBase
{
  static mint &base()
  {
    static mint val = []
    {
      mint x = local_oj(1000, mt());
      return x == mint(0) ? mint(1) : x;
    }();
    return val;
  }
  static mint inv_base()
  {
    static mint val = base().inv();
    return val;
  }
  static mint pow(int n)
  {
    static PowerTable<mint> pw(base());
    return pw.pow(n);
  }
  static mint inv_pow(int n)
  {
    static PowerTable<mint> pw(inv_base());
    return pw.pow(n);
  }
};

template <class mint = modint61, int id = INT_MIN>
struct RollingHashValue
{
private:
  mint val, pw, ipw;
  ll len;

  RollingHashValue(mint val, mint pw, mint ipw, ll len) : val(val), pw(pw), ipw(ipw), len(len) {}

  template <class V, class M, int i>
  friend struct RollingHash;

public:
  RollingHashValue() : val(0), pw(1), ipw(1), len(0) {}
  template <class V>
  RollingHashValue(const V &s) : RollingHashValue()
  {
    const mint b = base(), ib = RollingHashBase<mint, id>::inv_base();
    for (const auto &x : s)
    {
      val = val * b + x;
      pw *= b;
      ipw *= ib;
      len++;
    }
  }

  static mint base() { return RollingHashBase<mint, id>::base(); }
  mint value() const { return val; }
  ll size() const { return len; }

  RollingHashValue &operator+=(const RollingHashValue &rhs)
  {
    val = val * rhs.pw + rhs.val;
    pw *= rhs.pw;
    ipw *= rhs.ipw;
    len += rhs.len;
    return *this;
  }
  friend RollingHashValue operator+(RollingHashValue lhs, const RollingHashValue &rhs) { return lhs += rhs; }

  RollingHashValue remove_prefix(const RollingHashValue &prefix) const
  {
    assert(prefix.len <= len);
    mint suffix_pw = pw * prefix.ipw;
    return {val - prefix.val * suffix_pw, suffix_pw, ipw * prefix.pw, len - prefix.len};
  }

  friend bool operator==(const RollingHashValue &lhs, const RollingHashValue &rhs)
  {
    return lhs.len == rhs.len && lhs.val == rhs.val;
  }
  friend bool operator!=(const RollingHashValue &lhs, const RollingHashValue &rhs) { return !(lhs == rhs); }
  friend auto safe_hash_key(const RollingHashValue &x) { return pair{x.val, x.len}; }

  CPP_DUMP_DEFINE_DATA(val, len);
};

template <class V>
RollingHashValue(const V &) -> RollingHashValue<>;

template <class Derived>
struct RollingHashSequenceOps
{
private:
  const Derived &self() const { return static_cast<const Derived &>(*this); }

public:
  auto content() const
  {
    using T = remove_cv_t<typename Derived::element_type>;
    using V = conditional_t<is_same_v<T, char>, string, vc<T>>;
    V res;
    res.reserve(self().size());
    for (ll i = 0; i < self().size(); i++) res.push_back(self().get(i));
    return res;
  }

  RollingHashRange<Derived> substr(ll l, ll r) const &;
  RollingHashRange<Derived> substr(ll l = 0) const &;
  RollingHashRange<Derived> substr(ll, ll) const && = delete;
  RollingHashRange<Derived> substr(ll = 0) const && = delete;

  template <class Other>
  ll lcp(const Other &rhs) const
  {
    ll ok = 0, ng = min(self().size(), rhs.size()) + 1;
    while (ng - ok > 1)
    {
      ll mid = (ok + ng) / 2;
      (self().hash(0, mid) == rhs.hash(0, mid) ? ok : ng) = mid;
    }
    return ok;
  }

  template <class Other>
  ll lcs(const Other &rhs) const
  {
    ll ok = 0, ng = min(self().size(), rhs.size()) + 1;
    while (ng - ok > 1)
    {
      ll mid = (ok + ng) / 2;
      (self().hash(self().size() - mid, self().size()) == rhs.hash(rhs.size() - mid, rhs.size()) ? ok : ng) = mid;
    }
    return ok;
  }

  template <class Other>
  int compare(const Other &rhs) const
  {
    ll k = lcp(rhs);
    if (k == min(self().size(), rhs.size()))
      return self().size() < rhs.size() ? -1 : self().size() > rhs.size() ? 1 : 0;
    if (self().get(k) < rhs.get(k)) return -1;
    if (rhs.get(k) < self().get(k)) return 1;
    return 0;
  }

  template <class Other>
  bool operator==(const Other &rhs) const { return self().size() == rhs.size() && self().hash() == rhs.hash(); }
  template <class Other>
  bool operator!=(const Other &rhs) const { return !(*this == rhs); }
  template <class Other>
  bool operator<(const Other &rhs) const { return compare(rhs) < 0; }
  template <class Other>
  bool operator<=(const Other &rhs) const { return compare(rhs) <= 0; }
  template <class Other>
  bool operator>(const Other &rhs) const { return compare(rhs) > 0; }
  template <class Other>
  bool operator>=(const Other &rhs) const { return compare(rhs) >= 0; }
};

template <class Sequence>
struct RollingHashRange : RollingHashSequenceOps<RollingHashRange<Sequence>>
{
private:
  const Sequence *seq;
  ll l, r;

  template <class V, class mint, int id>
  friend struct RollingHashConcat;

public:
  using element_type = typename Sequence::element_type;

  RollingHashRange(const Sequence &seq, ll l, ll r) : seq(&seq), l(l), r(r)
  {
    assert(0 <= l && l <= r && r <= seq.size());
  }

  ll size() const { return r - l; }
  auto hash() const { return seq->hash(l, r); }
  auto hash(ll a, ll b) const
  {
    assert(0 <= a && a <= b && b <= size());
    return seq->hash(l + a, l + b);
  }
  decltype(auto) get(ll i) const
  {
    assert(0 <= i && i < size());
    return seq->get(l + i);
  }
};

template <class Derived>
RollingHashRange<Derived> RollingHashSequenceOps<Derived>::substr(ll l, ll r) const &
{
  return {self(), l, r};
}

template <class Derived>
RollingHashRange<Derived> RollingHashSequenceOps<Derived>::substr(ll l) const &
{
  return {self(), l, self().size()};
}

template <class L, class R>
struct RollingHashConcatView : RollingHashSequenceOps<RollingHashConcatView<L, R>>
{
private:
  L lhs;
  R rhs;

  template <class V, class mint, int id>
  friend struct RollingHashConcat;

public:
  using element_type = common_type_t<typename L::element_type, typename R::element_type>;

  RollingHashConcatView(L lhs, R rhs) : lhs(lhs), rhs(rhs) {}

  ll size() const { return lhs.size() + rhs.size(); }
  auto hash() const { return hash(0, size()); }
  auto hash(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    if (r <= lhs.size()) return lhs.hash(l, r);
    if (lhs.size() <= l) return rhs.hash(l - lhs.size(), r - lhs.size());
    return lhs.hash(l, lhs.size()) + rhs.hash(0, r - lhs.size());
  }
  decltype(auto) get(ll i) const
  {
    assert(0 <= i && i < size());
    return i < lhs.size() ? lhs.get(i) : rhs.get(i - lhs.size());
  }
};

template <class L, class R, enable_if_t<is_rolling_hash_view<L> && is_rolling_hash_view<R>, int> = 0>
auto operator+(const L &lhs, const R &rhs)
{
  return RollingHashConcatView<L, R>(lhs, rhs);
}

template <class V = string, class mint = modint61, int id = INT_MIN>
struct RollingHash : RollingHashSequenceOps<RollingHash<V, mint, id>>
{
private:
  const V *s;
  vc<mint> sm;

public:
  using element_type = typename V::value_type;

  explicit RollingHash(const V &s) : s(&s), sm(s.size() + 1)
  {
    assert(s.size() <= INT_MAX);
    for (ll i = 0; i < (ll)s.size(); i++) sm[i + 1] = sm[i] * base() + s[i];
    RollingHashBase<mint, id>::pow(s.size());
    RollingHashBase<mint, id>::inv_pow(s.size());
  }
  RollingHash(V &&) = delete;

  static mint base() { return RollingHashBase<mint, id>::base(); }
  ll size() const { return s->size(); }
  RollingHashValue<mint, id> hash() const { return hash(0, size()); }
  RollingHashValue<mint, id> hash(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    int len = r - l;
    mint pw = RollingHashBase<mint, id>::pow(len);
    return {sm[r] - sm[l] * pw, pw, RollingHashBase<mint, id>::inv_pow(len), len};
  }
  decltype(auto) get(ll i) const
  {
    assert(0 <= i && i < size());
    return (*s)[i];
  }
};

template <class V>
RollingHash(const V &) -> RollingHash<V>;

template <class V = string, class mint = modint61, int id = INT_MIN>
struct RollingHashConcat : RollingHashSequenceOps<RollingHashConcat<V, mint, id>>
{
private:
  using RH = RollingHash<V, mint, id>;
  using Value = RollingHashValue<mint, id>;
  struct Block
  {
    const RH *src;
    int l, r;
    Value pref;
  };

  vc<Block> blocks;

  void append_range(const RH &src, ll l, ll r) { append(src, l, r); }

  template <class Sequence>
  void append_range(const RollingHashRange<Sequence> &src, ll l, ll r)
  {
    append_range(*src.seq, src.l + l, src.l + r);
  }

  template <class L, class R>
  void append_range(const RollingHashConcatView<L, R> &src, ll l, ll r)
  {
    ll m = src.lhs.size();
    if (l < m) append_range(src.lhs, l, min(r, m));
    if (m < r) append_range(src.rhs, max(l, m) - m, r - m);
  }

  void append_range(const RollingHashConcat &src, ll l, ll r)
  {
    for (int i = 0; i < (int)src.blocks.size(); i++)
    {
      ll prv = i == 0 ? 0 : src.blocks[i - 1].pref.size();
      ll nxt = src.blocks[i].pref.size();
      if (r <= prv) break;
      if (nxt <= l) continue;
      const Block &b = src.blocks[i];
      append(*b.src, b.l + max(l, prv) - prv, b.l + min(r, nxt) - prv);
    }
  }

  void append(const RH &src, ll l, ll r)
  {
    if (l == r) return;
    assert(r <= INT_MAX);
    Value add = src.hash(l, r);
    if (!blocks.empty() && blocks.back().src == &src && blocks.back().r == l)
    {
      blocks.back().r = int(r);
      blocks.back().pref += add;
      return;
    }
    Value pref = blocks.empty() ? add : blocks.back().pref + add;
    blocks.push_back({&src, int(l), int(r), pref});
  }

  Value prefix_hash(ll r) const
  {
    assert(0 <= r && r <= size());
    if (r == 0) return {};
    auto it = lower_bound(blocks.begin(), blocks.end(), r, [](const Block &block, ll x)
    {
      return block.pref.size() < x;
    });
    ll before = it == blocks.begin() ? 0 : prev(it)->pref.size();
    Value res = it == blocks.begin() ? Value() : prev(it)->pref;
    return res + it->src->hash(it->l, it->l + r - before);
  }

public:
  using element_type = typename V::value_type;

  RollingHashConcat() = default;
  RollingHashConcat(const RollingHashRange<RH> &range) { *this += range; }
  template <class L, class R>
  RollingHashConcat(const RollingHashConcatView<L, R> &view) { append_range(view, 0, view.size()); }

  template <class L, class R>
  RollingHashConcat &operator=(const RollingHashConcatView<L, R> &rhs)
  {
    RollingHashConcat res(rhs);
    res.blocks.reserve(blocks.capacity());
    blocks = std::move(res.blocks);
    return *this;
  }

  void reserve(int n) { blocks.reserve(n); }
  ll size() const { return blocks.empty() ? 0 : blocks.back().pref.size(); }

  RollingHashConcat &operator+=(const RH &rhs)
  {
    append(rhs, 0, rhs.size());
    return *this;
  }
  RollingHashConcat &operator+=(const RollingHashRange<RH> &rhs)
  {
    append(*rhs.seq, rhs.l, rhs.r);
    return *this;
  }

  Value hash() const { return blocks.empty() ? Value() : blocks.back().pref; }
  Value hash(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    return prefix_hash(r).remove_prefix(prefix_hash(l));
  }
  decltype(auto) get(ll i) const
  {
    assert(0 <= i && i < size());
    auto it = lower_bound(blocks.begin(), blocks.end(), i + 1, [](const Block &block, ll x)
    {
      return block.pref.size() < x;
    });
    ll before = it == blocks.begin() ? 0 : prev(it)->pref.size();
    return it->src->get(it->l + i - before);
  }
};
